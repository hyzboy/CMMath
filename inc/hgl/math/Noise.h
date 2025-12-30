/**
 * Noise.h - 噪声函数库
 * 
 * 实现程序化生成和特效必备的噪声函数，包括：
 * - Perlin 噪声 (1D, 2D, 3D) 及分形布朗运动 (fBm)
 * - Simplex 噪声 (2D, 3D, 4D) 及分形布朗运动
 * - 噪声效果函数 (湍流、山脊、云团)
 * - 域变形和噪声重映射工具
 * 
 * 参考资料：
 * - Ken Perlin: "Improved Noise" (2002)
 * - Stefan Gustavson: "Simplex noise demystified"
 * - Inigo Quilez: https://iquilezles.org/articles/
 */

#pragma once

#include<hgl/math/Vector.h>

namespace hgl::math
{
    // ==================== Perlin Noise ====================
    
    /**
     * 1D Perlin 噪声
     * @param x 采样位置
     * @return 噪声值范围 [-1, 1]
     */
    float PerlinNoise1D(float x);
    
    /**
     * 2D Perlin 噪声
     * @param x, y 采样位置
     * @return 噪声值范围 [-1, 1]
     */
    float PerlinNoise2D(float x, float y);
    float PerlinNoise2D(const Vector2f &pos);
    
    /**
     * 3D Perlin 噪声
     * @param x, y, z 采样位置
     * @return 噪声值范围 [-1, 1]
     */
    float PerlinNoise3D(float x, float y, float z);
    float PerlinNoise3D(const Vector3f &pos);
    
    /**
     * 分形布朗运动 (fBm) - Perlin 噪声的多层叠加
     * @param pos 采样位置
     * @param octaves 层数（建议 3-8）
     * @param lacunarity 频率倍增系数（建议 2.0）
     * @param gain 振幅衰减系数（建议 0.5）
     * @return 噪声值范围 [-1, 1]
     */
    float PerlinFBM2D(const Vector2f &pos, int octaves = 4, float lacunarity = 2.0f, float gain = 0.5f);
    float PerlinFBM3D(const Vector3f &pos, int octaves = 4, float lacunarity = 2.0f, float gain = 0.5f);
    
    // ==================== Simplex Noise ====================
    
    /**
     * 2D Simplex 噪声（比 Perlin 更快）
     * @param x, y 采样位置
     * @return 噪声值范围 [-1, 1]
     */
    float SimplexNoise2D(float x, float y);
    float SimplexNoise2D(const Vector2f &pos);
    
    /**
     * 3D Simplex 噪声
     * @param x, y, z 采样位置
     * @return 噪声值范围 [-1, 1]
     */
    float SimplexNoise3D(float x, float y, float z);
    float SimplexNoise3D(const Vector3f &pos);
    
    /**
     * 4D Simplex 噪声（用于时间动画）
     * @param x, y, z, w 采样位置
     * @return 噪声值范围 [-1, 1]
     */
    float SimplexNoise4D(float x, float y, float z, float w);
    float SimplexNoise4D(const Vector3f &pos, float time);
    
    /**
     * Simplex 分形布朗运动
     */
    float SimplexFBM2D(const Vector2f &pos, int octaves = 4, float lacunarity = 2.0f, float gain = 0.5f);
    float SimplexFBM3D(const Vector3f &pos, int octaves = 4, float lacunarity = 2.0f, float gain = 0.5f);
    
    // ==================== 噪声效果函数 ====================
    
    /**
     * 湍流噪声（绝对值叠加，产生湍流效果）
     * @param pos 采样位置
     * @param octaves 层数
     * @return 噪声值范围 [0, 1]
     */
    float TurbulenceNoise2D(const Vector2f &pos, int octaves = 4);
    float TurbulenceNoise3D(const Vector3f &pos, int octaves = 4);
    
    /**
     * 山脊噪声（反转后取绝对值，产生山脊效果）
     * @param pos 采样位置
     * @param octaves 层数
     * @return 噪声值范围 [0, 1]
     */
    float RidgeNoise2D(const Vector2f &pos, int octaves = 4);
    float RidgeNoise3D(const Vector3f &pos, int octaves = 4);
    
    /**
     * 云团噪声（平方后叠加，产生柔和云团）
     * @param pos 采样位置
     * @param octaves 层数
     * @return 噪声值范围 [0, 1]
     */
    float BillowNoise2D(const Vector2f &pos, int octaves = 4);
    float BillowNoise3D(const Vector3f &pos, int octaves = 4);
    
    // ==================== 噪声工具函数 ====================
    
    /**
     * 域变形（Domain Warping）
     * 通过噪声扭曲采样位置，产生复杂图案
     */
    float DomainWarpedNoise2D(const Vector2f &pos, float warp_strength = 1.0f);
    float DomainWarpedNoise3D(const Vector3f &pos, float warp_strength = 1.0f);
    
    /**
     * 噪声重映射到 [0, 1] 范围
     */
    inline float RemapNoise01(float noise) { return noise * 0.5f + 0.5f; }
    
    /**
     * 噪声重映射到自定义范围
     */
    inline float RemapNoise(float noise, float min, float max) 
    { 
        return min + (noise * 0.5f + 0.5f) * (max - min); 
    }
}//namespace hgl::math
