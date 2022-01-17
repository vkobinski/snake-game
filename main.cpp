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
    float tongueTime = 0;

    private:
    void drawSnake(float fElapsedTime){
        olc::vi2d front = pSnake.posList.front();
        int xSize = 10;
        int ySize = 5;
        float bodyAngle;
        int aux;
        switch (pSnake.direction.x)
        {
        case 1:
            front.x += (pSnake.blockSize);
            front.y += (pSnake.blockSize/2);
            break;
        case -1:
            front.x -= xSize;
            front.y += (pSnake.blockSize/2);
        default:
            break;
        }
        switch (pSnake.direction.y)
        {
        case 1:
            aux = xSize;
            xSize = ySize;
            ySize = aux;
            front.x += ((pSnake.blockSize/2) - (xSize /2));
            front.y += (pSnake.blockSize);
            break;
        case -1:
            aux = xSize;
            xSize = ySize;
            ySize = aux;
            front.x += ((pSnake.blockSize/2) - (xSize /2));
            front.y -= ySize;
            break;
        default:
            break;
        }

        olc::vi2d size = olc::vi2d(xSize, ySize);

        if(tongueTime >= 1.8f){
            FillRect( front, size, olc::RED);
            tongueTime = 0;
        }
        else if(tongueTime >= 1.2f){
            FillRect(front, size, olc::RED);
        }

        int bodySelect = 5;
        for (auto const& i : pSnake.posList) {
            //FillRect(i, olc::vi2d(pSnake.blockSize,pSnake.blockSize), olc::GREEN);
            if(pSnake.posList.front() == i){
                FillRect(i, olc::vi2d(pSnake.blockSize, pSnake.blockSize), olc::GREEN);
                bodySelect = 0;
            }
            else if(bodySelect == 0){
                bodySelect = 1;
                FillRect(i, olc::vi2d(pSnake.blockSize, pSnake.blockSize), olc::DARK_GREEN);
            }
            else if(bodySelect == 1){
                bodySelect = 0;
                FillRect(i, olc::vi2d(pSnake.blockSize, pSnake.blockSize), olc::GREEN);
            }
        }

        tongueTime += fElapsedTime;
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
                new_x = rand() % (ScreenWidth());
                new_y = rand() % (ScreenHeight());

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
        int bS = pSnake.blockSize;
        if(applePos.x <= posSquare.x + bS && applePos.x >= posSquare.x && applePos.y <= posSquare.y + bS && applePos.y >= posSquare.y){
                    return true;
        }
        return false;
    }

    bool PointInSquare(olc::vi2d posSquare, olc::vi2d posTest) {
        int bS = pSnake.blockSize;
        if(posTest.x <= posSquare.x + bS && posTest.x >= posSquare.x && posTest.y <= posSquare.y + bS && posTest.y >= posSquare.y){
                    return true;
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
        drawSnake(fElapsedTime);

        if(apple){
            FillCircle(olc::vi2d(applePos.x-2,applePos.y-7), 3, olc::GREEN);
            FillCircle(olc::vi2d(applePos.x+2,applePos.y-7), 2, olc::GREEN);
            FillCircle(applePos, 5, olc::RED);
        }
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