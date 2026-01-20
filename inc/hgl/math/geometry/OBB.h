#pragma once

#include<hgl/math/Vector.h>
#include<hgl/math/Matrix.h>
#include<hgl/math/geometry/Plane.h>
#include<hgl/math/geometry/Triangle.h>

namespace hgl::math
{
    class Ray;
    class AABB;

    /**
     * Oriented Bounding Box
     */
    class OBB
    {
        Vector3f center;
        Matrix3f axis;          ///<轴矩阵
        Vector3f half_length;

        Plane planes[6];

    private:

        friend struct BoundingVolumes;

        void ComputePlanes();

        template<typename T>
        void SetFromPointsMinVolume(const T *points,size_t count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg);

        void SetFromPointsMinVolumeFloat(const float *points,size_t count,uint32_t component_count,float coarseStepDeg,float fineStepDeg,float ultraStepDeg);

    public:

        const Vector3f &GetCenter()const{return center;}
        const Vector3f &GetAxis(int n)const{return axis[n];}
        const Matrix3f &GetRotationMatrix()const{return axis;}
        const Vector3f &GetHalfExtend()const{return half_length;}

        /**
         * 获取尺寸（完整尺寸，不是半尺寸）
         */
        Vector3f GetSize() const { return half_length * 2.0f; }

        /**
         * 获取对角线长度
         */
        float GetDiagonal() const { return glm::length(half_length * 2.0f); }

        const Matrix4f GetMatrix(const float cube_size=1.0f)const;

    public:

        void Set(const Vector3f &c,const Vector3f &hl);
        void Set(const Vector3f &c,const Vector3f &a0,const Vector3f &a1,const Vector3f &a2,const Vector3f &hl);

        void SetFromPoints(const float *points,size_t count,uint32_t component_count,float coarseStepDeg=15.0f,float fineStepDeg=3.0f,float ultraStepDeg=0.5f);
        
        /**
         * 清空 OBB，设置为无效状态
         * @note 清空后 IsValid() 返回 false，IsEmpty() 返回 true
         */
        void Clear()
        {
            center = Vector3f(0, 0, 0);
            axis = Matrix3f(1.0f);  // 单位矩阵
            half_length = Vector3f(-1, -1, -1);  // 负值表示无效
            mem_zero(planes);
        }

    public:

        OBB()
        {
            Set(Vector3f(0,0,0),Vector3f(1,1,1));
        }

        OBB(const Vector3f &c,const Vector3f &hl)
        {
            Set(c,hl);
        }

        OBB(const Vector3f &c,const Vector3f &a0,const Vector3f &a1,const Vector3f &a2,const Vector3f &hl)
        {
            Set(c,a0,a1,a2,hl);
        }

    public:

        /**
         * 检查 OBB 是否为空（半长度接近零或为负）
         */
        bool IsEmpty()const{return !IsValid() || IsNearlyZero(half_length);}

        /**
         * 检查 OBB 是否有效（半长度为非负）
         * @note 刚 Clear() 后的 OBB 是无效的
         */
        bool IsValid()const
        {
            return half_length.x >= 0 && 
                   half_length.y >= 0 && 
                   half_length.z >= 0;
        }

        const Vector3f GetMin()const{return center-half_length;}
        const Vector3f GetMax()const{return center+half_length;}

        const Plane &GetFacePlanes(int i)const{return planes[i];}

        void GetCorners(Vector3f out[8])const;

    public: // 静态工厂方法

        /**
         * 从 AABB 创建轴对齐的 OBB
         */
        static OBB FromAABB(const AABB &aabb);

    public: // 碰撞检测 - 点相关

        /**
         * 检查点是否在OBB内
         */
        bool ContainsPoint(const Vector3f &point) const;

        /**
         * 计算点到OBB的最近点
         */
        Vector3f ClosestPoint(const Vector3f &point) const;

        /**
         * 计算点到OBB的距离
         */
        float DistanceToPoint(const Vector3f &point) const;

        /**
         * 计算点到OBB的距离平方
         */
        float DistanceToPointSquared(const Vector3f &point) const;

    public: // 碰撞检测 - OBB

        /**
         * 检查与另一个OBB是否相交(使用分离轴定理)
         */
        bool Intersects(const OBB &other) const;

        /**
         * 检查是否完全包含另一个OBB
         */
        bool Contains(const OBB &other) const;

        /**
         * 计算到另一个OBB的距离
         * @note 当前实现是近似算法，采样角点计算最近距离
         */
        float Distance(const OBB &other) const;

    public: // 碰撞检测 - 其他几何体

        /**
         * 检查与AABB是否相交
         */
        bool IntersectsAABB(const AABB &aabb) const;

        /**
         * 检查与球体是否相交
         */
        bool IntersectsSphere(const Vector3f &sphere_center, float sphere_radius) const;

        /**
         * 检查与射线是否相交
         */
        bool IntersectsRay(const Ray &ray, float &distance) const;
        bool IntersectsRay(const Ray &ray) const;
        bool IntersectsRay(const Ray &ray, float &t_min, float &t_max) const;

        /**
         * 检查与平面的关系
         * @return <0: 完全在平面后面, 0: 相交, >0: 完全在平面前面
         */
        int ClassifyPlane(const Plane &plane) const;

        /**
         * 检查是否与平面相交
         */
        bool IntersectsPlane(const Plane &plane) const;

        /**
         * 检查与三角形是否相交
         * @warning 当前实现不完整，可能产生假阴性（漏掉某些相交情况）
         * @note 只检查三角形顶点是否在 OBB 内，不检查边和面相交
         * TODO: 实现完整的 SAT (Separating Axis Theorem) 测试
         */
        bool IntersectsTriangle(const Triangle3f &triangle) const;

    public: // 工具函数

        /**
         * 扩展OBB以包含指定点
         * @note 只扩展半长度，不改变中心和朝向
         */
        void ExpandToInclude(const Vector3f &point);

        /**
         * 获取表面积
         */
        float GetSurfaceArea() const
        {
            Vector3f size = half_length * 2.0f;
            return 2.0f * (size.x * size.y + size.y * size.z + size.z * size.x);
        }

        /**
         * 获取体积
         */
        float GetVolume() const
        {
            return half_length.x * half_length.y * half_length.z * 8.0f;
        }

        /**
         * 合并另一个OBB(转换为AABB后合并，再转回OBB)
         * @note 会丢失原始的旋转信息，变成轴对齐的 OBB
         */
        void Merge(const OBB &other);

    public:

        OBB Transformed(const Matrix4f &)const;
    };//class OBB
}//namespace hgl::math
