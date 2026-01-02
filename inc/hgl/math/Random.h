/**
 * Random.h - 随机数生成工具
 * 
 * 实现快速、高质量的随机数生成工具，用于游戏开发。
 * 基于 PCG (Permuted Congruential Generator) 算法。
 * 
 * 参考：https://www.pcg-random.org/
 * 
 * 包含：
 * - PCG 随机数生成器
 * - 基础随机函数（Float, Int, Bool）
 * - 几何采样（圆、球、AABB 内随机点）
 * - 分布函数（高斯分布、指数分布）
 */

#pragma once

#include <hgl/math/VectorTypes.h>
#include <hgl/math/VectorOperations.h>
#include <cstdint>
#include <cmath>

namespace hgl::math
{
    // 前置声明
    class AABB;
    class AABB2D;
    
    // ==================== PCG 随机数生成器 ====================
    
    /**
     * @brief PCG 随机数生成器
     * 
     * 高质量、快速的伪随机数生成器。
     * 基于 PCG-XSH-RR 变体。
     */
    class RandomGenerator
    {
    private:
        uint64_t state;       ///< RNG 状态
        uint64_t increment;   ///< 增量（必须是奇数）
        
    public:
        /**
         * @brief 默认构造函数
         * 使用默认种子初始化
         */
        RandomGenerator();
        
        /**
         * @brief 使用指定种子构造
         * @param seed 随机种子
         */
        explicit RandomGenerator(uint64_t seed);
        
        /**
         * @brief 使用种子和序列初始化
         * @param seed 随机种子
         * @param sequence 序列号（用于生成不同的随机序列）
         */
        RandomGenerator(uint64_t seed, uint64_t sequence);
        
        /**
         * @brief 设置种子
         * @param seed 新的随机种子
         */
        void Seed(uint64_t seed);
        
        /**
         * @brief 设置种子和序列
         */
        void Seed(uint64_t seed, uint64_t sequence);
        
        /**
         * @brief 生成 32 位无符号整数
         * @return 随机的 uint32_t 值
         */
        uint32_t NextUInt32();
        
        /**
         * @brief 生成 64 位无符号整数
         * @return 随机的 uint64_t 值
         */
        uint64_t NextUInt64();
        
        /**
         * @brief 生成 [0, 1) 范围的浮点数
         * @return 随机浮点数
         */
        float NextFloat();
        
        /**
         * @brief 生成 [min, max) 范围的浮点数
         * @param min 最小值
         * @param max 最大值
         * @return 随机浮点数
         */
        float NextFloat(float min, float max);
        
        /**
         * @brief 生成 [min, max] 范围的整数
         * @param min 最小值
         * @param max 最大值
         * @return 随机整数
         */
        int NextInt(int min, int max);
        
        /**
         * @brief 生成布尔值
         * @param probability 返回 true 的概率 [0.0, 1.0]，默认 0.5
         * @return 随机布尔值
         */
        bool NextBool(float probability = 0.5f);
        
        Vector2f NextVector2InCircle();
        Vector3f NextVector3OnSphere();
        Vector2f NextVector2Gaussian();
    };
    
    // ==================== 全局默认生成器 ====================
    
    /**
     * @brief 获取线程局部的默认随机数生成器
     * @return 默认 RNG 的引用
     */
    RandomGenerator& GetDefaultRNG();
    
    /**
     * @brief 设置默认生成器的种子
     * @param seed 新的随机种子
     */
    void SeedDefaultRNG(uint64_t seed);
    
    // ==================== 便捷函数（使用默认生成器）====================
    
    /**
     * @brief 生成 [0, 1) 范围的随机浮点数
     */
    inline float RandomFloat()
    {
        return GetDefaultRNG().NextFloat();
    }
    
    /**
     * @brief 生成 [min, max) 范围的随机浮点数
     */
    inline float RandomFloat(float min, float max)
    {
        return GetDefaultRNG().NextFloat(min, max);
    }
    
    /**
     * @brief 生成 [min, max] 范围的随机整数
     */
    inline int RandomInt(int min, int max)
    {
        return GetDefaultRNG().NextInt(min, max);
    }
    
    /**
     * @brief 生成随机布尔值
     * @param probability 返回 true 的概率，默认 0.5
     */
    inline bool RandomBool(float probability = 0.5f)
    {
        return GetDefaultRNG().NextBool(probability);
    }
    
    // ==================== 2D 几何采样 ====================
    
    /**
     * @brief 在单位圆内均匀采样随机点
     * @return 单位圆内的随机点
     */
    Vector2f RandomInUnitCircle();
    
    /**
     * @brief 在单位圆上均匀采样随机点
     * @return 单位圆上的随机点（长度为 1）
     */
    Vector2f RandomOnUnitCircle();
    
    /**
     * @brief 在指定 AABB2D 内均匀采样随机点
     * @param aabb 2D 包围盒
     * @return AABB 内的随机点
     */
    Vector2f RandomInAABB2D(const AABB2D &aabb);
    
    // ==================== 3D 几何采样 ====================
    
    /**
     * @brief 在单位球内均匀采样随机点
     * @return 单位球内的随机点
     */
    Vector3f RandomInUnitSphere();
    
    /**
     * @brief 在单位球面上均匀采样随机点
     * @return 单位球面上的随机点（长度为 1）
     */
    Vector3f RandomOnUnitSphere();
    
    /**
     * @brief 在指定 AABB 内均匀采样随机点
     * @param aabb 3D 包围盒
     * @return AABB 内的随机点
     */
    Vector3f RandomInAABB(const AABB &aabb);
    
    // ==================== 分布函数 ====================
    
    /**
     * @brief 生成高斯分布（正态分布）的随机数
     * 使用 Box-Muller 变换
     * @param mean 均值（默认 0.0）
     * @param stddev 标准差（默认 1.0）
     * @return 符合正态分布的随机数
     */
    float RandomGaussian(float mean = 0.0f, float stddev = 1.0f);
    
    /**
     * @brief 生成指数分布的随机数
     * @param lambda 速率参数（默认 1.0）
     * @return 符合指数分布的随机数
     */
    float RandomExponential(float lambda = 1.0f);
    
    /**
     * @brief 生成 [0, 1) 范围的随机双精度浮点数
     */
    inline double RandomDouble()
    {
        return static_cast<double>(GetDefaultRNG().NextUInt32()) / static_cast<double>(0x100000000ULL);
    }
    
    /**
     * @brief 生成 [min, max) 范围的随机双精度浮点数
     */
    inline double RandomDouble(double min, double max)
    {
        return min + RandomDouble() * (max - min);
    }
    
}//namespace hgl::math
