#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 100;
const int PADDLE_HEIGHT = 20;
const int BALL_SIZE = 20;
const int BLOCK_WIDTH = 100;
const int BLOCK_HEIGHT = 30;
const float BALL_SPEED_INCREASE = 1.1f;

float paddleSpeed = 400.0f;  // Paddle speed ajustada
float ballSpeedX = 50.0f;   // Velocidad inicial de la pelota
float ballSpeedY = -300.0f;

struct Block {
    SDL_Rect rect;
    bool destroyed;
};

// Función para detectar colisión entre dos rectángulos
bool checkCollision(const SDL_Rect &a, const SDL_Rect &b) {
    return (a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y);
}

int main(int argc, char *args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Breakout Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Paddle setup
    SDL_Rect paddle = { SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - 50, PADDLE_WIDTH, PADDLE_HEIGHT };

    // Ball setup
    SDL_Rect ball = { SCREEN_WIDTH / 2 - BALL_SIZE / 2, SCREEN_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE };

    // Blocks setup
    std::vector<Block> blocks;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            Block block = { { j * (BLOCK_WIDTH + 10) + 35, i * (BLOCK_HEIGHT + 10) + 50, BLOCK_WIDTH, BLOCK_HEIGHT }, false };
            blocks.push_back(block);
        }
    }

    bool quit = false;
    SDL_Event e;

    Uint32 lastTime = SDL_GetTicks();
    const int FPS = 60;  // Controlar la velocidad del juego a 60 FPS
    const Uint32 frameDelay = 1000 / FPS;

    while (!quit) {
        Uint32 startTime = SDL_GetTicks();
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        // Event handling
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Paddle movement
        const Uint8 *keyState = SDL_GetKeyboardState(NULL);
        if (keyState[SDL_SCANCODE_LEFT]) {
            paddle.x -= paddleSpeed * deltaTime;  // Movimiento basado en deltaTime
        }
        if (keyState[SDL_SCANCODE_RIGHT]) {
            paddle.x += paddleSpeed * deltaTime;
        }
        if (paddle.x < 0) paddle.x = 0;
        if (paddle.x + PADDLE_WIDTH > SCREEN_WIDTH) paddle.x = SCREEN_WIDTH - PADDLE_WIDTH;

        // Ball movement
        ball.x += ballSpeedX * deltaTime;  // Movimiento basado en deltaTime
        ball.y += ballSpeedY * deltaTime;

        // Ball collision with walls
        if (ball.x <= 0 || ball.x + BALL_SIZE >= SCREEN_WIDTH) {
            ballSpeedX = -ballSpeedX;
        }
        if (ball.y <= 0) {
            ballSpeedY = -ballSpeedY;
        }

        // Ball collision with paddle
        if (checkCollision(ball, paddle)) {
            ballSpeedY = -ballSpeedY;
            ballSpeedX *= BALL_SPEED_INCREASE;
            ballSpeedY *= BALL_SPEED_INCREASE;
        }

        // Ball collision with bottom wall (Game Over)
        if (ball.y + BALL_SIZE >= SCREEN_HEIGHT) {
            std::cout << "Game Over!" << std::endl;
            quit = true;
        }

        // Ball collision with blocks
        for (auto &block : blocks) {
            if (!block.destroyed && checkCollision(ball, block.rect)) {
                block.destroyed = true;
                ballSpeedY = -ballSpeedY;
                break;
            }
        }

        // Check if all blocks are destroyed (You Win)
        bool allDestroyed = true;
        for (const auto &block : blocks) {
            if (!block.destroyed) {
                allDestroyed = false;
                break;
            }
        }
        if (allDestroyed) {
            std::cout << "You Win!" << std::endl;
            quit = true;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paddle);

        SDL_RenderFillRect(renderer, &ball);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for (const auto &block : blocks) {
            if (!block.destroyed) {
                SDL_RenderFillRect(renderer, &block.rect);
            }
        }

        SDL_RenderPresent(renderer);

        // Controlar la velocidad del juego
        Uint32 frameTime = SDL_GetTicks() - startTime;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

