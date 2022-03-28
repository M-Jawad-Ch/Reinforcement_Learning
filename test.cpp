#include<vector>
#include<fstream>
#include<iostream>


#include<SFML/Graphics.hpp>
#include"Headers/body.hpp"

void load(std::vector<Edge> &edges, sf::VertexArray &vertices)
{
    std :: cout << " Enter the enviorment name : ";
    std :: string fileName; std :: cin >> fileName;

    fileName = "Enviorments/" + fileName + ".bin";

    std :: ifstream fin(fileName);

    int vertexCount; sf::Vector2f temp; Edge tempE;

    fin.read((char*)&vertexCount, sizeof(int));

    for(int i = 0; i < vertexCount; i++)
    {
        fin.read((char*)&temp.x, sizeof(float));
        fin.read((char*)&temp.y, sizeof(float));

        tempE.p1 = temp;
        
        vertices.append(sf::Vertex(temp));

        fin.read((char*)&temp.x, sizeof(float));
        fin.read((char*)&temp.y, sizeof(float));

        tempE.p2 = temp;
        
        vertices.append(sf::Vertex(temp));
        edges.push_back(tempE);

        i++;
    }

    vertices.setPrimitiveType(sf::PrimitiveType::Lines);
}

int main()
{
    sf::VertexArray path, entities;
    std::vector<Edge> edges;

    load(edges, path);

    Body body( sf::Vector2f(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2), 10);

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Test", sf::Style::Fullscreen);
    sf::Event event;

    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case ( sf::Event::Closed ):
                    window.close();
                    break;

                case ( sf::Event::KeyPressed ):
                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
                        window.close();

                    //if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) )

                    break;
            }

            window.clear(sf::Color::Black);

            window.draw(path);
            window.draw(body.drawable());

            window.display();
        }
    }
}