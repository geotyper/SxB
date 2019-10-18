
#include "sxbCommon/Cursor.h"
#include "sxbCommon/Camera.h"

SXB_NAMESPACE_BEGIN


Camera::Camera()
{
    _reset();
}

void Camera::_reset()
{
    m_target.curr = { 0.0f, 0.0f, 0.0f };
    m_target.dest = { 0.0f, 0.0f, 0.0f };
    
    m_pos.curr = { 0.0f, 0.0f, -3.0f };
    m_pos.dest = { 0.0f, 0.0f, -3.0f };
    
    m_orbit[0] = 0.0f;
    m_orbit[1] = 0.0f;
}

void Camera::mtxLookAt(float* _outViewMtx)
{
    bx::mtxLookAt(_outViewMtx, m_pos.curr, m_target.curr);
}

void Camera::orbit(float _dx, float _dy)
{
    m_orbit[0] += _dx;
    m_orbit[1] += _dy;
}

void Camera::dolly(float _dz)
{
    const float cnear = 1.0f;
    const float cfar  = 100.0f;
    
    const bx::Vec3 toTarget     = bx::sub(m_target.dest, m_pos.dest);
    const float toTargetLen     = bx::length(toTarget);
    const float invToTargetLen  = 1.0f / (toTargetLen + bx::kFloatMin);
    const bx::Vec3 toTargetNorm = bx::mul(toTarget, invToTargetLen);
    
    float delta  = toTargetLen * _dz;
    float newLen = toTargetLen + delta;
    if ( (cnear  < newLen || _dz < 0.0f)
        &&   (newLen < cfar   || _dz > 0.0f) )
    {
        m_pos.dest = bx::mad(toTargetNorm, delta, m_pos.dest);
    }
}

void Camera::_consumeOrbit(float _amount)
{
    float consume[2];
    consume[0] = m_orbit[0] * _amount;
    consume[1] = m_orbit[1] * _amount;
    m_orbit[0] -= consume[0];
    m_orbit[1] -= consume[1];
    
    const bx::Vec3 toPos     = bx::sub(m_pos.curr, m_target.curr);
    const float toPosLen     = bx::length(toPos);
    const float invToPosLen  = 1.0f / (toPosLen + bx::kFloatMin);
    const bx::Vec3 toPosNorm = bx::mul(toPos, invToPosLen);
    
    float ll[2];
    bx::toLatLong(&ll[0], &ll[1], toPosNorm);
    ll[0] += consume[0];
    ll[1] -= consume[1];
    ll[1]  = bx::clamp(ll[1], 0.02f, 0.98f);
    
    const bx::Vec3 tmp  = bx::fromLatLong(ll[0], ll[1]);
    const bx::Vec3 diff = bx::mul(bx::sub(tmp, toPosNorm), toPosLen);
    
    m_pos.curr = bx::add(m_pos.curr, diff);
    m_pos.dest = bx::add(m_pos.dest, diff);
}

void Camera::update(float _dt)
{
    const float amount = bx::min(_dt / 0.12f, 1.0f);
    
    _consumeOrbit(amount);
    
    m_target.curr = bx::lerp(m_target.curr, m_target.dest, amount);
    m_pos.curr    = bx::lerp(m_pos.curr,    m_pos.dest,    amount);
}

void Camera::update(const Cursor & cursor, float _dt)
{
    if (cursor.touch[1].press)
    {
        // right
        dolly((float)cursor.touch[1].deltaX / 300.0 + (float)cursor.touch[1].deltaY / 300.0);
    }
    else if (cursor.touch[0].press)
    {
        // left
        orbit(-(float)cursor.touch[0].deltaX / 300.0, (float)cursor.touch[0].deltaY / 300.0);
    }
    
    if (cursor.mouse[sf::Mouse::Button::Right].press)
    {
        // right
        dolly((float)cursor.mouse[sf::Mouse::Button::Right].deltaX / 300.0 + (float)cursor.mouse[sf::Mouse::Button::Right].deltaY / 300.0);
    }
    else if (cursor.mouse[sf::Mouse::Button::Left].press)
    {
        orbit((float)cursor.mouse[sf::Mouse::Button::Left].deltaX / 300.0, (float)cursor.mouse[sf::Mouse::Button::Left].deltaY / 300.0);
    }
    
    dolly( cursor.wheelDelta / 100.0f );
    update(_dt);
}

void Camera::envViewMtx(float* _mtx)
{
    const bx::Vec3 toTarget     = bx::sub(m_target.curr, m_pos.curr);
    const float toTargetLen     = bx::length(toTarget);
    const float invToTargetLen  = 1.0f / (toTargetLen + bx::kFloatMin);
    const bx::Vec3 toTargetNorm = bx::mul(toTarget, invToTargetLen);
    
    const bx::Vec3 right = bx::normalize(bx::cross({ 0.0f, 1.0f, 0.0f }, toTargetNorm) );
    const bx::Vec3 up    = bx::normalize(bx::cross(toTargetNorm, right) );
    
    _mtx[ 0] = right.x;
    _mtx[ 1] = right.y;
    _mtx[ 2] = right.z;
    _mtx[ 3] = 0.0f;
    _mtx[ 4] = up.x;
    _mtx[ 5] = up.y;
    _mtx[ 6] = up.z;
    _mtx[ 7] = 0.0f;
    _mtx[ 8] = toTargetNorm.x;
    _mtx[ 9] = toTargetNorm.y;
    _mtx[10] = toTargetNorm.z;
    _mtx[11] = 0.0f;
    _mtx[12] = 0.0f;
    _mtx[13] = 0.0f;
    _mtx[14] = 0.0f;
    _mtx[15] = 1.0f;
}

SXB_NAMESPACE_END
