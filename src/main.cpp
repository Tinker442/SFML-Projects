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

        Ball(string col, sf::Vector2f vel,sf::Vector2i pos){
            velocity=vel;
            color=col;
            this->setPosition(pos.x,pos.y);
            this->setFillColor(colors[col]);

        };
        Ball(){
            generateRandomVelocity();
            color="White";
            this->setPosition(0,0);
            this->setFillColor(colors["White"]);
        };
        void setColor(string col){
            color = col;
            this->setFillColor(colors[col]);
        }
        string getColor(){
            return color;
        }

        sf::Vector2f getVelocity(){
            return velocity;
        }

        void setVelocity(sf::Vector2f vel){
            velocity=vel;
        }
        void generateRandomVelocity(int min =1,int max = 8){
            float xvel=rand()%(2*max/2 -min)+min;
            float yvel=rand()%(2*max/2-min) +min;
            velocity = {xvel,yvel};
        }
        void generateRandomPosition(sf::Vector2i w ,int radius){
            int xpos=rand()%(w.x - 2*radius);
            int ypos=rand()%(w.y - 2*radius);
            this->setPosition(xpos,ypos);}

        /* 
        Moves the ball and updates for any collisions
         */
        void update(sf::Vector2i w){
            
            this->move(velocity);

            int rad = this->getRadius();
            const sf::Vector2 pos = this->getPosition();

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

        friend ostream& operator<<(ostream& os, Ball& ball){
            os<<"Position: " <<ball.getPosition().x<<","<<ball.getPosition().y<<"\n";
            os<<"Velocity: " <<ball.getVelocity().x<<","<<ball.getVelocity().y<<"\n";
            os<<"Radius: " <<ball.getRadius() << "\n";
            os<<"Color: " <<ball.getColor()<<"\n\n";
            return os;
        }

};
/* Adds color scheme to sf::RectangleShape */
class Tile: public sf::RectangleShape{
    private:
        string color;
        map < string,sf::Color > off_colors {
            {"Black",sf::Color(50,50,50)},
            {"White",sf::Color(200,200,200)},
            {"Red",sf::Color(255,100,100)},
            {"Green",sf::Color(100,255,100)},
            {"Blue",sf::Color(100,100,255)},
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
            const int rad = ball.getRadius();

            sf::Vector2f size= this->getSize();

            sf::Vector2f velocity= ball.getVelocity();

            float dx = velocity.x;
            float dy = velocity.y;

            float ball_left_edge = b_pos.x;
            float ball_right_edge = b_pos.x+2*rad;
            float ball_bottom_edge = b_pos.y;
            float ball_top_edge = b_pos.y+2*rad;

            float tile_left_edge = t_pos.x;
            float tile_right_edge = t_pos.x+size.x;
            float tile_bottom_edge = t_pos.y;
            float tile_top_edge = t_pos.y+size.y;

            if(color != ball.getColor()){
                if (ball_bottom_edge<tile_top_edge && ball_top_edge>tile_bottom_edge){
                    if(ball_right_edge > tile_left_edge && ball_right_edge-tile_left_edge<=dx){
                        //bounce
                        velocity.x*=-ball.damping_factor;
                        //update color of tile
                        this->setColor(ball.getColor());
                    }
                    else if (ball_left_edge < tile_right_edge && ball_left_edge-tile_right_edge>=dx){
                        velocity.x*=-ball.damping_factor;
                        this->setColor(ball.getColor());
                    }

                }
                if (ball_right_edge>tile_left_edge && ball_left_edge<tile_right_edge){
                    if(ball_top_edge > tile_bottom_edge && ball_top_edge-tile_bottom_edge<=dy){
                        velocity.y*=-ball.damping_factor;

                        this->setColor(ball.getColor());
                    }
                    else if (ball_bottom_edge < tile_top_edge && ball_bottom_edge-tile_top_edge>=dy){
                        velocity.y*=-ball.damping_factor;

                        this->setColor(ball.getColor());
                    }

                }
                //check if the tile was changed with a ball still in it
                if(ball_bottom_edge>tile_bottom_edge && ball_top_edge<tile_top_edge && ball_left_edge>tile_left_edge && ball_right_edge<tile_right_edge){
                    this->setColor(ball.getColor());
                }

            }
            ball.setVelocity(velocity);
        }
};

void generateBalls(vector<Ball> &balls, map<string,int> colors, sf::Vector2i w, int rad = 20 ){
    for (auto color:colors){
        string col_string = color.first;
        int num = color.second;
        for (int i=0;i<num;i++){
            Ball ball;
            
            ball.generateRandomVelocity(1,8);
            ball.setColor(col_string);
            ball.generateRandomPosition(w,rad);
            ball.setRadius(rad);
            balls.push_back(ball);
        }
    }
    
}


int main()
{
    //Params
    const sf::Vector2i w={1920,1080}; //window size
    float rad = 30;
    int grid_size=120;
    int fps = 144;
    srand(time(NULL));
    //

    sf::RenderWindow window = sf::RenderWindow{ sf::VideoMode(w.x,w.y), "Breakout Pressure" };
    window.setFramerateLimit(fps);

    // Ball shape;
    sf::Vector2i pos;

    vector<Ball> balls;
    map<string,int> colors={
        {"White",1},
        {"Black",20},
        {"Red",4},
        {"Green",2},
        {"Blue",1}
    };

    generateBalls(balls,colors,w,rad);

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
                for (auto& ball:balls){
                    grid[r][c].ballCollision(ball);
                }
                window.draw(grid[r][c]);
            }
        }

        for (auto& ball:balls){

            ball.update(w);
            // cout<<ball;
            window.draw(ball);

        }

        window.display();
    }
}