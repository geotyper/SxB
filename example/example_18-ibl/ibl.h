
#ifndef IBL_H_426AA2232FB4B477BD71333A51418F16
#define IBL_H_426AA2232FB4B477BD71333A51418F16

#include <iostream>

#include <stdint.h>  // int32_t

#include <bgfx/bgfx.h>
#include <bgfx/examples/common/bgfx_utils.h>

#include <bx/timer.h>
#include <bx/math.h>

#include <tinystl/allocator.h>
#include <tinystl/vector.h>
#include <tinystl/string.h>
namespace stl = tinystl;

#include <bgfx/bgfx.h>
#include <bx/commandline.h>
#include <bx/endian.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <bx/string.h>

#include <bimg/decode.h>

#include <string>

#include <bx/macros.h>
#include <bx/uint32_t.h>

#include <bimg/decode.h>

#include <bx/allocator.h>
#include <bx/readerwriter.h>
#include <bx/file.h>

#include <bimg/bimg.h>

#include <sxbCommon/defines.h>
#include <sxbCommon/Mesh.h>
#include <sxbCommon/utils.h>
#include <sxbCommon/Camera.h>

#include <SFML/Window/Event.hpp>

#pragma mark -

static float s_texelHalf = 0.0f;

struct Uniforms
{
    enum { NumVec4 = 12 };
    
    void init()
    {
        u_params = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, NumVec4);
    }
    
    void submit()
    {
        bgfx::setUniform(u_params, m_params, NumVec4);
    }
    
    void destroy()
    {
        bgfx::destroy(u_params);
    }
    
    union
    {
        struct
        {
            union
            {
                float m_mtx[16];
                /* 0*/ struct { float m_mtx0[4]; };
                /* 1*/ struct { float m_mtx1[4]; };
                /* 2*/ struct { float m_mtx2[4]; };
                /* 3*/ struct { float m_mtx3[4]; };
            };
            /* 4*/ struct { float m_glossiness, m_reflectivity, m_exposure, m_bgType; };
            /* 5*/ struct { float m_metalOrSpec, m_unused5[3]; };
            /* 6*/ struct { float m_doDiffuse, m_doSpecular, m_doDiffuseIbl, m_doSpecularIbl; };
            /* 7*/ struct { float m_cameraPos[3], m_unused7[1]; };
            /* 8*/ struct { float m_rgbDiff[4]; };
            /* 9*/ struct { float m_rgbSpec[4]; };
            /*10*/ struct { float m_lightDir[3], m_unused10[1]; };
            /*11*/ struct { float m_lightCol[3], m_unused11[1]; };
        };
        
        float m_params[NumVec4*4];
    };
    
    bgfx::UniformHandle u_params;
};

struct LightProbe
{
    enum Enum
    {
        Bolonga,
        Kyoto,
        
        Count
    };
    
    void load(const char* _name)
    {
        char filePath[512];
        
        bx::snprintf(filePath, BX_COUNTOF(filePath), "textures/%s_lod.dds", _name);
        
        m_tex = sxb::Utils::loadTexture(filePath, BGFX_SAMPLER_U_CLAMP|BGFX_SAMPLER_V_CLAMP|BGFX_SAMPLER_W_CLAMP);
        
        bx::snprintf(filePath, BX_COUNTOF(filePath), "textures/%s_irr.dds", _name);
        m_texIrr = sxb::Utils::loadTexture(filePath, BGFX_SAMPLER_U_CLAMP|BGFX_SAMPLER_V_CLAMP|BGFX_SAMPLER_W_CLAMP);
    }
    
    void destroy()
    {
        bgfx::destroy(m_tex);
        bgfx::destroy(m_texIrr);
    }
    
    bgfx::TextureHandle m_tex;
    bgfx::TextureHandle m_texIrr;
};

struct Mouse
{
    Mouse()
    : m_dx(0.0f)
    , m_dy(0.0f)
    , m_prevMx(0.0f)
    , m_prevMy(0.0f)
    , m_scroll(0)
    , m_scrollPrev(0)
    {
    }
    
    void update(float _mx, float _my, int32_t _mz, uint32_t _width, uint32_t _height)
    {
        const float widthf  = float(int32_t(_width));
        const float heightf = float(int32_t(_height));
        
        // Delta movement.
        m_dx = float(_mx - m_prevMx)/widthf;
        m_dy = float(_my - m_prevMy)/heightf;
        
        m_prevMx = _mx;
        m_prevMy = _my;
        
        // Scroll.
        m_scroll = _mz - m_scrollPrev;
        m_scrollPrev = _mz;
    }
    
    float m_dx; // Screen space.
    float m_dy;
    float m_prevMx;
    float m_prevMy;
    int32_t m_scroll;
    int32_t m_scrollPrev;
};

struct Settings
{
    Settings()
    {
        m_envRotCurr = 0.0f;
        m_envRotDest = 0.0f;
        m_lightDir[0] = -0.8f;
        m_lightDir[1] = 0.2f;
        m_lightDir[2] = -0.5f;
        m_lightCol[0] = 1.0f;
        m_lightCol[1] = 1.0f;
        m_lightCol[2] = 1.0f;
        m_glossiness = 0.7f;
        m_exposure = 0.0f;
        m_bgType = 3.0f;
        m_radianceSlider = 2.0f;
        m_reflectivity = 0.85f;
        m_rgbDiff[0] = 1.0f;
        m_rgbDiff[1] = 1.0f;
        m_rgbDiff[2] = 1.0f;
        m_rgbSpec[0] = 1.0f;
        m_rgbSpec[1] = 1.0f;
        m_rgbSpec[2] = 1.0f;
        m_lod = 0.0f;
        m_doDiffuse = false;
        m_doSpecular = false;
        m_doDiffuseIbl = true;
        m_doSpecularIbl = true;
        m_showLightColorWheel = true;
        m_showDiffColorWheel = true;
        m_showSpecColorWheel = true;
        m_metalOrSpec = 0;
        m_meshSelection = 0;
    }
    
    float m_envRotCurr;
    float m_envRotDest;
    float m_lightDir[3];
    float m_lightCol[3];
    float m_glossiness;
    float m_exposure;
    float m_radianceSlider;
    float m_bgType;
    float m_reflectivity;
    float m_rgbDiff[3];
    float m_rgbSpec[3];
    float m_lod;
    bool  m_doDiffuse;
    bool  m_doSpecular;
    bool  m_doDiffuseIbl;
    bool  m_doSpecularIbl;
    bool  m_showLightColorWheel;
    bool  m_showDiffColorWheel;
    bool  m_showSpecColorWheel;
    int32_t m_metalOrSpec;
    int32_t m_meshSelection;
};

#pragma mark  -

#define maxFinger   3

struct m2
{
    bool press { false };
    int x { 0 };
    int y { 0 };
    int deltaX { 0 };
    int deltaY { 0 };
};

#pragma mark -

class ibl
{
public:
	ibl()
    : m_ready(false)
    {}
    
	~ibl()
	{
		bgfx::shutdown();
	}

public:
	bool init(void* nwh_);

	void update(const uint64_t & frame_ = 0);
    
    void touchBegin(const int &finger_, const int &x_, const int &y_)
    {
        if ( finger_ < maxFinger)
        {
            m_touch[finger_].press = true;
            m_touch[finger_].x = x_;
            m_touch[finger_].y = y_;
        }
    }
    
    void touchMove(const int &finger_, const int &x_, const int &y_)
    {
        if ( finger_ < maxFinger)
        {
            m_touch[finger_].press = true;
            m_touch[finger_].deltaX = x_ - m_touch[finger_].x;
            m_touch[finger_].deltaY = y_ - m_touch[finger_].y;
            m_touch[finger_].x = x_;
            m_touch[finger_].y = y_;
        }
    }
    
    void touchEnd(const int &finger_, const int &x_, const int &y_)
    {
        if ( finger_ < maxFinger)
        {
            m_touch[finger_].press = false;
            m_touch[finger_].deltaX = 0;
            m_touch[finger_].deltaY = 0;
            m_touch[finger_].x = 0;
            m_touch[finger_].y = 0;
        }
    }
    
    void mouseDown(const sf::Mouse::Button &type_, const int &x_, const int &y_)
    {
        if ( type_ < sf::Mouse::Button::ButtonCount )
        {
            m_cursor[type_].press = true;
            m_cursor[type_].x = x_;
            m_cursor[type_].y = y_;
        }
    }
    
    void mouseUp(const sf::Mouse::Button &type_, const int &x_, const int &y_)
    {
        if ( type_ < sf::Mouse::Button::ButtonCount )
        {
            m_cursor[type_].press = false;
            m_cursor[type_].deltaX = 0;
            m_cursor[type_].deltaY = 0;
            m_cursor[type_].x = 0;
            m_cursor[type_].y = 0;
        }
    }
    
    void mouseMove(const int &x_, const int &y_)
    {
        if (m_cursor[sf::Mouse::Button::Left].press)
        {
            m_cursor[sf::Mouse::Button::Left].deltaX = x_ - m_cursor[sf::Mouse::Button::Left].x;
            m_cursor[sf::Mouse::Button::Left].deltaY = y_ - m_cursor[sf::Mouse::Button::Left].y;
            m_cursor[sf::Mouse::Button::Left].x = x_;
            m_cursor[sf::Mouse::Button::Left].y = y_;
        }
        else if (m_cursor[sf::Mouse::Button::Right].press)
        {
            m_cursor[sf::Mouse::Button::Right].deltaX = x_ - m_cursor[sf::Mouse::Button::Right].x;
            m_cursor[sf::Mouse::Button::Right].deltaY = y_ - m_cursor[sf::Mouse::Button::Right].y;
            m_cursor[sf::Mouse::Button::Right].x = x_;
            m_cursor[sf::Mouse::Button::Right].y = y_;
        }
        else if (m_cursor[sf::Mouse::Button::Middle].press)
        {
            m_cursor[sf::Mouse::Button::Middle].deltaX = x_ - m_cursor[sf::Mouse::Button::Middle].x;
            m_cursor[sf::Mouse::Button::Middle].deltaY = y_ - m_cursor[sf::Mouse::Button::Middle].y;
            m_cursor[sf::Mouse::Button::Middle].x = x_;
            m_cursor[sf::Mouse::Button::Middle].y = y_;
        }
    }
    
private:
	bool m_ready;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
    
    m2      m_touch[maxFinger];
    m2      m_cursor[sf::Mouse::Button::ButtonCount];

    Uniforms m_uniforms;
    
    LightProbe m_lightProbes[LightProbe::Count];
    LightProbe::Enum m_currentLightProbe;
    
    bgfx::UniformHandle u_mtx;
    bgfx::UniformHandle u_params;
    bgfx::UniformHandle u_flags;
    bgfx::UniformHandle u_camPos;
    bgfx::UniformHandle s_texCube;
    bgfx::UniformHandle s_texCubeIrr;
    
    bgfx::ProgramHandle m_programMesh;
    bgfx::ProgramHandle m_programSky;
    
    sxb::Mesh           m_meshBunny;
    sxb::Mesh           m_meshOrb;
    sxb::Camera         m_camera;
    Mouse               m_mouse;
    
    Settings m_settings;
};


#endif // IBL_H_426AA2232FB4B477BD71333A51418F16
