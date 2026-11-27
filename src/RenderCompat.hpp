// Direct3D 8 type names, kept only where the concept still exists.
//
// The game refers to D3DXMATRIX, D3DCOLOR, D3DVIEWPORT8 and a few D3DRS_* values in
// a few hundred places. A matrix is a matrix and a colour is a colour, so those names
// survive and map onto the SDL/OpenGL renderer. There is deliberately no device object
// here: every call that used to configure Direct3D now calls the renderer directly at
// the call site, so nothing pretends D3D is still present.
#pragma once

#include <windows.h>

#include "Renderer.hpp"
#include "inttypes.hpp"

typedef RenderColor D3DCOLOR;
typedef RenderMatrix D3DXMATRIX;

#define D3DCOLOR_ARGB(a, r, g, b)                                             \
    ((D3DCOLOR)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | \
                ((b) & 0xff)))
#define D3DCOLOR_RGBA(r, g, b, a) D3DCOLOR_ARGB(a, r, g, b)

typedef struct D3DXVECTOR3
{
    f32 x, y, z;

    D3DXVECTOR3() : x(0), y(0), z(0) {}
    D3DXVECTOR3(f32 px, f32 py, f32 pz) : x(px), y(py), z(pz) {}

    D3DXVECTOR3 operator-() const { return D3DXVECTOR3(-x, -y, -z); }
    D3DXVECTOR3 operator-(const D3DXVECTOR3 &o) const
    {
        return D3DXVECTOR3(x - o.x, y - o.y, z - o.z);
    }
    D3DXVECTOR3 operator+(const D3DXVECTOR3 &o) const
    {
        return D3DXVECTOR3(x + o.x, y + o.y, z + o.z);
    }
    D3DXVECTOR3 operator*(f32 s) const { return D3DXVECTOR3(x * s, y * s, z * s); }
    D3DXVECTOR3 operator/(f32 s) const { return D3DXVECTOR3(x / s, y / s, z / s); }    D3DXVECTOR3 &operator-=(const D3DXVECTOR3 &o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    D3DXVECTOR3 &operator+=(const D3DXVECTOR3 &o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    D3DXVECTOR3 &operator*=(f32 s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    D3DXVECTOR3 &operator/=(f32 s)
    {
        x /= s;
        y /= s;
        z /= s;
        return *this;
    }
} D3DXVECTOR3;

// Scalar on the left, as in 2.0f * vec.
inline D3DXVECTOR3 operator*(f32 s, const D3DXVECTOR3 &v)
{
    return D3DXVECTOR3(v.x * s, v.y * s, v.z * s);
}

typedef struct D3DXQUATERNION
{
    f32 x, y, z, w;
} D3DXQUATERNION;

typedef struct D3DVIEWPORT8
{
    DWORD X;
    DWORD Y;
    DWORD Width;
    DWORD Height;
    f32 MinZ;
    f32 MaxZ;
} D3DVIEWPORT8, *LPD3DVIEWPORT8;

// Clear flags. Kept because call sites pass them straight through to the renderer.
#define D3DCLEAR_TARGET 0x00000001L
#define D3DCLEAR_ZBUFFER 0x00000002L

// --- maths -----------------------------------------------------------------------
//
// Pure functions, no device involved. Reimplemented in RenderCompat.cpp on top of the
// renderer's matrix helpers so the existing call sites keep working unchanged.

D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *out);
D3DXMATRIX *D3DXMatrixMultiply(D3DXMATRIX *out, const D3DXMATRIX *a,
                               const D3DXMATRIX *b);
D3DXMATRIX *D3DXMatrixRotationX(D3DXMATRIX *out, f32 angle);
D3DXMATRIX *D3DXMatrixRotationY(D3DXMATRIX *out, f32 angle);
D3DXMATRIX *D3DXMatrixRotationZ(D3DXMATRIX *out, f32 angle);
D3DXMATRIX *D3DXMatrixRotationQuaternion(D3DXMATRIX *out, const D3DXQUATERNION *q);
D3DXMATRIX *D3DXMatrixScaling(D3DXMATRIX *out, f32 x, f32 y, f32 z);
D3DXMATRIX *D3DXMatrixTranslation(D3DXMATRIX *out, f32 x, f32 y, f32 z);
D3DXMATRIX *D3DXMatrixLookAtLH(D3DXMATRIX *out, const D3DXVECTOR3 *eye,
                               const D3DXVECTOR3 *at, const D3DXVECTOR3 *up);
D3DXMATRIX *D3DXMatrixPerspectiveFovLH(D3DXMATRIX *out, f32 fovY, f32 aspect,
                                       f32 zn, f32 zf);
f32 D3DXVec3Length(const D3DXVECTOR3 *v);
f32 D3DXVec3LengthSq(const D3DXVECTOR3 *v);
f32 D3DXVec3Dot(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b);
D3DXVECTOR3 *D3DXVec3Cross(D3DXVECTOR3 *out, const D3DXVECTOR3 *a,
                           const D3DXVECTOR3 *b);
D3DXVECTOR3 *D3DXVec3Normalize(D3DXVECTOR3 *out, const D3DXVECTOR3 *v);
D3DXVECTOR3 *D3DXVec3TransformCoord(D3DXVECTOR3 *out, const D3DXVECTOR3 *v,
                                    const D3DXMATRIX *m);
D3DXVECTOR3 *D3DXVec3Project(D3DXVECTOR3 *out, const D3DXVECTOR3 *v,
                             const D3DVIEWPORT8 *viewport, const D3DXMATRIX *projection,
                             const D3DXMATRIX *view, const D3DXMATRIX *world);
