"""List every Direct3D 8 call the pre-migration sources made, with counts.

This is the inventory the GL backend has to account for: anything called here needs an
equivalent in the renderer, and anything the original relied on implicitly (device
defaults, state blocks) needs an explicit equivalent too.
"""

import re
import subprocess
from collections import Counter

OLD = '8fa016a'

files = subprocess.run(['git', 'ls-tree', '-r', '--name-only', OLD, 'src/'],
                        capture_output=True, text=True, check=True).stdout.split()
files = [f for f in files if f.endswith(('.cpp', '.hpp'))]

# Method calls on a device or interface: something->SetSomething(...)
call = re.compile(r'\b(\w+)\s*->\s*(Set\w+|Get\w+|Draw\w+|Present|CopyRects|'
                  r'Create\w+|BeginScene|EndScene|Clear|Release|Reset)\s*\(')

per_method = Counter()
per_file = Counter()

for path in files:
    text = subprocess.run(['git', 'show', f'{OLD}:{path}'],
                          capture_output=True, text=True).stdout
    for m in call.finditer(text):
        method = m.group(2)
        # Only keep the ones on D3D/D3DX-ish interfaces; sprite/anm objects also have
        # such methods and would drown the signal.
        line_start = text.rfind('\n', 0, m.start()) + 1
        line = text[line_start:text.find('\n', m.start())]
        if any(t in line for t in ('anm', 'vm->', 'sprite->', 'surface->', 'texture->')):
            continue
        per_method[method] += 1
        per_file[path] += 1

print('=== distinct methods called (top 60) ===')
for name, count in per_method.most_common(60):
    print(f'{count:5}  {name}')

print('\n=== files with the most calls ===')
for name, count in per_file.most_common(12):
    print(f'{count:5}  {name}')
