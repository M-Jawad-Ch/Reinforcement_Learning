#include<vector>
#include<fstream>
#include<iostream>


#include<SFML/Graphics.hpp>
#include"Headers/body.hpp"

void load(std::vector<Edge> &edges, sf::VertexArray &vertices, sf::Vector2f screen)
{
    //std :: cout << " Enter the environment name : ";
    //std :: string fileName; 
    
    std :: string fileName = "path1";

    fileName = "Environments/" + fileName + ".bin";

    std :: ifstream fin(fileName);

    int vertexCount; sf::Vector2f temp; Edge tempE;

    fin.read((char*)&vertexCount, sizeof(int));

    for(int i = 0; i < vertexCount; i++)
    {
        fin.read((char*)&temp.x, sizeof(float));
        fin.read((char*)&temp.y, sizeof(float));

        temp.x *= screen.x;
        temp.y *= screen.y;

        tempE.p1 = temp;
        
        vertices.append(sf::Vertex(temp));

        fin.read((char*)&temp.x, sizeof(float));
        fin.read((char*)&temp.y, sizeof(float));

        temp.x *= screen.x;
        temp.y *= screen.y;

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
    float pi = 22.0 / 7.0;

    Body body( sf::Vector2f(sf::VideoMode::getDesktopMode().width / 2, sf::VideoMode::getDesktopMode().height / 2), 10, 32);

    bool Wpress, Apress, Spress, Dpress;

    sf::ContextSettings settings; settings.antialiasingLevel = 16.0;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Test", sf::Style::Fullscreen, settings);

    window.setFramerateLimit(60);

    sf::Event event;

    sf::Vector2f screen( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height );

    load(edges, path, screen);

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
                {
                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
                    {
                        window.close();
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
                    {
                        window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x + 20, window.getView().getCenter().y ), window.getView().getSize() ) );
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
                    {
                        window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x - 20, window.getView().getCenter().y ), window.getView().getSize() ) );
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
                    {
                        window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x, window.getView().getCenter().y + 20 ), window.getView().getSize() ) );
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
                    {
                        window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x, window.getView().getCenter().y - 20 ), window.getView().getSize() ) );
                    }



                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
                        Wpress = true;
                    else Wpress = false;

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
                        Spress = true;
                    else Spress = false;

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
                        Apress = true;
                    else Apress = false;

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
                        Dpress = true;
                    else Dpress = false;
                    break;
                }

                case ( sf::Event::KeyReleased ):
                {
                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
                {
                    window.close();
                }

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
                {
                    window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x + 20, window.getView().getCenter().y ), window.getView().getSize() ) );
                }

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
                {
                    window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x - 20, window.getView().getCenter().y ), window.getView().getSize() ) );
                }

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
                {
                    window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x, window.getView().getCenter().y + 20 ), window.getView().getSize() ) );
                }

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
                {
                    window.setView( sf::View( sf::Vector2f( window.getView().getCenter().x, window.getView().getCenter().y - 20 ), window.getView().getSize() ) );
                }



                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
                    Wpress = true;
                else Wpress = false;

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
                    Spress = true;
                else Spress = false;

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A) )
                    Apress = true;
                else Apress = false;

                if ( sf::Keyboard::isKeyPressed(sf::Keyboard::D) )
                    Dpress = true;
                else Dpress = false;
                break;
            }

            default:
                break;
            }

            if ( Wpress )
            {
                body.translate(10);
            }

            if ( Spress )
            {
                body.translate(-10);
            }

            if ( Apress )
            {
                body.rotate(pi / 20);
            }

            if ( Dpress )
            {
                body.rotate(-pi / 20);
            }


            window.clear(sf::Color::Black);

            //window.draw(path);                                                    // Path visibility

            window.draw(body.drawable());

            std :: vector <sf::CircleShape> circle = body.inter(edges);

            for(int i = 0; i < circle.size(); i++)
            {
                window.draw( circle[i] );
            }

            window.display();
        }
    }
}