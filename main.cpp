// jumopking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "common_function.h"
#include "BaseObject.h"

BaseObject g_background;

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("game jump king dream", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SCREEN_WIDTH, 
                                SCREEN_HEIGHT, 
                                SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }

    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img/background.png", g_screen);
    if (ret == false)
        return false;

    return true;
}

void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    IMG_Quit();
}


int selectMenu(SDL_Renderer* renderer) {
    SDL_Event event;
    TTF_Font* font = TTF_OpenFont("fonts/EnterCommand.ttf", 16); // khai báo biến font ở đây
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Surface* surface;
    SDL_Texture* texture;
    int selected = 0;
    int menuCount = 2;
    const char* menuItems[] = { "Start", "Quit" };

    while (1) {
        // Vẽ menu
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        for (int i = 0; i < menuCount; i++) {
            if (i == selected) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            surface = TTF_RenderText_Solid(font, menuItems[i], color);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = { 0, 0, surface->w, surface->h };
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
            SDL_Rect r = { SCREEN_WIDTH / 2 - surface->w / 2, SCREEN_HEIGHT / 2 - surface->h / 2 + i * 40, surface->w * 2, surface->h * 2};
            SDL_RenderCopy(renderer, texture, NULL, &r);
        }
        SDL_RenderPresent(renderer);

        // Xử lý sự kiện
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                return MENU_QUIT;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_UP:
                    selected = (selected - 1 + menuCount) % menuCount;
                    break;
                case SDLK_DOWN:
                    selected = (selected + 1) % menuCount;
                    break;
                case SDLK_RETURN:
                    return selected;
                }
                break;
            }
        }
    }
    TTF_CloseFont(font);
}

int main(int argc, char* argv[])
{
    // Khởi tạo SDL2 và cửa sổ
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Không thể tạo cửa sổ: %s\n", SDL_GetError());
        return 1;
    }

    // Khởi tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) { 
        printf("Không thể tạo renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Khởi tạo TTF
    if (TTF_Init() == -1) {
        printf("Không thể khởi tạo TTF: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Chọn mục từ menu
    int selected = selectMenu(renderer);
    if (selected == MENU_START) {
        printf("game start\n");
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        if (InitData() == false)
            return -1;

        if (LoadBackground() == false)
            return -1;

        bool is_quit = false;
        while (!is_quit)
        {
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                {
                    is_quit = true;
                }
            }
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);

            g_background.Render(g_screen, NULL);

            SDL_RenderPresent(g_screen);
        }
    }
    else {
        printf("quit game\n");
    }

    // Giải phóng tài nguyên và thoát SDL2
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    close;
    return 0;
}