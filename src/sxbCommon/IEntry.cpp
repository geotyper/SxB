
#include <SFML/Config.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <SFML/Window/Event.hpp>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bgfx/examples/common/bgfx_utils.h>

#include <bx/timer.h>

#include "sxbCommon/IEntry.h"

SXB_NAMESPACE_BEGIN

IEntry::IEntry()
    : m_Window(new sf::WindowBase(sf::VideoMode(SXB_DEFAULT_WIDTH, SXB_DEFAULT_HEIGHT), "SFML window"))
    , m_ready(true)
{
    m_width = SXB_DEFAULT_WIDTH;
    m_height = SXB_DEFAULT_HEIGHT;
    m_debug = BGFX_DEBUG_TEXT;
    m_reset  = 0
    | BGFX_RESET_VSYNC
    | BGFX_RESET_MSAA_X16
    ;
    
    bgfx::PlatformData pd;
    pd.nwh = m_Window->getSystemHandle();
    bgfx::setPlatformData(pd);
    
    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
    bgfxInit.resolution.width = m_width;
    bgfxInit.resolution.height = m_height;
    bgfxInit.resolution.reset = m_reset;
    bgfx::init(bgfxInit);
}

IEntry::~IEntry()
{
    delete m_Window;
}

void IEntry::Run()
{
    OnPreInit();
    
    bgfx::setDebug(m_debug);
    
    bgfx::setViewClear(0
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , m_rgba
                       , 1.0f
                       , 0
                       );
    
    OnInit();

    while (m_Window->isOpen())
    {
        sf::Event event;
        while (m_Window->pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
               m_Window->close();
            m_Cursor.handleEvent(event);
        }
        
        int64_t now = bx::getHPCounter();
        
        static int64_t last = now;
        const int64_t frameTime = now - last;
        last = now;
        const double freq = double(bx::getHPFrequency() );
        m_dt = float(double(frameTime)/freq);
        
        m_camera.update(m_Cursor, m_dt);
        
        OnUpdate();
        
        bgfx::frame();
    }
}

SXB_NAMESPACE_END
