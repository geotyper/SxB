
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/timer.h>
#include <bx/allocator.h>

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>

#include <SFML/Window/Event.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "vs_ocornut_imgui.bin.h"
#include "fs_ocornut_imgui.bin.h"
#include "vs_imgui_image.bin.h"
#include "fs_imgui_image.bin.h"

#include "sxbCommon/Cursor.h"

#include "sxbCommon/imgui.h"

SXB_NAMESPACE_BEGIN

#define IMGUI_FLAGS_NONE        UINT8_C(0x00)
#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)

static const bgfx::EmbeddedShader s_embeddedShaders[] =
{
    BGFX_EMBEDDED_SHADER(vs_ocornut_imgui),
    BGFX_EMBEDDED_SHADER(fs_ocornut_imgui),
    BGFX_EMBEDDED_SHADER(vs_imgui_image),
    BGFX_EMBEDDED_SHADER(fs_imgui_image),
    
    BGFX_EMBEDDED_SHADER_END()
};

#pragma mark -
struct OcornutImguiContext
{
#pragma mark render
    void render(ImDrawData* _drawData)
    {
        const ImGuiIO& io = ::ImGui::GetIO();
        const float width  = io.DisplaySize.x;
        const float height = io.DisplaySize.y;
        
        bgfx::setViewName(m_viewId, "ImGui");
        bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);
        
        const bgfx::Caps* caps = bgfx::getCaps();
        {
            float ortho[16];
            bx::mtxOrtho(ortho, 0.0f, width, height, 0.0f, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
            bgfx::setViewTransform(m_viewId, NULL, ortho);
            bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width), uint16_t(height) );
        }
        
        // Render command lists
        for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii)
        {
            bgfx::TransientVertexBuffer tvb;
            bgfx::TransientIndexBuffer tib;
            
            const ImDrawList* drawList = _drawData->CmdLists[ii];
            uint32_t numVertices = (uint32_t)drawList->VtxBuffer.size();
            uint32_t numIndices  = (uint32_t)drawList->IdxBuffer.size();
            
            bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_layout);
            bgfx::allocTransientIndexBuffer(&tib, numIndices);
            
            ImDrawVert* verts = (ImDrawVert*)tvb.data;
            bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert) );
            
            ImDrawIdx* indices = (ImDrawIdx*)tib.data;
            bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx) );
            
            uint32_t offset = 0;
            for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
            {
                if (cmd->UserCallback)
                {
                    cmd->UserCallback(drawList, cmd);
                }
                else if (0 != cmd->ElemCount)
                {
                    uint64_t state = 0
                    | BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_A
                    | BGFX_STATE_MSAA
                    ;
                    
                    bgfx::TextureHandle th = m_texture;
                    bgfx::ProgramHandle program = m_program;
                    
                    if (NULL != cmd->TextureId)
                    {
                        union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
                        state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
                        ? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                        : BGFX_STATE_NONE
                        ;
                        th = texture.s.handle;
                        if (0 != texture.s.mip)
                        {
                            const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
                            bgfx::setUniform(u_imageLodEnabled, lodEnabled);
                            program = m_imageProgram;
                        }
                    }
                    else
                    {
                        state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
                    }
                    
                    const uint16_t xx = uint16_t(bx::max(cmd->ClipRect.x, 0.0f) );
                    const uint16_t yy = uint16_t(bx::max(cmd->ClipRect.y, 0.0f) );
                    bgfx::setScissor(xx, yy
                                     , uint16_t(bx::min(cmd->ClipRect.z, 65535.0f)-xx)
                                     , uint16_t(bx::min(cmd->ClipRect.w, 65535.0f)-yy)
                                     );
                    
                    bgfx::setState(state);
                    bgfx::setTexture(0, s_tex, th);
                    bgfx::setVertexBuffer(0, &tvb, 0, numVertices);
                    bgfx::setIndexBuffer(&tib, offset, cmd->ElemCount);
                    bgfx::submit(m_viewId, program);
                }
                
                offset += cmd->ElemCount;
            }
        }
    }
    
#pragma mark create
    void create(float _fontSize, bx::AllocatorI* _allocator)
    {
        m_allocator = _allocator;
        
        if (NULL == _allocator)
        {
            static bx::DefaultAllocator allocator;
            m_allocator = &allocator;
        }
        
        m_viewId = 255;
        m_last = bx::getHPCounter();
        
//        ::ImGui::SetAllocatorFunctions(memAlloc, memFree, NULL);
        
        m_imgui = ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO();
        
        io.DisplaySize = ImVec2(1280.0f, 720.0f);
        io.DeltaTime   = 1.0f / 60.0f;
        io.IniFilename = NULL;
        
        setupStyle(true);
        
#if USE_ENTRY
        io.KeyMap[ImGuiKey_Tab]        = (int)entry::Key::Tab;
        io.KeyMap[ImGuiKey_LeftArrow]  = (int)entry::Key::Left;
        io.KeyMap[ImGuiKey_RightArrow] = (int)entry::Key::Right;
        io.KeyMap[ImGuiKey_UpArrow]    = (int)entry::Key::Up;
        io.KeyMap[ImGuiKey_DownArrow]  = (int)entry::Key::Down;
        io.KeyMap[ImGuiKey_PageUp]     = (int)entry::Key::PageUp;
        io.KeyMap[ImGuiKey_PageDown]   = (int)entry::Key::PageDown;
        io.KeyMap[ImGuiKey_Home]       = (int)entry::Key::Home;
        io.KeyMap[ImGuiKey_End]        = (int)entry::Key::End;
        io.KeyMap[ImGuiKey_Insert]     = (int)entry::Key::Insert;
        io.KeyMap[ImGuiKey_Delete]     = (int)entry::Key::Delete;
        io.KeyMap[ImGuiKey_Backspace]  = (int)entry::Key::Backspace;
        io.KeyMap[ImGuiKey_Space]      = (int)entry::Key::Space;
        io.KeyMap[ImGuiKey_Enter]      = (int)entry::Key::Return;
        io.KeyMap[ImGuiKey_Escape]     = (int)entry::Key::Esc;
        io.KeyMap[ImGuiKey_A]          = (int)entry::Key::KeyA;
        io.KeyMap[ImGuiKey_C]          = (int)entry::Key::KeyC;
        io.KeyMap[ImGuiKey_V]          = (int)entry::Key::KeyV;
        io.KeyMap[ImGuiKey_X]          = (int)entry::Key::KeyX;
        io.KeyMap[ImGuiKey_Y]          = (int)entry::Key::KeyY;
        io.KeyMap[ImGuiKey_Z]          = (int)entry::Key::KeyZ;
        
        io.ConfigFlags |= 0
        | ImGuiConfigFlags_NavEnableGamepad
        | ImGuiConfigFlags_NavEnableKeyboard
        ;
        
        io.NavInputs[ImGuiNavInput_Activate]    = (int)entry::Key::GamepadA;
        io.NavInputs[ImGuiNavInput_Cancel]      = (int)entry::Key::GamepadB;
        //        io.NavInputs[ImGuiNavInput_Input]       = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_Menu]        = (int)entry::Key::;
        io.NavInputs[ImGuiNavInput_DpadLeft]    = (int)entry::Key::GamepadLeft;
        io.NavInputs[ImGuiNavInput_DpadRight]   = (int)entry::Key::GamepadRight;
        io.NavInputs[ImGuiNavInput_DpadUp]      = (int)entry::Key::GamepadUp;
        io.NavInputs[ImGuiNavInput_DpadDown]    = (int)entry::Key::GamepadDown;
        //        io.NavInputs[ImGuiNavInput_LStickLeft]  = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_LStickRight] = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_LStickUp]    = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_LStickDown]  = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_FocusPrev]   = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_FocusNext]   = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_TweakSlow]   = (int)entry::Key::;
        //        io.NavInputs[ImGuiNavInput_TweakFast]   = (int)entry::Key::;
#endif // USE_ENTRY
        
        bgfx::RendererType::Enum type = bgfx::getRendererType();
        m_program = bgfx::createProgram(
                                        bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_ocornut_imgui")
                                        , bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_ocornut_imgui")
                                        , true
                                        );
        
        u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
        m_imageProgram = bgfx::createProgram(
                                             bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_imgui_image")
                                             , bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_imgui_image")
                                             , true
                                             );
        
        m_layout
        .begin()
        .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
        .end();
        
        s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);
        
        uint8_t* data;
        int32_t width;
        int32_t height;
        {
            ImFontConfig config;
            config.FontDataOwnedByAtlas = false;
            config.MergeMode = false;
            //            config.MergeGlyphCenterV = true;
            
            const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
//            m_font[ImGui::Font::Regular] = io.Fonts->AddFontFromMemoryTTF( (void*)s_robotoRegularTtf,     sizeof(s_robotoRegularTtf),     _fontSize,      &config, ranges);
//            m_font[ImGui::Font::Mono   ] = io.Fonts->AddFontFromMemoryTTF( (void*)s_robotoMonoRegularTtf, sizeof(s_robotoMonoRegularTtf), _fontSize-3.0f, &config, ranges);
            
            config.MergeMode = true;
//            config.DstFont   = m_font[ImGui::Font::Regular];
            
//            for (uint32_t ii = 0; ii < BX_COUNTOF(s_fontRangeMerge); ++ii)
//            {
//                const FontRangeMerge& frm = s_fontRangeMerge[ii];
//
//                io.Fonts->AddFontFromMemoryTTF( (void*)frm.data
//                                               , (int)frm.size
//                                               , _fontSize-3.0f
//                                               , &config
//                                               , frm.ranges
//                                               );
//            }
        }
        
        io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);
        
        m_texture = bgfx::createTexture2D(
                                          (uint16_t)width
                                          , (uint16_t)height
                                          , false
                                          , 1
                                          , bgfx::TextureFormat::BGRA8
                                          , 0
                                          , bgfx::copy(data, width*height*4)
                                          );
        
//        ImGui::InitDockContext();
    }
    
#pragma mark destroy
    void destroy()
    {
//        ImGui::ShutdownDockContext();
        ImGui::DestroyContext(m_imgui);
        
        bgfx::destroy(s_tex);
        bgfx::destroy(m_texture);
        
        bgfx::destroy(u_imageLodEnabled);
        bgfx::destroy(m_imageProgram);
        bgfx::destroy(m_program);
        
        m_allocator = NULL;
    }
    
#pragma mark setupStyle
    void setupStyle(bool _dark)
    {
        // Doug Binks' darl color scheme
        // https://gist.github.com/dougbinks/8089b4bbaccaaf6fa204236978d165a9
        ImGuiStyle& style = ImGui::GetStyle();
        if (_dark)
        {
            ImGui::StyleColorsDark(&style);
        }
        else
        {
            ImGui::StyleColorsLight(&style);
        }
        
        style.FrameRounding    = 4.0f;
        style.WindowBorderSize = 0.0f;
    }
    
#pragma mark beginFrame
    void beginFrame(
                    const Cursor & cursor
                    , int _width
                    , int _height
                    , int _inputChar
                    , bgfx::ViewId _viewId
                    )
    {
        m_viewId = _viewId;
        
        ImGuiIO& io = ImGui::GetIO();
        if (_inputChar >= 0)
        {
            io.AddInputCharacter(_inputChar);
        }
        
        io.DisplaySize = ImVec2( (float)_width, (float)_height);
        
        const int64_t now = bx::getHPCounter();
        const int64_t frameTime = now - m_last;
        m_last = now;
        const double freq = double(bx::getHPFrequency() );
        io.DeltaTime = float(frameTime/freq);
        
        io.MousePos = ImVec2( (float)cursor.mouse.x, (float)cursor.mouse.y);
        io.MouseDown[0] = cursor.button_type[sf::Mouse::Button::Left];
        io.MouseDown[1] = cursor.button_type[sf::Mouse::Button::Right];
        io.MouseDown[2] = cursor.button_type[sf::Mouse::Button::Middle];
        io.MouseWheel = (float)(cursor.wheelDelta);
        
#if USE_ENTRY
        uint8_t modifiers = inputGetModifiersState();
        io.KeyShift = 0 != (modifiers & (entry::Modifier::LeftShift | entry::Modifier::RightShift) );
        io.KeyCtrl  = 0 != (modifiers & (entry::Modifier::LeftCtrl  | entry::Modifier::RightCtrl ) );
        io.KeyAlt   = 0 != (modifiers & (entry::Modifier::LeftAlt   | entry::Modifier::RightAlt  ) );
        for (int32_t ii = 0; ii < (int32_t)entry::Key::Count; ++ii)
        {
            io.KeysDown[ii] = inputGetKeyState(entry::Key::Enum(ii) );
        }
#endif // USE_ENTRY
        
        ImGui::NewFrame();
    }
    
#pragma mark endFrame
    void endFrame()
    {
        ImGui::Render();
        render(ImGui::GetDrawData() );
    }
    
    ImGuiContext*       m_imgui;
    bx::AllocatorI*     m_allocator;
    bgfx::VertexLayout  m_layout;
    bgfx::ProgramHandle m_program;
    bgfx::ProgramHandle m_imageProgram;
    bgfx::TextureHandle m_texture;
    bgfx::UniformHandle s_tex;
    bgfx::UniformHandle u_imageLodEnabled;
//    ImFont* m_font[ImGui::Font::Count];
    int64_t m_last;
    bgfx::ViewId m_viewId;
};

static OcornutImguiContext s_ctx;

void imguiCreate(float _fontSize, bx::AllocatorI* _allocator)
{
    s_ctx.create(_fontSize, _allocator);
}

void imguiDestroy()
{
    s_ctx.destroy();
}

void imguiBeginFrame(const Cursor & cursor, uint16_t _width, uint16_t _height, int _inputChar, bgfx::ViewId _viewId)
{
    s_ctx.beginFrame(cursor, _width, _height, _inputChar, _viewId);
}

void imguiEndFrame()
{
    s_ctx.endFrame();
}

SXB_NAMESPACE_END
