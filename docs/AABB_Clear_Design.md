# AABB Clear() 设计说明

## 问题背景

在测试 `AABB::Clear()` 时发现，清空后：
- `GetMin()` 返回 `(FLT_MAX, FLT_MAX, FLT_MAX)` 
- `GetMax()` 返回 `(-FLT_MAX, -FLT_MAX, -FLT_MAX)`
- 而不是直觉上的 `(0, 0, 0)`

## 设计解释：Inverse Bounding Box（反向包围盒）

这是几何库中的**标准设计模式**，被以下主流引擎/库采用：
- ✅ Unity (`Bounds.SetMinMax`)
- ✅ Unreal Engine (`FBox::Init`)
- ✅ DirectXMath (`BoundingBox::CreateFromPoints`)
- ✅ GLM
- ✅ Bullet Physics

### 核心思想

```cpp
void Clear()
{
    minPoint = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);    // 最小点设为"无限大"
    maxPoint = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX); // 最大点设为"无限小"
    center = Vector3f(0, 0, 0);                        // 中心和长度归零
    length = Vector3f(0, 0, 0);
}
```

### 为什么这样设计？

#### 1. **支持累积式构建**

```cpp
AABB box = AABB::Empty();  // 创建空的AABB

// 逐个添加点，自动扩展边界
for (auto& point : points) {
    box.ExpandToInclude(point);
}
```

如果 `min = (0,0,0)`, `max = (0,0,0)`，第一个负数坐标的点会导致错误结果。  
使用 inverse box，第一个点会**正确地**成为新的边界。

#### 2. **表示"无效"状态**

```cpp
bool AABB::IsValid() const
{
    return minPoint.x <= maxPoint.x && 
           minPoint.y <= maxPoint.y && 
           minPoint.z <= maxPoint.z;
}
```

当 `min > max` 时，`IsValid()` 返回 `false`，明确表示这是一个无效的 AABB。

#### 3. **与 IsEmpty() 配合**

```cpp
bool IsEmpty() const { return IsNearlyZero(length); }
```

- `IsEmpty()`: 检查尺寸是否为零（无体积）
- `IsValid()`: 检查边界是否合法（min ≤ max）

清空后的 AABB：
- ✅ `IsEmpty()` = `true`（长度为零）
- ✅ `IsValid()` = `false`（min > max，无效状态）

## 正确的测试方式

### ❌ 错误做法：检查内部值

```cpp
TEST_ASSERT(box.GetMin() == Vector3f(0,0,0), "Min应该是零");
TEST_ASSERT(box.GetMax() == Vector3f(0,0,0), "Max应该是零");
```

这依赖于实现细节，不是好的测试。

### ✅ 正确做法：检查公共契约

```cpp
// 检查状态
TEST_ASSERT(!box.IsValid(), "Clear后应该无效");
TEST_ASSERT(box.IsEmpty(), "Clear后应该为空");

// 检查行为
box.ExpandToInclude(Vector3f(5, 5, 5));
TEST_ASSERT(box.IsValid(), "添加点后应该有效");
TEST_ASSERT(box.GetMin() == Vector3f(5,5,5), "第一个点应成为边界");
```

## 示例对比

### 场景：构建包含多个点的 AABB

```cpp
std::vector<Vector3f> points = {
    Vector3f(-10, 5, 0),
    Vector3f(20, -15, 30),
    Vector3f(0, 0, 0)
};

// 使用 Inverse Box（正确）
AABB box1 = AABB::Empty();
for (auto& p : points) {
    box1.ExpandToInclude(p);
}
// 结果：Min=(-10, -15, 0), Max=(20, 5, 30) ✅

// 如果 Clear 设置为 (0,0,0)（错误）
AABB box2;
box2.SetMinMax(Vector3f(0,0,0), Vector3f(0,0,0));
box2.ExpandToInclude(Vector3f(-10, 5, 0));
// 结果：Min=(-10, 0, 0), Max=(0, 5, 0) ❌ 错误！丢失了x正向的点
```

## 结论

### 当前设计（推荐保持）

- ✅ 符合行业标准
- ✅ 支持累积式构建
- ✅ 状态语义清晰
- ✅ 代码简洁高效

### 测试修改

修改测试用例以匹配正确的设计理念：
- 检查 `IsValid()` 和 `IsEmpty()` 的返回值
- 验证 `ExpandToInclude()` 的行为
- 不依赖 `GetMin()/GetMax()` 的具体数值

## 参考资料

- [Real-Time Collision Detection](https://realtimecollisiondetection.net/) - Christer Ericson
- [Unreal Engine Source: FBox::Init](https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/Core/Public/Math/Box.h)
- [Unity Documentation: Bounds](https://docs.unity3d.com/ScriptReference/Bounds.html)
