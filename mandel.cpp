#include <iostream>
#include <complex>
#include <cstdint>
#include <random>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>


#define WIDTH 1280 
#define HEIGHT 1024

std::complex<float> f(std::complex<float> z, std::complex<float> c)
{
        return (z * z) + c;
}

uint32_t mandel(float re, float im, float r, std::complex<float> z)
{
        std::complex<float> num;
        std::complex<float> tmp;
        //std::complex<float> z;
        uint32_t i;
        num.real(re);
        num.imag(im);
        //z.real(1.134834f);
        //z.imag(-1.2345325f);
        //z.real(0.0f);
        //z.imag(0.0f);
        i = 0;
        do{
                tmp = f(z, num);
                z = tmp;
                i++;
                if(i >= 50){
                        return 0;
                }
        }while(std::abs(z) < r);
        return i;
}

int main(void)
{       
        uint32_t i;
        float re, im;
        SDL_Surface *surface;
        SDL_Window *window;
        SDL_Renderer *renderer;
        std::complex<float> num;
        std::complex<float> tmp;
        std::default_random_engine randgen;
        std::uniform_real_distribution<float> dist(-2.0, 2.0);

        float r;
        std::complex<float> z(0.0f, 0.0f);
        SDL_Init(SDL_INIT_VIDEO); 
        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE,
                                        &window, &renderer);
        SDL_RenderClear(renderer);
        r = 10;
        for(int k = 0; k < 10; k++){
                z.real(dist(randgen));
                z.imag(dist(randgen));
                for(int32_t y = 0; y < HEIGHT; y++){
                        for(int32_t x = 0; x < WIDTH; x++){
                                i = mandel(4.0*(x-WIDTH/2)/WIDTH, 4.0*(y-HEIGHT/2)/HEIGHT, r, z);
                                SDL_SetRenderDrawColor(renderer, 0xff - (uint8_t)(i*8), 
                                                         0x7F - (uint8_t)(i*8), (uint8_t)(i*8),
                                                          (uint8_t)(i*8));
                                SDL_RenderDrawPoint(renderer, x, y);
                        }
                }
                SDL_RenderPresent(renderer);
                SDL_Delay(5000);
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
