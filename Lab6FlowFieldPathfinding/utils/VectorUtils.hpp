#ifndef LAB1_VECTORUTILS_HPP
#define LAB1_VECTORUTILS_HPP

#include <SFML/System.hpp>

class VectorUtils : sf::NonCopyable
{
public:
    VectorUtils() = delete;

    template<class T>
    static sf::Vector2<T> angleToVector(T angle);

    template <class T>
    static float vectorToAngle(T vector);

    template<class T>
    static float getLength(sf::Vector2<T> v);

    template<class T>
    static sf::Vector2<T> normalize(sf::Vector2<T> v);

    static sf::Vector2f floor(sf::Vector2f v);
};

#include "VectorUtils.inl"
#endif //LAB1_VECTORUTILS_HPP
