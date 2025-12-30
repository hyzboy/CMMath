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

## 总结 / Summary

这次重构提高了代码的组织性和可维护性。虽然需要一些迁移工作，但长期来看会带来更清晰的架构和更好的开发体验。

This refactoring improves code organization and maintainability. While some migration work is needed, it will lead to clearer architecture and better development experience in the long run.

**关键要点 / Key Takeaways:**
- ✅ 混合算法实现保留在 CMMath（无需修改）
- ✅ 枚举定义从 CMCoreType 导入（透明变更）
- ✅ 颜色插值函数迁移到 CMCoreType（更好的语义）
- ✅ 提供了平滑的迁移路径和完整文档
