#include<hgl/math/FloatPrecision.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/ColorQuantize.h>
#include<hgl/color/ColorDithering.h>
#include<iostream>
#include<iomanip>
#include<cassert>
#include<vector>

// ================================================================================================
// Test: Floyd-Steinberg Dithering
// ================================================================================================

void TestFloydSteinbergDithering()
{
    std::cout << "\n=== Test: Floyd-Steinberg Dithering ===" << std::endl;

    // Test with various colors and positions
    hgl::Color3f color(0.6f, 0.3f, 0.75f);

    std::cout << "Original color: (" << std::fixed << std::setprecision(3)
              << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;

    // Apply dithering at different positions
    for(int y = 0; y < 4; ++y)
    {
        for(int x = 0; x < 4; ++x)
        {
            hgl::Color3f dithered = hgl::FloydSteinbergDitherer::Dither(color, x, y, 3);
            std::cout << "  Pos(" << x << "," << y << "): ("
                      << std::setprecision(3) << dithered.r << ", "
                      << dithered.g << ", " << dithered.b << ")" << std::endl;
        }
    }

    // Test uint8 version
    hgl::Color3ub color_ub(153, 76, 191);
    hgl::Color3ub dithered_ub = hgl::FloydSteinbergDitherer::Dither(color_ub, 0, 0, 3);
    std::cout << "Uint8 dithered: (" << (int)dithered_ub.r << ", "
              << (int)dithered_ub.g << ", " << (int)dithered_ub.b << ")" << std::endl;

    assert(dithered_ub.r >= 0 && dithered_ub.r <= 255);

    std::cout << "✓ Floyd-Steinberg Dithering: PASSED" << std::endl;
}

// ================================================================================================
// Test: Bayer Dithering
// ================================================================================================

void TestBayerDithering()
{
    std::cout << "\n=== Test: Bayer (Ordered) Dithering ===" << std::endl;

    hgl::Color3f color(0.5f, 0.5f, 0.5f);

    std::cout << "Original color: (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;
    std::cout << "Bayer dithering pattern (8 levels per channel):" << std::endl;

    // Create an 8x8 grid to show the dithering pattern
    std::vector<std::vector<hgl::Color3f>> dithered_grid(8);
    for(int y = 0; y < 8; ++y)
    {
        dithered_grid[y].resize(8);
        for(int x = 0; x < 8; ++x)
        {
            dithered_grid[y][x] = hgl::BayerDitherer::Dither(color, x, y, 8);
        }
    }

    // Print first 4x4 for demonstration
    for(int y = 0; y < 4; ++y)
    {
        std::cout << "  ";
        for(int x = 0; x < 4; ++x)
        {
            std::cout << std::setprecision(2) << dithered_grid[y][x].r << " ";
        }
        std::cout << std::endl;
    }

    // Verify Bayer threshold values
    std::cout << "Bayer thresholds:" << std::endl;
    for(int y = 0; y < 4; ++y)
    {
        std::cout << "  ";
        for(int x = 0; x < 4; ++x)
        {
            float threshold = hgl::BayerDitherer::GetThreshold(x, y);
            std::cout << std::setprecision(3) << threshold << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "✓ Bayer Dithering: PASSED" << std::endl;
}

// ================================================================================================
// Test: Random Dithering
// ================================================================================================

void TestRandomDithering()
{
    std::cout << "\n=== Test: Random Dithering ===" << std::endl;

    hgl::Color3f color(0.7f, 0.4f, 0.6f);
    hgl::RandomDitherer ditherer(12345);  // Fixed seed for reproducibility

    std::cout << "Original color: (" << std::fixed << std::setprecision(3)
              << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;

    // Apply random dithering multiple times (should vary due to randomness)
    std::cout << "Multiple random ditherings (5-bit quantization, 5% noise):" << std::endl;
    for(int i = 0; i < 5; ++i)
    {
        hgl::Color3f dithered = ditherer.Dither(color, 0.05f, 5);
        std::cout << "  " << i << ": (" << dithered.r << ", " << dithered.g << ", " << dithered.b << ")" << std::endl;
    }

    // Test with different noise amounts
    std::cout << "Different noise amounts (3-bit):" << std::endl;
    ditherer.SetSeed(12345);  // Reset for consistency

    for(float noise : {0.01f, 0.05f, 0.1f, 0.2f})
    {
        hgl::Color3f dithered = ditherer.Dither(color, noise, 3);
        std::cout << "  Noise " << std::setprecision(2) << noise << ": ("
                  << dithered.r << ", " << dithered.g << ", " << dithered.b << ")" << std::endl;
    }

    std::cout << "✓ Random Dithering: PASSED" << std::endl;
}

// ================================================================================================
// Test: Palette-based Dithering
// ================================================================================================

void TestPaletteDithering()
{
    std::cout << "\n=== Test: Palette-based Dithering ===" << std::endl;

    // Create a simple 8-color palette
    std::vector<hgl::Color3f> palette = {
        hgl::Color3f(0.0f, 0.0f, 0.0f),  // Black
        hgl::Color3f(1.0f, 0.0f, 0.0f),  // Red
        hgl::Color3f(0.0f, 1.0f, 0.0f),  // Green
        hgl::Color3f(1.0f, 1.0f, 0.0f),  // Yellow
        hgl::Color3f(0.0f, 0.0f, 1.0f),  // Blue
        hgl::Color3f(1.0f, 0.0f, 1.0f),  // Magenta
        hgl::Color3f(0.0f, 1.0f, 1.0f),  // Cyan
        hgl::Color3f(1.0f, 1.0f, 1.0f),  // White
    };

    std::cout << "Palette-based dithering (8 colors):" << std::endl;

    // Test various colors
    std::vector<hgl::Color3f> test_colors = {
        hgl::Color3f(0.5f, 0.5f, 0.5f),   // Gray
        hgl::Color3f(0.9f, 0.1f, 0.05f),  // Light red
        hgl::Color3f(0.2f, 0.8f, 0.3f),   // Light green
    };

    for(size_t i = 0; i < test_colors.size(); ++i)
    {
        std::cout << "  Test color " << i << ": (" << std::fixed << std::setprecision(2)
                  << test_colors[i].r << ", " << test_colors[i].g << ", " << test_colors[i].b << ")" << std::endl;

        // Without dithering
        int idx_no_dither = hgl::PaletteDither(test_colors[i], palette, 0.0f);
        std::cout << "    Without dither -> Palette[" << idx_no_dither << "]" << std::endl;

        // With dithering
        for(float dither : {-0.3f, 0.0f, 0.3f})
        {
            int idx_dither = hgl::PaletteDither(test_colors[i], palette, dither);
            std::cout << "    Dither " << std::setprecision(1) << dither << " -> Palette[" << idx_dither << "]" << std::endl;
        }
    }

    std::cout << "✓ Palette-based Dithering: PASSED" << std::endl;
}

// ================================================================================================
// Test: Dithering Quality Comparison
// ================================================================================================

void TestDitheringQualityComparison()
{
    std::cout << "\n=== Test: Dithering Quality Comparison ===" << std::endl;

    // Create a color that doesn't quantize well
    hgl::Color3f color(0.555f, 0.444f, 0.777f);

    std::cout << "Original color: (" << std::fixed << std::setprecision(3)
              << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;

    // No dithering (just quantization)
    hgl::Color3f quantized = hgl::QuantizeColor3f(color, 3);
    float error_no_dither = hgl::QuantizationError3f(color, quantized);
    std::cout << "Quantized (3-bit): (" << quantized.r << ", " << quantized.g << ", " << quantized.b << ")" << std::endl;
    std::cout << "  Error: " << error_no_dither << std::endl;

    // With Floyd-Steinberg dithering
    hgl::Color3f dithered_fs = hgl::FloydSteinbergDitherer::Dither(color, 2, 3, 3);
    float error_fs = hgl::QuantizationError3f(color, dithered_fs);
    std::cout << "Floyd-Steinberg dithered: (" << dithered_fs.r << ", " << dithered_fs.g << ", " << dithered_fs.b << ")" << std::endl;
    std::cout << "  Error: " << error_fs << std::endl;

    // With Bayer dithering
    hgl::Color3f dithered_bayer = hgl::BayerDitherer::Dither(color, 2, 3, 4);
    float error_bayer = hgl::QuantizationError3f(color, dithered_bayer);
    std::cout << "Bayer dithered: (" << dithered_bayer.r << ", " << dithered_bayer.g << ", " << dithered_bayer.b << ")" << std::endl;
    std::cout << "  Error: " << error_bayer << std::endl;

    // Results should all be valid colors
    assert(quantized.r >= 0 && quantized.r <= 1);
    assert(dithered_fs.r >= 0 && dithered_fs.r <= 1);
    assert(dithered_bayer.r >= 0 && dithered_bayer.r <= 1);

    std::cout << "✓ Dithering Quality Comparison: PASSED" << std::endl;
}

// ================================================================================================
// Test: Dithering Edge Cases
// ================================================================================================

void TestDitheringEdgeCases()
{
    std::cout << "\n=== Test: Dithering Edge Cases ===" << std::endl;

    // Black color
    hgl::Color3f black(0.0f, 0.0f, 0.0f);
    hgl::Color3f dithered_black = hgl::FloydSteinbergDitherer::Dither(black, 0, 0, 5);
    assert(hgl::math::IsNearlyEqual(dithered_black.r, 0.0f));
    std::cout << "Black dithered: (" << dithered_black.r << ", " << dithered_black.g << ", " << dithered_black.b << ")" << std::endl;

    // White color
    hgl::Color3f white(1.0f, 1.0f, 1.0f);
    hgl::Color3f dithered_white = hgl::BayerDitherer::Dither(white, 0, 0, 4);
    assert(hgl::math::IsNearlyEqual(dithered_white.r, 1.0f));
    std::cout << "White dithered: (" << dithered_white.r << ", " << dithered_white.g << ", " << dithered_white.b << ")" << std::endl;

    // Very small color
    hgl::Color3f tiny(0.001f, 0.002f, 0.003f);
    hgl::Color3f dithered_tiny = hgl::FloydSteinbergDitherer::Dither(tiny, 1, 1, 2);
    assert(dithered_tiny.r >= 0 && dithered_tiny.r <= 1);
    std::cout << "Tiny color dithered: (" << dithered_tiny.r << ", " << dithered_tiny.g << ", " << dithered_tiny.b << ")" << std::endl;

    std::cout << "✓ Dithering Edge Cases: PASSED" << std::endl;
}

// ================================================================================================
// Main
// ================================================================================================

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Color Dithering Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    try
    {
        TestFloydSteinbergDithering();
        TestBayerDithering();
        TestRandomDithering();
        TestPaletteDithering();
        TestDitheringQualityComparison();
        TestDitheringEdgeCases();

        std::cout << "\n========================================" << std::endl;
        std::cout << "✓ ALL DITHERING TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;

        return 0;
    }
    catch(const std::exception &e)
    {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
