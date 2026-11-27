"""Extract every SetRenderState / SetTextureStageState / SetTransform argument.

These are the device states whose D3D defaults the GL backend has to reproduce by hand.
A missing one shows up as a visual bug far from the call site, which is exactly the class
of problem being chased.
"""

import re
import subprocess
from collections import Counter

OLD = '8fa016a'

files = subprocess.run(['git', 'ls-tree', '-r', '--name-only', OLD, 'src/'],
                        capture_output=True, text=True, check=True).stdout.split()
files = [f for f in files if f.endswith(('.cpp', '.hpp'))]

state_call = re.compile(
    r'(SetRenderState|SetTextureStageState|SetTransform|SetVertexShader|'
    r'SetTexture|SetViewport|Clear|DrawPrimitiveUP)\s*\(([^;]{0,160})', re.S)

counts = Counter()
samples = {}

for path in files:
    text = subprocess.run(['git', 'show', f'{OLD}:{path}'],
                          capture_output=True, text=True).stdout
    for m in state_call.finditer(text):
        method = m.group(1)
        args = ' '.join(m.group(2).split())
        # Normalise the first argument: the enum or constant names the state.
        first = args.split(',')[0].strip()
        key = f'{method}({first}'
        counts[key] += 1
        samples.setdefault(key, f'{path}: {method}({args[:110]}')

print('=== device state touched, by first argument ===')
for key, count in sorted(counts.items(), key=lambda kv: -kv[1]):
    print(f'{count:4}  {key}')
    print(f'        {samples[key]}')
