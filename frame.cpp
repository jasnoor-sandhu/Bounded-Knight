#include "frame.h"
#include <cstdio>
#include <cstdlib>

void FRAME::Destroy()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void FRAME::Render()
{
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            WIDTH * sizeof(uint32_t)
    );
    //Rendering
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int FRAME::Init()
{
    window = SDL_CreateWindow(
            "SDL framebuffer",
            WIDTH * PIXEL_SIZE,
            HEIGHT * PIXEL_SIZE,
            0
    );
    if (window == NULL)
    {
        fprintf(stderr, "SDL_CreateWindow failed: %s \n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window,NULL);
    if (renderer == NULL)
    {
        fprintf(stderr, "SDL_CreateRenderer failed: %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING,WIDTH,HEIGHT);
    if (texture == NULL)
    {
        fprintf(stderr, "SDL_CreateTexture failed: %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    return 0;
}

void FRAME::put_pixel( int x, int y, uint32_t color)
{
    if ( x<0 || y <0 || x>= WIDTH || y>= HEIGHT){
        return;
    }
    framebuffer[WIDTH*y+x] = color;
}

void FRAME::clear(uint32_t color)
{
    for (int x = 0; x < WIDTH * HEIGHT ; x++){
        framebuffer[x] = color;
    }
}

uint32_t FRAME::getColor(int x, int y)
{
    return framebuffer[y*WIDTH+x];
}

void FRAME::SaveImage(std::string FileName)
{
    SDL_Surface *surface = SDL_RenderReadPixels(renderer, NULL);
    if (surface) {
        SDL_SaveBMP(surface, "frame.bmp");
        SDL_DestroySurface(surface);

        system("mkdir -p Output");
        printf("Output saved in 'Output' directory\n");

        FILE *pipe = popen("ls ./Output/*.jpg | sed 's/.jpg//' | sort -n | tail -1", "r");

        int n = 0;
        if (pipe) {
            fscanf(pipe, "./Output/Out_%d", &n);
            pclose(pipe);
        }

        printf("Largest number: %d\n", n);

        std::string cmd = std::string("sips -s format jpeg frame.bmp --out ./Output/") + FileName + std::to_string(n+1) + std::string(".jpg");
        system(cmd.c_str());
    }
}
