// @date 2016/07/26
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <forwards.hpp>
#include <graphics/renderer.hpp>
#include <codebase/type/enumeration.hpp>
#include <math/rect.hpp>
#include <math/color.hpp>

struct SDL_Window;

NS_LEMON_GRAPHICS_BEGIN

enum class RasterizationMode : uint8_t
{
    POINT,
    LINE,
    FILL
};

// graphics device subsystem. manages the window device, renedering state and gpu resources
struct RendererBackend
{
    virtual ~RendererBackend() {}

    // restore OpenGL context and reinitialize state, requires an open window. returns true if successful
    bool initialize(SDL_Window*);
    // release OpenGL context and handle the device lost of GPU resources
    void dispose();

    // reset all the graphics state to default
    void reset_cached_state();

    // begin frame rendering. return true if device available and can reneder
    bool begin_frame();
    // end frame rendering and swap buffers
    void end_frame();
    // clear any or all of rendertarget, depth buffer and stencil buffer
    void clear(ClearOption, const math::Color& color = {0.f, 0.f, 0.f, 0.f}, float depth = 1.f, unsigned stencil = 0);

    // set current shader program
    void set_shader(unsigned);
    // set index buffer
    void set_index_buffer(unsigned);
    // set vertex buffer
    void set_vertex_buffer(unsigned);
    // set texture
    void set_texture(unsigned, unsigned, unsigned);

    // specify whether front- or back-facing polygons can be culled
    void set_cull_face(bool, CullFace);
    // define front- and back-facing polygons
    void set_front_face(FrontFaceOrder);
    // define the scissor box
    void set_scissor_test(bool, const math::Rect2i& scissor = {{0, 0}, {0, 0}});
    // set front and back function and reference value for stencil testing
    void set_stencil_test(bool, CompareEquation, unsigned reference, unsigned mask);
    // set front and back stencil write actions
    void set_stencil_write(StencilWriteEquation sfail, StencilWriteEquation dpfail, StencilWriteEquation dppass, unsigned mask);
    // specify the value used for depth buffer comparisons
    void set_depth_test(bool, CompareEquation);
    // enable or disable writing into the depth buffer with bias
    void set_depth_write(bool, float slope_scaled = 0.f, float constant = 0.f);
    // set blending mode
    void set_color_blend(bool, BlendEquation, BlendFactor, BlendFactor);
    // enable and disable writing of frame buffer color components
    void set_color_write(ColorMask);
    // set the viewport
    void set_viewport(const math::Rect2i&);

    // prepare for draw call. setup corresponding frame/vertex buffer object
    void prepare_draw();
    // draw non-indexed geometry
    void draw(PrimitiveType, unsigned start, unsigned count);
    // draw indexed geometry
    void draw_index(PrimitiveType, unsigned start, unsigned count);

    // check if we have valid window and OpenGL context
    bool is_device_lost() const;

protected:
    SDL_Window* _window = nullptr;
    void* _context = 0;
    int32_t _system_frame_object = 0;

    // render states
    RenderState _render_state;
    math::Rect2i _viewport;

    unsigned _bound_fbo;
    unsigned _bound_program = 0;
    unsigned _bound_vbo = 0;
    unsigned _bound_ibo = 0;
    unsigned _active_texunit = 0;
    unsigned _bound_texture = 0;
    unsigned _bound_textype = 0;
};

NS_LEMON_GRAPHICS_END