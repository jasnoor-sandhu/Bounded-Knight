#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

#define WIDTH 16000 
#define HEIGHT 16000
#define PIXEL_SIZE 0.050
#define FRAME_RATE 120

struct POINT{
    int x;
    int y;
};
const struct POINT center = {WIDTH/2,HEIGHT/2};

class FRAME{
public:
    uint32_t framebuffer[WIDTH * HEIGHT];
    uint32_t index = 0;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;

    int Init();
    void Render();
    void Destroy();
    void SaveImage(std::string FileName = "Out_");
    void put_pixel( int x, int y, uint32_t color);
    void clear(uint32_t color);
    uint32_t getColor(int x, int y);
};
