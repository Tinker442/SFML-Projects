#include <SFML/Graphics.hpp>
#include <iostream>
//test
int main()
{
    const sf::Vector2i w={640,480};
    auto window = sf::RenderWindow{ sf::VideoMode(w.x,w.y), "CMake SFML Project" };
    window.setFramerateLimit(144);

    sf::Vector2f vel= {4,4};
    float r = 50.0;
    sf::CircleShape shape(r);
    shape.setPosition(10,0);
    shape.setFillColor(sf::Color(100,250,50));

    while (window.isOpen())
    {   
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        //
        
        shape.move(vel);
        const sf::Vector2 pos = shape.getPosition();
        std::cout << pos.x <<","<< pos.y << " : ";

        if(pos.x+2*r > w.x || pos.x<0){
            vel.x*=-1;
        }
        if(pos.y+2*r > w.y || pos.y<0){
            vel.y*=-1;
        }



        window.draw(shape);
        

        //
        window.display();
    }
}