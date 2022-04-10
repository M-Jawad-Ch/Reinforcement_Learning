#include"edges.cpp"
#include"brain.cpp"

#include<vector>

#include<SFML/Graphics.hpp>

class Agent
{
    void setDirection()
    {
        direction = front - center;
        Edge e; e.p1 = center; e.p2 = front;

        direction.x /= -e.magnitude();
        direction.y /= -e.magnitude();
    }

    sf::Vector2f transform(sf::Vector2f p, sf::Vector2f origin, float angle)
    {
        p -= origin;

        sf::Vector2f resultant;

        resultant.x = p.x * cos(angle) + p.y * sin(angle);
        resultant.y = -p.x * sin(angle) + p.y * cos(angle);

        resultant += origin;

        return resultant;
    }

    void setRays(int rayCount)
    {
        float pi = 22.0 / 7.0;

        for(float x = 0; x <= pi; x += pi / rayCount)
        {
            Edge e;
            e.p1 = center;

            e.p2.x = 10000; e.p2.y = 10000;
            e.p2 = transform(e.p2, center, x);

            rays.push_back(e);
        }
    }

    Edge e1, e2, e3, e4;

    public:
    sf::Vector2f center, front;
    sf::Vector2f direction;
    std::vector<Edge> rays;
    Brain brain;


    Agent(int layers, const sf::Vector2f &cent, float scale, int rayCount)
    {
        sf::Vector2f p1, p2, p3, p4;

        p1.x = -1.5; p1.y = -2;
        p2.x = 1.5; p2.y = -2;
        p3.x = 1.5; p3.y = 2;
        p4.x = -1.5; p4.y = 2;
        front.x = 0; front.y = -2;

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

        setDirection();

        setRays(rayCount - 1);

        for(int i = 0; i < rays.size(); i++)
        {
            rays[i].p2 = transform(rays[i].p2, center, (22.0/7.0) / 4);
        }

        brain = Brain(layers);
    }

    bool touch( const std :: vector <Edge> &edges )
    {
        sf::Vector2f temp;
        for(int i = 0; i < edges.size(); i++)
        {
            if ( Intersection(e1, edges[i], temp) )
                return true;
            else if ( Intersection(e2, edges[i], temp) )
                return true;
            else if ( Intersection(e3, edges[i], temp) )
                return true;
            else if ( Intersection(e4, edges[i], temp) )
                return true;
        }

        return false;
    }

    void translate(float stepSize)
    {
        setDirection();

        sf::Vector2f step = direction * -stepSize;

        e1.p1 += step; e1.p2 += step;
        e2.p1 += step; e2.p2 += step;
        e3.p1 += step; e3.p2 += step;
        e4.p1 += step; e4.p2 += step;

        center += step; front += step;

        for(int i = 0; i < rays.size(); i++)
        {
            rays[i].p1 += step;
            rays[i].p2 += step;
        }
    }

    void rotate(float angle)
    {
        e1.p1 = transform(e1.p1, center, angle);
        e1.p2 = transform(e1.p2, center, angle);

        e2.p1 = transform(e2.p1, center, angle);
        e2.p2 = transform(e2.p2, center, angle);

        e3.p1 = transform(e3.p1, center, angle);
        e3.p2 = transform(e3.p2, center, angle);

        e4.p1 = transform(e4.p1, center, angle);
        e4.p2 = transform(e4.p2, center, angle);

        front = transform(front, center, angle);

        for(int i = 0; i < rays.size(); i++)
        {
            rays[i].p2 = transform(rays[i].p2, center, angle);
        }

        setDirection();
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

    std::vector<sf::CircleShape> inter(const std::vector<Edge> &entites)
    {
        std::vector<sf::CircleShape> array;

        for(int i = 0; i < rays.size(); i++)
        {
            sf::Vector2f point, prevPoint = rays[i].p2;

            for(int j = 0; j < entites.size(); j++)
            {
                if ( Intersection(rays[i], entites[j], point) )
                {
                    Edge e1, e2; e1.p1 = center; e1.p2 = point;
                    e2.p1 = center; e2.p2 = prevPoint;

                    if ( e1.magnitude() < e2.magnitude() )
                    {   
                        prevPoint = point;
                    }
                }
            }

            sf::CircleShape circle(1, 8);
            circle.setPosition(prevPoint);
            circle.setOrigin(1, 1);
            array.push_back(circle);
        }

        return array;
    }
};