#include<math.h>
#include<SFML/System.hpp>

class Edge
{
    public:

    sf::Vector2f p1, p2;
};

bool Intersection(const Edge &edge1, const Edge &edge2, sf::Vector2f &interPoint)
{
    float m1, m2;

    //m1 = edge1.p2.x
}