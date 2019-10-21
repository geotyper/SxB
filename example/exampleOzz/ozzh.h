
#ifndef _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_
#define _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_

#include <sxbCommon/Header.h>

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



SXB_ENTRY_BEGIN

bool _InitOzz();
bool _UpdateOzz(float dt);

ozz::animation::Skeleton m_skeleton;
ozz::animation::Animation m_animation;
ozz::animation::SamplingCache m_cache;
ozz::Vector<ozz::math::SoaTransform>::Std m_locals;
ozz::Vector<ozz::math::Float4x4>::Std m_models;

bgfx::TextureHandle m_tex;
bgfx::TextureHandle m_texIrr;

bgfx::UniformHandle s_texCube;
bgfx::UniformHandle s_texCubeIrr;

bgfx::UniformHandle m_texColor;
bgfx::TextureHandle m_fieldstoneTex;

bgfx::ProgramHandle m_program;
bgfx::ProgramHandle m_program_cube;
bgfx::ProgramHandle m_program_TextureLighting;
bgfx::ProgramHandle m_programMesh;

sxb::Mesh           m_orbMesh;
sxb::Mesh           m_bunnyMesh;
sxb::Mesh           m_columnMesh;
sxb::Mesh           m_floorMesh;
sxb::Mesh           m_coordMesh;

Uniforms            m_uniforms;
Settings            m_settings;

sxb::MeshState      m_floorState;

bx::Vec3            m_bunnyScale{1.0f, 1.0f, 1.0f};
bx::Vec3            m_bunnyRotation{0.0f, bx::kPi, 0.0f};
bx::Vec3            m_bunnyTranslate{0.0f, -0.8f, 0.0f};

SXB_ENTRY_END

#endif // _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_
