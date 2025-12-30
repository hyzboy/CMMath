/**
 * CoreType.h - 核心类型定义
 * 
 * 定义跨平台的基础类型别名。
 */

#pragma once

#include <cstdint>

namespace hgl
{
    // 基础类型别名
    using uint = unsigned int;
    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    
    // Half float 类型
    using half_float = uint16_t;
    
}//namespace hgl
