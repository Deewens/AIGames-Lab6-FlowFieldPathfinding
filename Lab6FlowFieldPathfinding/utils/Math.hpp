#ifndef LAB1_MATH_HPP
#define LAB1_MATH_HPP

class Math
{
public:
    constexpr static const float PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348;
    constexpr static const float TWO_PI = 6.2831853071795864769252867665590057683943387987502116419498891846156328125724179972560696;

    static float convertDegToRad(float degrees);

    static float convertRadToDeg(float radians);

    static float mapToRange(float angle);

    static float wrapUnsigned(float angle);
};

#endif //LAB1_MATH_HPP
