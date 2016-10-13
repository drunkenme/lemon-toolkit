// @date 2016/10/09
// @author Mao Jingkai(oammix@gmail.com)

#include <graphics/private/vertex_array_cache.hpp>
#include <graphics/private/backend.hpp>
#include <graphics/private/program.hpp>
#include <graphics/private/vertex_buffer.hpp>
#include <graphics/private/uniform_buffer.hpp>

NS_LEMON_GRAPHICS_BEGIN

RenderStateCache::RenderStateCache(Renderer& renderer) : _renderer(renderer)
{}

void RenderStateCache::begin_frame()
{
    _active_program.invalidate();
    _active_uniforms.clear();
}

void RenderStateCache::bind_program(Handle program_handle)
{
    if( _active_program == program_handle )
        return;

    auto program = static_cast<ProgramGL*>(_renderer.get<Program>(program_handle));
    glUseProgram(program == nullptr ? 0 : program->get_handle());
    CHECK_GL_ERROR();
}

void RenderStateCache::bind_uniform_buffer(Handle program_handle, Handle uniform_handle)
{
    auto found = _active_uniforms.find(program_handle);
    if( found != _active_uniforms.end() && found->second == uniform_handle )
        return;

    auto program = static_cast<ProgramGL*>(_renderer.get<Program>(program_handle));
    auto uniform = static_cast<UniformBufferGL*>(_renderer.get<UniformBuffer>(uniform_handle));

    if( program == nullptr || uniform == nullptr )
        return;

    unsigned unit = 0;
    for( const auto& pair : uniform->get_textures() )
    {
        glActiveTexture(GL_TEXTURE0+unit);

        auto texture = static_cast<TextureGL*>(_renderer.get<Texture>(pair.second));
        glBindTexture(GL_TEXTURE_2D, texture == nullptr ? 0 : texture->get_handle());

        auto location = program->get_uniform_location(pair.first.c_str());
        if( location == -1 )
            continue;

        glUniform1i(location, unit++);
    }
    CHECK_GL_ERROR();

    for( const auto& pair : uniform->get_uniforms() )
    {
        auto location = program->get_uniform_location(pair.first.c_str());
        if( location == -1 )
            continue;

        if( pair.second.is<math::Vector<1, float>>() )
        {
            auto& v1 = pair.second.get<math::Vector<1, float>>();
            glUniform1f(location, v1[0]);
        }
        else if( pair.second.is<math::Vector<2, float>>() )
        {
            auto& v2 = pair.second.get<math::Vector<2, float>>();
            glUniform2f(location, v2[0], v2[1]);
        }
        else if( pair.second.is<math::Vector<3, float>>() )
        {
            auto& v3 = pair.second.get<math::Vector<3, float>>();
            glUniform3f(location, v3[0], v3[1], v3[2]);
        }
        else if( pair.second.is<math::Vector<4, float>>() )
        {
            auto& v4 = pair.second.get<math::Vector<4, float>>();
            glUniform4f(location, v4[0], v4[1], v4[2], v4[3]);
        }
        // OpenGL use column-major layout, so we always transpose our matrix
        else if( pair.second.is<math::Matrix<2, 2, float>>() )
        {
            auto& m2 = pair.second.get<math::Matrix<2, 2, float>>();
            glUniformMatrix2fv(location, 1, GL_TRUE, (float*)&m2);
        }
        else if( pair.second.is<math::Matrix<3, 3, float>>() )
        {
            auto& m3 = pair.second.get<math::Matrix<3, 3, float>>();
            glUniformMatrix3fv(location, 1, GL_TRUE, (float*)&m3);
        }
        else if( pair.second.is<math::Matrix<4, 4, float>>() )
        {
            auto& m4 = pair.second.get<math::Matrix<4, 4, float>>();
            glUniformMatrix4fv(location, 1, GL_TRUE, (float*)&m4);
        }
    }
    CHECK_GL_ERROR();

    _active_uniforms[program_handle] = uniform_handle;
}

static const unsigned GL_ELEMENT_FORMAT[] =
{
    GL_BYTE,
    GL_UNSIGNED_BYTE,
    GL_SHORT,
    GL_UNSIGNED_SHORT,
    GL_FIXED,
    GL_FLOAT
};

VertexArrayObjectCache::VertexArrayObjectCache()
{
#ifndef GL_ES_VERSION_2_0
    _vao_support = true;
#else
    _vao_support = false;
#endif
}

void VertexArrayObjectCache::bind(ProgramGL& program, VertexBufferGL& vb)
{
    GLuint glprogram = program.get_handle();
    GLuint glvb = vb.get_handle();

    if( glprogram == 0 || glvb == 0 )
    {
        LOGW("failed to bind vertex array object without decent program and vertex_buffer.");
        return;
    }

    if( _vao_support )
    {
        const uint64_t k = (uint64_t)glprogram | (uint64_t)glvb;

        auto found = _vaos.find(k);
        if( found != _vaos.end() )
        {
            glBindVertexArray(found->second);
            return;
        }

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        _vaos.insert(std::make_pair(k, vao));
    }

    glBindBuffer(GL_ARRAY_BUFFER, glvb);
    auto& layout = vb.get_layout();
    for( uint8_t i = 0; i < VertexAttribute::kVertexAttributeCount; i++ )
    {
        VertexAttribute::Enum va = (VertexAttribute::Enum)i;
        if( layout.has(va) )
        {
            auto localtion = program.get_attribute_location(va);
            if( localtion == -1 )
                continue;

            auto attribute = layout.get_attribute(va);
            glEnableVertexAttribArray(localtion);
            glVertexAttribPointer(
                /*index*/ localtion,
                /*size*/ attribute.size,
                /*type*/ GL_ELEMENT_FORMAT[value(attribute.format)],
                /*normalized*/ attribute.normalized,
                /*stride*/ layout.get_stride(),
                /*pointer*/ (uint8_t*)0+layout.get_offset(va));
        }
    }

    CHECK_GL_ERROR();
}

void VertexArrayObjectCache::unbind()
{
    if( !_vao_support )
        return;

    glBindVertexArray(0);
}

void VertexArrayObjectCache::free(ProgramGL& program)
{
    if( !_vao_support )
        return;

    const uint64_t k = ((uint64_t)program.get_handle()) << 32;
    for( auto it = _vaos.cbegin(); it != _vaos.cend(); )
    {
        if( (it->first & 0xFFFFFFFF00000000) == k )
        {
            glDeleteVertexArrays(1, &it->second);
            _vaos.erase(it++);
        }
        else
            ++it;
    }
    CHECK_GL_ERROR();
}

void VertexArrayObjectCache::free(VertexBufferGL& vb)
{
    if( !_vao_support )
        return;

    const uint64_t k = (uint64_t)vb.get_handle();
    for( auto it = _vaos.cbegin(); it != _vaos.cend(); )
    {
        if( (it->first & 0x00000000FFFFFFFF) == k )
        {
            glDeleteVertexArrays(1, &it->second);
            _vaos.erase(it++);
        }
        else
            ++it;
    }
    CHECK_GL_ERROR();
}

NS_LEMON_GRAPHICS_END
