
#ifndef _DD_H_99F11526BB24353FA8123FC0898C517A_
#define _DD_H_99F11526BB24353FA8123FC0898C517A_

#include "hh.h"

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

#pragma mark ozz
// Skeleton archive can be specified as an option.
OZZ_OPTIONS_DECLARE_STRING(skeleton,
                           "Path to the skeleton (ozz archive format).",
                           "media/skeleton.ozz", false)

// Animation archive can be specified as an option.
OZZ_OPTIONS_DECLARE_STRING(animation,
                           "Path to the animation (ozz archive format).",
                           "media/animation.ozz", false)

#endif // _DD_H_99F11526BB24353FA8123FC0898C517A_
