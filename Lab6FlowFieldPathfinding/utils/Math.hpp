#pragma once

class Math
{
public:
    constexpr static float PI = 3.14159265358979323846264338327950288419716f;
    constexpr static float TAU = 6.28318530717958647692528676655900576839433f;

    /**
     * \brief Convert an angle from degrees to radians
     * \param degrees angle in degrees
     * \return converted angle in radians
     */
    static float convertDegToRad(float degrees);

    /**
     * \brief Convert an angle from radians to degrees
     * \param radians angle in radians
     * \return converted angle in degrees
     */
    static float convertRadToDeg(float radians);

    /**
     * \brief Wrap an angle in the range [0,360] to the range [-180,180]
     * \param angle Angle in degrees [0,360] to wrap
     * \return wrapped angle in the range [-180,180]
     */
    static float wrapUnsigned(float angle);
};
