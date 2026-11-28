// Helpers for owning raw pointers.
//
// These came in with the reconstruction as dxutil.hpp, a copy of a Microsoft DirectX SDK
// header. Only four pointer-cleanup macros were ever used, and nothing here has anything to
// do with DirectX, so the name now says what the file actually is.
//
// The macros are kept as macros because they assign to the pointer variable, which a
// function template cannot do.

#pragma once

#define SAFE_DELETE(p)                                                                             \
    {                                                                                              \
        if (p)                                                                                     \
        {                                                                                          \
            delete (p);                                                                            \
            (p) = NULL;                                                                            \
        }                                                                                          \
    }

#define SAFE_DELETE_ARRAY(p)                                                                       \
    {                                                                                              \
        if (p)                                                                                     \
        {                                                                                          \
            delete[] (p);                                                                          \
            (p) = NULL;                                                                            \
        }                                                                                          \
    }

#define SAFE_FREE(p)                                                                               \
    {                                                                                              \
        if (p)                                                                                     \
        {                                                                                          \
            free(p);                                                                               \
            (p) = NULL;                                                                            \
        }                                                                                          \
    }

#define SAFE_RELEASE(p)                                                                            \
    {                                                                                              \
        if (p)                                                                                     \
        {                                                                                          \
            (p)->Release();                                                                        \
            (p) = NULL;                                                                            \
        }                                                                                          \
    }
