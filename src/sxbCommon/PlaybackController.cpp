
#include <bx/math.h>

#include "sxbCommon/PlaybackController.h"

SXB_NAMESPACE_BEGIN

void PlaybackController::Update( float dt )
{
    float time = m_fTimeRatio;
    
    if ( m_bPlay )
    {
        time = m_fTimeRatio + dt * m_fPlaybackSpeed / m_fDuration;
        _SetTimeRatio( time );
    }
}

void PlaybackController::Reset()
{
    m_fPreviousTimeRatio = m_fTimeRatio = 0.0f;
    m_fPlaybackSpeed = 1.0f;
    m_fDuration = 0.0f;
    m_bPlay = false;
    m_bLoop = true;
    m_bEnded = false;
    m_endCallback = nullptr;
}

void PlaybackController::_SetTimeRatio( float time )
{
    m_fPreviousTimeRatio = m_fTimeRatio;
    m_fTimeRatio = time - bx::floor( time );
    if ( !m_bEnded &&
        (( m_fPlaybackSpeed > 0 && m_fPreviousTimeRatio > m_fTimeRatio) ||
        ( m_fPlaybackSpeed < 0 && m_fPreviousTimeRatio < m_fTimeRatio)))
    {
        if ( !m_bLoop )
        {
            SetTimeRatio( m_fPlaybackSpeed > 0 ? 1.0f : 0.0f );
            m_bPlay = false;
            m_bEnded = true;
        }
        if ( m_endCallback )
        {
            m_endCallback();
        }
    }
}

SXB_NAMESPACE_END
