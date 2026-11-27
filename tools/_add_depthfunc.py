from pathlib import Path

p = Path('src/Renderer.cpp')
s = p.read_text(encoding='utf-8')

s = s.replace('#define GL_DEPTH_TEST 0x0B71',
              '#define GL_DEPTH_TEST 0x0B71\n#define GL_LEQUAL 0x0203', 1)

s = s.replace('typedef void (__stdcall *PFN_glDepthMask)(GLboolean);',
              'typedef void (__stdcall *PFN_glDepthMask)(GLboolean);\n'
              'typedef void (__stdcall *PFN_glDepthFunc)(GLenum);', 1)

s = s.replace('PFN_glDepthMask glDepthMask;',
              'PFN_glDepthMask glDepthMask;\nPFN_glDepthFunc glDepthFunc;', 1)

s = s.replace('    LOAD(glDepthMask);',
              '    LOAD(glDepthMask);\n    LOAD(glDepthFunc);', 1)

p.write_text(s, encoding='utf-8')
print('added glDepthFunc plumbing')
