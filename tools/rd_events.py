"""Dump the event list of a RenderDoc capture.

Run through qrenderdoc's Python console:
    qrenderdoc --python tools/rd_events.py -- <capture.rdc>

Writes a text file next to the capture with one line per event, marking draws and
listing the pipeline state that decides what reaches the screen.
"""

import sys

import renderdoc as rd


def main():
    capture = sys.argv[-1]
    cap = rd.OpenCaptureFile()
    status = cap.OpenFile(capture, '', None)
    if status != rd.ResultCode.Succeeded:
        print('cannot open capture:', status)
        return

    if not cap.LocalReplaySupport():
        print('capture cannot be replayed locally')
        return

    status, controller = cap.OpenCapture(rd.ReplayOptions(), None)
    if status != rd.ResultCode.Succeeded:
        print('cannot open replay:', status)
        return

    out_path = capture.rsplit('.', 1)[0] + '_events.txt'
    with open(out_path, 'w', encoding='utf-8') as f:
        actions = controller.GetRootActions()
        count = [0]

        def walk(action_list, depth):
            for action in action_list:
                if action.children:
                    walk(action.children, depth + 1)
                    continue
                count[0] += 1
                if count[0] > 400:
                    return
                name = action.eventId
                flags = []
                if action.flags & rd.ActionFlags.Drawcall:
                    flags.append('DRAW')
                if action.flags & rd.ActionFlags.Clear:
                    flags.append('CLEAR')
                if action.flags & rd.ActionFlags.Present:
                    flags.append('PRESENT')
                f.write(f'{name:>6} {"|".join(flags):20} {action.customName or ""}\n')

        walk(actions, 0)

        # Pipeline state at the last draw of the frame, which is what produced the image.
        controller.SetFrameEvent(controller.GetMaxActionIndex(), True)
        pipe = controller.GetPipelineState()
        f.write('\n=== viewport ===\n')
        for vp in pipe.GetViewports():
            f.write(f'  x={vp.x} y={vp.y} w={vp.width} h={vp.height} minDepth={vp.minDepth} maxDepth={vp.maxDepth}\n')
        f.write(f'\nblend enabled: {pipe.IsBlendEnabled(0)}\n')
        f.write(f'depth test: {pipe.GetDepthTestStateForContext()}\n')

    print('wrote', out_path)
    controller.Shutdown()
    cap.Shutdown()


main()
