#pragma once

#include<hgl/math/MathConstants.h>

namespace hgl::math
{
    struct Degrees
    {
        explicit constexpr Degrees(double v) noexcept : value(v) {}
        double value;
    };

    struct Radians
    {
        explicit constexpr Radians(double v) noexcept : value(v) {}
        double value;
    };

    class Angle
    {
    public:
        constexpr Angle() noexcept : radians_(0.0) {}
        constexpr Angle(Radians rad) noexcept : radians_(rad.value) {}
        constexpr Angle(Degrees deg) noexcept : radians_(deg.value * math::pi / 180.0) {}

        static constexpr Angle FromRadians(double value) noexcept { return Angle(Radians(value)); }
        static constexpr Angle FromDegrees(double value) noexcept { return Angle(Degrees(value)); }

        constexpr double AsRadians() const noexcept { return radians_; }
        constexpr double AsDegrees() const noexcept { return radians_ * 180.0 / math::pi; }

        constexpr Angle &operator+=(const Angle &rhs) noexcept { radians_ += rhs.radians_; return *this; }
        constexpr Angle &operator-=(const Angle &rhs) noexcept { radians_ -= rhs.radians_; return *this; }
        constexpr Angle &operator*=(double rhs) noexcept { radians_ *= rhs; return *this; }
        constexpr Angle &operator/=(double rhs) noexcept { radians_ /= rhs; return *this; }

        friend constexpr Angle operator+(Angle lhs, const Angle &rhs) noexcept { lhs += rhs; return lhs; }
        friend constexpr Angle operator-(Angle lhs, const Angle &rhs) noexcept { lhs -= rhs; return lhs; }
        friend constexpr Angle operator*(Angle lhs, double rhs) noexcept { lhs *= rhs; return lhs; }
        friend constexpr Angle operator*(double lhs, Angle rhs) noexcept { rhs *= lhs; return rhs; }
        friend constexpr Angle operator/(Angle lhs, double rhs) noexcept { lhs /= rhs; return lhs; }

        friend constexpr bool operator==(const Angle &lhs, const Angle &rhs) noexcept { return lhs.radians_ == rhs.radians_; }
        friend constexpr bool operator!=(const Angle &lhs, const Angle &rhs) noexcept { return !(lhs == rhs); }
        friend constexpr bool operator<(const Angle &lhs, const Angle &rhs) noexcept { return lhs.radians_ < rhs.radians_; }
        friend constexpr bool operator>(const Angle &lhs, const Angle &rhs) noexcept { return rhs < lhs; }
        friend constexpr bool operator<=(const Angle &lhs, const Angle &rhs) noexcept { return !(rhs < lhs); }
        friend constexpr bool operator>=(const Angle &lhs, const Angle &rhs) noexcept { return !(lhs < rhs); }

    private:
        double radians_;
    };

    namespace angle_literals
    {
        constexpr Angle operator""_rad(long double v) noexcept { return Angle::FromRadians(static_cast<double>(v)); }
        constexpr Angle operator""_deg(long double v) noexcept { return Angle::FromDegrees(static_cast<double>(v)); }
        constexpr Angle operator""_rad(unsigned long long v) noexcept { return Angle::FromRadians(static_cast<double>(v)); }
        constexpr Angle operator""_deg(unsigned long long v) noexcept { return Angle::FromDegrees(static_cast<double>(v)); }
    }
}
