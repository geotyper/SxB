
#include <sxbCommon/Header.h>

#include "logo.h"

SXB_ENTRY_BEGIN

int  m_count{ 0 };

int     m_x;
int     m_y;

double m_residentMem{ 0 };
double m_virtualMem{ 0 };

SXB_ENTRY_END

void Entry::OnPreInit()
{
    m_rgba = 0x443355FF;
}

void Entry::OnInit()
{
    bgfx::setViewRect(0, 0, 0, m_width, m_height);
}

void Entry::OnUpdate()
{
    sxb::Utils::getMem(m_residentMem, m_virtualMem);
    
    m_x = m_Window->getSize().x;
    m_y = m_Window->getSize().y;
    
    bgfx::touch(0);
    
    // Use debug font to print information about this example.
    bgfx::dbgTextClear();
    bgfx::dbgTextImage(
                       bx::max<uint16_t>(uint16_t(SXB_DEFAULT_WIDTH / 2 / 8), 20) - 20
                       , bx::max<uint16_t>(uint16_t(SXB_DEFAULT_HEIGHT / 2 / 16), 6) - 6
                       , 40
                       , 12
                       , s_logo
                       , 160
                       );
    bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
    
    bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
    bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
    
    bgfx::dbgTextPrintf(0, 5, 0x0f, "                                    ");
    bgfx::dbgTextPrintf(0, 7, 0x0f, "                                    ");
    bgfx::dbgTextPrintf(0, 9, 0x0f, "                                    ");
    
    bgfx::dbgTextPrintf(0, 5, 0x0f, "%d", m_count);
    bgfx::dbgTextPrintf(0, 7, 0x0f, "resize: (%d, %d)", m_x, m_y);
    bgfx::dbgTextPrintf(0, 9, 0x0f, "mem(resident,virtual): (%.3fm, %.3fm)", m_residentMem, m_virtualMem);
    
    for ( int i = 0; i < 5; ++i )
    {
        bgfx::dbgTextPrintf(0, 11 + i, 0x0f, "                                    ");
        if (m_Cursor.touch[i].press)
        {
            bgfx::dbgTextPrintf(0, 11 + i, 0x0f, "\x1b[14;mtouch%d: (%d, %d)\x1b[0m", i, m_Cursor.touch[i].x, m_Cursor.touch[i].y);
        }
        else
        {
            bgfx::dbgTextPrintf(0, 11 + i, 0x0f, "touch%d: (%d, %d)", i, m_Cursor.touch[i].x, m_Cursor.touch[i].y);
        }
    }
    
    bgfx::dbgTextPrintf(0, 20, 0x0f, "\x1b[14;mouse: (%d, %d)\x1b[0m", m_Cursor.mouse[sf::Mouse::Button::Left].x, m_Cursor.mouse[sf::Mouse::Button::Left].y);
    
    const bgfx::Stats* stats = bgfx::getStats();
    bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters."
                        , stats->width
                        , stats->height
                        , stats->textWidth
                        , stats->textHeight
                        );
    m_count++;
}

SXB_ENTRY_MAIN
