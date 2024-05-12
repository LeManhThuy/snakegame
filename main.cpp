    #include <iostream>
    #include <SDL.h>
    #include <time.h>
    #include <sstream>
    using namespace std;

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int BLOCK_SIZE = 20;
    const int GRID_WIDTH = SCREEN_WIDTH / BLOCK_SIZE;
    const int GRID_HEIGHT = SCREEN_HEIGHT / BLOCK_SIZE;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* snakeTexture = NULL;
    SDL_Texture* fruitTexture = NULL;
    SDL_Texture* backgroundTexture = NULL;
    SDL_Rect snakeHeadRect;
    SDL_Rect fruitRect;
    SDL_Rect backgroundRect;

    bool gameOver;
    int x, y, fruitX, fruitY, score;
    int tailX[100], tailY[100];
    int nTail;
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;




    void Setup()
    {
        gameOver = false;
        dir = STOP;
        x = GRID_WIDTH / 2;
        y = GRID_HEIGHT / 2;
        fruitX = rand() % GRID_WIDTH;
        fruitY = rand() % GRID_HEIGHT;
        score = 0;
        nTail = 0;
    }




    void LoadTextures()
    {
        SDL_Surface* snakeSurface = SDL_LoadBMP("ran.bmp");
        snakeTexture = SDL_CreateTextureFromSurface(renderer, snakeSurface);
        SDL_FreeSurface(snakeSurface);

        SDL_Surface* fruitSurface = SDL_LoadBMP("trung.bmp");
        fruitTexture = SDL_CreateTextureFromSurface(renderer, fruitSurface);
        SDL_FreeSurface(fruitSurface);

        SDL_Surface* backgroundSurface = SDL_LoadBMP("map2.bmp");
        backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
        SDL_FreeSurface(backgroundSurface);
    }

    void Draw()
    {
        // Draw background
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        // Draw snake head
        snakeHeadRect.x = x * BLOCK_SIZE;
        snakeHeadRect.y = y * BLOCK_SIZE;
        snakeHeadRect.w = BLOCK_SIZE;
        snakeHeadRect.h = BLOCK_SIZE;
        SDL_RenderCopy(renderer, snakeTexture, NULL, &snakeHeadRect);

        // Draw snake tail
        for (int i = 0; i < nTail; i++)
        {
            SDL_Rect tailRect = { tailX[i] * BLOCK_SIZE, tailY[i] * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE };
            SDL_RenderCopy(renderer, snakeTexture, NULL, &tailRect);
        }

        // Draw fruit
        fruitRect.x = fruitX * BLOCK_SIZE;
        fruitRect.y = fruitY * BLOCK_SIZE;
        fruitRect.w = BLOCK_SIZE;
        fruitRect.h = BLOCK_SIZE;
        SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);

        SDL_RenderPresent(renderer);

    }

    void Input()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                gameOver = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    if (dir != RIGHT)
                        dir = LEFT;
                    break;
                case SDLK_RIGHT:
                    if (dir != LEFT)
                        dir = RIGHT;
                    break;
                case SDLK_UP:
                    if (dir != DOWN)
                        dir = UP;
                    break;
                case SDLK_DOWN:
                    if (dir != UP)
                        dir = DOWN;
                    break;
                }
            }
        }
    }

    void Logic()
    {
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < nTail; i++)
        {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        switch (dir)
        {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        }

        // Wrap around screen
        if (x >= GRID_WIDTH) x = 0;
        else if (x < 0) x = GRID_WIDTH - 1;
        if (y >= GRID_HEIGHT) y = 0;
        else if (y < 0) y = GRID_HEIGHT - 1;

        // Check collision with tail
        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameOver = true;

        // Check collision with fruit
        if (x == fruitX && y == fruitY)
        {
            score += 10;
            fruitX = rand() % GRID_WIDTH;
            fruitY = rand() % GRID_HEIGHT;
            nTail++;
        }
    }

    void Cleanup()
    {
        SDL_DestroyTexture(snakeTexture);
        SDL_DestroyTexture(fruitTexture);
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    int main(int argc, char* args[])
    {
        srand(time(NULL));
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        LoadTextures();
        Setup();

        // Set background rectangle to cover entire window
        backgroundRect.x = 0;
        backgroundRect.y = 0;
        backgroundRect.w = SCREEN_WIDTH;
        backgroundRect.h = SCREEN_HEIGHT;

        while (!gameOver)
        {
            Input();
            Logic();
            Draw();
            SDL_Delay(100);
        }

        Cleanup();
        return 0;
    }

