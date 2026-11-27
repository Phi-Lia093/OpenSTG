// Direct3D 8 maths, reimplemented without Direct3D.
//
// Only the matrix and vector helpers live here: they are pure functions over floats
// and contain nothing device-specific, so the call sites that use them keep working
// verbatim. Everything that actually configured the device was rewritten where it was
// called, and there is no device object left to emulate.
#include "RenderCompat.hpp"

#include <cmath>

D3DXMATRIX *D3DXMatrixIdentity(D3DXMATRIX *out)
{
    out->Identity();
    return out;
}

D3DXMATRIX *D3DXMatrixMultiply(D3DXMATRIX *out, const D3DXMATRIX *a,
                               const D3DXMATRIX *b)
{
    return MatrixMultiply(out, a, b);
}

D3DXMATRIX *D3DXMatrixRotationX(D3DXMATRIX *out, f32 angle)
{
    return MatrixRotationX(out, angle);
}

D3DXMATRIX *D3DXMatrixRotationY(D3DXMATRIX *out, f32 angle)
{
    return MatrixRotationY(out, angle);
}

D3DXMATRIX *D3DXMatrixRotationZ(D3DXMATRIX *out, f32 angle)
{
    return MatrixRotationZ(out, angle);
}

D3DXMATRIX *D3DXMatrixRotationQuaternion(D3DXMATRIX *out, const D3DXQUATERNION *q)
{
    // Quaternion (x, y, z, w) to rotation matrix, D3DX's convention.
    f32 x = q->x, y = q->y, z = q->z, w = q->w;
    f32 xx = x * x, yy = y * y, zz = z * z;
    f32 xy = x * y, xz = x * z, yz = y * z;
    f32 wx = w * x, wy = w * y, wz = w * z;

    out->Identity();
    out->m[0][0] = 1.0f - 2.0f * (yy + zz);
    out->m[0][1] = 2.0f * (xy + wz);
    out->m[0][2] = 2.0f * (xz - wy);
    out->m[1][0] = 2.0f * (xy - wz);
    out->m[1][1] = 1.0f - 2.0f * (xx + zz);
    out->m[1][2] = 2.0f * (yz + wx);
    out->m[2][0] = 2.0f * (xz + wy);
    out->m[2][1] = 2.0f * (yz - wx);
    out->m[2][2] = 1.0f - 2.0f * (xx + yy);
    return out;
}

D3DXMATRIX *D3DXMatrixScaling(D3DXMATRIX *out, f32 x, f32 y, f32 z)
{
    return MatrixScaling(out, x, y, z);
}

D3DXMATRIX *D3DXMatrixTranslation(D3DXMATRIX *out, f32 x, f32 y, f32 z)
{
    return MatrixTranslation(out, x, y, z);
}

D3DXMATRIX *D3DXMatrixLookAtLH(D3DXMATRIX *out, const D3DXVECTOR3 *eye,
                               const D3DXVECTOR3 *at, const D3DXVECTOR3 *up)
{
    const f32 e[3] = {eye->x, eye->y, eye->z};
    const f32 a[3] = {at->x, at->y, at->z};
    const f32 u[3] = {up->x, up->y, up->z};
    return MatrixLookAtLH(out, e, a, u);
}

D3DXMATRIX *D3DXMatrixPerspectiveFovLH(D3DXMATRIX *out, f32 fovY, f32 aspect,
                                       f32 zn, f32 zf)
{
    return MatrixPerspectiveFovLH(out, fovY, aspect, zn, zf);
}

f32 D3DXVec3Length(const D3DXVECTOR3 *v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

f32 D3DXVec3LengthSq(const D3DXVECTOR3 *v)
{
    return v->x * v->x + v->y * v->y + v->z * v->z;
}

f32 D3DXVec3Dot(const D3DXVECTOR3 *a, const D3DXVECTOR3 *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

D3DXVECTOR3 *D3DXVec3Cross(D3DXVECTOR3 *out, const D3DXVECTOR3 *a,
                           const D3DXVECTOR3 *b)
{
    D3DXVECTOR3 result;
    result.x = a->y * b->z - a->z * b->y;
    result.y = a->z * b->x - a->x * b->z;
    result.z = a->x * b->y - a->y * b->x;
    *out = result;
    return out;
}

D3DXVECTOR3 *D3DXVec3Normalize(D3DXVECTOR3 *out, const D3DXVECTOR3 *v)
{
    f32 length = D3DXVec3Length(v);
    if (length == 0.0f)
    {
        out->x = out->y = out->z = 0.0f;
        return out;
    }
    f32 inv = 1.0f / length;
    out->x = v->x * inv;
    out->y = v->y * inv;
    out->z = v->z * inv;
    return out;
}

D3DXVECTOR3 *D3DXVec3TransformCoord(D3DXVECTOR3 *out, const D3DXVECTOR3 *v,
                                    const D3DXMATRIX *m)
{
    f32 x = v->x * m->m[0][0] + v->y * m->m[1][0] + v->z * m->m[2][0] + m->m[3][0];
    f32 y = v->x * m->m[0][1] + v->y * m->m[1][1] + v->z * m->m[2][1] + m->m[3][1];
    f32 z = v->x * m->m[0][2] + v->y * m->m[1][2] + v->z * m->m[2][2] + m->m[3][2];
    f32 w = v->x * m->m[0][3] + v->y * m->m[1][3] + v->z * m->m[2][3] + m->m[3][3];
    if (w != 0.0f)
    {
        f32 inv = 1.0f / w;
        x *= inv;
        y *= inv;
        z *= inv;
    }
    out->x = x;
    out->y = y;
    out->z = z;
    return out;
}

D3DXVECTOR3 *D3DXVec3Project(D3DXVECTOR3 *out, const D3DXVECTOR3 *v,
                             const D3DVIEWPORT8 *viewport, const D3DXMATRIX *projection,
                             const D3DXMATRIX *view, const D3DXMATRIX *world)
{
    // world -> view -> projection, then perspective divide and viewport mapping.
    f32 wx = v->x * world->m[0][0] + v->y * world->m[1][0] + v->z * world->m[2][0] +
             world->m[3][0];
    f32 wy = v->x * world->m[0][1] + v->y * world->m[1][1] + v->z * world->m[2][1] +
             world->m[3][1];
    f32 wz = v->x * world->m[0][2] + v->y * world->m[1][2] + v->z * world->m[2][2] +
             world->m[3][2];
    f32 ww = v->x * world->m[0][3] + v->y * world->m[1][3] + v->z * world->m[2][3] +
             world->m[3][3];

    f32 vx = wx * view->m[0][0] + wy * view->m[1][0] + wz * view->m[2][0] +
             ww * view->m[3][0];
    f32 vy = wx * view->m[0][1] + wy * view->m[1][1] + wz * view->m[2][1] +
             ww * view->m[3][1];
    f32 vz = wx * view->m[0][2] + wy * view->m[1][2] + wz * view->m[2][2] +
             ww * view->m[3][2];
    f32 vw = wx * view->m[0][3] + wy * view->m[1][3] + wz * view->m[2][3] +
             ww * view->m[3][3];

    f32 px = vx * projection->m[0][0] + vy * projection->m[1][0] + vz * projection->m[2][0] +
             vw * projection->m[3][0];
    f32 py = vx * projection->m[0][1] + vy * projection->m[1][1] + vz * projection->m[2][1] +
             vw * projection->m[3][1];
    f32 pz = vx * projection->m[0][2] + vy * projection->m[1][2] + vz * projection->m[2][2] +
             vw * projection->m[3][2];
    f32 pw = vx * projection->m[0][3] + vy * projection->m[1][3] + vz * projection->m[2][3] +
             vw * projection->m[3][3];

    if (pw == 0.0f)
    {
        out->x = out->y = out->z = 0.0f;
        return out;
    }
    f32 invW = 1.0f / pw;
    px *= invW;
    py *= invW;
    pz *= invW;

    // NDC (-1..1) to viewport pixels. D3D's y axis points down in screen space, and
    // D3DXVec3Project flips it here.
    out->x = (px + 1.0f) * 0.5f * (f32)viewport->Width + (f32)viewport->X;
    out->y = (1.0f - py) * 0.5f * (f32)viewport->Height + (f32)viewport->Y;
    out->z = pz * (viewport->MaxZ - viewport->MinZ) + viewport->MinZ;
    return out;
}
