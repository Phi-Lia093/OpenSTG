from pathlib import Path

# 1) Replace the return-address probe with an explicit source tag that call sites set.
p = Path('src/Renderer.cpp')
s = p.read_text(encoding='utf-8')

s = s.replace('''                // The caller's return address tells a blit apart from a batched sprite
                // draw or the 3D path, which is what identifies a stray full-screen quad.
                void *caller = _ReturnAddress();
                fprintf(log,
                        "#%u verts=%d gl=%u tex=%d x=%.1f..%.1f y=%.1f..%.1f "
                        "z=%.4f..%.4f color=%08x from=%p\\n",
                        drawLog, vertexCount, glTexture, useTexture ? 1 : 0, minX, maxX,
                        minY, maxY, minZ, maxZ, vertices[0].color, caller);''',
'''                fprintf(log,
                        "#%u verts=%d gl=%u tex=%d x=%.1f..%.1f y=%.1f..%.1f "
                        "z=%.4f..%.4f color=%08x from=%s\\n",
                        drawLog, vertexCount, glTexture, useTexture ? 1 : 0, minX, maxX,
                        minY, maxY, minZ, maxZ, vertices[0].color,
                        g_DrawSource ? g_DrawSource : "?");''')

# Declare the tag just above RendererDrawTriangles.
anchor = 'void RendererDrawTriangles(const RenderVertex *vertices, i32 vertexCount,'
if 'g_DrawSource' not in s.split(anchor)[0]:
    s = s.replace(anchor,
                  'const char *g_DrawSource = NULL;\n\n' + anchor, 1)

p.write_text(s, encoding='utf-8')
print('renderer tagged')

# 2) Tag each call site.
edits = [
    ('src/AnmManager.cpp',
     '    RendererDrawTriangles(scratch, (i32)n, texture, texture != NULL);',
     '    g_DrawSource = "flush";\n'
     '    RendererDrawTriangles(scratch, (i32)n, texture, texture != NULL);\n'
     '    g_DrawSource = NULL;'),
    ('src/AnmManager.cpp',
     '    RendererDrawTriangles(triangles, 6, texture, true);',
     '    g_DrawSource = "draw3";\n'
     '    RendererDrawTriangles(triangles, 6, texture, true);\n'
     '    g_DrawSource = NULL;'),
    ('src/Renderer.cpp',
     '    RendererDrawTriangles(quad, 6, src, true);',
     '    g_DrawSource = "blit";\n'
     '    RendererDrawTriangles(quad, 6, src, true);\n'
     '    g_DrawSource = NULL;'),
]

for path, old, new in edits:
    pp = Path(path)
    text = pp.read_text(encoding='utf-8')
    if old not in text:
        print(f'MISS in {path}: {old.strip()[:50]}')
        continue
    text = text.replace(old, new, 1)
    pp.write_text(text, encoding='utf-8')
    print(f'tagged {path}: {old.strip()[:40]}')

# 3) Extern declaration for the tag in the other translation units.
hpp = Path('src/Renderer.hpp')
ht = hpp.read_text(encoding='utf-8')
if 'g_DrawSource' not in ht:
    marker = 'void RendererDrawTriangles(const RenderVertex *vertices, i32 vertexCount,'
    ht = ht.replace(marker,
                    '// Diagnostic tag naming the path a draw originated from.\n'
                    'extern const char *g_DrawSource;\n\n' + marker, 1)
    hpp.write_text(ht, encoding='utf-8')
    print('declared g_DrawSource in Renderer.hpp')
