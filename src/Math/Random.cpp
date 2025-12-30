/**
 * Random.cpp - 随机数生成工具实现
 * 
 * PCG 算法和随机采样函数的实现。
 */

#include <hgl/math/Random.h>
#include <hgl/math/geometry/AABB.h>
#include <hgl/math/geometry/AABB2D.h>
#include <chrono>
#include <cmath>

namespace hgl::math
{
    // ==================== PCG 常量 ====================
    
    // PCG 算法的魔数
    constexpr uint64_t PCG_DEFAULT_MULTIPLIER = 6364136223846793005ULL;
    constexpr uint64_t PCG_DEFAULT_INCREMENT = 1442695040888963407ULL;
    
    // ==================== RandomGenerator 实现 ====================
    
    RandomGenerator::RandomGenerator()
        : state(0x853c49e6748fea9bULL)
        , increment(PCG_DEFAULT_INCREMENT)
    {
        // 使用当前时间作为种子
        auto now = std::chrono::high_resolution_clock::now();
        auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        Seed(static_cast<uint64_t>(nanos));
    }
    
    RandomGenerator::RandomGenerator(uint64_t seed)
        : state(0)
        , increment(PCG_DEFAULT_INCREMENT)
    {
        Seed(seed);
    }
    
    RandomGenerator::RandomGenerator(uint64_t seed, uint64_t sequence)
        : state(0)
        , increment((sequence << 1u) | 1u)
    {
        Seed(seed, sequence);
    }
    
    void RandomGenerator::Seed(uint64_t seed)
    {
        state = 0;
        increment = PCG_DEFAULT_INCREMENT;
        NextUInt32();  // 推进一次
        state += seed;
        NextUInt32();  // 推进一次
    }
    
    void RandomGenerator::Seed(uint64_t seed, uint64_t sequence)
    {
        state = 0;
        increment = (sequence << 1u) | 1u;
        NextUInt32();
        state += seed;
        NextUInt32();
    }
    
    uint32_t RandomGenerator::NextUInt32()
    {
        uint64_t old_state = state;
        
        // 推进状态（LCG）
        state = old_state * PCG_DEFAULT_MULTIPLIER + increment;
        
        // 计算输出（XSH-RR）
        uint32_t xorshifted = static_cast<uint32_t>(((old_state >> 18u) ^ old_state) >> 27u);
        uint32_t rot = static_cast<uint32_t>(old_state >> 59u);
        
        return (xorshifted >> rot) | (xorshifted << ((~rot + 1u) & 31));
    }
    
    uint64_t RandomGenerator::NextUInt64()
    {
        uint64_t high = static_cast<uint64_t>(NextUInt32());
        uint64_t low = static_cast<uint64_t>(NextUInt32());
        return (high << 32) | low;
    }
    
    float RandomGenerator::NextFloat()
    {
        // 使用高 24 位生成 [0, 1) 范围的浮点数
        return static_cast<float>(NextUInt32() >> 8) / static_cast<float>(1 << 24);
    }
    
    float RandomGenerator::NextFloat(float min, float max)
    {
        return min + NextFloat() * (max - min);
    }
    
    int RandomGenerator::NextInt(int min, int max)
    {
        if (min >= max)
            return min;
        
        uint32_t range = static_cast<uint32_t>(max - min + 1);
        uint32_t threshold = (~range + 1u) % range;
        
        // 使用拒绝采样避免模偏差
        uint32_t result;
        do {
            result = NextUInt32();
        } while (result < threshold);
        
        return min + static_cast<int>(result % range);
    }
    
    bool RandomGenerator::NextBool(float probability)
    {
        return NextFloat() < probability;
    }
    
    // ==================== 全局默认生成器 ====================
    
    RandomGenerator& GetDefaultRNG()
    {
        thread_local RandomGenerator default_rng;
        return default_rng;
    }
    
    void SeedDefaultRNG(uint64_t seed)
    {
        GetDefaultRNG().Seed(seed);
    }
    
    // ==================== 2D 几何采样 ====================
    
    Vector2f RandomInUnitCircle()
    {
        // 使用拒绝采样
        RandomGenerator& rng = GetDefaultRNG();
        Vector2f point;
        float dist_squared;
        
        do {
            point.x = rng.NextFloat(-1.0f, 1.0f);
            point.y = rng.NextFloat(-1.0f, 1.0f);
            dist_squared = point.x * point.x + point.y * point.y;
        } while (dist_squared > 1.0f);
        
        return point;
    }
    
    Vector2f RandomOnUnitCircle()
    {
        RandomGenerator& rng = GetDefaultRNG();
        float angle = rng.NextFloat(0.0f, 2.0f * pi);
        return Vector2f(std::cos(angle), std::sin(angle));
    }
    
    Vector2f RandomInAABB2D(const AABB2D &aabb)
    {
        RandomGenerator& rng = GetDefaultRNG();
        return Vector2f(
            rng.NextFloat(aabb.min.x, aabb.max.x),
            rng.NextFloat(aabb.min.y, aabb.max.y)
        );
    }
    
    // ==================== 3D 几何采样 ====================
    
    Vector3f RandomInUnitSphere()
    {
        // 使用拒绝采样
        RandomGenerator& rng = GetDefaultRNG();
        Vector3f point;
        float dist_squared;
        
        do {
            point.x = rng.NextFloat(-1.0f, 1.0f);
            point.y = rng.NextFloat(-1.0f, 1.0f);
            point.z = rng.NextFloat(-1.0f, 1.0f);
            dist_squared = point.x * point.x + point.y * point.y + point.z * point.z;
        } while (dist_squared > 1.0f);
        
        return point;
    }
    
    Vector3f RandomOnUnitSphere()
    {
        // Marsaglia 方法生成均匀分布的单位球面点
        RandomGenerator& rng = GetDefaultRNG();
        float z = rng.NextFloat(-1.0f, 1.0f);
        float phi = rng.NextFloat(0.0f, 2.0f * pi);
        float r = std::sqrt(1.0f - z * z);
        
        return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
    }
    
    Vector3f RandomInAABB(const AABB &aabb)
    {
        RandomGenerator& rng = GetDefaultRNG();
        const Vector3f& min = aabb.GetMin();
        const Vector3f& max = aabb.GetMax();
        
        return Vector3f(
            rng.NextFloat(min.x, max.x),
            rng.NextFloat(min.y, max.y),
            rng.NextFloat(min.z, max.z)
        );
    }
    
    // ==================== 分布函数 ====================
    
    float RandomGaussian(float mean, float stddev)
    {
        // Box-Muller 变换
        RandomGenerator& rng = GetDefaultRNG();
        
        static bool has_spare = false;
        static float spare;
        
        if (has_spare)
        {
            has_spare = false;
            return mean + stddev * spare;
        }
        
        has_spare = true;
        
        float u1, u2;
        do {
            u1 = rng.NextFloat();
            u2 = rng.NextFloat();
        } while (u1 <= 1e-7f);  // 避免 log(0)
        
        float radius = std::sqrt(-2.0f * std::log(u1));
        float theta = 2.0f * pi * u2;
        
        spare = radius * std::sin(theta);
        return mean + stddev * radius * std::cos(theta);
    }
    
    float RandomExponential(float lambda)
    {
        RandomGenerator& rng = GetDefaultRNG();
        float u = rng.NextFloat();
        
        // 避免 log(0)
        while (u <= 1e-7f)
        {
            u = rng.NextFloat();
        }
        
        return -std::log(u) / lambda;
    }
    
}//namespace hgl::math
