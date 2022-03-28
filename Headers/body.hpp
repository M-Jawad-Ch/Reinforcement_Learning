#include"edges.hpp"

class Body
{
    public:
    
    Edge e1, e2, e3, e4;
    sf::Vector2f center, front;

    Body(sf::Vector2f cent, float scale)
    {
        sf::Vector2f p1, p2, p3, p4;

        p1.x = -1.5; p1.y = 2;
        p2.x = 1.5; p2.y = 2;
        p3.x = 1.5; p3.y = -2;
        p4.x = -1.5; p4.y = -2;
        front.x = 0; front.y = 2;

        p1 *= scale; p2 *= scale;
        p3 *= scale; p4 *= scale;
        front *= scale;

        p1 += cent; p2 += cent;
        p3 += cent; p4 += cent;
        front += cent;

        e1.p1 = p1; e1.p2 = p2;
        e2.p1 = p2; e2.p2 = p3;
        e3.p1 = p3; e3.p2 = p4;
        e4.p1 = p4; e4.p2 = p1;

        center = cent;
    }

    sf::VertexArray drawable()
    {
        sf::VertexArray array;

        array.append(sf::Vertex(e1.p1));
        array.append(sf::Vertex(e1.p2));
        array.append(sf::Vertex(e2.p1));
        array.append(sf::Vertex(e2.p2));
        array.append(sf::Vertex(e3.p1));
        array.append(sf::Vertex(e3.p2));
        array.append(sf::Vertex(e4.p1));
        array.append(sf::Vertex(e4.p2));

        array.setPrimitiveType(sf::PrimitiveType::Lines);

        return array;
    }
};