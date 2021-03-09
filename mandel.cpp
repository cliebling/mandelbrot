/*
 * Copyright (C) 2020 Charles R. Liebling
 *
 * mandel.cpp
 * Draws a Mandelbrot set that moves in response to the mouse cursor.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */ 
#include <iostream>
#include <complex>
#include <cstdint>
#include <cstdlib>
#include <random>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mouse.h>
#define MAXWIDTH 2560
#define MAXHEIGHT 1440
#define MAXRADIUS 20.0
#define WIDTH 640 
#define HEIGHT 480

std::complex<float> f(std::complex<float> z, std::complex<float> c)
{
        return (z * z) + c;
}

uint32_t mandel(float re, float im, float r, std::complex<float> z)
{
        std::complex<float> num;
        std::complex<float> tmp;
        uint32_t i;
        num.real(re);
        num.imag(im);
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

uint32_t draw_mandel(int32_t width, int32_t height, float r,
                     std::complex<float> z, SDL_Renderer *renderer)
{
        int32_t i;
        for(int32_t y = 0; y < height; y++){
                for(int32_t x = 0; x < width; x++){
                        i = mandel(4.0*(x-width/2)/width,
                                   4.0*(y-height/2)/height, r, z);
                        SDL_SetRenderDrawColor(renderer, 0xff - (uint8_t)(i*8), 
                                         0x7F - (uint8_t)(i*8), (uint8_t)(i*8),
                                         (uint8_t)(i*8));
                        SDL_RenderDrawPoint(renderer, x, y);
                }
        }
        return 0;
}

int main(int argc, char **argv)
{       
        uint32_t i;
        float re, im;
        float tmpvalf;
        float r;
        int32_t error;
        int32_t width, height, tmpval;
        int32_t mx, my, mxp, myp;
        int32_t opt;
        bool change, quit;
        SDL_Surface *surface;
        SDL_Window *window;
        SDL_Renderer *renderer;
        std::complex<float> num;
        std::complex<float> tmp;
        std::default_random_engine randgen;
        std::uniform_real_distribution<float> dist(-2.0, 2.0);
        std::complex<float> z(0.0f, 0.0f);
        
        width = WIDTH;
        height = HEIGHT;
        r = 10.0;
        while((opt = getopt(argc, argv, "r:x:y:")) != -1){
                switch (opt) {
                        case 'r':
                                tmpval = strtof(optarg, NULL);
                                if((0 < tmpval)&&(tmpval <= MAXRADIUS)){
                                        r = tmpval;
                                }
                                break;
                        case 'x':
                                tmpval = strtol(optarg, NULL, 10);
                                if((0 < tmpval)&&(tmpval <= MAXWIDTH)){
                                        width = tmpval;
                                }
                                break;
                        case 'y':
                                tmpval = strtol(optarg, NULL, 10);
                                if((0 < tmpval)&&(tmpval <= MAXHEIGHT)){
                                        height = tmpval;
                                }
                                break;
                        default:
                                break;
                }
        }
        error = SDL_Init(SDL_INIT_VIDEO);
        if(error != 0){
                std::cerr<<"Could not initialize video, SDL_Init returned ";
                std::cerr<<error<<std::endl;
                return error;
        }

        error = SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_RESIZABLE,
                                        &window, &renderer);
        if(error != 0){
                std::cerr<<"Error: SDL_CreateWindowAndRenderer returned ";
                std::cerr<<error<<std::endl;
                SDL_Quit();
                return error;
        }
        SDL_RenderClear(renderer);
        mx = 0;
        my = 0;
        mxp = 0;
        myp = 0;
        quit = false;
        change = true;
        while(!quit){
                SDL_PumpEvents();
                if(SDL_GetMouseState(&mx, &my) & SDL_BUTTON(SDL_BUTTON_LEFT)){
                        quit = true;
                }
                if((mx != mxp) || (my != myp)){
                        mxp = mx;
                        myp = my;
                        z.real(float(mx - width/2)/float(width));
                        z.imag(float(my - height/2)/float(height));
                        draw_mandel(width, height, r, z, renderer);
                        SDL_RenderPresent(renderer);
                }
        }
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
