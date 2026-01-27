#include<hgl/math/FloatPrecision.h>
#include<hgl/color/Color3f.h>
#include<hgl/color/Color3ub.h>
#include<hgl/color/Color4f.h>
#include<hgl/color/Color4ub.h>
#include<hgl/color/ColorLerp.h>
#include<hgl/color/CMYKf.h>
#include<hgl/color/HSL.h>
#include<iostream>
#include<iomanip>
#include<cassert>

namespace hgl
{
    void PrintColor3f(const Color3f &c, const char *name = nullptr)
    {
        if(name) std::cout << name << ": ";
        std::cout << std::fixed << std::setprecision(3)
                  << "(" << c.r << ", " << c.g << ", " << c.b << ")" << std::endl;
    }

    void PrintColor3ub(const Color3ub &c, const char *name = nullptr)
    {
        if(name) std::cout << name << ": ";
        std::cout << "(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ")" << std::endl;
    }

    void PrintColor4f(const Color4f &c, const char *name = nullptr)
    {
        if(name) std::cout << name << ": ";
        std::cout << std::fixed << std::setprecision(3)
                  << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")" << std::endl;
    }

    void PrintColor4ub(const Color4ub &c, const char *name = nullptr)
    {
        if(name) std::cout << name << ": ";
        std::cout << "(" << (int)c.r << ", " << (int)c.g << ", " << (int)c.b << ", " << (int)c.a << ")" << std::endl;
    }
}

// ================================================================================================
// Test: Basic Color Conversion
// ================================================================================================

void TestBasicColorConversion()
{
    std::cout << "\n=== Test: Basic Color Conversion ===" << std::endl;

    // Color3f construction
    hgl::Color3f red(1.0f, 0.0f, 0.0f);
    hgl::Color3f green(0.0f, 1.0f, 0.0f);
    hgl::Color3f blue(0.0f, 0.0f, 1.0f);
    hgl::Color3f white(1.0f, 1.0f, 1.0f);

    hgl::PrintColor3f(red, "Red");
    hgl::PrintColor3f(green, "Green");
    hgl::PrintColor3f(blue, "Blue");
    hgl::PrintColor3f(white, "White");

    // Color3ub construction
    hgl::Color3ub red_ub(255, 0, 0);
    hgl::Color3ub green_ub(0, 255, 0);

    hgl::PrintColor3ub(red_ub, "Red (uint8)");
    hgl::PrintColor3ub(green_ub, "Green (uint8)");

    // Test conversion Color3ub -> Color3f
    hgl::Color3f red_from_ub(red_ub);
    hgl::PrintColor3f(red_from_ub, "Red from uint8");
    assert(hgl::math::IsNearlyEqual(red_from_ub.r, 1.0f));

    std::cout << "✓ Basic Color Conversion: PASSED" << std::endl;
}

// ================================================================================================
// Test: Color Interpolation
// ================================================================================================

void TestColorInterpolation()
{
    std::cout << "\n=== Test: Color Interpolation ===" << std::endl;

    hgl::Color3f start(1.0f, 0.0f, 0.0f);  // Red
    hgl::Color3f end(0.0f, 0.0f, 1.0f);    // Blue

    hgl::Color3f mid = hgl::ColorLerp(start, end, 0.5f);
    hgl::PrintColor3f(mid, "Interpolated (0.5)");

    // Should be approximately (0.5, 0, 0.5) purple
    assert(hgl::math::IsNearlyEqual(mid.r, 0.5f));
    assert(hgl::math::IsNearlyEqual(mid.g, 0.0f));
    assert(hgl::math::IsNearlyEqual(mid.b, 0.5f));

    // Test boundary cases
    hgl::Color3f at_start = hgl::ColorLerp(start, end, 0.0f);
    assert(hgl::math::IsNearlyEqual(at_start.r, start.r));

    hgl::Color3f at_end = hgl::ColorLerp(start, end, 1.0f);
    assert(hgl::math::IsNearlyEqual(at_end.b, end.b));

    std::cout << "✓ Color Interpolation: PASSED" << std::endl;
}

// ================================================================================================
// Test: CMYK Color Space
// ================================================================================================

void TestCMYKColorSpace()
{
    std::cout << "\n=== Test: CMYK Color Space ===" << std::endl;

    // RGB to CMYK
    hgl::Color3f rgb_red(1.0f, 0.0f, 0.0f);
    hgl::CMYKf cmyk_red(rgb_red);

    std::cout << "Red in CMYK: C=" << std::setprecision(3) << cmyk_red.x
              << " M=" << cmyk_red.y << " Y=" << cmyk_red.z << " K=" << cmyk_red.w << std::endl;

    // Red should be M=1, Y=1, C=0, K=0
    assert(hgl::math::IsNearlyEqual(cmyk_red.x, 0.0f));  // C
    assert(hgl::math::IsNearlyEqual(cmyk_red.y, 1.0f));  // M
    assert(hgl::math::IsNearlyEqual(cmyk_red.z, 1.0f));  // Y
    assert(hgl::math::IsNearlyEqual(cmyk_red.w, 0.0f));  // K

    // CMYK to RGB
    hgl::Color3f rgb_back = cmyk_red.toRGB();
    hgl::PrintColor3f(rgb_back, "CMYK back to RGB");
    assert(hgl::math::IsNearlyEqual(rgb_back.r, rgb_red.r));
    assert(hgl::math::IsNearlyEqual(rgb_back.g, rgb_red.g));
    assert(hgl::math::IsNearlyEqual(rgb_back.b, rgb_red.b));

    // Test black and white
    hgl::Color3f black(0.0f, 0.0f, 0.0f);
    hgl::CMYKf cmyk_black(black);
    assert(hgl::math::IsNearlyEqual(cmyk_black.w, 1.0f));  // K=1

    hgl::Color3f white(1.0f, 1.0f, 1.0f);
    hgl::CMYKf cmyk_white(white);
    assert(hgl::math::IsNearlyEqual(cmyk_white.w, 0.0f));  // K=0

    std::cout << "✓ CMYK Color Space: PASSED" << std::endl;
}

// ================================================================================================
// Test: Color Constants
// ================================================================================================

void TestColorConstants()
{
    std::cout << "\n=== Test: Color Constants ===" << std::endl;

    // Test predefined colors
    std::cout << "Predefined Colors:" << std::endl;
    std::cout << "  CyanCMYKf: (" << hgl::CyanCMYKf.x << ", " << hgl::CyanCMYKf.y
              << ", " << hgl::CyanCMYKf.z << ", " << hgl::CyanCMYKf.w << ")" << std::endl;

    std::cout << "  MagentaCMYKf: (" << hgl::MagentaCMYKf.x << ", " << hgl::MagentaCMYKf.y
              << ", " << hgl::MagentaCMYKf.z << ", " << hgl::MagentaCMYKf.w << ")" << std::endl;

    std::cout << "  BlackCMYKf: (" << hgl::BlackCMYKf.x << ", " << hgl::BlackCMYKf.y
              << ", " << hgl::BlackCMYKf.z << ", " << hgl::BlackCMYKf.w << ")" << std::endl;

    std::cout << "✓ Color Constants: PASSED" << std::endl;
}

// ================================================================================================
// Test: Color Arithmetic (inherited from GLM)
// ================================================================================================

void TestColorArithmetic()
{
    std::cout << "\n=== Test: Color Arithmetic ===" << std::endl;

    hgl::Color3f c1(0.5f, 0.3f, 0.2f);
    hgl::Color3f c2(0.2f, 0.4f, 0.8f);

    // Addition
    hgl::Color3f sum = c1 + c2;
    hgl::PrintColor3f(sum, "c1 + c2");
    assert(hgl::math::IsNearlyEqual(sum.r, 0.7f));

    // Subtraction
    hgl::Color3f diff = c1 - c2;
    hgl::PrintColor3f(diff, "c1 - c2");
    assert(hgl::math::IsNearlyEqual(diff.r, 0.3f));

    // Scalar multiplication
    hgl::Color3f scaled = c1 * 2.0f;
    hgl::PrintColor3f(scaled, "c1 * 2.0");
    assert(hgl::math::IsNearlyEqual(scaled.r, 1.0f));

    std::cout << "✓ Color Arithmetic: PASSED" << std::endl;
}

// ================================================================================================
// Test: Boundary Cases
// ================================================================================================

void TestBoundaryCases()
{
    std::cout << "\n=== Test: Boundary Cases ===" << std::endl;

    // Over-range values should be clamped
    hgl::Color3f over(1.5f, 2.0f, -0.5f);
    hgl::PrintColor3f(over, "Over-range values");

    assert(over.r <= 1.0f && over.r >= 0.0f);
    assert(over.g <= 1.0f && over.g >= 0.0f);
    assert(over.b <= 1.0f && over.b >= 0.0f);

    // Edge case: interpolation with t < 0 and t > 1
    hgl::Color3f red(1.0f, 0.0f, 0.0f);
    hgl::Color3f blue(0.0f, 0.0f, 1.0f);

    hgl::Color3f before = hgl::ColorLerp(red, blue, -0.5f);
    hgl::PrintColor3f(before, "Lerp t=-0.5 (should be red)");
    assert(hgl::math::IsNearlyEqual(before.r, red.r));

    hgl::Color3f after = hgl::ColorLerp(red, blue, 1.5f);
    hgl::PrintColor3f(after, "Lerp t=1.5 (should be blue)");
    assert(hgl::math::IsNearlyEqual(after.b, blue.b));

    std::cout << "✓ Boundary Cases: PASSED" << std::endl;
}

// ================================================================================================
// Main
// ================================================================================================

int main()
{
    std::cout << "========================================" << std::endl;
    std::cout << "Color Library - Comprehensive Tests" << std::endl;
    std::cout << "========================================" << std::endl;

    try
    {
        TestBasicColorConversion();
        TestColorInterpolation();
        TestCMYKColorSpace();
        TestColorConstants();
        TestColorArithmetic();
        TestBoundaryCases();

        std::cout << "\n========================================" << std::endl;
        std::cout << "✓ ALL TESTS PASSED!" << std::endl;
        std::cout << "========================================" << std::endl;

        return 0;
    }
    catch(const std::exception &e)
    {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
