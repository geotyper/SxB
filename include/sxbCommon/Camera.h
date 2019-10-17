
#ifndef _CAMERA_H_C4630C9041FCF4F0817438934D22A400_
#define _CAMERA_H_C4630C9041FCF4F0817438934D22A400_

#include <bx/allocator.h>
#include <bx/readerwriter.h>
#include <bx/file.h>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

#include <string>

#include "sxbCommon/defines.h"

SXB_NAMESPACE_BEGIN

class Cursor;
class Camera
{
public:
    Camera();
    
public:
    void mtxLookAt(float* _outViewMtx);
    void envViewMtx(float* _mtx);
    
public:
    void orbit(float _dx, float _dy);
    void dolly(float _dz);
    
public:
    void update(float _dt);
    void update(const Cursor & cursor, float _dt);
    
protected:
    void _reset();
    void _consumeOrbit(float _amount);
    
public:
    struct Interp3f
    {
        bx::Vec3 curr;
        bx::Vec3 dest;
    };
    
    Interp3f m_target;
    Interp3f m_pos;
    float m_orbit[2];
};

SXB_NAMESPACE_END

#endif // _CAMERA_H_C4630C9041FCF4F0817438934D22A400_
