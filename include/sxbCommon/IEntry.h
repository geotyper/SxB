
#ifndef _ENTRY_H_CCC92BD5A3C6886C389B0525AEBCB8E6_
#define _ENTRY_H_CCC92BD5A3C6886C389B0525AEBCB8E6_

#include "sxbCommon/defines.h"
#include "sxbCommon/Cursor.h"
#include "sxbCommon/Camera.h"

namespace sf {
    class WindowBase;
}

#define SXB_ENTRY_MAIN \
int main(int argc, char *argv[]) { \
Entry entry; \
entry.Run(); \
return 0; } \

#define SXB_ENTRY_BEGIN \
class Entry : public sxb::IEntry {\
public: \
void OnPreInit(); \
void OnInit(); \
void OnUpdate();

#define SXB_ENTRY_END };

SXB_NAMESPACE_BEGIN

class IEntry
{
public:
    IEntry();
    ~IEntry();
    
    void Run();
    
protected:
    virtual void OnPreInit() = 0;
    virtual void OnInit() = 0;
    virtual void OnUpdate() = 0;
    
protected:
    sf::WindowBase *    m_Window;
    Camera              m_camera;
    Cursor              m_Cursor;
    
    bool                m_ready;
    
    uint32_t            m_width;
    uint32_t            m_height;
    uint32_t            m_debug;
    uint32_t            m_reset;
    
    float               m_dt;
    
    uint32_t            m_rgba{ 0x303030ff };
};

SXB_NAMESPACE_END

#endif // _ENTRY_H_CCC92BD5A3C6886C389B0525AEBCB8E6_
