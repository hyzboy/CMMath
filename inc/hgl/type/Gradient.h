#pragma once

// ⚠️ DEPRECATED: This file is deprecated and will be removed in a future version.
// Please use <hgl/math/Gradient.h> from CMMath library instead.
#warning "hgl/type/Gradient.h is deprecated. Use <hgl/math/Gradient.h> from CMMath instead."

#include <hgl/math/Gradient.h>

// 为了向后兼容，在 hgl 命名空间中创建别名
namespace hgl
{
    using math::Gradient;
    using math::GradientStop;
    using math::FloatGradient;
    using math::VectorGradient2f;
    using math::VectorGradient3f;
    
    // Note: Color gradient aliases are not provided here.
    // Use Gradient<float, Color3f> after including <hgl/math/Color.h>
}
