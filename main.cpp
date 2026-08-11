#include <iostream>
#include "frame.h"

class Knight{
public:
    int index = 0; // Index start at 0 and increments as Knight takes steps.
    struct POINT currentPosition = center; //Current position in framebuffer.
    uint32_t color;
    int tag;
    int direction;//Direction the Knight is Going
                  //0->right, 1->Up, 2-> Left, 3->Down
    
    //DirectionHelpers
    int K_ones;
    int K_zeros;
    int K_gap;
    int K_gap_count;

    void goToNextPosition();
    bool changeDirection();
    bool isCurrentPositionSafe();
    void play();
};

FRAME frame;

class GAME
{
public:
    Knight BLACK;
    Knight RED;
    int computePerFrame;
    int turn;

    void init();
    void compute();
    uint32_t getMyColor();
    void nextTurn();

};

GAME game;

void GAME::init()
{
    computePerFrame = 2000000;
    turn = 1;//0->finished; 1-> black, 2-> red

    BLACK.color = 0x000000;
    BLACK.tag = 1;
    RED.color = 0xFF0000;
    RED.tag = 2;
    BLACK.direction = 0;
    RED.direction = 0;

    BLACK.K_ones = 1;
    BLACK.K_zeros = 0;
    BLACK.K_gap = 1;
    BLACK.K_gap_count = 0;
    RED.K_ones = 1;
    RED.K_zeros = 0;
    RED.K_gap = 1;
    RED.K_gap_count = 0;

}

bool Knight::changeDirection()
{
    if (K_ones < 3) {
        K_ones++;
        return true;
    }

    if (K_zeros < K_gap) {
        K_zeros++;
        return false;
    }

    K_zeros = 0;
    K_gap_count++;

    if (K_gap_count == 2) {
        K_gap++;
        K_gap_count = 0;
    }
    return true;
}


void Knight::goToNextPosition()
{
    switch (direction){
        case 0:
            currentPosition.x += 1;
            if (changeDirection())
                direction = 1;
            break;
        case 1:
            currentPosition.y -=1;
            if (changeDirection())
                direction = 2;
            break;
        case 2:
            currentPosition.x -=1;
            if (changeDirection())
                direction = 3;
            break;
        case 3:
            currentPosition.y +=1;
            if (changeDirection())
                direction = 0;
            break;
        default:
            fprintf(stderr,"Unexpected Direction Value\n");
            break;
    }
}

bool Knight::isCurrentPositionSafe()
{
    const int padding = 5;
    if (currentPosition.x >= WIDTH-padding || currentPosition.y >= HEIGHT-padding)
    {
        game.turn = 0;
        return false;
    }
    if (frame.getColor(currentPosition.x,currentPosition.y) != 0xFFFFFF)
        return false;

    //Knight Logic
    struct POINT Knight_moves[8] = {
                { 1,  2}, { 1, -2}, {-1,  2}, {-1, -2}, // x=+-1, y=+-2
                { 2,  1}, { 2, -1}, {-2,  1}, {-2, -1}  // x=+-2, y=+-1
    };
    
    for (int i = 0; i < 8; i++) {
        uint32_t getcolor = frame.getColor(currentPosition.x+(Knight_moves[i].x),currentPosition.y+(Knight_moves[i].y));
        if ( getcolor != 0xFFFFFF )
        {
            if ( getcolor != color )
                return false;
        }
    }
    return true;

}

uint32_t GAME::getMyColor()
{
    uint32_t color = 0xFFFFFF;
    switch (turn){
        case 1: //black
            color = BLACK.color;
            break;
        case 2: //red
            color = RED.color;
            break;
        default:
            break;
    }
    return color;
}
void GAME::nextTurn()
{
    if (turn == 1)
        turn =2;
    else if (turn == 2)
        turn =1;
}

void Knight::play()
{
    if (index == 0){
        if(isCurrentPositionSafe())
        {
            frame.put_pixel(currentPosition.x,currentPosition.y,color);
            game.nextTurn();
        }
        index ++;
        return;
    }
    goToNextPosition();
    if(isCurrentPositionSafe())
    {
        frame.put_pixel(currentPosition.x,currentPosition.y,color);
        game.nextTurn();
    }
    index++;
}

void GAME::compute()
{
    if(turn==0) return;
    for (int i = 0;i<computePerFrame;i++){
        if (turn == 1)
            BLACK.play();
        else if (turn == 2)
            RED.play();
    }
}

int main()
{
    srand(time(NULL));
    int ret = frame.Init();
    if (ret != 0 )
        return ret;
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL INIT failed: %s \n", SDL_GetError());
        return EXIT_FAILURE;
    }

    const double target_frame = 1.0 / (double)FRAME_RATE;
    uint8_t is_running = 1;

    game.init();

    frame.clear(0xFFFFFF);

    while(is_running)
    {
        uint64_t start = SDL_GetPerformanceCounter();
        //Polling
        while(SDL_PollEvent(&frame.event))
        {
            if (frame.event.type == SDL_EVENT_QUIT)
                is_running = 0;
        }

        //frame.clear(0xFFFFFF);
        //working area
        game.compute();


        //Rendering
        frame.Render();
        uint64_t end = SDL_GetPerformanceCounter();
        double elapsed = (double)(end-start) / (double)SDL_GetPerformanceFrequency();
        if ( elapsed < target_frame)
        {
            SDL_Delay((target_frame - elapsed) * 1000.0);
        }
        frame.index += 1;
    }
    frame.SaveImage();
    frame.Destroy();

    SDL_Quit();
    return EXIT_SUCCESS;
}
