
#include "ozzh.h"

static const uint64_t s_ptState[]
{
    UINT64_C(0),
    BGFX_STATE_PT_TRISTRIP,
    BGFX_STATE_PT_LINES,
    BGFX_STATE_PT_LINESTRIP,
    BGFX_STATE_PT_POINTS,
};

struct PosColorVertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;
    
    static void init()
    {
        ms_decl
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
    };
    
    static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosColorVertex::ms_decl;

static PosColorVertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff0000ff },    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff0000ff },
    { 1.0f, -1.0f,  1.0f, 0xff0000ff },
    {-1.0f,  1.0f, -1.0f, 0xff0000ff },
    { 1.0f,  1.0f, -1.0f, 0xff0000ff },
    {-1.0f, -1.0f, -1.0f, 0xff0000ff },
    { 1.0f, -1.0f, -1.0f, 0xff0000ff },
};

static PosColorVertex s_coorVertices[] =
{
    // x
    {1000.0f    ,  0.0f     ,  0.0f     , 0xff0000ff },
    {0.0f       ,  0.0f     ,  0.0f     , 0xff0000ff },
    {0.0f       ,  0.0f     ,  0.0f     , 0xffffffff },
    {-1000.0f   ,  0.0f     ,  0.0f     , 0xffffffff },
    // y
    {0.0f       ,  1000.0f  ,  0.0f     , 0xff00ff00 },
    {0.0f       ,  -0.0f    ,  0.0f     , 0xff00ff00 },
    {0.0f       ,  0.0f     ,  0.0f     , 0xffffffff },
    {0.0f       ,  -1000.0f ,  0.0f     , 0xffffffff },
    // z
    {0.0f       ,  0.0f     ,  1000.0f  , 0xffff0000 },
    {0.0f       ,  0.0f     ,  -0.0f    , 0xffff0000 },
    {0.0f       ,  0.0f     ,  0.0f     , 0xffffffff },
    {0.0f       ,  0.0f     ,  -1000.0f , 0xffffffff },
};

static const uint16_t s_coorList[] =
{
    0, 1,
    2, 3,
    4, 5,
    6, 7,
    8, 9,
    10, 11,
};

static const uint16_t s_cubeTriList[] =
{
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7,
};

static const uint16_t s_cubeLineList[] =
{
    0, 1,
    0, 2,
    0, 4,
    1, 3,
    1, 5,
    2, 3,
    2, 6,
    3, 7,
    4, 5,
    4, 6,
    5, 7,
    6, 7,
};

static const uint16_t s_cubePoints[] =
{
    0, 1, 2, 3, 4, 5, 6, 7
};

struct PosNormalTexcoordVertex
{
    float    m_x;
    float    m_y;
    float    m_z;
    uint32_t m_normal;
    float    m_u;
    float    m_v;
    
    static void init()
    {
        ms_decl
        .begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
    }
    
    static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosNormalTexcoordVertex::ms_decl;

static const float s_texcoord = 5.0f;
static PosNormalTexcoordVertex s_hplaneVertices[] =
{
    { -1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), s_texcoord, s_texcoord },
    {  1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), s_texcoord, 0.0f       },
    { -1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), 0.0f,       s_texcoord },
    {  1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), 0.0f,       0.0f       },
};

static const uint16_t s_planeIndices[] =
{
    0, 1, 2,
    1, 3, 2,
};

bool LoadSkeleton(const char* _filename, ozz::animation::Skeleton* _skeleton) {
    assert(_filename && _skeleton);
    ozz::log::Out() << "Loading skeleton archive " << _filename << "."
    << std::endl;
    ozz::io::File file(_filename, "rb");
    if (!file.opened()) {
        ozz::log::Err() << "Failed to open skeleton file " << _filename << "."
        << std::endl;
        return false;
    }
    ozz::io::IArchive archive(&file);
    if (!archive.TestTag<ozz::animation::Skeleton>()) {
        ozz::log::Err() << "Failed to load skeleton instance from file "
        << _filename << "." << std::endl;
        return false;
    }
    
    // Once the tag is validated, reading cannot fail.
    archive >> *_skeleton;
    
    return true;
}

bool LoadAnimation(const char* _filename,
                   ozz::animation::Animation* _animation) {
    assert(_filename && _animation);
    ozz::log::Out() << "Loading animation archive: " << _filename << "."
    << std::endl;
    ozz::io::File file(_filename, "rb");
    if (!file.opened()) {
        ozz::log::Err() << "Failed to open animation file " << _filename << "."
        << std::endl;
        return false;
    }
    ozz::io::IArchive archive(&file);
    if (!archive.TestTag<ozz::animation::Animation>()) {
        ozz::log::Err() << "Failed to load animation instance from file "
        << _filename << "." << std::endl;
        return false;
    }
    
    // Once the tag is validated, reading cannot fail.
    archive >> *_animation;
    
    return true;
}

void Entry::OnPreInit()
{
    m_rgba = 0x303030ff;
    m_LookAtViewId.push_back(0);
}

void Entry::OnInit()
{
    _InitOzz();

    m_uniforms.init();

    m_tex = sxb::Utils::loadTexture("textures/bolonga_lod.dds", BGFX_SAMPLER_U_CLAMP|BGFX_SAMPLER_V_CLAMP|BGFX_SAMPLER_W_CLAMP);
    m_texIrr = sxb::Utils::loadTexture("textures/bolonga_irr.dds", BGFX_SAMPLER_U_CLAMP|BGFX_SAMPLER_V_CLAMP|BGFX_SAMPLER_W_CLAMP);
    
    s_texCube    = bgfx::createUniform("s_texCube",    bgfx::UniformType::Sampler);
    s_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
    
    m_ready &= sxb::Utils::loadProgram("vs_ibl_mesh.bin", "fs_ibl_mesh.bin", m_programMesh);
    m_ready &= sxb::Utils::loadProgram("vs_stencil_color_lighting.bin", "fs_stencil_color_lighting.bin", m_program);
    m_ready &= sxb::Utils::loadProgram("vs_cubes.bin", "fs_cubes.bin", m_program_cube);
    m_ready &= sxb::Utils::loadProgram("vs_stencil_texture_lighting.bin", "fs_stencil_texture_lighting.bin", m_program_TextureLighting);
    
    
    bgfx::VertexLayout pcvDecl, flDecl;
    pcvDecl.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    flDecl.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .end();
    
    m_coordMesh.load(s_coorVertices, sizeof(s_coorVertices), pcvDecl, s_coorList, sizeof(s_coorList));
    m_floorMesh.load(s_hplaneVertices, sizeof(s_hplaneVertices), flDecl, s_planeIndices, sizeof(s_planeIndices));
    
    m_ready &= m_bunnyMesh.load("meshes/bunny.bin");
    m_ready &= m_orbMesh.load("meshes/orb.bin");
    m_ready &= m_columnMesh.load("meshes/column.bin");
    
    sxb::Utils::loadTexture("textures/fieldstone-rgba.dds", m_fieldstoneTex);
    
    m_texColor = bgfx::createUniform("m_texColor", bgfx::UniformType::Sampler);
    
    m_floorState.m_program = m_program_TextureLighting;
    m_floorState.m_state = BGFX_STATE_WRITE_RGB
    | BGFX_STATE_WRITE_Z
    | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_SRC_COLOR)
    | BGFX_STATE_DEPTH_TEST_LESS
    | BGFX_STATE_CULL_CCW
    | BGFX_STATE_MSAA;
    m_floorState.m_viewId = 0;
    m_floorState.m_numTextures = 1;
    m_floorState.m_textures[0].m_texture = m_fieldstoneTex;
    m_floorState.m_textures[0].m_sampler = m_texColor;
    m_floorState.m_textures[0].m_stage = 0;
    m_floorState.m_textures[0].m_flags = UINT32_MAX;
}

void Entry::OnUpdate()
{
    m_uniforms.m_glossiness   = m_settings.m_glossiness;
    m_uniforms.m_reflectivity = m_settings.m_reflectivity;
    m_uniforms.m_exposure     = m_settings.m_exposure;
    m_uniforms.m_bgType       = m_settings.m_bgType;
    m_uniforms.m_metalOrSpec   = float(m_settings.m_metalOrSpec);
    m_uniforms.m_doDiffuse     = float(m_settings.m_doDiffuse);
    m_uniforms.m_doSpecular    = float(m_settings.m_doSpecular);
    m_uniforms.m_doDiffuseIbl  = float(m_settings.m_doDiffuseIbl);
    m_uniforms.m_doSpecularIbl = float(m_settings.m_doSpecularIbl);
    bx::memCopy(m_uniforms.m_rgbDiff,  m_settings.m_rgbDiff,  3*sizeof(float) );
    bx::memCopy(m_uniforms.m_rgbSpec,  m_settings.m_rgbSpec,  3*sizeof(float) );
    bx::memCopy(m_uniforms.m_lightDir, m_settings.m_lightDir, 3*sizeof(float) );
    bx::memCopy(m_uniforms.m_lightCol, m_settings.m_lightCol, 3*sizeof(float) );
    
    float bunnyMtx[16];
    bx::mtxSRT(bunnyMtx
               , 1.0f
               , 1.0f
               , 1.0f
               , 0.0f
               , 1.56f
               , 0.0f
               , 0.0f
               , 2.0f
               , 0.0f
               );
    const float dist = 14.0f;
    const float columnPositions[4][3] =
    {
        {  dist, 0.0f,  dist },
        { -dist, 0.0f,  dist },
        {  dist, 0.0f, -dist },
        { -dist, 0.0f, -dist },
    };
    float columnMtx[4][16];
    for (uint8_t ii = 0; ii < 4; ++ii)
    {
        bx::mtxSRT(columnMtx[ii]
                   , 1.0f
                   , 1.0f
                   , 1.0f
                   , 0.0f
                   , 0.0f
                   , 0.0f
                   , columnPositions[ii][0]
                   , columnPositions[ii][1]
                   , columnPositions[ii][2]
                   );
    }
    float floorMtx[16];
    bx::mtxSRT(floorMtx
               , 20.0f  //scaleX
               , 20.0f  //scaleY
               , 20.0f  //scaleZ
               , 0.0f   //rotX
               , 0.0f   //rotY
               , 0.0f   //rotZ
               , 0.0f   //translateX
               , 0.0f   //translateY
               , 0.0f   //translateZ
               );

    _UpdateOzz(m_dt);
    
    // Submit bunny.
    float mtx[16];
    bx::mtxSRT(mtx, 1.0f, 1.0f, 1.0f, 0.0f, bx::kPi, 0.0f, 0.0f, -0.80f, 0.0f);
    bgfx::setTexture(0, s_texCube, m_tex);
    bgfx::setTexture(1, s_texCubeIrr, m_texIrr);
    m_uniforms.submit();
    
    m_bunnyMesh.submit(0, m_programMesh, mtx, BGFX_STATE_MASK);
    
    for (uint8_t ii = 0; ii < 4; ++ii)
    {
        m_columnMesh.submit(0, m_program, columnMtx[ii], BGFX_STATE_DEFAULT);
    }
    
    if ( !m_models.empty() )
    {
        int iii = 0;
        for ( int i = 0; i < 4; ++i )
        {
            mtx[(4 * i) + 0] = m_models[iii].cols[i][0];
            mtx[(4 * i) + 1] = m_models[iii].cols[i][1];
            mtx[(4 * i) + 2] = m_models[iii].cols[i][2];
            mtx[(4 * i) + 3] = m_models[iii].cols[i][3];
        }
    }
    m_orbMesh.submit(0, m_programMesh, mtx, BGFX_STATE_MASK);
    m_bunnyMesh.submit(0, m_program, bunnyMtx, BGFX_STATE_DEFAULT);
    m_coordMesh.submit(0, m_program_cube, NULL, BGFX_STATE_DEFAULT | BGFX_STATE_PT_LINES);
    
    sxb::MeshState ss[1];
    ss[0] = m_floorState;
    //        m_floorMesh.submit(ss, (uint8_t)1, floorMtx, 1);

}

bool Entry::_InitOzz()
{
    // Reading skeleton.
    if (!LoadSkeleton(OPTIONS_skeleton, &m_skeleton)) {
        return false;
    }
    
    // Reading animation.
    if (!LoadAnimation(OPTIONS_animation, &m_animation)) {
        return false;
    }
    // Allocates runtime buffers.
    const int num_joints = m_skeleton.num_joints();
    const int num_soa_joints = m_skeleton.num_soa_joints();
    m_locals.resize(num_soa_joints);
    m_models.resize(num_joints);
    
    // Allocates a cache that matches animation requirements.
    m_cache.Resize(num_joints);
    
    return true;
}

bool Entry::_UpdateOzz(float dt)
{
    // Samples optimized animation at t = animation_time_.
    ozz::animation::SamplingJob sampling_job;
    sampling_job.animation = &m_animation;
    sampling_job.cache = &m_cache;
    static float last_ratio = 0;
    float now_ratio = last_ratio + (dt / m_animation.duration());
     now_ratio = now_ratio - floorf(now_ratio);
    last_ratio = now_ratio;
    
    sampling_job.ratio = now_ratio;
    sampling_job.output = make_range(m_locals);
    if (!sampling_job.Run()) {
        return false;
    }
    
    // Converts from local space to model space matrices.
    ozz::animation::LocalToModelJob ltm_job;
    ltm_job.skeleton = &m_skeleton;
    ltm_job.input = make_range(m_locals);
    ltm_job.output = make_range(m_models);
    if (!ltm_job.Run()) {
        return false;
    }
    
    return true;
}

void Entry::OnEnd()
{
    
}

SXB_ENTRY_MAIN
