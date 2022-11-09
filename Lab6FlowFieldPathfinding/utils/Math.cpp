#include "Math.hpp"

#include <cmath>
#include <cassert>

float Math::convertDegToRad(float degrees)
{
    return degrees * PI / 180;
}

float Math::convertRadToDeg(float radians)
{
    return radians * 180 / PI;
}

float Math::mapToRange(float degrees)
{
    float radians = convertDegToRad(degrees);

/*    while (radians < -PI || PI < radians)
    {
        if (radians > PI)
        {
            radians -= 2 * PI;
        } else if (radians < -PI)
        {
            radians += 2 * PI;
        }
    }*/

    return convertRadToDeg(radians);
}

constexpr float positiveRemainder(float a, float b)
{
    assert(b > 0.0f);
    const float val = a - static_cast<float>(static_cast<int>(a / b)) * b;
    if (val >= 0.f)
        return val;
    else
        return val + b;
}

float Math::wrapUnsigned(float angle)
{
    return positiveRemainder(angle + 180, 360) - 180;
}


