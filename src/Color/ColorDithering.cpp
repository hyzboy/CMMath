#include<hgl/color/ColorDithering.h>
#include<algorithm>
#include<cmath>
#include<random>

namespace hgl
{
    //==================================================================================================
    // Anonymous Namespace for Hidden Implementation Details
    //==================================================================================================
    namespace
    {
        // Bayer matrix (4x4) - normalized to [0, 1]
        constexpr float BAYER_4x4[4][4] = {
            {  0.0f / 16.0f,  8.0f / 16.0f,  2.0f / 16.0f, 10.0f / 16.0f },
            { 12.0f / 16.0f,  4.0f / 16.0f, 14.0f / 16.0f,  6.0f / 16.0f },
            {  3.0f / 16.0f, 11.0f / 16.0f,  1.0f / 16.0f,  9.0f / 16.0f },
            { 15.0f / 16.0f,  7.0f / 16.0f, 13.0f / 16.0f,  5.0f / 16.0f }
        };
    } // anonymous namespace

    //==================================================================================================
    // RandomDitherer Implementation Details (Pimpl)
    //==================================================================================================

    class RandomDitherer::RandomImpl
    {
    public:
        std::mt19937 generator;
        std::uniform_real_distribution<float> distribution{0.0f, 1.0f};

        RandomImpl(uint32_t seed)
            : generator(seed == 0 ? std::random_device{}() : seed)
        {
        }
    };


    //==================================================================================================
    // Floyd-Steinberg Dithering Implementation
    //==================================================================================================

    Color3f FloydSteinbergDitherer::Dither(const Color3f &original, int error_x, int error_y, int bits)
    {
        // Add pseudo-random error based on position
        float dither_pattern = 0.0f;
        dither_pattern += (error_x * 7 + error_y * 13) % 256 / 256.0f;

        Color3f dithered = original;
        dithered.r += (dither_pattern - 0.5f) * 0.1f;
        dithered.g += (dither_pattern - 0.5f) * 0.1f;
        dithered.b += (dither_pattern - 0.5f) * 0.1f;

        // Clamp to valid range
        dithered.r = std::max(0.0f, std::min(1.0f, dithered.r));
        dithered.g = std::max(0.0f, std::min(1.0f, dithered.g));
        dithered.b = std::max(0.0f, std::min(1.0f, dithered.b));

        // Quantize after dithering
        return QuantizeColor3f(dithered, bits);
    }

    Color3ub FloydSteinbergDitherer::Dither(const Color3ub &original, int error_x, int error_y, int bits)
    {
        Color3f original_f(original.r / 255.0f, original.g / 255.0f, original.b / 255.0f);
        Color3f dithered_f = Dither(original_f, error_x, error_y, bits);
        return Color3ub(
            uint8(dithered_f.r * 255.0f),
            uint8(dithered_f.g * 255.0f),
            uint8(dithered_f.b * 255.0f)
        );
    }

    //==================================================================================================
    // Bayer Dithering Implementation
    //==================================================================================================

    Color3f BayerDitherer::Dither(const Color3f &original, int x, int y, int levels)
    {
        if(levels < 2) levels = 2;
        if(levels > 256) levels = 256;

        float threshold = BAYER_4x4[y & 3][x & 3];

        Color3f result;

        // 正确算法：将 [0,1] 映射到 [0, levels-1]，然后根据阈值舍入
        float r_scaled = original.r * (levels - 1);
        float g_scaled = original.g * (levels - 1);
        float b_scaled = original.b * (levels - 1);

        float r_floor = std::floor(r_scaled);
        float g_floor = std::floor(g_scaled);
        float b_floor = std::floor(b_scaled);

        // 小数部分与阈值比较，决定向上还是向下舍入
        result.r = ((r_scaled - r_floor) > threshold ? r_floor + 1.0f : r_floor) / (levels - 1);
        result.g = ((g_scaled - g_floor) > threshold ? g_floor + 1.0f : g_floor) / (levels - 1);
        result.b = ((b_scaled - b_floor) > threshold ? b_floor + 1.0f : b_floor) / (levels - 1);

        // Clamp to valid range
        result.r = std::max(0.0f, std::min(1.0f, result.r));
        result.g = std::max(0.0f, std::min(1.0f, result.g));
        result.b = std::max(0.0f, std::min(1.0f, result.b));

        return result;
    }

    Color3ub BayerDitherer::Dither(const Color3ub &original, int x, int y, int levels)
    {
        Color3f original_f(original.r / 255.0f, original.g / 255.0f, original.b / 255.0f);
        Color3f dithered_f = Dither(original_f, x, y, levels);
        return Color3ub(
            uint8(dithered_f.r * 255.0f),
            uint8(dithered_f.g * 255.0f),
            uint8(dithered_f.b * 255.0f)
        );
    }

    float BayerDitherer::GetThreshold(int x, int y)
    {
        return BAYER_4x4[y & 3][x & 3];
    }

    //==================================================================================================
    // Random Dithering Implementation
    //==================================================================================================

    RandomDitherer::RandomDitherer(uint32_t seed)
        : pImpl(new RandomImpl(seed))
    {
    }

    RandomDitherer::~RandomDitherer()
    {
        delete pImpl;
    }

    Color3f RandomDitherer::Dither(const Color3f &original, float noise_amount, int bits)
    {
        float noise_r = (pImpl->distribution(pImpl->generator) - 0.5f) * 2.0f * noise_amount;
        float noise_g = (pImpl->distribution(pImpl->generator) - 0.5f) * 2.0f * noise_amount;
        float noise_b = (pImpl->distribution(pImpl->generator) - 0.5f) * 2.0f * noise_amount;

        Color3f dithered(
            std::max(0.0f, std::min(1.0f, original.r + noise_r)),
            std::max(0.0f, std::min(1.0f, original.g + noise_g)),
            std::max(0.0f, std::min(1.0f, original.b + noise_b))
        );

        return QuantizeColor3f(dithered, bits);
    }

    Color3ub RandomDitherer::Dither(const Color3ub &original, float noise_amount, int bits)
    {
        Color3f original_f(original.r / 255.0f, original.g / 255.0f, original.b / 255.0f);
        Color3f dithered_f = Dither(original_f, noise_amount, bits);
        return Color3ub(
            uint8(dithered_f.r * 255.0f),
            uint8(dithered_f.g * 255.0f),
            uint8(dithered_f.b * 255.0f)
        );
    }

    void RandomDitherer::SetSeed(uint32_t seed)
    {
        pImpl->generator.seed(seed == 0 ? std::random_device{}() : seed);
    }

    //==================================================================================================
    // Palette-based Dithering Implementation
    //==================================================================================================

    int PaletteDither(const Color3f &original, const std::vector<Color3f> &palette,
                     float dither_pattern)
    {
        Color3f dithered = original;
        dithered.r += dither_pattern * 0.1f;
        dithered.g += dither_pattern * 0.1f;
        dithered.b += dither_pattern * 0.1f;

        dithered.r = std::max(0.0f, std::min(1.0f, dithered.r));
        dithered.g = std::max(0.0f, std::min(1.0f, dithered.g));
        dithered.b = std::max(0.0f, std::min(1.0f, dithered.b));

        return FindNearestPaletteColor(dithered, palette);
    }

} // namespace hgl
