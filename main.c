#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE 720
#define R 360

void serrc(int code){
    if(code<0){
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(-1);
    }
}

void serrp(void *ptr){
    if(ptr == NULL){
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(-1);
    }
}

void sdl_ellipse(SDL_Renderer* r, int x0, int y0, int radiusX, int radiusY)
{
    float pi  = 3.14159265358979323846264338327950288419716939937510;
    float pih = pi / 2.0; //half of pi

    const int prec = 27; // precision value; value of 1 will draw a diamond, 27 makes pretty smooth circles.
    float theta = 0;     // angle that will be increased each loop

    int x  = (float)radiusX * cos(theta);//start point
    int y  = (float)radiusY * sin(theta);//start point
    int x1 = x;
    int y1 = y;

    float step = pih/(float)prec; // amount to add to theta each time (degrees)
    for(theta=step;  theta <= pih;  theta+=step)//step through only a 90 arc (1 quadrant)
    {
        x1 = (float)radiusX * cosf(theta) + 0.5; //new point (+.5 is a quick rounding method)
        y1 = (float)radiusY * sinf(theta) + 0.5; //new point (+.5 is a quick rounding method)

        if( (x != x1) || (y != y1) )//only draw if coordinate changed
        {
            SDL_RenderDrawLine(r, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
            SDL_RenderDrawLine(r, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
            SDL_RenderDrawLine(r, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
            SDL_RenderDrawLine(r, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
        }
        x = x1;//save new previous point
        y = y1;//save new previous point
    }
    if(x!=0)
    {
        x=0;
        SDL_RenderDrawLine(r, x0 + x, y0 - y,    x0 + x1, y0 - y1 );//quadrant TR
        SDL_RenderDrawLine(r, x0 - x, y0 - y,    x0 - x1, y0 - y1 );//quadrant TL
        SDL_RenderDrawLine(r, x0 - x, y0 + y,    x0 - x1, y0 + y1 );//quadrant BL
        SDL_RenderDrawLine(r, x0 + x, y0 + y,    x0 + x1, y0 + y1 );//quadrant BR
    }
}

SDL_Rect rect = {0, 0, SIZE, SIZE};

void render_sim(long int iter, unsigned long int sim_step){
    long int ins = 0;
    long int total = 0;
    float per = 0;
    SDL_Window *window = SDL_CreateWindow("PI Approximation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 720, 720, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    serrp(window);
    serrp(renderer);
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        sdl_ellipse(renderer, R, R, R, R);
        SDL_RenderPresent(renderer);
        while(total<iter){
            for(long int j = 0; j < sim_step; ++j){
                double x = rand()%SIZE;
                double y = rand()%SIZE;
                if(pow((R-x),2)+pow((R-y),2)<=pow(R, 2)){
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
                    ++ins;
                }else{
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1);
                }
                ++total;
                SDL_RenderDrawPoint(renderer, x, y);
            }
            printf("Simulation per: %.2f%%\n", 100*total/(double)iter);
            printf("PI: %.10lf\n", 4.0*((double)ins/(double)total));
            SDL_RenderPresent(renderer);
            SDL_Delay(1);
        }
        printf("PI: %.10lf\n", 4.0*((double)ins/(double)total));
}

int main(int argc, char ** argv){
    srand(time(0));

    long int ins = 0;
    long int total = 0;

    int sim = 0;

    long int iter = 10000000L;
    if(sim){
        render_sim(iter, 10000);
    }else{
        long m = 0;
        unsigned long bN = 1;
        double dBmBM = 1. / RAND_MAX / RAND_MAX;
    
        unsigned int TSS_x = bN + 1;
        unsigned int TSS_y = bN - 1;
        for(long i = 0; i < iter; i++) {
            double x = rand_r( &TSS_x );
            double y = rand_r( &TSS_y );
            m += (1  >= ( x * x + y * y ) * dBmBM);
        }
        printf("Pi approximation: %lf\n", (double)(4.0*m/(double)iter));
    }
    return 0;
}