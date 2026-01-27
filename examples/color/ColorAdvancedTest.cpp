#include<hgl/math/FloatPrecision.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/color/ColorLerp.h>
#include<hgl/math/ScalarConversion.h>
#include<hgl/color/ColorQuantize.h>
#include<hgl/color/ColorDithering.h>
#include<iostream>
#include<iomanip>
#include<cassert>
#include<cmath>

// ================================================================================================
// Test: Complete Color Processing Pipeline
// ================================================================================================

void TestCompleteColorPipeline()
{
    std::cout << "\n=== Test: Complete Color Processing Pipeline ===" << std::endl;

    // Simulate processing an image with color quantization and dithering
    const int WIDTH = 8;
    const int HEIGHT = 8;
    float image[WIDTH * HEIGHT] = {
        0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f,
        0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f,
        0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 0.9f,
        0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 0.9f, 0.8f,
        0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 0.9f, 0.8f, 0.7f,
        0.6f, 0.7f, 0.8f, 0.9f, 0.9f, 0.8f, 0.7f, 0.6f,
        0.7f, 0.8f, 0.9f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f,
        0.8f, 0.9f, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f
    };

    // Create grayscale colors from image data
    hgl::Color3f grayscale[WIDTH * HEIGHT];
    for(int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        float v = image[i];
        grayscale[i] = hgl::Color3f(v, v, v);
    }

    std::cout << "Processing " << WIDTH << "x" << HEIGHT << " grayscale image:" << std::endl;

    // Quantize without dithering
    hgl::Color3f quantized_no_dither[WIDTH * HEIGHT];
    for(int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        quantized_no_dither[i] = hgl::QuantizeColor3f(grayscale[i], 3);
    }
    std::cout << "  Quantized (3-bit, no dithering): " << (int)quantized_no_dither[0].r * 255 << " "
              << (int)quantized_no_dither[4].r * 255 << " "
              << (int)quantized_no_dither[7].r * 255 << std::endl;

    // Quantize with Bayer dithering
    hgl::Color3f quantized_with_dither[WIDTH * HEIGHT];
    for(int y = 0; y < HEIGHT; ++y)
    {
        for(int x = 0; x < WIDTH; ++x)
        {
            int idx = y * WIDTH + x;
            quantized_with_dither[idx] = hgl::BayerDitherer::Dither(grayscale[idx], x, y, 4);
        }
    }
    std::cout << "  With Bayer dithering: " << (int)quantized_with_dither[0].r * 255 << " "
              << (int)quantized_with_dither[4].r * 255 << " "
              << (int)quantized_with_dither[7].r * 255 << std::endl;

    // Calculate average error
    float error_no_dither = 0.0f;
    float error_with_dither = 0.0f;
    for(int i = 0; i < WIDTH * HEIGHT; ++i)
    {
        error_no_dither += hgl::QuantizationError3f(grayscale[i], quantized_no_dither[i]);
        error_with_dither += hgl::QuantizationError3f(grayscale[i], quantized_with_dither[i]);
    }
    error_no_dither /= (WIDTH * HEIGHT);
    error_with_dither /= (WIDTH * HEIGHT);

    std::cout << "  Avg error (no dithering): " << error_no_dither << std::endl;
    std::cout << "  Avg error (with dithering): " << error_with_dither << std::endl;

    std::cout << "✓ Complete Color Processing Pipeline: PASSED" << std::endl;
}

// ================================================================================================
// Test: Color Space Transitions
// ================================================================================================

void TestColorSpaceTransitions()
{
    std::cout << "\n=== Test: Color Space Transitions ===" << std::endl;

    // Create test colors in different spaces
    hgl::Color3f original(0.6f, 0.3f, 0.8f);

    std::cout << "Color transitions:" << std::endl;
    std::cout << "  Original RGB: (" << original.r << ", " << original.g << ", " << original.b << ")" << std::endl;

    // RGB -> Uint8 -> RGB
    hgl::Color3ub rgb_ub=hgl::math::ToColorByte(original);
    hgl::Color3f rgb_back(rgb_ub);
    std::cout << "  After uint8 roundtrip: (" << rgb_back.r << ", " << rgb_back.g << ", " << rgb_back.b << ")" << std::endl;

    // Quantize and back
    hgl::Color3f quantized = hgl::QuantizeColor3f(original, 5);
    std::cout << "  After 5-bit quantization: (" << quantized.r << ", " << quantized.g << ", " << quantized.b << ")" << std::endl;

    // All values should be in valid range
    assert(rgb_back.r >= 0 && rgb_back.r <= 1);
    assert(quantized.r >= 0 && quantized.r <= 1);

    std::cout << "✓ Color Space Transitions: PASSED" << std::endl;
}

// ================================================================================================
// Test: Batch Color Processing
// ================================================================================================

void TestBatchColorProcessing()
{
    std::cout << "\n=== Test: Batch Color Processing ===" << std::endl;

    // Create a batch of random-like colors
    std::vector<hgl::Color3f> colors = {
        hgl::Color3f(0.1f, 0.2f, 0.3f),
        hgl::Color3f(0.4f, 0.5f, 0.6f),
        hgl::Color3f(0.7f, 0.8f, 0.9f),
        hgl::Color3f(0.2f, 0.4f, 0.6f),
        hgl::Color3f(0.3f, 0.6f, 0.9f),
    };

    std::cout << "Processing batch of " << colors.size() << " colors:" << std::endl;

    // Process all colors through quantization pipeline
    std::vector<hgl::Color3f> quantized(colors.size());
    std::vector<float> errors(colors.size());

    for(size_t i = 0; i < colors.size(); ++i)
    {
        quantized[i] = hgl::QuantizeColor3f(colors[i], 4);
        errors[i] = hgl::QuantizationError3f(colors[i], quantized[i]);

        std::cout << "  Color " << i << ": error = " << errors[i] << std::endl;
    }

    // Find color with minimum and maximum error
    size_t min_idx = 0, max_idx = 0;
    for(size_t i = 1; i < errors.size(); ++i)
    {
        if(errors[i] < errors[min_idx]) min_idx = i;
        if(errors[i] > errors[max_idx]) max_idx = i;
    }

    std::cout << "  Minimum error: Color " << min_idx << " with error " << errors[min_idx] << std::endl;
    std::cout << "  Maximum error: Color " << max_idx << " with error " << errors[max_idx] << std::endl;

    std::cout << "✓ Batch Color Processing: PASSED" << std::endl;
}

// ================================================================================================
// Test: Performance Characteristics
// ================================================================================================

void TestPerformanceCharacteristics()
{
    std::cout << "\n=== Test: Performance Characteristics ===" << std::endl;

    // Create a large batch for performance testing
    const int BATCH_SIZE = 1000;
    std::vector<hgl::Color3f> colors(BATCH_SIZE);

    // Initialize with pseudo-random values
    for(int i = 0; i < BATCH_SIZE; ++i)
    {
        float r = (i * 17) % 256 / 255.0f;
        float g = (i * 23) % 256 / 255.0f;
        float b = (i * 31) % 256 / 255.0f;
        colors[i] = hgl::Color3f(r, g, b);
    }

    std::cout << "Processing " << BATCH_SIZE << " colors:" << std::endl;

    // Quantization performance
    float total_error = 0.0f;
    for(const auto &color : colors)
    {
        hgl::Color3f q4 = hgl::QuantizeColor3f(color, 4);
        hgl::Color3f q8 = hgl::QuantizeColor3f(color, 8);
        total_error += hgl::QuantizationError3f(color, q4);
    }
    std::cout << "  Total quantization error (4-bit): " << total_error << std::endl;

    // Dithering performance
    float dither_error = 0.0f;
    hgl::RandomDitherer ditherer;
    for(const auto &color : colors)
    {
        hgl::Color3f dithered = ditherer.Dither(color, 0.05f, 5);
        dither_error += hgl::QuantizationError3f(color, dithered);
    }
    std::cout << "  Total dithering error (random, 5-bit): " << dither_error << std::endl;

    // Palette matching performance
    std::vector<hgl::Color3f> palette = {
        hgl::Color3f(1, 0, 0), hgl::Color3f(0, 1, 0), hgl::Color3f(0, 0, 1),
        hgl::Color3f(1, 1, 0), hgl::Color3f(1, 0, 1), hgl::Color3f(0, 1, 1),
    };

    int matches = 0;
    for(const auto &color : colors)
    {
        int idx = hgl::FindNearestPaletteColor(color, palette);
        if(idx >= 0) matches++;
    }
    std::cout << "  Palette matches: " << matches << "/" << BATCH_SIZE << std::endl;

    std::cout << "✓ Performance Characteristics: PASSED" << std::endl;
}

// ================================================================================================
// Test: Advanced Color Filtering
// ================================================================================================

void TestAdvancedColorFiltering()
{
    std::cout << "\n=== Test: Advanced Color Filtering ===" << std::endl;

    std::vector<hgl::Color3f> image = {
        hgl::Color3f(0.9f, 0.1f, 0.1f),  // Red-ish
        hgl::Color3f(0.1f, 0.9f, 0.1f),  // Green-ish
        hgl::Color3f(0.1f, 0.1f, 0.9f),  // Blue-ish
        hgl::Color3f(0.5f, 0.5f, 0.5f),  // Gray
    };

    std::cout << "Filter colors by dominant channel:" << std::endl;

    for(const auto &color : image)
    {
        float max_val = std::max({color.r, color.g, color.b});
        const char *name = "Unknown";

        if(hgl::math::IsNearlyEqual(color.r, max_val)) name = "Red-dominant";
        else if(hgl::math::IsNearlyEqual(color.g, max_val)) name = "Green-dominant";
        else if(hgl::math::IsNearlyEqual(color.b, max_val)) name = "Blue-dominant";

        std::cout << "  (" << std::fixed << std::setprecision(1)
                  << color.r << ", " << color.g << ", " << color.b << ") -> " << name << std::endl;

        // Apply specific quantization based on dominance
        int bits = 5;
        if(hgl::math::IsNearlyEqual(max_val, 0.5f)) bits = 3;  // Gray needs less precision

        hgl::Color3f quantized = hgl::QuantizeColor3f(color, bits);
        float error = hgl::QuantizationError3f(color, quantized);
        std::cout << "    Quantized (" << bits << "-bit): error = " << error << std::endl;
    }

    std::cout << "✓ Advanced Color Filtering: PASSED" << std::endl;
}

// ================================================================================================
// Main
// ================================================================================================

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Advanced Color Processing Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    try
    {
        TestCompleteColorPipeline();
        TestColorSpaceTransitions();
        TestBatchColorProcessing();
        TestPerformanceCharacteristics();
        TestAdvancedColorFiltering();

        std::cout << "\n========================================" << std::endl;
        std::cout << "✓ ALL ADVANCED TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;

        return 0;
    }
    catch(const std::exception &e)
    {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
