"""Compare the original D3D Draw3 against the current GL port, item by item."""

import re
import subprocess
from pathlib import Path

OLD = '8fa016a'

orig = subprocess.run(['git', 'show', f'{OLD}:src/AnmManager.cpp'],
                      capture_output=True, text=True, encoding='utf-8',
                      errors='replace').stdout
start = orig.index('ZunResult AnmManager::Draw3')
orig_draw3 = orig[start:orig.index('AnmVm::GetFloatVarValue')]

cur = Path('src/AnmManager.cpp').read_text(encoding='utf-8')
cstart = cur.index('ZunResult AnmManager::Draw3')
cur_draw3 = cur[cstart:cur.index('AnmVm::GetFloatVarValue', cstart)]

# Behaviours the D3D version expressed through device state, each with the marker that
# proves the GL version reproduces it.
checks = [
    ('world transform from vm->matrix and scale',
     'worldTransformMatrix = vm->matrix', 'worldTransformMatrix = vm->matrix'),
    ('rotation applied in X/Y/Z order',
     'D3DXMatrixRotationX', 'D3DXMatrixRotationX'),
    ('anchor offsets for x', 'anchor & 1', 'anchor & 1'),
    ('anchor offsets for y', 'anchor & 2', 'anchor & 2'),
    ('anm offset added to world', 'this->offset.x', 'this->offset.x'),
    ('uvMatrix folded into texture coords', 'uv = vm->uvMatrix', 'uvMatrix'),
    ('uv scroll added', 'uvScrollPos.x', 'uvScrollPos.x'),
    ('per-sprite texture rebind', 'currentTexture != this->textures', 'texture'),
    ('blend/z state synced', 'SetRenderStateForVm', 'SetRenderStateForVm'),
    ('texture factor used for colouring', 'D3DTA_TFACTOR', '?'),
]

print(f'{"D3D behaviour":45} {"present in GL port"}')
print('-' * 70)
for label, orig_marker, cur_marker in checks:
    in_orig = orig_marker in orig_draw3
    in_cur = cur_marker in cur_draw3 if cur_marker != '?' else False
    mark = 'yes' if in_cur else 'NO'
    if label.startswith('texture factor'):
        mark = 'NO  (hardcoded 1:1 multiply)'
    print(f'{label:45} {mark}')

print()
print('=== current Draw3 texture-coordinate code ===')
for line in cur_draw3.splitlines():
    if 'uv' in line.lower() and ('=' in line):
        print(' ', line.strip())
