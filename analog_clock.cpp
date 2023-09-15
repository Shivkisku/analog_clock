#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <ctime>

#define S_N_L (radius - 20)
#define S_N_C 0xFF0000  // Red
#define M_N_L (radius - 40)
#define M_N_C 0xFF4500  // Light Red
#define H_N_L (radius - (radius / 2))
#define H_N_C 0x00FFFF  // Cyan

float cx, cy;
float radius = 200; // Increase the radius for a larger clock
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void draw_face(float radius);
void get_time(int& h, int& m, int& s);
void second_needle(int s);
void minute_needle(int m, int s);
void hour_needle(int h, int m, int s);

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Analog Clock", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              2 * (int)radius + 50, 2 * (int)radius + 50, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    cx = (2 * (int)radius + 50) / 2.0;
    cy = (2 * (int)radius + 50) / 2.0;

    float x, y;
    int hour, minute, second;
    draw_face(radius);
    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        get_time(hour, minute, second);
        second_needle(second);
        minute_needle(minute, second);
        hour_needle(hour, minute, second);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

// Functions Definitions

void draw_face(float radius)
{
    int theta = 0;
    float x, y;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, cx, cy - radius, cx, cy + radius);  // Draw clock border

    for (int i = 0; i < 12; i++) {
        x = cx + radius * cos(theta * M_PI / 180);
        y = cy + radius * sin(theta * M_PI / 180);
        SDL_RenderDrawLine(renderer, cx, cy, x, y);

        theta += 30;
    }

    for (int i = 0; i < 360; i += 6) {
        x = cx + (radius - 5) * cos(i * M_PI / 180);
        y = cy + (radius - 5) * sin(i * M_PI / 180);
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

void get_time(int& h, int& m, int& s)
{
    time_t rawtime;
    struct tm* t;
    time(&rawtime);
    t = localtime(&rawtime);
    h = t->tm_hour;
    m = t->tm_min;
    s = t->tm_sec;
}

void second_needle(int s)
{
    float angle = -90;
    float sx, sy;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    sx = cx + S_N_L * cos((angle + s * 6 - 6) * M_PI / 180);
    sy = cy + S_N_L * sin((angle + s * 6 - 6) * M_PI / 180);
    SDL_RenderDrawLine(renderer, cx, cy, sx, sy);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    sx = cx + S_N_L * cos((angle + s * 6) * M_PI / 180);
    sy = cy + S_N_L * sin((angle + s * 6) * M_PI / 180);
    SDL_RenderDrawLine(renderer, cx, cy, sx, sy);
}

void minute_needle(int m, int s)
{
    float angle = -90;
    float sx, sy;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    sx = cx + M_N_L * cos((angle + m * 6 - 6) * M_PI / 180);
    sy = cy + M_N_L * sin((angle + m * 6 - 6) * M_PI / 180);
    SDL_RenderDrawLine(renderer, cx, cy, sx, sy);

    SDL_SetRenderDrawColor(renderer, 255, 69, 0, 255);
    sx = cx + M_N_L * cos((angle + m * 6) * M_PI / 180);
    sy = cy + M_N_L * sin((angle + m * 6) * M_PI / 180);
    SDL_RenderDrawLine(renderer, cx, cy, sx, sy);
}

void hour_needle(int h, int m, int s)
{
    float angle = -90;
    float sx, sy;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    sx = cx + H_N_L * cos((angle + h * 30 - (m * 30 / 60)) * M_PI / 180);
    sy = cy + H_N_L * sin((angle + h * 30 - (m * 30 / 60)) * M_PI / 180);
    SDL_RenderDrawLine(renderer, cx, cy, sx, sy);

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    sx = cx + H_N_L * cos((angle + h * 30 + (m * 30 / 60)) * M_PI / 180);
    sy = cy + H_N_L * sin((angle + h * 30 + (m * 30 / 60)) * M_PI / 180);
    SDL_RenderDrawLine(renderer, cx, cy, sx, sy);
}
