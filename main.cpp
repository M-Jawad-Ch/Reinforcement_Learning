#include<vector>
#include<fstream>
#include<iostream>


#include<SFML/Graphics.hpp>
#include"Headers/agent.cpp"

void load(std::vector<Edge> &edges, sf::VertexArray &vertices, sf::Vector2f screen, std::vector<sf::Vector2f> &rewards, sf::Vector2f &spawn)
{
    std :: cout << " Enter the environment name : ";
    std :: string fileName; std :: cin >> fileName;

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

    int rewardCount; sf::Vector2f tempV;

    fin.read((char*)&rewardCount, sizeof(int));

    for(int i = 0; i < rewardCount; i++)
    {
        fin.read((char*)&tempV.x, sizeof(float));
        fin.read((char*)&tempV.y, sizeof(float));

        tempV.x *= screen.x;
        tempV.y *= screen.y;

        rewards.push_back( tempV );
    }

    fin.read((char*)&spawn.x, sizeof(float));
    fin.read((char*)&spawn.y, sizeof(float));

    spawn.x *= screen.x;
    spawn.y *= screen.y;

    fin.close();
}

void getInputs(Matrix &input, const Agent &agent, const std :: vector <sf::CircleShape> &circle)
{
    Edge temp;

    for(int i = 0; i < circle.size(); i++)
    {
        temp.p1 = agent.center;
        temp.p2 = circle[i].getPosition();

        input.arr[i] = 1.0 / temp.magnitude();
    }
}

void act(int choice, Agent &agent)
{
    if ( choice == 0 ) agent.translate(1);
    else if ( choice == 1 ) agent.translate(-1);
    else if ( choice == 2 ) agent.rotate( (22.0/7.0) / 40);
    else agent.rotate( (-22.0/7.0) / 40);
}

void getTragets(float *targets, std::vector<Edge> &edges, sf::Vector2f target, Agent &agent)
{
    float dist = 1000000000, tempDist = 1000000000;
    int choice = 0;

    for(int i = 0; i < 4; i++)
    {
        Agent temp = agent;

        act(i, temp);

        if ( !agent.touch(edges) )
        {
            Edge e; e.p1 = agent.front; e.p2 = target;
            tempDist = e.magnitude();

            if ( tempDist < dist )
            {
                dist = tempDist;

                choice = i;
            }
        }
    }

    for(int i = 0; i < 4; i++)
    {
        if ( i == choice ) targets[i] = 1;
        else targets[i] = 0;
    }
}

int Matrix::threads = 4, Layer::threads = 4;

int main()
{
    float pi = 22.0 / 7.0, size = 5.0 / 1300, targets[4] = {0};
    float width = 800, height = 600;
    int rayCount = 8, tar;

    sf::VertexArray path, entities;
    std::vector<Edge> edges;
    sf::Vector2f screen( width, height ), spawn;
    std::vector<sf::Vector2f> rewards;

    load(edges, path, screen, rewards, spawn);

    tar = 0;

    Agent agent(2, spawn, size * width, rayCount);
    agent.brain.layers[0] = Layer(6, rayCount);
    agent.brain.layers[1] = Layer(4, 6);

    Matrix *input = new Matrix(rayCount, 1, 1);
    sf::Vector2f target;
    bool Wpress, Apress, Spress, Dpress, training = false;
    sf::ContextSettings settings; settings.antialiasingLevel = 4.0;
    sf::RenderWindow window(sf::VideoMode(width, height), "Renderer", sf::Style::Default, settings);
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

                    if ( sf::Keyboard::isKeyPressed(sf::Keyboard::T) )
                    {
                        if ( !training ) training = true;
                        else training = false;
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
        }

        window.clear(sf::Color::Black);

        //window.draw(path);

        window.draw(agent.drawable());

        sf::VertexArray v; v.append( rewards[tar] );
        window.draw(v);
    
        std :: vector <sf::CircleShape> circle = agent.inter(edges);
        for(int i = 0; i < rayCount; i++)
        {
            window.draw( circle[i] );
        }

        window.display();

        if ( agent.touch(edges) )
        {
            Brain temp = agent.brain;
            agent = Agent(2, spawn, size * width, rayCount);
            agent.brain = temp;
        }

        if (!training)
        {
            if ( Wpress )
            {
                agent.translate(5);
                Wpress = false;
            }

            if ( Spress )
            {
                agent.translate(-5);
                Spress = false;
            }

            if ( Apress )
            {
                agent.rotate(pi / 20);
                Apress = false;
            }

            if ( Dpress )
            {
                agent.rotate(-pi / 20);
                Dpress = false;
            }
        }
        else
        {
            target = rewards[ tar ];

            getInputs(*input, agent, circle);

            agent.brain.Decide(*input);

            int max = 0;

            for(int i = 0; i < 4; i++)
            {
                if ( agent.brain.layers[ agent.brain.count - 1 ].WeightedSums.arr[i] > agent.brain.layers[ agent.brain.count - 1 ].WeightedSums.arr[max] )
                    max = i;
            }

            act(max, agent);

            getTragets(targets, edges, target, agent);

            agent.brain.Train(targets, *input);

            Edge tempE; tempE.p1 = agent.center; tempE.p2 = rewards[ tar ];


            if ( tempE.magnitude() < size * width )
            {
                tar++;

                if ( tar >= rewards.size() ) return 0;
            }
        }
    }

    if (input) delete[] input;
}