
#include <bx/math.h>

#include "sxbCommon/PlaybackController.h"

SXB_NAMESPACE_BEGIN

void PlaybackController::Play()
{
    m_bPlay = true;
}

void PlaybackController::Pause()
{
    m_bPlay = false;
}

void PlaybackController::Stop()
{
    m_bPlay = false;
    m_fTimeRatio = 0.0f;
}

void PlaybackController::Update( float dt )
    {
    float time = m_fTimeRatio;
    
    if ( m_bPlay )
    {
        time = m_fTimeRatio + dt * m_fPlaybackSpeed / m_fDuration;
    }
    
    _SetTimeRatio( time );
}

void PlaybackController::Reset()
{
    m_fPreviousTimeRatio = m_fTimeRatio = 0.0f;
    m_fPlaybackSpeed = 1.0f;
    m_fDuration = 0.0f;
    m_bPlay = false;
}

void PlaybackController::SetEndCallback( void (*callback)() )
{
    m_endCallback = callback;
}

void PlaybackController::_SetTimeRatio( float time )
{
    m_fPreviousTimeRatio = m_fTimeRatio;
    if ( m_bLoop )
    {
        m_fTimeRatio = time - bx::floor( time );
        if ( m_endCallback && ( m_fTimeRatio - 1.0f ) <= bx::kNearZero )
        {
            m_endCallback();
        }
    }
    else
    {
        m_fTimeRatio = bx::clamp( time, 0.0f, 1.0f );
        if ( m_endCallback && ( time - 1.0f ) <= bx::kNearZero )
        {
            m_endCallback();
        }
    }
}

SXB_NAMESPACE_END
