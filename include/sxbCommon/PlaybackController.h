
#ifndef _PLAYBACKCONTROLLER_H_3B8EA5267842E2BF46BFFFBBE20F55CA_
#define _PLAYBACKCONTROLLER_H_3B8EA5267842E2BF46BFFFBBE20F55CA_

#include "sxbCommon/defines.h"

SXB_NAMESPACE_BEGIN

class PlaybackController
{
public:
    void    SetTimeRatio( float time )
    {
        m_fPreviousTimeRatio = m_fTimeRatio = bx::clamp( time, 0.0f, 1.0f );
        m_bEnded = false;
    }
    float   GetTimeRatio() const { return m_fTimeRatio; }
    float   GetPreviousTimeRatio() const { return m_fPreviousTimeRatio; }
    
    void    Play()
    {
        m_bPlay = true;
        if ( m_bEnded )
        {
            SetTimeRatio( m_fPlaybackSpeed > 0 ? 0.0f : 1.0f );
        }
    }
    bool    IsPlaying()
    {
        return m_bPlay;
    }
    
    void    Pause()
    {
        m_bPlay = false;
    }
    void    Stop()
    {
        SetTimeRatio( m_fPlaybackSpeed > 0 ? 0.0f : 1.0f );
        m_bPlay = false;
        m_bEnded = true;
    }
    
    bool    IsStopped()
    {
        return m_bEnded;
    }
    
    void    Update( float dt );
    void    Reset();
    
public:
    void    SetEndCallback( void (*callback)() ) { m_endCallback = callback; }
    
public:
    void    SetPlaybackSpeed( float speed ) { m_fPlaybackSpeed = speed; }
    float   GetPlaybackSpeed() const { return m_fPlaybackSpeed; }
    
    void    SetDuration( float duration ) { m_fDuration = duration; }
    float   GetDuration() const { return m_fDuration; }
    
    void    SetLoop( bool loop ) { m_bLoop = loop; }
    bool    GetLoop() const { return m_bLoop; }
    
protected:
    void    _SetTimeRatio( float time );
    
private:
    float   m_fTimeRatio{ 0.0f };
    float   m_fPreviousTimeRatio{ 0.0f };
    float   m_fPlaybackSpeed{ 1.0f };
    float   m_fDuration{ 0.0f };
    bool    m_bPlay{ false };
    bool    m_bLoop{ true };
    bool    m_bEnded{ false };
    
    void    (*m_endCallback)(){ nullptr };
};

SXB_NAMESPACE_END

#endif // _PLAYBACKCONTROLLER_H_3B8EA5267842E2BF46BFFFBBE20F55CA_
