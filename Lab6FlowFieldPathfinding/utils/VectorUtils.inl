#include "VectorUtils.hpp"
#include "Math.hpp"

#include <cmath>

template <class T>
float VectorUtils::getLength(sf::Vector2<T> v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

template <class T>
sf::Vector2<T> VectorUtils::normalize(sf::Vector2<T> v)
{
    return v / getLength(v);
}

template <class T>
sf::Vector2<T> VectorUtils::angleToVector(T angle)
{
    const float radian = Math::convertDegToRad(angle);

    return sf::Vector2<T>(
        Math::convertRadToDeg(std::cos(radian)),
        Math::convertRadToDeg(std::sin(radian))
    );
}

template <class T>
float VectorUtils::vectorToAngle(T v)
{
    return Math::convertRadToDeg(std::atan2(v.y, v.x));
}

inline sf::Vector2f VectorUtils::floor(sf::Vector2f v)
{
    return {std::floor(v.x), std::floor(v.y)};
}
