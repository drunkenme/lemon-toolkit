// @date 2016/07/26
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <defines.hpp>

#define NS_FLOW2D_GFX_BEGIN namespace lemon { namespace graphics {
#define NS_FLOW2D_GFX_END } }

NS_FLOW2D_GFX_BEGIN

const static size_t kMaxVertexAttributes = 8;
const static size_t kMaxTextures = 8;
const static size_t kMaxUniforms = 32;

struct Device;
struct GraphicsObject;
struct IndexBuffer;
struct VertexBuffer;
struct Shader;
struct Texture;
struct Texture2D;

NS_FLOW2D_GFX_END