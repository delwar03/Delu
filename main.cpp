#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

int player_score = 0;
int bot_score = 0;

class Ball {
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw () {
        DrawCircle(x, y, radius, Yellow);
    }

    void Update () {
        x += speed_x;
        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        }
        if(x + radius >= GetScreenWidth()) // bot will get some marks
        {
            bot_score++;
            ResetBall();
        }
        if(x - radius <= 0) // player will get some marks
        {
            player_score++;
            ResetBall();
        }
        
    }

    void ResetBall () {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {1, -1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
protected:
    void LimitMovement () {
        if(y <= 0) {
            y = 0;
        }
        if(y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }

public:
    float x, y;
    float width, height;
    int speed;

    void Draw () {
        DrawRectangleRounded(Rectangle {x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update () {
        if(IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        if(IsKeyDown(KEY_UP)) {
            y = y - speed;
        }

        LimitMovement();
    }
};

class CPU_Paddle: public Paddle {
public:
    void Update (int ball_y) {
        if(y + height / 2 > ball_y) {
            y = y - speed;
        } else {
            y = y + speed;
        }

        LimitMovement();
    }
};

Ball ball;
Paddle player;
CPU_Paddle bot;

int main () {


    cout << "Game started\n" << endl;
    
    const int screen_width = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "Delu");
    SetTargetFPS(60);

    // Initializing Ball
    ball.x = screen_width / 2.0;
    ball.y = screen_height / 2.0;
    ball.radius = 20;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // Initializing Player
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height / 2 - player.height / 2;
    player.speed = 6;

    // Initializing Bot
    bot.width = 25;
    bot.height = 120;
    bot.x = 10;
    bot.y = screen_height / 2 - bot.height / 2;
    bot.speed = 6;

    while(WindowShouldClose() == false) {
        BeginDrawing();


            // Updating
            ball.Update();
            player.Update();
            bot.Update(ball.y);


            // Check if collision occurs
            if(CheckCollisionCircleRec(Vector2 {ball.x, ball.y}, ball.radius, Rectangle {player.x, player.y, player.width, player.height})) {
                ball.speed_x *= -1;
            }

            if(CheckCollisionCircleRec(Vector2 {ball.x, ball.y}, ball.radius, Rectangle {bot.x, bot.y, bot.width, bot.height})) {
                ball.speed_x *= -1;
            }



            // Drwaring 
            ClearBackground(Dark_Green);
            DrawRectangle(screen_width / 2, 0, screen_width / 2, screen_height, Green);
            DrawCircle(screen_width / 2, screen_height / 2, 150, Light_Green);
            DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, WHITE);
            ball.Draw();
            bot.Draw();
            player.Draw();
            DrawText(TextFormat("%i", bot_score), screen_width / 4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * screen_width / 4 - 20, 20, 80, WHITE);


        EndDrawing();
    }


    CloseWindow();

    return 0;
}