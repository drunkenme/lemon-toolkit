// @date 2016/07/28
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <defines.hpp>

#define NS_FLOW2D_CORE_BEGIN namespace lemon { namespace core {
#define NS_FLOW2D_CORE_END } }

NS_FLOW2D_CORE_BEGIN

static const size_t kEntPoolChunkSize = 512;
static const size_t kEntMaxComponents = 128;
static const size_t kDbgMaxTracebackFrames = 63;

struct Subsystem;
struct EventManager;
struct EntityManager;
struct Context;

NS_FLOW2D_CORE_END