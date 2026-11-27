from pathlib import Path

p = Path('src/Renderer.cpp')
s = p.read_text(encoding='utf-8')

# Typedef, pointer, load.
s = s.replace('typedef void (__stdcall *PFN_glReadPixels)',
              'typedef void (__stdcall *PFN_glGetTexImage)(GLenum, GLint, GLenum, GLenum, void *);\n'
              'typedef void (__stdcall *PFN_glReadPixels)', 1)

s = s.replace('PFN_glReadPixels glReadPixels;',
              'PFN_glReadPixels glReadPixels;\nPFN_glGetTexImage glGetTexImage;', 1)

s = s.replace('    LOAD(glReadPixels);',
              '    LOAD(glReadPixels);\n    LOAD(glGetTexImage);', 1)

# Public helper that writes a texture's contents to a PPM for inspection.
anchor = 'void RendererReadPixels(u8 *rgba, i32 width, i32 height)'
if 'RendererDumpTexture' not in s:
    helper = '''// Writes a texture's contents to a binary PPM. Reading it back from the GPU is the only
// way to see what the shader is really sampling, as opposed to what the loader intended.
void RendererDumpTexture(RenderTexture *texture, const char *path)
{
    if (!texture || !path || !g_Renderer.glGetTexImage)
    {
        return;
    }
    u32 bytes = (u32)(texture->width * texture->height) * 4;
    u8 *rgba = (u8 *)malloc(bytes);
    if (!rgba)
    {
        return;
    }
    Entry::glBindTexture(GL_TEXTURE_2D, texture->glTexture);
    g_Renderer.currentTexture = texture->glTexture;
    Entry::glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

    FILE *ppm = fopen(path, "wb");
    if (ppm)
    {
        fprintf(ppm, "P6\\n%d %d\\n255\\n", texture->width, texture->height);
        for (i32 i = 0; i < texture->width * texture->height; i++)
        {
            // Composite over black so transparent texels do not read as white.
            u32 a = rgba[i * 4 + 3];
            u8 rgb[3];
            for (i32 c = 0; c < 3; c++)
            {
                rgb[c] = (u8)((u32)rgba[i * 4 + c] * a / 255);
            }
            fwrite(rgb, 1, 3, ppm);
        }
        fclose(ppm);
    }
    free(rgba);
}

'''
    s = s.replace(anchor, helper + anchor, 1)

p.write_text(s, encoding='utf-8')
print('added glGetTexImage and RendererDumpTexture')
