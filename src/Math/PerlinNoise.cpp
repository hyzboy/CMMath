/**
 * PerlinNoise.cpp - Perlin 噪声实现
 * 
 * 基于 Ken Perlin 的改进噪声算法 (2002)
 * 使用梯度向量和置换表实现可重复的伪随机噪声
 * 
 * 参考：
 * - Ken Perlin: "Improved Noise" reference implementation
 * - https://mrl.cs.nyu.edu/~perlin/noise/
 */

#include<hgl/math/Noise.h>
#include<cmath>

namespace hgl::math
{
    // 置换表 - 用于伪随机数生成，可重复性保证
    // 这是 Ken Perlin 原始实现中使用的排列
    static const int permutation[256] = {
        151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
        8,99,37,240,21,10,23,190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
        35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,
        134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
        55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,73,209,76,132,187,208,89,
        18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,
        250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
        189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,221,153,101,155,167,43,
        172,9,129,22,39,253,19,98,108,110,79,113,224,232,178,185,112,104,218,246,97,
        228,251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,
        107,49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,127,4,150,254,
        138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
    };

    // 扩展置换表到 512，避免索引溢出
    static int p[512];
    static bool permutation_initialized = false;

    // 初始化扩展置换表
    static void InitializePermutation()
    {
        if (!permutation_initialized)
        {
            for (int i = 0; i < 256; i++)
                p[i] = p[256 + i] = permutation[i];
            permutation_initialized = true;
        }
    }

    // 淡入淡出函数 - 6t^5 - 15t^4 + 10t^3 (改进的平滑插值)
    static inline float Fade(float t)
    {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    // 线性插值
    static inline float Lerp(float t, float a, float b)
    {
        return a + t * (b - a);
    }

    // 1D 梯度函数
    static inline float Grad1D(int hash, float x)
    {
        // 使用 hash 的最低位决定梯度方向
        return (hash & 1) ? -x : x;
    }

    // 2D 梯度函数
    static inline float Grad2D(int hash, float x, float y)
    {
        // 使用 hash 选择 8 个梯度方向之一
        int h = hash & 7;
        float u = h < 4 ? x : y;
        float v = h < 4 ? y : x;
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
    }

    // 3D 梯度函数
    static inline float Grad3D(int hash, float x, float y, float z)
    {
        // 使用 hash 选择 12 个梯度方向之一
        int h = hash & 15;
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : (h == 12 || h == 14) ? x : z;
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }

    // ==================== 1D Perlin Noise ====================

    float PerlinNoise1D(float x)
    {
        InitializePermutation();

        // 计算单元格坐标
        int X = static_cast<int>(std::floor(x)) & 255;
        
        // 计算单元格内相对位置
        x -= std::floor(x);
        
        // 计算淡入淡出曲线
        float u = Fade(x);
        
        // 获取梯度索引
        int A = p[X];
        int B = p[X + 1];
        
        // 线性插值
        return Lerp(u, Grad1D(A, x), Grad1D(B, x - 1.0f));
    }

    // ==================== 2D Perlin Noise ====================

    float PerlinNoise2D(float x, float y)
    {
        InitializePermutation();

        // 计算单元格坐标
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        
        // 计算单元格内相对位置
        x -= std::floor(x);
        y -= std::floor(y);
        
        // 计算淡入淡出曲线
        float u = Fade(x);
        float v = Fade(y);
        
        // 获取梯度索引
        int A = p[X] + Y;
        int B = p[X + 1] + Y;
        
        // 双线性插值
        return Lerp(v,
            Lerp(u, Grad2D(p[A], x, y), Grad2D(p[B], x - 1.0f, y)),
            Lerp(u, Grad2D(p[A + 1], x, y - 1.0f), Grad2D(p[B + 1], x - 1.0f, y - 1.0f))
        );
    }

    float PerlinNoise2D(const Vector2f &pos)
    {
        return PerlinNoise2D(pos.x, pos.y);
    }

    // ==================== 3D Perlin Noise ====================

    float PerlinNoise3D(float x, float y, float z)
    {
        InitializePermutation();

        // 计算单元格坐标
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;
        
        // 计算单元格内相对位置
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);
        
        // 计算淡入淡出曲线
        float u = Fade(x);
        float v = Fade(y);
        float w = Fade(z);
        
        // 获取梯度索引
        int A = p[X] + Y;
        int AA = p[A] + Z;
        int AB = p[A + 1] + Z;
        int B = p[X + 1] + Y;
        int BA = p[B] + Z;
        int BB = p[B + 1] + Z;
        
        // 三线性插值
        return Lerp(w,
            Lerp(v,
                Lerp(u, Grad3D(p[AA], x, y, z), Grad3D(p[BA], x - 1.0f, y, z)),
                Lerp(u, Grad3D(p[AB], x, y - 1.0f, z), Grad3D(p[BB], x - 1.0f, y - 1.0f, z))
            ),
            Lerp(v,
                Lerp(u, Grad3D(p[AA + 1], x, y, z - 1.0f), Grad3D(p[BA + 1], x - 1.0f, y, z - 1.0f)),
                Lerp(u, Grad3D(p[AB + 1], x, y - 1.0f, z - 1.0f), Grad3D(p[BB + 1], x - 1.0f, y - 1.0f, z - 1.0f))
            )
        );
    }

    float PerlinNoise3D(const Vector3f &pos)
    {
        return PerlinNoise3D(pos.x, pos.y, pos.z);
    }

    // ==================== 分形布朗运动 (fBm) ====================

    float PerlinFBM2D(const Vector2f &pos, int octaves, float lacunarity, float gain)
    {
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float result = 0.0f;
        float max_value = 0.0f;  // 用于归一化

        for (int i = 0; i < octaves; i++)
        {
            result += PerlinNoise2D(pos.x * frequency, pos.y * frequency) * amplitude;
            max_value += amplitude;
            
            amplitude *= gain;
            frequency *= lacunarity;
        }

        // 归一化到 [-1, 1]
        return result / max_value;
    }

    float PerlinFBM3D(const Vector3f &pos, int octaves, float lacunarity, float gain)
    {
        float amplitude = 1.0f;
        float frequency = 1.0f;
        float result = 0.0f;
        float max_value = 0.0f;

        for (int i = 0; i < octaves; i++)
        {
            result += PerlinNoise3D(pos.x * frequency, pos.y * frequency, pos.z * frequency) * amplitude;
            max_value += amplitude;
            
            amplitude *= gain;
            frequency *= lacunarity;
        }

        return result / max_value;
    }

}//namespace hgl::math
