
#include "ibl.h"

struct PosColorTexCoord0Vertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_rgba;
    float m_u;
    float m_v;
    
    static void init()
    {
        ms_decl
        .begin()
        .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .end();
    }
    
    static bgfx::VertexLayout ms_decl;
};

bgfx::VertexLayout PosColorTexCoord0Vertex::ms_decl;

void screenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft = false, float _width = 1.0f, float _height = 1.0f)
{
    if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Vertex::ms_decl) )
    {
        bgfx::TransientVertexBuffer vb;
        bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Vertex::ms_decl);
        PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)vb.data;
        
        const float zz = 0.0f;
        
        const float minx = -_width;
        const float maxx =  _width;
        const float miny = 0.0f;
        const float maxy = _height*2.0f;
        
        const float texelHalfW = s_texelHalf/_textureWidth;
        const float texelHalfH = s_texelHalf/_textureHeight;
        const float minu = -1.0f + texelHalfW;
        const float maxu =  1.0f + texelHalfW;
        
        float minv = texelHalfH;
        float maxv = 2.0f + texelHalfH;
        
        if (_originBottomLeft)
        {
            std::swap(minv, maxv);
            minv -= 1.0f;
            maxv -= 1.0f;
        }
        
        vertex[0].m_x = minx;
        vertex[0].m_y = miny;
        vertex[0].m_z = zz;
        vertex[0].m_rgba = 0xffffffff;
        vertex[0].m_u = minu;
        vertex[0].m_v = minv;
        
        vertex[1].m_x = maxx;
        vertex[1].m_y = miny;
        vertex[1].m_z = zz;
        vertex[1].m_rgba = 0xffffffff;
        vertex[1].m_u = maxu;
        vertex[1].m_v = minv;
        
        vertex[2].m_x = maxx;
        vertex[2].m_y = maxy;
        vertex[2].m_z = zz;
        vertex[2].m_rgba = 0xffffffff;
        vertex[2].m_u = maxu;
        vertex[2].m_v = maxv;
        
        bgfx::setVertexBuffer(0, &vb);
    }
}

void Entry::OnPreInit()
{
    m_rgba = 0x303030ff;
    m_OrbitCameraList.push_back(1);
}

void Entry::OnInit()
{
    // Uniforms.
    m_uniforms.init();
    
    // Vertex declarations.
    PosColorTexCoord0Vertex::init();
    
    m_lightProbes[LightProbe::Bolonga].load("bolonga");
    m_lightProbes[LightProbe::Kyoto  ].load("kyoto");
    m_currentLightProbe = LightProbe::Bolonga;
    
    u_mtx        = bgfx::createUniform("u_mtx",        bgfx::UniformType::Mat4);
    u_params     = bgfx::createUniform("u_params",     bgfx::UniformType::Vec4);
    u_flags      = bgfx::createUniform("u_flags",      bgfx::UniformType::Vec4);
    u_camPos     = bgfx::createUniform("u_camPos",     bgfx::UniformType::Vec4);
    s_texCube    = bgfx::createUniform("s_texCube",    bgfx::UniformType::Sampler);
    s_texCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler);
    
    m_ready &= sxb::Utils::loadProgram("vs_ibl_mesh.bin", "fs_ibl_mesh.bin", m_programMesh);
    m_ready &= sxb::Utils::loadProgram("vs_ibl_skybox.bin", "fs_ibl_skybox.bin", m_programSky);
    
    m_meshBunny.load("meshes/bunny.bin");
    m_meshOrb.load("meshes/orb.bin");
}

void Entry::OnGui()
{
    
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
    
    bx::memCopy(m_uniforms.m_cameraPos, &m_camera.m_pos.curr.x, 3*sizeof(float) );
    
    // View Transform 0.
    float view[16];
    bx::mtxIdentity(view);
    
    const bgfx::Caps* caps = bgfx::getCaps();
    
    float proj[16];
    bx::mtxOrtho(proj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0, caps->homogeneousDepth);
    bgfx::setViewTransform(0, view, proj);
    
    // View rect.
    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height) );
    
    // Env rotation.
    const float amount = bx::min(m_dt/0.12f, 1.0f);
    m_settings.m_envRotCurr = bx::lerp(m_settings.m_envRotCurr, m_settings.m_envRotDest, amount);
    //        m_settings.m_envRotCurr = 3.19999957;
    //        m_settings.m_envRotDest = 3.19999909;
    
    // Env mtx.
    float mtxEnvView[16];
    m_camera.envViewMtx(mtxEnvView);
    float mtxEnvRot[16];
    bx::mtxRotateY(mtxEnvRot, m_settings.m_envRotCurr);
    bx::mtxMul(m_uniforms.m_mtx, mtxEnvView, mtxEnvRot); // Used for Skybox.
    
    // Submit view 0.
    bgfx::setTexture(0, s_texCube, m_lightProbes[m_currentLightProbe].m_tex);
    bgfx::setTexture(1, s_texCubeIrr, m_lightProbes[m_currentLightProbe].m_texIrr);
    bgfx::setState(BGFX_STATE_WRITE_RGB|BGFX_STATE_WRITE_A);
    screenSpaceQuad( (float)m_width, (float)m_height, true);
    m_uniforms.submit();
    bgfx::submit(0, m_programSky);
    
    // Submit view 1.
    bx::memCopy(m_uniforms.m_mtx, mtxEnvRot, 16*sizeof(float)); // Used for IBL.
    if (0 == m_settings.m_meshSelection)
    {
        // Submit bunny.
        float mtx[16];
        bx::mtxSRT(mtx, 1.0f, 1.0f, 1.0f, 0.0f, bx::kPi, 0.0f, 0.0f, -0.80f, 0.0f);
        bgfx::setTexture(0, s_texCube,    m_lightProbes[m_currentLightProbe].m_tex);
        bgfx::setTexture(1, s_texCubeIrr, m_lightProbes[m_currentLightProbe].m_texIrr);
        m_uniforms.submit();
        m_meshBunny.submit(1, m_programMesh, mtx, BGFX_STATE_MASK);
        //            meshSubmit(m_meshBunny, 1, m_programMesh, mtx);
    }
    else
    {
        // Submit orbs.
        for (float yy = 0, yend = 5.0f; yy < yend; yy+=1.0f)
        {
            for (float xx = 0, xend = 5.0f; xx < xend; xx+=1.0f)
            {
                const float scale   =  1.2f;
                const float spacing =  2.2f;
                const float yAdj    = -0.8f;
                
                float mtx[16];
                bx::mtxSRT(mtx
                           , scale/xend
                           , scale/xend
                           , scale/xend
                           , 0.0f
                           , 0.0f
                           , 0.0f
                           , 0.0f      + (xx/xend)*spacing - (1.0f + (scale-1.0f)*0.5f - 1.0f/xend)
                           , yAdj/yend + (yy/yend)*spacing - (1.0f + (scale-1.0f)*0.5f - 1.0f/yend)
                           , 0.0f
                           );
                
                m_uniforms.m_glossiness   =        xx*(1.0f/xend);
                m_uniforms.m_reflectivity = (yend-yy)*(1.0f/yend);
                m_uniforms.m_metalOrSpec = 0.0f;
                m_uniforms.submit();
                
                bgfx::setTexture(0, s_texCube,    m_lightProbes[m_currentLightProbe].m_tex);
                bgfx::setTexture(1, s_texCubeIrr, m_lightProbes[m_currentLightProbe].m_texIrr);
                m_meshOrb.submit(1, m_programMesh, mtx, BGFX_STATE_MASK);
                //                    meshSubmit(m_meshOrb, 1, m_programMesh, mtx);
            }
        }
    }
}

void Entry::OnEnd()
{
    
}

SXB_ENTRY_MAIN
