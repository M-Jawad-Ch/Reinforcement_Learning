#include<math.h>
#include<SFML/System.hpp>

#include<iostream>

class Edge
{
    public:

    sf::Vector2f p1, p2;

    sf::Vector2f midPoint()
    {
        return sf::Vector2f((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
    }

    float magnitude()
    {
        return sqrt( pow(p2.x - p1.x,2) + pow(p2.y - p1.y, 2) );
    }
};

bool Intersection(const Edge edge1, const Edge edge2, sf::Vector2f &interPoint)
{
    float m1, m2;

    m1 = (edge1.p2.y - edge1.p1.y) / (edge1.p2.x - edge1.p1.x);
    m2 = (edge2.p2.y - edge2.p1.y) / (edge2.p2.x - edge2.p1.x);

    float x = ( m2 * edge2.p1.x - m1 * edge1.p1.x + edge1.p1.y - edge2.p1.y) / ( m2 - m1 );

    float y = m1 * ( x - edge1.p1.x ) + edge1.p1.y;

    if ( x <= fmax(edge1.p1.x, edge1.p2.x) && x <= fmax(edge2.p1.x, edge2.p2.x) )
    {
        if ( x >= fmin(edge1.p1.x, edge1.p2.x) && x >= fmin(edge2.p1.x, edge2.p2.x) )
        {
            if ( y <= fmax(edge1.p1.y, edge1.p2.y) && y <= fmax(edge2.p1.y, edge2.p2.y) )
            {
                if ( y >= fmin(edge1.p1.y, edge1.p2.y) && y >= fmin(edge2.p1.y, edge2.p2.y) )
                {
                    interPoint.x = x;
                    interPoint.y = y;

                    return true;
                }
            }
        }
    }

    return false;
}