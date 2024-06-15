#include <iostream>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <cmath>
#include <iomanip>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

const int SIZE = 256+1;

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0)
    {
        cout << SDL_GetError();
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("MAP", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SIZE * 4, SIZE * 4, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            cout << "Ablak készítés sikertelen " << SDL_GetError();
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                cout << "Megjelenítõ készítés sikertelen" << SDL_GetError();
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    cout << "IMG betöltés sikertelen " << SDL_GetError();
                    success = false;
                }
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }
           
        }
    }
    return success;
}

SDL_Texture* loadTexture(std::string path)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        cout << "1 IMG betöltése sikertelen " << SDL_GetError();
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            cout << "1 IMG betöltése sikertelen" << SDL_GetError();
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void close()
{
    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);
    gWindow = NULL;
    gRenderer = NULL;
    SDL_Quit();
    IMG_Quit();
}

int main(int argc, char* argv[])
{
    float** terr = new float* [SIZE];
    for (int i = 0; i < SIZE; i++)
        terr[i] = new float[SIZE];
    srand(time(NULL));
    float randomness = 0.75; float negRandomness = -0.75;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            terr[i][j] = 0.0;
        }

    }
    terr[0][0] = round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
    terr[SIZE - 1][SIZE - 1] = round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
    terr[0][SIZE - 1] = round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
    terr[SIZE - 1][0] = round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
    int currSize = SIZE - 1;
    int s = currSize, nr = 0;
    while (s != 0)
    {
        s /= 2;
        nr++;
    }
    int moves = 0;
    int currnr = 0;
    //map gen
    while (currSize >= 1)
    {
        if (currSize != SIZE - 1)
            for (int i = 0; i < SIZE; i += currSize) ///square
            {
                for (int j = 0; j < SIZE; j += currSize)
                {
                    if (terr[i][j] == 0.0) ///clear tile
                    {
                        moves++;
                        if (i < currSize && j >= currSize && j + currSize <= SIZE - 1 && i + currSize <= SIZE - 1)
                        {
                            float average = round(((terr[i + currSize][j] + terr[i][j + currSize] + terr[i][j - currSize]) / 3) * 100) / 100;
                            terr[i][j] = average + round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
                            if (terr[i][j]<0.01 && terr[i][j] > -0.01)
                                terr[i][j] = 0.0;

                        }
                        else if (i + currSize > SIZE - 1 && j >= currSize && j + currSize <= SIZE - 1 && i >= currSize)
                        {
                            float average = round(((terr[i - currSize][j] + terr[i][j + currSize] + terr[i][j - currSize]) / 3) * 100) / 100;
                            terr[i][j] = average + round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
                            if (terr[i][j]<0.01 && terr[i][j] > -0.01)
                                terr[i][j] = 0.0;

                        }
                        else if (j < currSize && i >= currSize && i + currSize <= SIZE - 1 && j + currSize <= SIZE - 1)
                        {
                            float average = round(((terr[i - currSize][j] + terr[i + currSize][j] + terr[i][j + currSize]) / 3) * 100) / 100;
                            terr[i][j] = average + round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
                            if (terr[i][j]<0.01 && terr[i][j] > -0.01)
                                terr[i][j] = 0.0;

                        }
                        else if (j + currSize > SIZE - 1 && i >= currSize && i + currSize <= SIZE - 1 && j >= currSize)
                        {
                            float average = round(((terr[i - currSize][j] + terr[i + currSize][j] + terr[i][j - currSize]) / 3) * 100) / 100;
                            terr[i][j] = average + round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
                            if (terr[i][j]<0.01 && terr[i][j] > -0.01)
                                terr[i][j] = 0.0;

                        }

                        else if (j + currSize <= SIZE - 1 && i >= currSize && i + currSize <= SIZE - 1 && j >= currSize)
                        {
                            float average = round(((terr[i - currSize][j] + terr[i + currSize][j] + terr[i][j + currSize] + terr[i][j - currSize]) / 4) * 100) / 100;
                            terr[i][j] = average + round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
                            if (terr[i][j]<0.01 && terr[i][j] > -0.01)
                                terr[i][j] = 0.0;

                        }

                    }

                }
            }

        if (currSize != 1)
            for (int i = 0; i < SIZE - currSize; i += currSize) ///diamond
            {
                moves++;
                for (int j = 0; j < SIZE - currSize; j += currSize)
                {
                    float average = round(((terr[i][j] + terr[i + currSize][j] + terr[i + currSize][j + currSize] + terr[i][j + currSize]) / 4) * 100) / 100;
                    terr[int(i + currSize / 2)][int(j + currSize / 2)] = average + round((((float(rand()) / float(RAND_MAX)) * (randomness - negRandomness)) + negRandomness) * 100) / 100;
                    if (terr[i][j]<0.01 && terr[i][j] > -0.01)
                        terr[i][j] = 0.0;
                }

            }
        currnr++;
        currSize /= 2;
        randomness = randomness / 1.15;
        negRandomness = negRandomness / 1.15;
        cout << "Loading: " << currnr * 100 / nr << "%" << endl;
        system("CLS");
    }
    cout << "Loaded!" << endl << endl;
    if (!init())
    {
        cout << "Felépítés sikertelen!" << endl;
    }
    else
    {
        bool quit = false;
        SDL_Event e;
        SDL_Texture* water = loadTexture("water.png");
        SDL_Texture* sand = loadTexture("sand.png");
        SDL_Texture* grass = loadTexture("grass.png");
        SDL_Texture* mountain = loadTexture("mountain.png");
        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
            }
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    if (terr[i][j] < -0.2)
                    {
                        SDL_Rect block;
                        block.x = i * 4;
                        block.y = j * 4;
                        block.w = 4;
                        block.h = 4;
                        SDL_RenderSetViewport(gRenderer, &block);
                        SDL_RenderCopy(gRenderer, water, NULL, NULL);
                    }
                    else if (terr[i][j] < 0)
                    {
                        SDL_Rect block;
                        block.x = i *4;
                        block.y = j * 4;
                        block.w = 4;
                        block.h = 4;
                        SDL_RenderSetViewport(gRenderer, &block);
                        SDL_RenderCopy(gRenderer, sand, NULL, NULL);
                    }
                    else if (terr[i][j] < 0.5)
                    {
                        SDL_Rect block;
                        block.x = i * 4;
                        block.y = j * 4;
                        block.w = 4;
                        block.h = 4;
                        SDL_RenderSetViewport(gRenderer, &block);
                        SDL_RenderCopy(gRenderer, grass, NULL, NULL);
                    }
                    else
                    {
                        SDL_Rect block;
                        block.x = i * 4;
                        block.y = j * 4;
                        block.w = 4;
                        block.h = 4;
                        SDL_RenderSetViewport(gRenderer, &block);
                        SDL_RenderCopy(gRenderer, mountain, NULL, NULL);
                    }
                    

                }
            }
            

            SDL_RenderPresent(gRenderer);
        }
        
        SDL_DestroyTexture(water);
        water = NULL;
        SDL_DestroyTexture(sand);
        sand = NULL;
        SDL_DestroyTexture(grass);
        grass = NULL;
        SDL_DestroyTexture(mountain);
        mountain = NULL;
    }
    for (int i = 0; i < SIZE; i++)
        delete[] terr[i];
    delete[] terr;

    close();

    return 0;
}
