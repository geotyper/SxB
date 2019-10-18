
#include "cube.h"

void Entry::OnPreInit()
{
    m_LookAtViewId.push_back(0);
}

void Entry::OnInit()
{
    bgfx::setViewRect(0, 0, 0, SXB_DEFAULT_WIDTH, SXB_DEFAULT_HEIGHT);
    
    bgfx::VertexLayout pcvDecl;
    pcvDecl.begin()
    .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    .end();
    m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), pcvDecl);
    m_ibh = bgfx::createIndexBuffer(bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
    
    m_ready = sxb::Utils::loadProgram("vs_cubes.bin", "fs_cubes.bin", m_program);
}

void Entry::OnUpdate()
{
    bgfx::touch(0);
    sxb::Utils::getMem(m_residentMem, m_virtualMem);
    
    bgfx::dbgTextPrintf(0, 5, 0x0f, "                                    ");
    bgfx::dbgTextPrintf(0, 7, 0x0f, "                                    ");
    bgfx::dbgTextPrintf(0, 5, 0x0f, "%d", m_count);
    bgfx::dbgTextPrintf(0, 7, 0x0f, "mem(resident,virtual): (%.3fm, %.3fm)", m_residentMem, m_virtualMem);
    
    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);
    
    float mtx[16];
    bx::mtxRotateXY(mtx, m_count * 0.01f, m_count * 0.01f);
    bgfx::setTransform(mtx);
    
    bgfx::submit(0, m_program);
}

void Entry::OnEnd()
{
    bgfx::destroy(m_program);
    
    bgfx::destroy(m_vbh);
    bgfx::destroy(m_ibh);
}

SXB_ENTRY_MAIN
