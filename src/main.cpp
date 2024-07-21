#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <map>
//test
using namespace std;

/* Adds velocity and color scheme to sf::CircleShape */
class Ball: public sf::CircleShape{
    private:
        //pixel per fps speed
        sf::Vector2f velocity;
        //color scheme name
        string color; 

        //color scheme
        map< std::string, sf::Color > colors {
        {"Black",sf::Color::Black},
        {"White",sf::Color::White},
        {"Red",sf::Color::Red},
        {"Green",sf::Color::Green},
        {"Blue",sf::Color::Blue},
        };
        
    public:
        //amount to change velocity on collisions
        float damping_factor=1;

        Ball(string col, sf::Vector2f vel){
            velocity=vel;
            color=col;
            this->setFillColor(colors[col]);

        };
        Ball(){
            velocity={2,2};
            color="White";
            this->setFillColor(colors["White"]);
        };
        string getColor(){
            return color;
        }

        sf::Vector2f getVelocity(){
            return velocity;
        }

        void setVelocity(sf::Vector2f vel){
            velocity=vel;
        }

        /* 
        Moves the ball and updates for any collisions
         */
        void update(sf::Vector2i w){
            
            this->move(velocity);

            int rad = this->getRadius();
            const sf::Vector2 pos = this->getPosition();
            // cout << pos.x <<","<< pos.y << " : ";

            //collision
            if(pos.x+2*rad > w.x || pos.x<0){
                velocity.x*=-damping_factor;
            }
            if(pos.y+2*rad > w.y || pos.y<0){
                velocity.y*=-damping_factor;
            }

            //overlap correction
            if(pos.y<0){
                this->setPosition(pos.x,0);
            }else if(pos.y>w.y-2*rad){
                this->setPosition(pos.x,w.y-2*rad);
            }  
            if(pos.x<0){
                this->setPosition(0,pos.y);
            }else if(pos.x>w.x-2*rad){
                this->setPosition(w.x-2*rad,pos.y);
            } 
        }

};
/* Adds color scheme to sf::RectangleShape */
class Tile: public sf::RectangleShape{
    private:
        string color;
        map < string,sf::Color > off_colors {
            {"Black",sf::Color(50,50,50)},
            {"White",sf::Color(200,200,200)},
            {"Red",sf::Color(255,50,50)},
            {"Green",sf::Color(50,255,50)},
            {"Blue",sf::Color(50,50,255)},
        };
    public:
        void setColor(string col){
            color=col;
            this->setFillColor(off_colors[col]);
            
        }
        string getColor(){
            return color;
        }
        /* Checks if a given ball has collided with this tile and updates the tile color scheme and ball velocity */
        void ballCollision(Ball& ball){
            const sf::Vector2 b_pos=ball.getPosition();
            const sf::Vector2 t_pos=this->getPosition();

            sf::Vector2f size= this->getSize();

            sf::Vector2f velocity= ball.getVelocity();

            float ball_left_edge = b_pos.x;
            float ball_right_edge = b_pos.x+2*ball.getRadius();
            float ball_bottom_edge = b_pos.y;
            float ball_top_edge = b_pos.y+2*ball.getRadius();

            float tile_left_edge = t_pos.x;
            float tile_right_edge = t_pos.x+size.x;
            float tile_bottom_edge = t_pos.y;
            float tile_top_edge = t_pos.y+size.y;

            if(color != ball.getColor()){
                if (ball_bottom_edge<tile_top_edge && ball_top_edge>tile_bottom_edge){
                    if(ball_right_edge > tile_left_edge && ball_left_edge<tile_left_edge && velocity.x>0){
                        velocity.x*=-ball.damping_factor;
                        this->setColor(ball.getColor());
                    }
                    if (ball_left_edge < tile_right_edge && ball_right_edge>tile_right_edge && velocity.x<0){
                        velocity.x*=-ball.damping_factor;
                        this->setColor(ball.getColor());
                    }

                }
                if (ball_right_edge>tile_left_edge && ball_left_edge<tile_right_edge){
                    if(ball_top_edge > tile_bottom_edge && ball_bottom_edge<tile_bottom_edge && velocity.y>0){
                        velocity.y*=-ball.damping_factor;
                        this->setColor(ball.getColor());
                    }
                    if (ball_bottom_edge < tile_top_edge && ball_top_edge>tile_top_edge && velocity.y<0){
                        velocity.y*=-ball.damping_factor;
                        this->setColor(ball.getColor());
                    }

                }

            }
            ball.setVelocity(velocity);
        }
};

sf::Vector2f generateRandomVelocity(int min =1,int max = 8){
    float xvel=rand()%(2*max/2 -min)+min;
    float yvel=rand()%(2*max/2-min) +min;
    return {xvel,yvel};
}
sf::Vector2i generateRandomPosition(sf::Vector2i w ,int radius){
    int xpos=rand()%(w.x - 2*radius);
    int ypos=rand()%(w.y - 2*radius);
    return {xpos,ypos};
}


int main()
{
    //Params
    const sf::Vector2i w={1920,1080}; //window size
    // sf::Vector2f vel= {2,-1};
    float rad = 20;
    int grid_size=120;
    int fps = 144;
    srand(time(NULL));
    //

    sf::RenderWindow window = sf::RenderWindow{ sf::VideoMode(w.x,w.y), "Breakout Pressure" };
    window.setFramerateLimit(fps);

    // Ball shape;
    sf::Vector2i pos;

    Ball wb("White",generateRandomVelocity());
    pos=generateRandomPosition(w,rad);
    // wb.setPosition(10,w.y-2*rad);
    wb.setPosition(pos.x,pos.y);
    wb.setRadius(rad);

    Ball bb("Black",generateRandomVelocity());
    pos=generateRandomPosition(w,rad);
    // bb.setPosition(w.x-2*rad,10);
    bb.setPosition(pos.x,pos.y);
    bb.setRadius(rad);

    Ball rb("Red",generateRandomVelocity());
    pos=generateRandomPosition(w,rad);
    // rb.setPosition(w.x-2*rad,w.y-2*rad);
    rb.setPosition(pos.x,pos.y);
    rb.setRadius(rad);

    Ball ub("Blue",generateRandomVelocity());
    pos=generateRandomPosition(w,rad);
    // ub.setPosition(10,10);
    ub.setPosition(pos.x,pos.y);
    ub.setRadius(rad);
   
    sf::Vector2i tdim = {w.x/grid_size,w.y/grid_size}; //4,3
    Tile grid[tdim.x][tdim.y];
    //initialize the tile grid
    for (int r=0;r<tdim.x;r++){
        for (int c=0;c<tdim.y;c++){
            grid[r][c].setSize(sf::Vector2f(grid_size,grid_size));
            grid[r][c].setPosition(r*grid_size,c*grid_size);
            // grid[r][c].setColor("Black");
            grid[r][c].setFillColor(sf::Color(0,r*5,c*5));
        }
    }




    while (window.isOpen())
    {   
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // window.clear(sf::Color::Black);
        
        for (int r=0;r<tdim.x;r++){
            for (int c=0;c<tdim.y;c++){
                grid[r][c].ballCollision(wb);
                grid[r][c].ballCollision(bb);
                grid[r][c].ballCollision(rb);
                grid[r][c].ballCollision(ub);
                window.draw(grid[r][c]);
            }
        }

        wb.update(w);
        window.draw(wb);
        bb.update(w);
        window.draw(bb);
        rb.update(w);
        window.draw(rb);
        ub.update(w);
        window.draw(ub);
        

        //
        window.display();
    }
}