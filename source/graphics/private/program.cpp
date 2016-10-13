// @date 2016/08/03
// @author Mao Jingkai(oammix@gmail.com)

#include <graphics/private/program.hpp>
#include <graphics/private/backend.hpp>

NS_LEMON_GRAPHICS_BEGIN

GLuint compile(GLenum type, const char* source)
{
    GLint status;
    
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        char buf[1024];
        GLint len;
        glGetShaderInfoLog(shader, 1024, &len, buf);

        LOGW("failed to compile, %s. source:\n %s\n", buf, source);
        glDeleteShader(shader);
        return 0;
    }

    CHECK_GL_ERROR();
    return shader;
}

bool ProgramGL::initialize(const char* vsraw, const char* psraw)
{
    ENSURE_NOT_RENDER_PHASE;

    if( vsraw == nullptr || psraw == nullptr )
    {
        LOGW("failed to restore shader without vertex/fragment shader.");
        return false;
    }

    dispose();

    _object = glCreateProgram();
    if( _object == 0 )
    {
        LOGW("failed to create program object.");
        return false;
    }

    auto vs = compile(GL_VERTEX_SHADER, vsraw);
    if( vs == 0 )
        return false;

    auto fs = compile(GL_FRAGMENT_SHADER, psraw);
    if( fs == 0 )
        return false;

    glAttachShader(_object, vs);
    glAttachShader(_object, fs);
    glLinkProgram(_object);

    glDetachShader(_object, fs);
    glDetachShader(_object, vs);
    glDeleteShader(fs);
    glDeleteShader(vs);

    GLint status;
    glGetProgramiv(_object, GL_LINK_STATUS, &status);
    if( status == 0 )
    {
        char buf[1024];
        GLint len;
        glGetProgramInfoLog(_object, 1024, &len, buf);
        LOGW("failed to link program: %s\n", buf);
        return false;
    }

    _vertex_shader = vsraw;
    _fragment_shader = psraw;
    CHECK_GL_ERROR();
    return true;
}

void ProgramGL::dispose()
{
    ENSURE_NOT_RENDER_PHASE;

    if( _object != 0 )
        glDeleteProgram(_object);

    _object = 0;
    _uniforms.clear();
    _attribute_names.clear();
}

GLint ProgramGL::get_uniform_location(const char* name)
{
    if( _object == 0 )
    {
        LOGW("failed to get uniform location without a decent handle.");
        return -1;
    }

    auto hash = math::StringHash(name);
    auto found = _uniforms.find(hash);
    if( found != _uniforms.end() )
        return found->second;

    auto location = glGetUniformLocation(_object, name);
    if( location == -1 )
        LOGW("failed to locate uniform %s of program %d.", name, _object);

    _uniforms.insert(std::make_pair(hash, location));
    return location;
}

GLint ProgramGL::get_attribute_location(const char* name)
{
    if( _object == 0 )
    {
        LOGW("failed to get attribute location without a decent handle.");
        return -1;
    }

    auto hash = math::StringHash(name);
    auto found = _attributes.find(hash);
    if( found != _attributes.end() )
        return found->second;

    auto location = glGetAttribLocation(_object, name);
    if( location == -1 )
        LOGW("failed to localte attribute %s of program %d.", name, _object);

    _attributes.insert(std::make_pair(hash, location));
    return location;
}

GLint ProgramGL::get_attribute_location(VertexAttribute::Enum va)
{
    auto it = _attribute_names.find(value(va));
    if( it != _attribute_names.end() )
    {
        auto location = _attributes.find(it->second);
        ENSURE(location != _attributes.end());
        return location->second;
    }

    // use default name to search attribute location
    return get_attribute_location(VertexAttribute::name(va));
}

bool ProgramGL::set_attribute_name(VertexAttribute::Enum va, const char* name)
{
    auto location = get_attribute_location(name);
    _attribute_names.insert(std::make_pair(value(va), math::StringHash(name)));
    return location != -1;
}

NS_LEMON_GRAPHICS_END
