"""Dump the draw calls and pipeline state of a RenderDoc capture.

Run with qrenderdoc's python:
    qrenderdoc --py tools/rd_dump2.py <capture.rdc> [output.txt]
"""

import sys

import renderdoc as rd


def main():
    argv = sys.argv[1:]
    if not argv:
        print('usage: rd_dump2.py <capture.rdc> [out.txt]')
        return

    capture = argv[0]
    out_path = argv[1] if len(argv) > 1 else capture.rsplit('.', 1)[0] + '_dump.txt'

    cap = rd.OpenCaptureFile()
    if cap.OpenFile(capture, '', None) != rd.ResultCode.Succeeded:
        print('cannot open', capture)
        return

    ok, controller = cap.OpenCapture(rd.ReplayOptions(), None)
    if not ok == rd.ResultCode.Succeeded:
        print('cannot replay', ok)
        return

    out = open(out_path, 'w', encoding='utf-8')
    draws = []

    def walk(actions):
        for a in actions:
            if a.children:
                walk(a.children)
                continue
            if a.flags & (rd.ActionFlags.Drawcall | rd.ActionFlags.Drawcall):
                draws.append(a)

    walk(controller.GetRootActions())

    out.write(f'total draws: {len(draws)}\n\n')

    # Inspect the last few draws: those are the ones that decide the final image.
    for action in draws[-8:]:
        controller.SetFrameEvent(action.eventId, True)
        pipe = controller.GetPipelineState()
        out.write(f'--- event {action.eventId} {action.customName or ""} ---\n')
        out.write(f'  numIndices={action.numIndices} numInstances={action.numInstances}\n')
        out.write(f'  depth test enabled={pipe.IsDepthTestEnabled()} '
                  f'write={pipe.IsDepthWriteEnabled()} '
                  f'func={pipe.GetDepthTestStateForContext().function}\n')
        out.write(f'  blend enabled={pipe.IsBlendEnabled(0)}\n')
        for unit, tex in enumerate(pipe.GetUsedTextures(rd.ShaderStage.Fragment)):
            out.write(f'  sampler{unit}: {tex.resourceId}\n')
        for vp in pipe.GetViewports():
            out.write(f'  viewport x={vp.x} y={vp.y} w={vp.width} h={vp.height}\n')
        # Blend factor and colour mask decide whether output reaches the target.
        out.write(f'  colWriteMask(rt0)={pipe.GetColorWriteMask(0)}\n')

    out.close()
    print('wrote', out_path)
    controller.Shutdown()
    cap.Shutdown()


main()
