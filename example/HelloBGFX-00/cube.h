
#ifndef _CUBE_H_2AA8FDAF22E7C21931959563A005530C
#define _CUBE_H_2AA8FDAF22E7C21931959563A005530C

#include <sxbCommon/Header.h>

struct PosColorVertex
{
	float x;
	float y;
	float z;
	uint32_t abgr;
};

static PosColorVertex cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
	0, 1, 2,
	1, 3, 2,
	4, 6, 5,
	5, 6, 7,
	0, 2, 4,
	4, 2, 6,
	1, 5, 3,
	5, 7, 3,
	0, 4, 1,
	4, 5, 1,
	2, 3, 6,
	6, 3, 7,
};

SXB_ENTRY_BEGIN

bgfx::ProgramHandle         m_program;
bgfx::VertexBufferHandle    m_vbh;
bgfx::IndexBufferHandle     m_ibh;
double                      m_residentMem;
double                      m_virtualMem;

SXB_ENTRY_END

#endif // _CUBE_H_2AA8FDAF22E7C21931959563A005530C
