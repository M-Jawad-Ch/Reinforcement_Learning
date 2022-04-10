#include<SFML/Graphics.hpp>
#include<vector>
#include<iostream>
#include<fstream>

void save(sf::VertexArray &path, sf::Vector2f screen, sf::VertexArray &rewards, sf::Vector2f spawn)
{
    std::vector<float> data;
    
    for(int i = 0; i < path.getVertexCount(); i++)
    {
        data.push_back( path[i].position.x / screen.x );
        data.push_back( path[i].position.y / screen.y );
    }

    std :: cout << " Enter the name of your path : ";
    std :: string fileName; std :: cin >> fileName;
    fileName = "Environments/" + fileName + ".bin";

    std :: ofstream fout(fileName);
    int vertexCount = path.getVertexCount();

    fout.write((char*)&vertexCount, sizeof(int));

    for(int i = 0; i < data.size(); i++)
    {
        fout.write((char*)&data[i], sizeof(float));
    }

    data.clear();

    for(int i = 0; i < rewards.getVertexCount(); i++)
    {
        data.push_back( rewards[i].position.x / screen.x );
        data.push_back( rewards[i].position.y / screen.y );
    }

    int rewardCount = rewards.getVertexCount();
    fout.write((char*)&rewardCount, sizeof(int));

    for(int i = 0; i < data.size(); i++)
    {
        fout.write((char*)&data[i], sizeof(float));
    }

    spawn.x /= screen.x;
    spawn.y /= screen.y;

    fout.write((char*)&spawn.x, sizeof(float));
    fout.write((char*)&spawn.y, sizeof(float));

    fout.close();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Path Editor", sf::Style::Fullscreen);
    sf::Event event;

    sf::VertexArray path, rewards;
    path.setPrimitiveType(sf::PrimitiveType::Lines);
    bool sameObj = false, target = false; int startOfPath = 0;
    sf::Vector2f spawn;

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
                        if ( !target )
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
                        else
                        {
                            rewards.append( sf::Vertex(sf::Vector2f(sf::Mouse::getPosition(window))) );
                            rewards[ rewards.getVertexCount() - 1 ].color = sf::Color::Red;
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
                        save(path, sf::Vector2f( sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height ), rewards, spawn);
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::C) )
                    {
                        path.clear();
                        rewards.clear();
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::P) )
                    {
                        spawn = sf::Vector2f(sf::Mouse::getPosition(window));
                    }

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::T) )
                    {
                        if ( target ) target = false;
                        else target = true;
                    }

                    break;
            }

            window.clear(sf::Color::Black);

            window.draw(path);

            window.draw(rewards);

            window.display();
        }
    }
}