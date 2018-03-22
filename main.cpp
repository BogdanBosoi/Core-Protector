#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <SDL2/SDL_TTF.h>
#include <SDL2/SDL_Mixer.h>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include "script.h"
///#include "determined.h"
#define PI 3.141259265

const int WIDTH = 1024, HEIGHT = 768;

int digit_number(int n)
{
    int s = 0;
    while(n != 0)
        n/=10, s++;
    return s;
}
class tile
{
public:
    SDL_Surface* img;
    SDL_Texture* tex;
    SDL_Rect obj;
    double degree = 0.0;
    char name[10];
    ///enum script_stance{lines, rotated, normal} stance;

    //double degree = 0;
    void set_values(char *name, int posx, int posy, int endx, int endy, SDL_Renderer* renderer)
    {
        obj.x = posx;
        obj.y = posy;
        obj.w = endx;
        obj.h = endy;
        img = IMG_Load(name);
        tex = SDL_CreateTextureFromSurface(renderer, img);
    }


};

class textObj
{
    public:
    SDL_Texture *tex;
    SDL_Color White = {255, 255, 255};
    SDL_Surface *img;
    char text[50];
    SDL_Rect obj;
    void set_values(char name[], int posx, int posy, int wid, int hei, SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font)
    {
        strcpy(text, name);
        obj.x = posx;
        obj.y = posy;
        obj.w = wid;
        obj.h = hei;
        img = TTF_RenderText_Solid(font, text, White);
        tex = SDL_CreateTextureFromSurface(renderer, img);

    }

    void change_text(char name[], SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font)
    {
        img = TTF_RenderText_Solid(font, name, White);
        tex = SDL_CreateTextureFromSurface(renderer, img);
    }

};

void turn_white(SDL_Texture* texP)
{
    SDL_SetTextureColorMod(texP, 255, 255, 255);
}





int main(int argc, char **argv)
{
    int score = 0;
    float diaVelocity = 10, movingVel = 20;
    int energyAbsorbed = 0;
    int stance = 1;
    void (*script)(int&, int&);
    script = &script2;
    unsigned int current_time;
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music *music = NULL;
    music = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(music, -1);
    int diagTiming = 0;
    TTF_Font* font = TTF_OpenFont("font.ttf", 16);

    SDL_Window* window = SDL_CreateWindow("UpperTale",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WIDTH,
                                          HEIGHT,
                                          SDL_WINDOW_ALLOW_HIGHDPI);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 20, 20, 255, 0);
    SDL_RenderClear(renderer);
    int pointSpeed = 2;
    tile core, player, arrow[201], brokenTile, halfTile, circle, diamagnetism;
    bool diagVis = 0;
    diamagnetism.set_values("images\\circle.png", WIDTH/2 - 24, HEIGHT/2 - 24, 48, 48, renderer);
    textObj scoreText, Score;
    scoreText.set_values("Score:", WIDTH - 160, 32, 80, 32, window, renderer, font);
    Score.set_values("0", WIDTH - 80, 32, 8, 32, window, renderer, font);
    core.set_values("images\\tile.png", WIDTH/2 - 16, HEIGHT/2 - 16, 32, 32, renderer);
    player.set_values("images\\shield.png", WIDTH/2 - 24, HEIGHT/2 - 56, 48, 16, renderer);
    circle.set_values("images\\circle.png", WIDTH/2 - 24, HEIGHT/2 - 24, 48, 48, renderer);
    brokenTile.set_values("images\\brokentile.png", WIDTH/2 - 16, HEIGHT/2 - 16, 32, 32, renderer);
    halfTile.set_values("images\\halftile.png", WIDTH/2 - 25, HEIGHT/2 - 16, 51, 32, renderer);
    int maxN = 2;
    srand(time(NULL));
    for(int i =1; i <= 200; i++)
    {
        arrow[i].set_values("images\\arrow.png", 0, 0, 32, 32, renderer);
        script(arrow[i].obj.x, arrow[i].obj.y);
    }


    int angle = 90;
    int radius = 60;
    double xx = radius * cos(angle * PI/180);
    double yy = radius * sin(angle * PI/180);
    double hyp[201];
    double Dx[201], Dy[201], aaangle[201];
    double fluctuantAngle[201];
    for(int i = 1; i <= 200; i++)
    {
        Dx[i] = core.obj.x - arrow[i].obj.x;
        Dy[i] = core.obj.y - arrow[i].obj.y;
        aaangle[i] = atan2(Dy[i], Dx[i]);
    }

    float circleRation = 4;
    SDL_RenderCopyEx(renderer, core.tex, NULL, &core.obj, core.degree, NULL, SDL_FLIP_NONE);
    SDL_RenderCopyEx(renderer, player.tex, NULL, &player.obj, player.degree, NULL, SDL_FLIP_NONE);
    for(int i = 1; i <= maxN; i++)
        SDL_RenderCopyEx(renderer, arrow[i].tex, NULL, &arrow[i].obj, aaangle[i] * 180/PI + 90, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(renderer);
    short timer = 0;
    bool quit = 0, checker = 0;
    char tScore[30];
    short timeRation = 20;
    int numb = digit_number(score);
    SDL_ShowWindow(window);
    while(!quit)
    {
        SDL_ShowWindow(window);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            }

        }
        current_time = SDL_GetTicks();
        if(current_time % 1000 == 0)
            if(script == &script1)
                script = &script2;
            else if(script == &script2)
                script = &script3;
            else if(script == &script3)
                script = &script1;
        //if(score % 200 == 0)
        ///pointSpeed = 2 + (rand() % score/100);

        core.degree += 5;
        if(core.degree > 360)
            core.degree = 0;
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if(currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D] || currentKeyStates[SDL_SCANCODE_A]
           || currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_W])
        {
            if(currentKeyStates[SDL_SCANCODE_LEFT])
                player.degree -= 8, angle -= 8;
            if(currentKeyStates[SDL_SCANCODE_RIGHT])
                player.degree += 8, angle += 8;
            if(currentKeyStates[SDL_SCANCODE_D])
                player.degree = 90, angle = 180;
            if(currentKeyStates[SDL_SCANCODE_A])
                player.degree = 270, angle = 0;
            if(currentKeyStates[SDL_SCANCODE_S])
                player.degree = 0, angle = 270;
            if(currentKeyStates[SDL_SCANCODE_W])
                player.degree = 180, angle = 90;
            xx = cos(angle * PI/180);
            yy = sin(angle * PI/180);
            if(angle < 0)
                angle = 360;
            if(angle > 360)
                angle = 0;
            player.obj.x = core.obj.x - radius * cos(angle * PI / 180) - 8;
            player.obj.y = core.obj.y - radius * sin(angle * PI / 180) + 8;
        }

        if(player.degree < 0)
            player.degree = 360;
        if(player.degree > 360)
            player.degree = 0;
        for(int i = 1; i <= maxN; i++)
        {



            /*hyp[i] = sqrt(Dx[i] * Dx[i] + Dy[i] * Dy[i]);
            Dx[i]/=hyp[i];
            Dy[i]/=hyp[i];
            arrow[i].obj.x += Dx[i];
            arrow[i].obj.y += Dy[i];*/
            arrow[i].obj.x = arrow[i].obj.x + (pointSpeed * cos(aaangle[i]));
            arrow[i].obj.y = arrow[i].obj.y + (pointSpeed * sin(aaangle[i]));
            Dx[i] = core.obj.x - arrow[i].obj.x;
            Dy[i] = core.obj.y - arrow[i].obj.y;
            aaangle[i] = atan2(Dy[i], Dx[i]);
            ///fluctuantAngle[i] = aaangle[i] * 180/PI;

            /*else
            {
                arrow[i].obj.x = core.obj.x - Dx[i] * cos(fluctuantAngle[i] * PI/180);
                arrow[i].obj.y = core.obj.y - Dy[i] * sin(fluctuantAngle[i] * PI/180);
                toValue(Dx[i], core.obj.x);
                toValue(Dy[i], core.obj.y);
                fluctuantAngle[i]+=0.005;
                if(fluctuantAngle[i] > 360)
                    fluctuantAngle[i] = 0;
                if(fluctuantAngle[i] < 0)
                    fluctuantAngle[i] =  360;
            }*/
        }

        if(currentKeyStates[SDL_SCANCODE_SPACE] && diagVis == 0)
        {
            diagVis = 1;
            if(stance == 0)
            {
                diamagnetism.obj.x = WIDTH/2 - 24;
                diamagnetism.obj.y = HEIGHT/2 - 24;
                diamagnetism.obj.w = 48;
                diamagnetism.obj.h = 48;
               /// for(int i = 1; i <= maxN; i++)
                  ///  send(arrow[i].obj, 50, aaangle[i]);
            }
            else if(stance == 1)
            {
                diamagnetism.obj.x = 128;
                diamagnetism.obj.y = 0;
                diamagnetism.obj.w = HEIGHT;
                diamagnetism.obj.h = HEIGHT;
                //for(int i = 1; i <= maxN; i++)
                  //  send(arrow[i].obj, -50, aaangle[i]);
            }
        }



        if(currentKeyStates[SDL_SCANCODE_Z])
        {
            stance++;
            if(stance > 2)
                stance = 0;
        }
        for(int i = 1; i <= maxN; i++)
        {
            if (check_collision(core.obj, arrow[i].obj, 1, 1))
            {
                    Mix_PauseMusic();
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, brokenTile.tex, NULL, &brokenTile.obj);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(500);
                    for(int i = 1; i <= 50; i++)
                    {
                        int temp = rand() % 4;
                        int intitx = brokenTile.obj.x;
                        int intity = brokenTile.obj.y;
                        if(rand() % 2 == 1)
                            brokenTile.obj.x += temp;
                        else
                            brokenTile.obj.x -= temp;
                        temp = rand() % 4;
                        if(rand() % 2 == 1)
                            brokenTile.obj.y += temp;
                        else
                            brokenTile.obj.y -= temp;
                        SDL_RenderCopy(renderer, brokenTile.tex, NULL, &brokenTile.obj);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(50);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                        SDL_RenderClear(renderer);
                        brokenTile.obj.x = intitx;
                        brokenTile.obj.y = intity;

                    }
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, halfTile.tex, NULL, &halfTile.obj);
                    SDL_RenderPresent(renderer);
                    char det[] = "*But it refused...";
                    char outt[50];
                    SDL_Delay(200);
                    textObj determination;
                    determination.set_values("", WIDTH/2 - 100, HEIGHT/2 - 50, 16, 16, window, renderer, font);
                    SDL_Delay(1000);
                    for(int j = 0; j < strlen(det); j++)
                    {
                        outt[j] = det[j];
                        outt[j + 1] = '\0';
                        determination.change_text(outt, window, renderer, font);
                        determination.obj.w = (j + 1) * 16;
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, halfTile.tex, NULL, &halfTile.obj);
                        SDL_RenderCopy(renderer, determination.tex, NULL, &determination.obj);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(100);
                    }
                    SDL_Delay(500);
                    for(int i = 1; i <= 25; i++)
                    {
                        int temp = rand() % 4;
                        int intitx = halfTile.obj.x;
                        int intity = halfTile.obj.y;
                        if(rand() % 2 == 1)
                            halfTile.obj.x += temp;
                        else
                            halfTile.obj.x -= temp;
                        temp = rand() % 4;
                        if(rand() % 2 == 1)
                            halfTile.obj.y += temp;
                        else
                            halfTile.obj.y -= temp;
                        SDL_RenderCopy(renderer, halfTile.tex, NULL, &halfTile.obj);
                        SDL_RenderCopy(renderer, determination.tex, NULL, &determination.obj);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(50);
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                        SDL_RenderClear(renderer);
                        halfTile.obj.x = intitx;
                        halfTile.obj.y = intity;
                    }
                    SDL_Delay(200);
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                    SDL_RenderClear(renderer);
                    SDL_RenderCopy(renderer, core.tex, NULL, &core.obj);
                    SDL_RenderCopy(renderer, determination.tex, NULL, &determination.obj);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(500);
                    for(int j = 1; j <= maxN; j++)
                        script(arrow[j].obj.x, arrow[j].obj.y);
                    Mix_ResumeMusic();
            }
            if(check_collision(player.obj, arrow[i].obj, 180/PI * player.degree, aaangle[i]))
                {
                    script(arrow[i].obj.x, arrow[i].obj.y);
                    SDL_SetTextureColorMod(player.tex, 255, 0, 0);
                    timer = 100;
                    score += 10;
                    numb = digit_number(score);
                    if(maxN <= 200)
                        maxN = 2 + score/100;
                    if(rand() % 3 == 1)
                        energyAbsorbed += 5;
                }
        }

        itoa(score, tScore, 10);

        SDL_SetRenderDrawColor(renderer, score, score - 50, score - 100, 0);
        SDL_RenderClear(renderer);
        SDL_SetTextureColorMod(core.tex, rand() % 255, rand() % 255, rand() % 255);
        SDL_RenderCopyEx(renderer, core.tex, NULL, &core.obj, core.degree, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, player.tex, NULL, &player.obj, player.degree, NULL, SDL_FLIP_NONE);
        SDL_RenderCopy(renderer, circle.tex, NULL, &circle.obj);

        if(diagVis == 1)
        {
            if(stance == 0)
            {
                for(int i = 1; i <= maxN; i++)
                    send(arrow[i].obj, 2, aaangle[i]);
                diamagnetism.obj.x -= diaVelocity, diamagnetism.obj.y -= diaVelocity, diamagnetism.obj.w += diaVelocity * 2, diamagnetism.obj.h += diaVelocity*2;
                diaVelocity += 0.5;
                if(diamagnetism.obj.x < 0 || diamagnetism.obj.y < 0)
                    diagVis = 0, diaVelocity = 10;

            }
            if(stance == 1)
            {
                for(int i = 1; i <= maxN; i++)
                    send(arrow[i].obj, -2, aaangle[i]);
                    diaVelocity += 0.5;
                diamagnetism.obj.x += diaVelocity, diamagnetism.obj.y += diaVelocity, diamagnetism.obj.w -= diaVelocity * 2, diamagnetism.obj.h -= diaVelocity * 2;
                if(diamagnetism.obj.x > WIDTH/2 || diamagnetism.obj.y > HEIGHT/2)
                    diagVis = 0, diaVelocity = 10;
            }
            SDL_RenderCopy(renderer, diamagnetism.tex, NULL, &diamagnetism.obj);
        }
        SDL_RenderCopy(renderer, scoreText.tex, NULL, &scoreText.obj);
        Score.change_text(tScore, window, renderer, font);
        Score.obj.w = 16 * numb;
        SDL_RenderCopy(renderer, Score.tex, NULL, &Score.obj);
        for(int i = 1; i <= maxN ; i++)
        {
            SDL_RenderCopyEx(renderer, arrow[i].tex, NULL, &arrow[i].obj, aaangle[i] * 180/PI + 90, NULL, SDL_FLIP_NONE);
            SDL_SetTextureColorMod(arrow[i].tex, score, score + 50, score + 100);
        }
        SDL_RenderPresent(renderer);
        if(timer > 0)
            timer -= 5;
        else
            turn_white(player.tex);
        timeRation -= 2;
        if(timeRation == 0)
        {
            ///if(circle.obj.x >= WIDTH/2 - 72 && circle.obj.y >= HEIGHT/2 - 72)
                circle.obj.x -=5*circleRation, circle.obj.y -= 5 * circleRation, circle.obj.w += 10 * circleRation, circle.obj.h += 10 * circleRation;
                /*if(circleRation < 0)
                    circleRation -= 0.5;
                else
                    circleRation += 0.5;*/
            if(circle.obj.x >= WIDTH/2 - 144 && circle.obj.y >= HEIGHT/2 - 144)
                SDL_SetTextureColorMod(circle.tex, rand() % 255, rand() % 255, rand() % 255), circleRation *= -1;///circle.obj.x = WIDTH/2 - 24, circle.obj.y = HEIGHT/2 - 24, circle.obj.w = 48, circle.obj.h = 48;
            if(circle.obj.x <= WIDTH/2 - 24 && circle.obj.y <= HEIGHT/2 - 24)
                SDL_SetTextureColorMod(circle.tex, rand() % 255, rand() % 255, rand() % 255), circleRation *= -1;
             /*if(checker == 1)
                circle.obj.x += 1, circle.obj.y += 1, circle.obj.w -= 2, circle.obj.h -= 2;
            /*if(circle.obj.x == WIDTH/2 - 24 && circle.obj.y == HEIGHT/2 - 24)
                checker = 0;*/
        }
        if(timeRation < 0)
            timeRation = 20;
        SDL_Delay(10);
    }
    return 0;
}
