#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<fstream>

void save(sf::VertexArray &path, sf::Vector2f screen)
{
    std::vector<float> data;
    
    for(int i = 0; i < path.getVertexCount(); i++)
    {
        data.push_back( path[i].position.x / screen.x );
        data.push_back( path[i].position.y / screen.y );
    }

    std :: cout << " Enter the name of your path : ";
    std :: string fileName; std :: cin >> fileName;
    fileName = "Enviorments/" + fileName + ".bin";

    std :: ofstream fout(fileName);
    int vertexCount = path.getVertexCount();
    fout.write((char*)&vertexCount, sizeof(int));

    for(int i = 0; i < data.size(); i++)
    {
        fout.write((char*)&data[i], sizeof(float));
    }

    fout.close();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Path Editor", sf::Style::Fullscreen);
    sf::Event event;

    sf::VertexArray path;
    path.setPrimitiveType(sf::PrimitiveType::Lines);
    bool sameObj = false; int startOfPath = 0;

    while(window.isOpen())
    {
        while(window.pollEvent(event))
        {
            switch (event.type)
            {
                case ( sf::Event::Closed ):
                    window.close();
                    break;
                
                case ( sf::Event::MouseButtonPressed ):
                    if ( sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                    {
                        if ( path.getVertexCount() > 0 && sameObj )
                        {
                            path.append( sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window))) );
                            path.append( path[ path.getVertexCount() - 1 ] );
                        }
                        else
                        {
                            path.append( sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window))) );
                            startOfPath = path.getVertexCount() - 1;
                            sameObj = true;
                        }
                    }
                    else if ( sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) )
                    {
                        path.append( path[startOfPath] );
                        sameObj = false;
                    }

                    break;

                case ( sf::Event::KeyPressed ):
                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) )
                        window.close();

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S) )
                    {
                        window.close();
                        save(path, sf::Vector2f( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height ));
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::C) )
                        path.clear();

                    break;
            }

            window.clear(sf::Color::Black);

            window.draw(path);

            window.display();
        }
    }
}