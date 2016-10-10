// @date 2016/08/03
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <graphics/graphics.hpp>
#include <graphics/private/opengl.hpp>

NS_LEMON_GRAPHICS_BEGIN

struct IndexBufferGL : public IndexBuffer
{
    IndexBufferGL(Renderer& renderer) : IndexBuffer(renderer) {}

    bool initialize(const void*, unsigned, IndexElementFormat, MemoryUsage);
    void dispose();

    bool update_data(const void*);
    bool update_data(const void*, unsigned, unsigned, bool discard = false);

    GLuint get_handle() const { return _object; }
    IndexElementFormat get_element_format() const { return _format; }
    unsigned get_size() const { return _size; }

protected:
    unsigned _size;
    GLuint _object;
    GLenum _usage;
    IndexElementFormat _format;
};

NS_LEMON_GRAPHICS_END
