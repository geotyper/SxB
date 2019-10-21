
#ifndef _IMGUI_H_5672A0FD6D3CADBEF229D18A0CFA2E5A_
#define _IMGUI_H_5672A0FD6D3CADBEF229D18A0CFA2E5A_

#include "sxbCommon/defines.h"

SXB_NAMESPACE_BEGIN

struct Cursor;
void imguiCreate(float _fontSize = 18.0f, bx::AllocatorI* _allocator = NULL);
void imguiDestroy();

void imguiBeginFrame(const Cursor & cursor, uint16_t _width, uint16_t _height, int _inputChar = -1, bgfx::ViewId _view = 255);
void imguiEndFrame();

namespace imgui
{
inline bool MouseOverArea()
{
    return false
    || ImGui::IsAnyItemHovered()
    || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)
    ;
}
}

SXB_NAMESPACE_END

#endif // _IMGUI_H_5672A0FD6D3CADBEF229D18A0CFA2E5A_
