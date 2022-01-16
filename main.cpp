#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <list>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class SnakeGame : public olc::PixelGameEngine
{
    public:
    SnakeGame()
    {
        sAppName = "Snake Game";
    }

    public:
    bool lose ;
    bool won  ;
    bool apple;
    olc::vi2d applePos;

    public:
    typedef struct Snake{
        olc::vi2d pos;
        olc::vi2d direction;
        int blockSize;
        int size;
        float speed;
        int points;
        std::list<olc::vi2d> posList;
    }Snake;

    private:
    Snake pSnake;

    private:
    void drawSnake(){
        for (auto const& i : pSnake.posList) {
            DrawRect(i, olc::vi2d(pSnake.blockSize,pSnake.blockSize), olc::WHITE);
        }
    }

    private:
    void addBlock()
    {
        pSnake.size++;
        olc::vi2d lastBlock = olc::vi2d(pSnake.posList.back().x, pSnake.posList.back().y);
        lastBlock.x += (pSnake.blockSize * -pSnake.direction.x);
        lastBlock.y += (pSnake.blockSize * -pSnake.direction.y);
        pSnake.posList.push_back(lastBlock);
    }

    private:
    void changeDirection(int x, int y){
        pSnake.direction = olc::vi2d(x,y);
    }

    private:
    void moveAllBlocks(){
        olc::vi2d last = pSnake.posList.front();
        pSnake.pos.x += (pSnake.blockSize * pSnake.direction.x); 
        pSnake.pos.y += (pSnake.blockSize * pSnake.direction.y);
        last.x += (pSnake.blockSize * pSnake.direction.x);
        last.y += (pSnake.blockSize * pSnake.direction.y);
        for(auto& pos : pSnake.posList){
            olc::vi2d aux;
            aux = pos;
            pos = last;
            last = aux;
        }
    }

    private:
    void testBorder(){
        if(pSnake.pos.x > ScreenWidth()){
            pSnake.pos.x = 0;
            pSnake.posList.front().x = 0;
        }
        else if(pSnake.pos.y > ScreenHeight()){
            pSnake.pos.y = 0;
            pSnake.posList.front().y = 0;
        }
        else if(pSnake.pos.x < 0){
            pSnake.pos.x = ScreenWidth();
            pSnake.posList.front().x = ScreenWidth();
        }
        else if(pSnake.pos.y < 0){
            pSnake.pos.y = ScreenHeight();
            pSnake.posList.front().y = ScreenHeight();
        }
        else{
            return;
        }
    }

    private:
    void createApple(){
        if(!apple){
            srand(time(NULL));
            bool inside_snake = true;
            int new_x;
            int new_y;   
            while(inside_snake){
                new_x = rand() % ScreenWidth();
                new_y = rand() % ScreenHeight();

                for(auto& pos : pSnake.posList){
                    if(PointInSquare(pos, olc::vi2d(new_x, new_y))){
                        inside_snake = true;
                    }  
                    else{
                        inside_snake = false;
                    }
                } 
            }
            applePos = olc::vi2d(new_x, new_y);
            apple = true;
        }
    }

    private:
    int pointsGenerator(){
        return (pSnake.speed * pSnake.size) * 10;
    }

    private:
    void testAppleHit(){
            if(apple){
                if(PointInSquareApple(pSnake.posList.front())){
                    apple = false;
                    pSnake.points += pointsGenerator();
                    if(pSnake.speed > 0.45){
                        pSnake.speed -= 0.01f;
                    }
                    addBlock();
                }    
            }
    }
    
    private:
    bool PointInSquareApple(olc::vi2d posSquare) {
        
        olc::vi2d squareApple = olc::vi2d(applePos.x + pSnake.blockSize, applePos.y + pSnake.blockSize);

        for(int y = 0; y < posSquare.y + pSnake.blockSize; y+=2){
            for(int x = 0; x < posSquare.x + pSnake.blockSize; x+=2){
                if(applePos.x <= x && squareApple.x >= x && applePos.y <= y && squareApple.y >= y){
                    return true;
                }
            }
        }
        return false;
    }

    bool PointInSquare(olc::vi2d posSquare, olc::vi2d posTest) {
        
        for(int y = 0; y < posSquare.y + pSnake.blockSize; y++){
            for(int x = 0; x < posSquare.x + pSnake.blockSize; x++){
                if(posTest.x == x && posTest.y == y){
                    return true;
                }
            }
        }
        return false;
    }

    private:
    void testLose(){
        olc::vi2d first = pSnake.posList.front();
        for(auto& pos : pSnake.posList){
            if(first.x + (pSnake.blockSize * pSnake.direction.x) == pos.x && first.y + (pSnake.blockSize * pSnake.direction.y) == pos.y){
                lose = true;
            }
        }
    }

    public:
	bool OnUserCreate() override
	{
        pSnake = {
            olc::vi2d(ScreenWidth()/2,ScreenHeight()/2),
            olc::vi2d(1,0),
            25,
            1,
            0.25f,
            0
        };
        pSnake.posList.push_front(pSnake.pos);
        addBlock();
        lose  = false;
        won   = false;
        apple = false;
		return true;
	}

    private:
    float sumTime = 0.0f;

	bool OnUserUpdate(float fElapsedTime) override
	{
        testBorder();
        testLose();
        FillRect(olc::vi2d(0,0), olc::vi2d(ScreenWidth(),ScreenHeight()), olc::BLACK);
        DrawString(olc::vi2d(10,10), "Points: " + std::to_string(pSnake.points), olc::WHITE);
        if(GetKey(olc::Key::RIGHT).bPressed){
            changeDirection(1,0);
        }
        if(GetKey(olc::Key::LEFT).bPressed){
            changeDirection(-1,0);
        }
        if(GetKey(olc::Key::UP).bPressed){
            changeDirection(0,-1);
        }
        if(GetKey(olc::Key::DOWN).bPressed){
            changeDirection(0,1);
        }
        if(GetKey(olc::Key::R).bPressed && lose){
            OnUserCreate();
        }
        sumTime += fElapsedTime;
        if(sumTime >= pSnake.speed && !lose){
            sumTime = 0;
            moveAllBlocks();
            testAppleHit();
            createApple();
        }
        drawSnake();

        if(apple)
            FillCircle(applePos, 4, olc::RED);
        if(lose){
            DrawString(olc::vi2d(ScreenWidth()/2 - ((GetTextSize("Game Over").x/2)*4), ScreenHeight()/2 - ((GetTextSize("Game Over").y/2)*4)), "Game Over", olc::WHITE, 4U);
            DrawString(olc::vi2d(ScreenWidth()/2 - ((GetTextSize("Press R to Restart").x/2)*2), (ScreenHeight()/2 + 40) - ((GetTextSize("Press R to Restart").y/2)*2)), "Press R to Restart", olc::WHITE, 2U);
        }
		return true;
	}

};

int main() {
    SnakeGame sg;
    if(sg.Construct(800,600,4,4))
        sg.Start();
    return 0;
}