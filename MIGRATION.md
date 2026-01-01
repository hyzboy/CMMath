# CMMath 迁移指南 / Migration Guide

本文档说明了 CMMath 库的架构重构以及如何迁移现有代码。

This document explains the architectural refactoring of CMMath library and how to migrate existing code.

---

## 为什么进行此重构 / Why This Refactoring

### 中文说明

为了提高代码的可维护性和清晰性，我们对 CMMath 和 CMCoreType 之间的职责进行了重新划分：

**之前的问题：**
- `AlphaBlendMode` 枚举在 CMMath 和 CMCoreType 中重复定义
- 颜色插值函数分散在 CMMath 和 CMCoreType 中，造成功能重复
- 不清晰的职责划分导致维护困难

**重构目标：**
- ✅ **CMMath** 专注于通用数学算法和混合算法实现
- ✅ **CMCoreType** 作为颜色类型和颜色操作的权威位置
- ✅ 避免代码重复，减少维护成本
- ✅ 提供清晰的 API 和迁移路径

### English Explanation

To improve code maintainability and clarity, we have reorganized the responsibilities between CMMath and CMCoreType:

**Previous Issues:**
- `AlphaBlendMode` enum was duplicated in both CMMath and CMCoreType
- Color interpolation functions scattered across CMMath and CMCoreType, causing duplication
- Unclear responsibility separation leading to maintenance difficulties

**Refactoring Goals:**
- ✅ **CMMath** focuses on general math algorithms and blend algorithm implementations
- ✅ **CMCoreType** serves as the authoritative source for color types and color operations
- ✅ Avoid code duplication and reduce maintenance costs
- ✅ Provide clear APIs and migration paths

---

## 主要变更 / Major Changes

### 1. AlphaBlendMode 枚举 / AlphaBlendMode Enum

**变更内容：**
- CMMath 的 `AlphaBlend.h` 不再定义 `AlphaBlendMode` 枚举
- 现在从 CMCoreType 的 `<hgl/color/AlphaBlend.h>` 导入枚举定义
- 所有混合算法实现仍保留在 CMMath 中（这是 CMMath 的核心职责）

**影响：**
- 如果你的代码只使用 `#include<hgl/math/AlphaBlend.h>`，无需修改
- 枚举值和名称完全相同，行为不变
- 混合函数签名和行为完全不变

### 2. 颜色插值函数 / Color Interpolation Functions

**已弃用的函数：**

以下函数在 `Lerp3D.h` 中被标记为 `deprecated`：
- `LerpColor()` 
- `LerpColorSmooth()`
- `LerpColorCubic()`
- `LerpColorBezier()`

**原因：**
- CMCoreType 提供了更专业的颜色插值 API
- 使用颜色类型（Color3f）比 Vector3f 更语义化
- CMCoreType 支持更多颜色空间和插值模式

---

## 迁移步骤 / Migration Steps

### 步骤 1: 更新 AlphaBlend 相关代码

#### 场景 A: 如果你只使用 CMMath 的混合函数

**无需修改！** 代码继续正常工作。

```cpp
// 旧代码 / Old code - 仍然有效 / Still works
#include<hgl/math/AlphaBlend.h>

using namespace hgl::graph;

Vector3f result = AlphaBlend(base, blend, 0.5f, AlphaBlendMode::Normal);
```

#### 场景 B: 如果你需要直接访问枚举定义

现在枚举从 CMCoreType 导入，但使用方式不变：

```cpp
// 旧代码 / Old code
#include<hgl/math/AlphaBlend.h>
using namespace hgl::graph;
AlphaBlendMode mode = AlphaBlendMode::Multiply;

// 新代码 / New code - 完全相同！/ Exactly the same!
#include<hgl/math/AlphaBlend.h>
using namespace hgl::graph;
AlphaBlendMode mode = AlphaBlendMode::Multiply;  // 仍然有效 / Still works
```

### 步骤 2: 迁移颜色插值函数

#### 从 LerpColor() 迁移

**旧代码 / Old Code:**
```cpp
#include<hgl/math/Lerp3D.h>
using namespace hgl::graph;

Vector3f color1(1.0f, 0.0f, 0.0f);  // 红色
Vector3f color2(0.0f, 0.0f, 1.0f);  // 蓝色
Vector3f result = LerpColor(color1, color2, 0.5f);
```

**新代码 / New Code:**
```cpp
#include<hgl/color/Color.h>
using namespace hgl;

Color3f color1(1.0f, 0.0f, 0.0f);  // 红色
Color3f color2(0.0f, 0.0f, 1.0f);  // 蓝色
Color3f result = color1.lerp(color2, 0.5f);
// 或者 / Or: Color3f result = Color3f::lerp(color1, color2, 0.5f);
```

#### 从 LerpColorSmooth() 迁移

**旧代码 / Old Code:**
```cpp
#include<hgl/math/Lerp3D.h>
using namespace hgl::graph;

Vector3f result = LerpColorSmooth(color1, color2, 0.5f);
```

**新代码 / New Code:**
```cpp
#include<hgl/color/ColorLerp.h>
using namespace hgl;

Color3f result = ColorLerp(color1, color2, 0.5f, LerpType::Cosine);
// 或使用其他缓动函数 / Or use other easing functions
```

#### 从 LerpColorCubic() 迁移

**旧代码 / Old Code:**
```cpp
#include<hgl/math/Lerp3D.h>
using namespace hgl::graph;

Vector3f result = LerpColorCubic(color1, color2, 0.5f);
```

**新代码 / New Code:**
```cpp
#include<hgl/color/ColorLerp.h>
using namespace hgl;

Color3f result = ColorLerp(color1, color2, 0.5f, LerpType::Cubic);
```

#### 从 LerpColorBezier() 迁移

**旧代码 / Old Code:**
```cpp
#include<hgl/math/Lerp3D.h>
using namespace hgl::graph;

Vector3f result = LerpColorBezier(c0, c1, c2, c3, 0.5f);
```

**新代码 / New Code:**
```cpp
#include<hgl/color/ColorLerp.h>
using namespace hgl;

Color3f result = ColorLerp(c0, c1, c2, c3, 0.5f, LerpType::Bezier);
```

---

## API 对照表 / API Reference Table

| CMMath (旧 / Old) | CMCoreType (新 / New) | 说明 / Notes |
|-------------------|----------------------|-------------|
| `LerpColor(a, b, t)` | `Color3f::lerp(a, b, t)` | 线性插值 / Linear interpolation |
| `LerpColorSmooth(a, b, t)` | `ColorLerp(a, b, t, LerpType::Cosine)` | 余弦插值 / Cosine interpolation |
| `LerpColorCubic(a, b, t)` | `ColorLerp(a, b, t, LerpType::Cubic)` | 三次插值 / Cubic interpolation |
| `LerpColorBezier(c0, c1, c2, c3, t)` | `ColorLerp(c0, c1, c2, c3, t, LerpType::Bezier)` | 贝塞尔插值 / Bezier interpolation |

---

## 优势 / Benefits

### 使用 CMCoreType 的颜色 API 的优势：

1. **类型安全**：`Color3f` 比 `Vector3f` 更明确表达颜色语义
2. **更多功能**：支持多种颜色空间（RGB, HSV, HSL）
3. **专业化**：颜色特定的操作（gamma 校正、色调调整等）
4. **统一接口**：所有颜色操作集中在 CMCoreType

### Benefits of Using CMCoreType Color API:

1. **Type Safety**: `Color3f` explicitly expresses color semantics vs `Vector3f`
2. **More Features**: Supports multiple color spaces (RGB, HSV, HSL)
3. **Specialization**: Color-specific operations (gamma correction, hue adjustment, etc.)
4. **Unified Interface**: All color operations centralized in CMCoreType

---

## 时间表 / Timeline

- **现在 / Now**: 旧函数标记为 deprecated 但仍可用
- **下一个主版本 / Next Major Version**: 可能移除已弃用的函数
- **建议 / Recommendation**: 尽早迁移以避免未来的破坏性变更

---

## 需要帮助? / Need Help?

如果你在迁移过程中遇到问题：

1. 查看 CMCoreType 的颜色 API 文档
2. 参考本文档中的示例代码
3. 在项目 issue 中提问

If you encounter issues during migration:

1. Check CMCoreType's color API documentation
2. Refer to example code in this document
3. Ask questions in project issues

---

## 3. Gradient 类迁移 / Gradient Class Migration

### 变更内容 / Changes

**之前：**
- `Gradient` 类位于 CMCore 库的 `inc/hgl/type/Gradient.h`
- 虽然是数学插值工具，但放在了类型库中

**现在：**
- `Gradient` 类迁移到 CMMath 库的 `inc/hgl/math/Gradient.h`
- 作为数学插值工具，更符合其本质功能

### 迁移步骤 / Migration Steps

#### 旧代码 / Old Code:

```cpp
#include <hgl/type/Gradient.h>

using namespace hgl;

Gradient<float, Color3f> colorGradient;
colorGradient.Add(0.0f, Color3f(1, 0, 0));  // 红色
colorGradient.Add(0.5f, Color3f(0, 1, 0));  // 绿色
colorGradient.Add(1.0f, Color3f(0, 0, 1));  // 蓝色

Color3f result;
colorGradient.Get(result, 0.25f);
```

#### 新代码 / New Code:

```cpp
#include <hgl/math/Gradient.h>

using namespace hgl::math;

Gradient<float, Color3f> colorGradient;
colorGradient.Add(0.0f, Color3f(1, 0, 0));  // 红色
colorGradient.Add(0.5f, Color3f(0, 1, 0));  // 绿色
colorGradient.Add(1.0f, Color3f(0, 0, 1));  // 蓝色

Color3f result;
colorGradient.Get(result, 0.25f);
```

### 主要改进 / Key Improvements

1. **更清晰的职责划分**：数学插值工具放在数学库中
2. **更好的语义**：`hgl::math::Gradient` 比 `hgl::Gradient` 更明确表达其数学本质
3. **依赖关系更清晰**：CMMath 提供数学算法，CMCore 提供基础类型
4. **提供预定义类型别名**：
   - `FloatGradient` - 浮点数渐变
   - `VectorGradient2f` - 2D 向量渐变
   - `VectorGradient3f` - 3D 向量渐变
   - 注意：颜色渐变请使用 `Gradient<float, Color3f>` 并包含 `<hgl/math/Color.h>`

### 使用示例 / Usage Examples

#### 颜色渐变 / Color Gradient

```cpp
#include <hgl/math/Gradient.h>
#include <hgl/math/Color.h>

using namespace hgl::math;

// 使用 Gradient 模板定义颜色渐变
Gradient<float, Color3f> gradient;

// 添加关键点
gradient.Add(0.0f, Colors::Red);
gradient.Add(0.5f, Colors::Yellow);
gradient.Add(1.0f, Colors::Blue);

// 获取插值结果
Color3f color;
gradient.Get(color, 0.25f);  // 红色到黄色之间 25% 的位置

// 或使用操作符
Color3f color2 = gradient(0.75f);  // 黄色到蓝色之间 75% 的位置
```

#### 数值动画 / Numeric Animation

```cpp
#include <hgl/math/Gradient.h>

using namespace hgl::math;

FloatGradient animation;
animation.Add(0.0f, 0.0f);    // 起始值
animation.Add(0.5f, 100.0f);  // 中间值
animation.Add(1.0f, 50.0f);   // 结束值

float value = animation(0.3f);  // 获取 30% 时间点的值
```

#### 向量路径 / Vector Path

```cpp
#include <hgl/math/Gradient.h>

using namespace hgl::math;

VectorGradient3f path;
path.Add(0.0f, Vector3f(0, 0, 0));
path.Add(0.5f, Vector3f(10, 5, 0));
path.Add(1.0f, Vector3f(20, 0, 0));

Vector3f position;
path.Get(position, 0.7f);  // 获取路径上 70% 位置的坐标
```

---

## 总结 / Summary

这次重构提高了代码的组织性和可维护性。虽然需要一些迁移工作，但长期来看会带来更清晰的架构和更好的开发体验。

This refactoring improves code organization and maintainability. While some migration work is needed, it will lead to clearer architecture and better development experience in the long run.

**关键要点 / Key Takeaways:**
- ✅ 混合算法实现保留在 CMMath（无需修改）
- ✅ 枚举定义从 CMCoreType 导入（透明变更）
- ✅ 颜色插值函数迁移到 CMCoreType（更好的语义）
- ✅ 提供了平滑的迁移路径和完整文档

---

## Vector Lerp 函数 / Vector Lerp Functions

### 功能 / Features

为所有向量类型提供统一的 Lerp 插值函数，包括整数向量和浮点向量。

Provides unified Lerp interpolation functions for all vector types, including integer and floating-point vectors.

### 使用方法 / Usage

#### 整数向量插值 / Integer Vector Interpolation

```cpp
#include <hgl/math/VectorLerp.h>
using namespace hgl::math;

// Vector2u8 插值
Vector2u8 v1(0, 0);
Vector2u8 v2(255, 255);
Vector2u8 result = Lerp(v1, v2, 0.5f);  // 结果: (128, 128)

// Vector3u8 RGB 颜色插值
Vector3u8 red(255, 0, 0);
Vector3u8 blue(0, 0, 255);
Vector3u8 purple = Lerp(red, blue, 0.5f);  // 结果: (128, 0, 128)

// Vector4u8 RGBA 颜色插值（带透明度）
Vector4u8 c1(255, 0, 0, 255);
Vector4u8 c2(0, 0, 255, 128);
Vector4u8 mixed = Lerp(c1, c2, 0.5f);  // 插值包括 alpha 通道
```

#### 浮点向量插值 / Float Vector Interpolation

```cpp
#include <hgl/math/VectorLerp.h>
using namespace hgl::math;

// Vector2f 插值
Vector2f a(0.0f, 0.0f);
Vector2f b(1.0f, 1.0f);
Vector2f result = Lerp(a, b, 0.5f);  // 结果: (0.5, 0.5)

// Vector3f 插值
Vector3f p1(1.0f, 2.0f, 3.0f);
Vector3f p2(4.0f, 5.0f, 6.0f);
Vector3f mid = Lerp(p1, p2, 0.5f);  // 结果: (2.5, 3.5, 4.5)

// Vector4f 插值
Vector4f v1(1.0f, 2.0f, 3.0f, 4.0f);
Vector4f v2(5.0f, 6.0f, 7.0f, 8.0f);
Vector4f result = Lerp(v1, v2, 0.25f);
```

#### 颜色插值便捷函数 / Color Interpolation Convenience Functions

```cpp
#include <hgl/math/Color.h>
using namespace hgl::math;

// uint8 RGB 颜色插值
Vector3u8 red(255, 0, 0);
Vector3u8 green(0, 255, 0);
Vector3u8 yellow = ColorLerpU8(red, green, 0.5f);

// uint8 RGBA 颜色插值
Vector4u8 color1(255, 0, 0, 255);
Vector4u8 color2(0, 0, 255, 128);
Vector4u8 mixed = ColorLerpU8(color1, color2, 0.5f);
```

### Vector Clamp 函数 / Vector Clamp Functions

```cpp
#include <hgl/math/Clamp.h>
using namespace hgl::math;

// Vector2u8/Vector3u8/Vector4u8 已经在有效范围内
Vector3u8 color(255, 128, 64);
Vector3u8 clamped = ClampVector(color);  // 返回原值

// 将浮点向量转换为 uint8 向量（自动钳制）
Vector2f fv2(300.0f, -50.0f);
Vector2u8 uv2 = ClampVectorU8(fv2);  // 结果: (255, 0)

Vector3f fv3(300.0f, 128.5f, -10.0f);
Vector3u8 uv3 = ClampVectorU8(fv3);  // 结果: (255, 129, 0)

Vector4f fv4(400.0f, 200.0f, 100.0f, 50.0f);
Vector4u8 uv4 = ClampVectorU8(fv4);  // 结果: (255, 200, 100, 50)
```

### 标量插值 / Scalar Interpolation

```cpp
#include <hgl/math/VectorLerp.h>
using namespace hgl::math;

// uint8 插值
uint8_t a = 0;
uint8_t b = 255;
uint8_t mid = Lerp(a, b, 0.5f);  // 结果: 128

// float 插值
float x = 0.0f;
float y = 10.0f;
float z = Lerp(x, y, 0.3f);  // 结果: 3.0

// double 插值
double d1 = 0.0;
double d2 = 100.0;
double d3 = Lerp(d1, d2, 0.25);  // 结果: 25.0
```

### 特性 / Features

- **自动钳制**: 整数向量插值自动钳制到有效范围 [0, 255]
- **类型安全**: 为每种向量类型提供专门的重载
- **统一接口**: 所有向量类型使用相同的 `Lerp()` 函数名
- **颜色支持**: 特别优化了 RGB 和 RGBA 颜色插值

### 注意事项 / Notes

1. 整数向量插值使用浮点数计算，然后钳制并四舍五入到整数
2. 参数 `t` 通常在 [0, 1] 范围内，但可以超出此范围
3. 颜色插值在 RGB 空间进行（如需 HSV 空间插值，使用 `ColorLerpHSV`）
4. 所有函数都是内联的，性能开销最小

---
