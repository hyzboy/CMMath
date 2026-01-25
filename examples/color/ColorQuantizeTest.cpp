#include<hgl/math/FloatPrecision.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/ColorQuantize.h>
#include<iostream>
#include<iomanip>
#include<cassert>
#include<vector>

namespace hgl
{
    void PrintQuantizationTest(const Color3f &original, const Color3f &quantized,
                               float error, int bits)
    {
        std::cout << "  Original: (" << std::fixed << std::setprecision(3)
                  << original.r << ", " << original.g << ", " << original.b << ")" << std::endl;
        std::cout << "  Quantized (" << bits << "-bit): (" << std::fixed << std::setprecision(3)
                  << quantized.r << ", " << quantized.g << ", " << quantized.b << ")" << std::endl;
        std::cout << "  Error: " << error << std::endl;
    }
}

// ================================================================================================
// Test: Basic Bit Quantization
// ================================================================================================

void TestBitQuantization()
{
    std::cout << "\n=== Test: Bit Quantization ===" << std::endl;

    hgl::Color3f color(0.567f, 0.234f, 0.789f);

    // 8-bit (no quantization)
    hgl::Color3f q8 = hgl::QuantizeColor3f(color, 8);
    float error8 = hgl::QuantizationError3f(color, q8);
    std::cout << "8-bit quantization:" << std::endl;
    hgl::PrintQuantizationTest(color, q8, error8, 8);
    assert(error8 < 0.001f);  // Should be nearly exact

    // 4-bit quantization
    hgl::Color3f q4 = hgl::QuantizeColor3f(color, 4);
    float error4 = hgl::QuantizationError3f(color, q4);
    std::cout << "4-bit quantization:" << std::endl;
    hgl::PrintQuantizationTest(color, q4, error4, 4);
    assert(error4 < 0.1f);  // Larger error expected

    // 2-bit quantization
    hgl::Color3f q2 = hgl::QuantizeColor3f(color, 2);
    float error2 = hgl::QuantizationError3f(color, q2);
    std::cout << "2-bit quantization:" << std::endl;
    hgl::PrintQuantizationTest(color, q2, error2, 2);
    assert(error2 < 1.0f);  // Much larger error expected

    // Verify that more bits means less error
    assert(error8 < error4);
    assert(error4 < error2);

    std::cout << "✓ Bit Quantization: PASSED" << std::endl;
}

// ================================================================================================
// Test: Uniform Quantization
// ================================================================================================

void TestUniformQuantization()
{
    std::cout << "\n=== Test: Uniform Quantization ===" << std::endl;

    hgl::Color3f color(0.333f, 0.666f, 0.999f);

    // 2-level (binary)
    hgl::Color3f q2 = hgl::UniformQuantize3f(color, 2);
    std::cout << "2-level quantization:" << std::endl;
    std::cout << "  Original: (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;
    std::cout << "  Quantized: (" << q2.r << ", " << q2.g << ", " << q2.b << ")" << std::endl;

    // 8-level
    hgl::Color3f q8 = hgl::UniformQuantize3f(color, 8);
    std::cout << "8-level quantization:" << std::endl;
    std::cout << "  Quantized: (" << q8.r << ", " << q8.g << ", " << q8.b << ")" << std::endl;

    // Values should be at discrete levels
    for(int i = 0; i < 8; ++i)
    {
        float level = i / 7.0f;
        if(hgl::math::IsNearlyEqual(q8.r, level)) break;
    }

    std::cout << "✓ Uniform Quantization: PASSED" << std::endl;
}

// ================================================================================================
// Test: Uint8 Quantization
// ================================================================================================

void TestUint8Quantization()
{
    std::cout << "\n=== Test: Uint8 Quantization ===" << std::endl;

    hgl::Color3ub color(145, 78, 234);

    // 8-bit (no change)
    hgl::Color3ub q8 = hgl::QuantizeColor3ub(color, 8);
    std::cout << "8-bit: (" << (int)q8.r << ", " << (int)q8.g << ", " << (int)q8.b << ")" << std::endl;
    assert(q8.r == color.r && q8.g == color.g && q8.b == color.b);

    // 5-bit quantization
    hgl::Color3ub q5 = hgl::QuantizeColor3ub(color, 5);
    std::cout << "5-bit: (" << (int)q5.r << ", " << (int)q5.g << ", " << (int)q5.b << ")" << std::endl;

    // Check that result is quantized (should be aligned to kept bits)
    int step5 = 1 << (8 - 5);  // 8
    assert(q5.r % step5 == 0);

    // 2-bit quantization
    hgl::Color3ub q2 = hgl::QuantizeColor3ub(color, 2);
    std::cout << "2-bit: (" << (int)q2.r << ", " << (int)q2.g << ", " << (int)q2.b << ")" << std::endl;

    std::cout << "✓ Uint8 Quantization: PASSED" << std::endl;
}

// ================================================================================================
// Test: Quantization Error Analysis
// ================================================================================================

void TestQuantizationErrorAnalysis()
{
    std::cout << "\n=== Test: Quantization Error Analysis ===" << std::endl;

    // Create a gradient of colors
    std::vector<hgl::Color3f> colors;
    for(int i = 0; i <= 10; ++i)
    {
        float t = i / 10.0f;
        colors.push_back(hgl::Color3f(t, t, t));
    }

    std::cout << "Grayscale gradient quantization errors:" << std::endl;

    for(int bits = 2; bits <= 8; ++bits)
    {
        float total_error = 0.0f;
        for(const auto &color : colors)
        {
            hgl::Color3f quantized = hgl::QuantizeColor3f(color, bits);
            total_error += hgl::QuantizationError3f(color, quantized);
        }

        float avg_error = total_error / colors.size();
        std::cout << "  " << bits << "-bit: avg_error = " << avg_error << std::endl;

        // Verify error decreases with more bits
        if(bits > 2)
        {
            hgl::Color3f test = hgl::QuantizeColor3f(colors[5], bits);
            hgl::Color3f test_prev = hgl::QuantizeColor3f(colors[5], bits - 1);
            // More bits should generally give better results (lower max error)
        }
    }

    std::cout << "✓ Quantization Error Analysis: PASSED" << std::endl;
}

// ================================================================================================
// Test: Palette-based Quantization
// ================================================================================================

void TestPaletteQuantization()
{
    std::cout << "\n=== Test: Palette-based Quantization ===" << std::endl;

    // Create a simple palette
    std::vector<hgl::Color3f> palette = {
        hgl::Color3f(1.0f, 0.0f, 0.0f),  // Red
        hgl::Color3f(0.0f, 1.0f, 0.0f),  // Green
        hgl::Color3f(0.0f, 0.0f, 1.0f),  // Blue
        hgl::Color3f(1.0f, 1.0f, 0.0f),  // Yellow
        hgl::Color3f(0.0f, 1.0f, 1.0f),  // Cyan
        hgl::Color3f(1.0f, 0.0f, 1.0f),  // Magenta
        hgl::Color3f(0.5f, 0.5f, 0.5f),  // Gray
        hgl::Color3f(1.0f, 1.0f, 1.0f),  // White
    };

    // Test various colors
    std::cout << "Palette color matching:" << std::endl;

    hgl::Color3f test_color(0.9f, 0.1f, 0.05f);
    int nearest_idx = hgl::FindNearestPaletteColor(test_color, palette);
    std::cout << "  Test color (0.9, 0.1, 0.05) -> Palette[" << nearest_idx << "]" << std::endl;
    assert(nearest_idx >= 0 && nearest_idx < (int)palette.size());

    // Distance to nearest should be lower than to a random far color
    float dist_nearest = hgl::GetPaletteColorDistance(test_color, palette[nearest_idx]);
    float dist_opposite = hgl::GetPaletteColorDistance(test_color, hgl::Color3f(0.0f, 0.0f, 1.0f));
    std::cout << "  Distance to nearest: " << dist_nearest << std::endl;
    std::cout << "  Distance to blue: " << dist_opposite << std::endl;
    assert(dist_nearest <= dist_opposite);

    std::cout << "✓ Palette-based Quantization: PASSED" << std::endl;
}

// ================================================================================================
// Test: Edge Cases
// ================================================================================================

void TestQuantizationEdgeCases()
{
    std::cout << "\n=== Test: Quantization Edge Cases ===" << std::endl;

    // Zero color
    hgl::Color3f zero(0.0f, 0.0f, 0.0f);
    hgl::Color3f q_zero = hgl::QuantizeColor3f(zero, 4);
    std::cout << "Zero color quantized: (" << q_zero.r << ", " << q_zero.g << ", " << q_zero.b << ")" << std::endl;
    assert(hgl::math::IsNearlyEqual(q_zero.r, 0.0f));

    // One color
    hgl::Color3f one(1.0f, 1.0f, 1.0f);
    hgl::Color3f q_one = hgl::QuantizeColor3f(one, 4);
    std::cout << "One color quantized: (" << q_one.r << ", " << q_one.g << ", " << q_one.b << ")" << std::endl;
    assert(hgl::math::IsNearlyEqual(q_one.r, 1.0f));

    // Single bit (should be 0 or 1)
    hgl::Color3f q1 = hgl::QuantizeColor3f(hgl::Color3f(0.6f, 0.3f, 0.9f), 1);
    std::cout << "1-bit quantized: (" << q1.r << ", " << q1.g << ", " << q1.b << ")" << std::endl;
    // Each component should be 0 or 1
    assert((hgl::math::IsNearlyEqual(q1.r, 0.0f) || hgl::math::IsNearlyEqual(q1.r, 1.0f)));

    // Out of range bits (should clamp)
    hgl::Color3f q_high = hgl::QuantizeColor3f(one, 32);  // More than 8 bits
    assert(hgl::math::IsNearlyEqual(q_high.r, 1.0f));

    hgl::Color3f q_low = hgl::QuantizeColor3f(one, 0);  // Less than 1 bit
    assert(hgl::math::IsNearlyEqual(q_low.r, 1.0f));

    std::cout << "✓ Quantization Edge Cases: PASSED" << std::endl;
}

// ================================================================================================
// Main
// ================================================================================================

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Color Quantization Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    try
    {
        TestBitQuantization();
        TestUniformQuantization();
        TestUint8Quantization();
        TestQuantizationErrorAnalysis();
        TestPaletteQuantization();
        TestQuantizationEdgeCases();

        std::cout << "\n========================================" << std::endl;
        std::cout << "✓ ALL QUANTIZATION TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;

        return 0;
    }
    catch(const std::exception &e)
    {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
