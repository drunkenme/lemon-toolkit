// @date 2016/07/26
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <defines.hpp>

#define NS_FLOW2D_GFX_BEGIN namespace flow2d { namespace graphics {
#define NS_FLOW2D_GFX_END } }

NS_FLOW2D_GFX_BEGIN

const static size_t kMaxVertexBuffers = 8;

using ResourceHandle = size_t;

enum class PrimitiveType : uint8_t
{
    TRIANGLE_LIST = 0,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    LINE_LIST,
    LINE_STRIP,
    POINT_LIST,
};

enum class Orientation : uint8_t
{
    LANDSCAPE_LEFT = 0,
    LANDSCAPE_RIGHT,
    PORTRAIT,
    PORTRAIT_UPSIDE_DOWN
};

enum class CompareMode : uint8_t
{
    ALWAYS = 0,
    EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
};

enum class StencilOp : uint8_t
{
    KEEP = 0,
    ZERO,
    REF,
    INCR,
    DECR
};

enum class CullMode : uint8_t
{
    NONE = 0,
    CCW,
    CW
};

enum class FillMode : uint8_t
{
    SOLID = 0,
    WIREFRAME,
    POINT
};

enum class BlendMode : uint8_t
{
    REPLACE = 0,
    ADD,
    MULTIPLY,
    ALPHA,
    ADDALPHA,
    PREMULALPHA,
    INVDESTALPHA,
    SUBTRACT,
    SUBTRACTALPHA
};

// vertex/index buffer lock state
enum class LockState
{
    NONE = 0,
    HARDWARE,
    SHADOW,
    SCRATCH
};

// arbitrary vertex declaration element datatypes
enum class VertexElementType
{
    INT,
    FLOAT,
    VECTOR2,
    VECTOR3,
    VECTOR4,
    UBYTE4,
    UBYTE4_NORM
};

// arbitrary vertex declaration element semantics
enum class VertexElementSemantic
{
    POSITION,
    NORMAL,
    BINORMAL,
    TANGENT,
    TEXCOORD,
    COLOR,
    BLENDWEIGHTS,
    BLENDINDICES,
    OBJECTINDEX
};

struct Resource;
struct Shader;
struct IndexBuffer;
struct VertexBuffer;
struct GraphicsEngine;

NS_FLOW2D_GFX_END