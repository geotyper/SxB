
#ifndef _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_
#define _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_

#include "dd.h"

class ozzh
{
public:
    ozzh(){}
    ~ozzh(){}
    
public:
    bool init(void* nwh_);
    bool update(const sxb::Cursor & cursor);
    
public:
    bool _InitOzz();
    bool _UpdateOzz(float dt);
    
private:
    ozz::animation::Skeleton m_skeleton;
    ozz::animation::Animation m_animation;
    ozz::animation::SamplingCache m_cache;
    ozz::Vector<ozz::math::SoaTransform>::Std m_locals;
    ozz::Vector<ozz::math::Float4x4>::Std m_models;
    
private:
    bool m_ready;
    
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
    
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
    
    sxb::Camera         m_camera;
    
    Uniforms            m_uniforms;
    Settings            m_settings;
    
    sxb::MeshState      m_floorState;
};

#endif // _OZZH_H_2F87C1877DE740CE73F81CFCF58F03A1_
