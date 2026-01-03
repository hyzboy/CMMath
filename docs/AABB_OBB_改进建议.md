# AABB 和 OBB 改进建议

## 高优先级问题（建议立即修复）

### 1. **修复 Clear() 方法** ?? HIGH

**AABB.h**
```cpp
void Clear()
{
    // 使用无效值表示空状态
    minPoint = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
    maxPoint = Vector3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    center = Vector3f(0, 0, 0);
    length = Vector3f(0, 0, 0);
    mem_zero(planes);
    mem_zero(face_center_point);
}

// 添加新方法
bool IsValid() const 
{ 
    return minPoint.x <= maxPoint.x && 
           minPoint.y <= maxPoint.y && 
           minPoint.z <= maxPoint.z; 
}
```

**OBB.h**
```cpp
void Clear()
{
    center = Vector3f(0, 0, 0);
    axis = Matrix3f(1.0f);  // 单位矩阵
    half_length = Vector3f(-1, -1, -1);  // 负值表示无效
    mem_zero(planes);
}

bool IsValid() const 
{ 
    return half_length.x >= 0 && 
           half_length.y >= 0 && 
           half_length.z >= 0; 
}

bool IsEmpty() const 
{ 
    return !IsValid() || IsNearlyZero(half_length); 
}
```

### 2. **标记或移除不完整的方法** ? HIGH

**AABB.cpp 和 OBB.cpp**
```cpp
bool AABB::IntersectsTriangle(const Triangle3f &triangle) const
{
    // TODO: 未实现完整的 SAT 测试
    // 目前只做基本检查，可能漏掉某些相交情况
    
    // 检查三角形顶点是否在AABB内
    if (ContainsPoint(triangle[0]) || 
        ContainsPoint(triangle[1]) || 
        ContainsPoint(triangle[2]))
        return true;

    // 检查 AABB 中心是否在三角形内
    // ...更多基本检查...
    
    // 注意：这不是完整实现！
    return false;
}
```

**或者直接标记为已知限制：**
```cpp
// 在头文件中添加注释
/**
 * 检查与三角形是否相交
 * @warning 当前实现不完整，可能产生假阴性（漏掉某些相交情况）
 * @note 只检查三角形顶点，不检查边和面相交
 */
bool IntersectsTriangle(const Triangle3f &triangle) const;
```

### 3. **修复 OBB::IntersectsAABB() 的不完整实现** ?? MEDIUM

**OBB.cpp**
```cpp
bool OBB::IntersectsAABB(const AABB &aabb) const
{
    // ... 现有代码 ...

    // 测试9个叉积轴
    Vector3f aabb_axes[3] = {
        Vector3f(1, 0, 0),
        Vector3f(0, 1, 0),
        Vector3f(0, 0, 1)
    };
    
    Vector3f aabb_extent = aabb.GetLength() * 0.5f;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Vector3f cross_axis = glm::cross(axis[i], aabb_axes[j]);
            float len = glm::length(cross_axis);
            if (len < 1e-6f) continue;  // 平行轴
            
            cross_axis /= len;

            // 计算投影半径
            float ra = 0.0f;
            for (int k = 0; k < 3; k++)
                ra += half_length[k] * std::abs(glm::dot(axis[k], cross_axis));

            float rb = 0.0f;
            for (int k = 0; k < 3; k++)
                rb += aabb_extent[k] * std::abs(cross_axis[k]);

            if (std::abs(glm::dot(t, cross_axis)) > ra + rb)
                return false;
        }
    }

    return true;
}
```

## 中优先级改进（建议添加）

### 4. **添加工厂方法** ?? MEDIUM

**AABB.h**
```cpp
public:
    /**
     * 从两个点创建 AABB（自动计算 min/max）
     */
    static AABB FromTwoPoints(const Vector3f &p1, const Vector3f &p2)
    {
        AABB result;
        result.SetMinMax(MinVector(p1, p2), MaxVector(p1, p2));
        return result;
    }

    /**
     * 从中心和尺寸创建 AABB
     */
    static AABB FromCenterAndSize(const Vector3f &center, const Vector3f &size)
    {
        Vector3f half_size = size * 0.5f;
        AABB result;
        result.SetMinMax(center - half_size, center + half_size);
        return result;
    }

    /**
     * 创建空的无效 AABB
     */
    static AABB Empty()
    {
        AABB result;
        result.Clear();
        return result;
    }
```

**OBB.h**
```cpp
public:
    /**
     * 从 AABB 创建轴对齐的 OBB
     */
    static OBB FromAABB(const AABB &aabb)
    {
        return OBB(aabb.GetCenter(), aabb.GetLength() * 0.5f);
    }

    /**
     * 从变换后的 AABB 创建 OBB
     */
    static OBB FromAABBTransformed(const AABB &aabb, const Matrix4f &transform);
```

### 5. **添加缩放和偏移方法** ?? MEDIUM

**AABB.h**
```cpp
public:
    /**
     * 返回缩放后的 AABB（从中心缩放）
     */
    AABB Scaled(float factor) const
    {
        Vector3f half_size = length * 0.5f * factor;
        AABB result;
        result.SetMinMax(center - half_size, center + half_size);
        return result;
    }

    /**
     * 返回缩放后的 AABB（非均匀缩放）
     */
    AABB Scaled(const Vector3f &scale) const
    {
        Vector3f half_size = length * 0.5f;
        Vector3f new_half_size(
            half_size.x * scale.x,
            half_size.y * scale.y,
            half_size.z * scale.z
        );
        AABB result;
        result.SetMinMax(center - new_half_size, center + new_half_size);
        return result;
    }

    /**
     * 返回偏移后的 AABB
     */
    AABB Offset(const Vector3f &offset) const
    {
        AABB result;
        result.SetMinMax(minPoint + offset, maxPoint + offset);
        return result;
    }

    /**
     * 返回扩展/收缩后的 AABB（每个方向扩展相同距离）
     */
    AABB Expanded(float amount) const
    {
        Vector3f expansion(amount, amount, amount);
        AABB result;
        result.SetMinMax(minPoint - expansion, maxPoint + expansion);
        return result;
    }

    /**
     * 原地缩放
     */
    void Scale(float factor)
    {
        *this = Scaled(factor);
    }

    /**
     * 原地偏移
     */
    void Translate(const Vector3f &offset)
    {
        SetMinMax(minPoint + offset, maxPoint + offset);
    }
```

### 6. **添加更多查询方法** ?? LOW

**AABB.h**
```cpp
public:
    /**
     * 获取尺寸（等同于 GetLength，但更直观）
     */
    const Vector3f &GetSize() const { return length; }

    /**
     * 获取半尺寸
     */
    Vector3f GetExtent() const { return length * 0.5f; }

    /**
     * 获取对角线长度
     */
    float GetDiagonal() const { return glm::length(length); }

    /**
     * 获取最长边的长度
     */
    float GetMaxExtent() const 
    { 
        return std::max({length.x, length.y, length.z}); 
    }

    /**
     * 获取最短边的长度
     */
    float GetMinExtent() const 
    { 
        return std::min({length.x, length.y, length.z}); 
    }

    /**
     * 获取周长（所有边长度之和）
     */
    float GetPerimeter() const
    {
        return 4.0f * (length.x + length.y + length.z);
    }
```

**OBB.h**
```cpp
public:
    /**
     * 获取尺寸
     */
    Vector3f GetSize() const { return half_length * 2.0f; }

    /**
     * 获取对角线长度
     */
    float GetDiagonal() const { return glm::length(half_length * 2.0f); }

    /**
     * 获取最大范围
     */
    float GetMaxExtent() const
    {
        return std::max({half_length.x, half_length.y, half_length.z});
    }
```

### 7. **改进 SetCornerLength 命名** ?? LOW

**AABB.h**
```cpp
public:
    /**
     * 按最小点和尺寸设置盒子范围
     * @param min_point 最小点（不是"corner"，避免歧义）
     * @param size 盒子尺寸
     */
    void SetMinAndSize(const Vector3f &min_point, const Vector3f &size)
    {
        minPoint = min_point;
        length = size;
        maxPoint = min_point + size;
        center = (minPoint + maxPoint) / 2.0f;
        Update();
    }

    // 保留旧方法以兼容性，但标记为 deprecated
    [[deprecated("Use SetMinAndSize instead")]]
    void SetCornerLength(const Vector3f &c, const Vector3f &l)
    {
        SetMinAndSize(c, l);
    }
```

## 低优先级改进

### 8. **改进 OBB::Distance() 的准确性** ?? LOW

```cpp
float OBB::Distance(const OBB &other) const
{
    if (Intersects(other))
        return 0.0f;

    // 更准确的方法：计算两个 OBB 的最近点
    // 这需要更复杂的算法，当前实现是保守估计
    
    // 简化但更准确的方法：采样多个点
    Vector3f corners1[8], corners2[8];
    GetCorners(corners1);
    other.GetCorners(corners2);

    float min_dist = FLT_MAX;
    
    // 计算每个角点到另一个 OBB 的距离
    for (int i = 0; i < 8; i++)
    {
        float d1 = DistanceToPoint(corners2[i]);
        float d2 = other.DistanceToPoint(corners1[i]);
        min_dist = std::min(min_dist, std::min(d1, d2));
    }

    return min_dist;
}
```

### 9. **添加序列化支持** ?? LOW

```cpp
// AABB.h
public:
    /**
     * 序列化到字节数组
     */
    void Serialize(float *buffer) const
    {
        memcpy(buffer, &minPoint, sizeof(Vector3f));
        memcpy(buffer + 3, &maxPoint, sizeof(Vector3f));
    }

    /**
     * 从字节数组反序列化
     */
    void Deserialize(const float *buffer)
    {
        Vector3f min, max;
        memcpy(&min, buffer, sizeof(Vector3f));
        memcpy(&max, buffer + 3, sizeof(Vector3f));
        SetMinMax(min, max);
    }
```

## 总结

### 必须修复（HIGH）
1. ? 修复 `Clear()` 方法的空状态表示
2. ? 标记或修复不完整的 `IntersectsTriangle()` 
3. ? 完善 `OBB::IntersectsAABB()` 的 9 个叉积轴测试

### 建议添加（MEDIUM）
4. ? 添加工厂方法（FromTwoPoints, FromCenterAndSize 等）
5. ? 添加缩放和偏移方法
6. ? 完善 OBB::IntersectsAABB 的 SAT 测试

### 可选改进（LOW）
7. 添加更多查询方法
8. 改进命名（SetCornerLength → SetMinAndSize）
9. 改进 OBB::Distance() 的准确性
10. 添加序列化支持

## 兼容性说明

所有改进都是**向后兼容**的：
- 只添加新方法，不修改现有方法签名
- `Clear()` 的行为改进不会破坏现有代码
- 可以使用 `[[deprecated]]` 标记过时的方法
