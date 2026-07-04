#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define BLOCK_VELOCITY  3.0f
#define BLOCK_WIDTH     15
#define BLOCK_HEIGHT    80



#define BALL_VELOCITY   3.0f       
#define BALL_RADIUS     10


//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct Block{
    Vector2 position;
    Vector2 size;
    Rectangle hitbox;
    float velocity;
    Color color;
} Block;

typedef struct Ball{
    Vector2 position;
    Vector2 velocity;
    int radius;
    int bounceCount;
    Color color;
} Ball;

typedef struct Button{
    Vector2 position;
    Rectangle bounds;
    Color color;

} Button;

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static const int screenWidth = 800;
static const int screenHeight = 450;

Block player1 = {0};
Block player2 = {0};

Ball ball = {0};

Button btn1p = {0};
Button btn2p = {0};

GameScreen currentScreen = LOGO;

static int scoreP1 = 0;
static int scoreP2 = 0;


static bool pause = false;
static bool gameEnds = false;
static bool onePlayer = false;

static int framesCounter = 0;
static int scoreTimer = 0;
static float scoreLinePosX = 0;

static int logoTextWidth = 0;
static int titleTextWidth = 0;
static int btn1pTextWidth = 0;
static int btn2pTextWidth = 0;
static int winp1TextWidth = 0;
static int winp2TextWidth = 0;
static int restartTextWidth = 0;
static int backToMenuTextWidth = 0;


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);             // Initialize game
static void initBlock(Block* block, float posX);   //initialize player
static void initBall(Ball* ball);     //initialize ball
static void initButton(Button* button, float posY);

static void DrawLogo(void);

static void UpdateDrawTitle(void);
static void outlineButton(Button* btn);

static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void updatePlayerVelocity(int buttonUp, int buttonDown, Block* player);
static void moveBot();
static void reflectBallFromPlayer(Block* player);
static void reflectBallFromEdge();
static void speedUpBall();
static void resetBallPos();

static void UpdateEnding(void);     
static void DrawEnding(void);



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    InitWindow(screenWidth, screenHeight, "Ray-Pong");
    SetTargetFPS(60);              

    logoTextWidth = MeasureText("WAYSELL", 50);
    titleTextWidth = MeasureText("RAY-PONG", 40);
    btn1pTextWidth = MeasureText("1 Player", 35);
    btn2pTextWidth = MeasureText("2 Player", 35);
    winp1TextWidth = MeasureText("PLAYER 1 WON", 40);
    winp2TextWidth = MeasureText("PLAYER 2 WON", 40);
    restartTextWidth = MeasureText("Press [R] to play again", 30);
    backToMenuTextWidth = MeasureText("Press [M] to go back to main menu", 30);

    framesCounter = 0;
    InitGame();

    while (!WindowShouldClose()){
        switch (currentScreen)
        {
            case LOGO:
            {
                DrawLogo();
                framesCounter++;    // Count frames
 
                // Wait for 2 seconds (120 frames) before jumping to TITLE screen
                if (framesCounter > 120)
                {

                    currentScreen = TITLE;
                    framesCounter = 0;
                }
            } break;
            case TITLE:
            {                
                UpdateDrawTitle();
            } break;
            case GAMEPLAY:
            {
                

                UpdateGame();
                DrawGame();
            } break;
            case ENDING:
            {
                UpdateEnding();
                DrawEnding();

            } break;
            default: break;
        }


    }
    
    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(){

    

    scoreP1 = 9;
    scoreP2 = 9;
    pause = false;
    gameEnds = false;

    initBlock(&player1, 30);
    initBlock(&player2, screenWidth-40);
    initBall(&ball);

    initButton(&btn1p, screenHeight/2);
    initButton(&btn2p, screenHeight/2 + 64);

  

}


void DrawLogo(){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);

    DrawText("WAYSELL", screenWidth/2 - logoTextWidth/2, screenHeight/2 -25, 50, BLACK);


    EndDrawing();

}


void UpdateDrawTitle(){
    BeginDrawing();

    ClearBackground(BLACK);
    //Draw title

    DrawText("RAY-PONG", screenWidth/2 -titleTextWidth/2, screenHeight/2 -80, 40, WHITE);
            
    //Draw 1 player button
    DrawRectangleV(btn1p.position, (Vector2){btn1p.bounds.width, btn1p.bounds.height}, WHITE);
    DrawText("1 Player", screenWidth/2 - btn1pTextWidth/2, btn1p.position.y+6, 35, BLACK);
    outlineButton(&btn1p);
    

    //Draw 2 player button
    DrawRectangleV(btn2p.position, (Vector2){btn2p.bounds.width, btn2p.bounds.height}, WHITE);
    DrawText("2 Player", screenWidth/2 - btn2pTextWidth/2, btn2p.position.y+6, 35, BLACK);
    outlineButton(&btn2p);
    

    EndDrawing();
}
void outlineButton(Button* btn){
    if (CheckCollisionPointRec(GetMousePosition(), btn->bounds))
    {
        DrawRectangleRec(btn->bounds, Fade(RED, 0.2f));
        DrawRectangleLinesEx(btn->bounds, 3.0f, RED);

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onePlayer = btn == &btn1p;      //is user clicked for 1 player option
            currentScreen = GAMEPLAY;
        }
    }else{
        DrawRectangleRec(btn->bounds, Fade(DARKGRAY, 0.2f));
        DrawRectangleLinesEx(btn->bounds, 3.0f, DARKGRAY);
    }


}



// Update game (one frame)
void UpdateGame(){
    updatePlayerVelocity(KEY_W, KEY_S, &player1);

    if(onePlayer) moveBot();
    else updatePlayerVelocity(KEY_UP, KEY_DOWN, &player2);

    if( scoreTimer <= 0){
        ball.position.x += ball.velocity.x;
        ball.position.y += ball.velocity.y;
    }

    
    bool isBallCollidingPlayer2 = CheckCollisionCircleRec(ball.position, ball.radius, player2.hitbox);
    bool isBallCollidingPlayer1 = CheckCollisionCircleRec(ball.position, ball.radius, player1.hitbox);
    bool isBallCollidingEdge = ball.position.y >= screenHeight - ball.radius || ball.position.y <= ball.radius;


    if(isBallCollidingPlayer1)reflectBallFromPlayer(&player1); 
    else if(isBallCollidingPlayer2)reflectBallFromPlayer(&player2);

    if(isBallCollidingEdge) reflectBallFromEdge();
    
    if(ball.bounceCount >= 5){
        speedUpBall();
        ball.bounceCount = 0;
    }
    
    if(ball.position.x - ball.radius <= 0) {
        scoreP2 ++; 
        scoreLinePosX = 0;
        resetBallPos();
    }
    else if(ball.position.x + ball.radius >= screenWidth) {
        scoreP1 ++; 
        scoreLinePosX = screenWidth;
        resetBallPos();
    }

    if(scoreP1 == 10 || scoreP2 == 10){
        currentScreen = ENDING;
    }

}
void updatePlayerVelocity(int buttonUp, int buttonDown, Block* player){
    if(IsKeyDown(buttonUp) && player->position.y >= 0){
        player->position.y -= player->velocity;
    } 
    else if(IsKeyDown(buttonDown) && player->position.y <= screenHeight - player->size.y){
        player->position.y += player->velocity;
    } 
    player->hitbox.y = player->position.y;

}
void moveBot(){
    if((ball.position.y > player2.position.y +  player2.hitbox.height/2) && player2.position.y <= screenHeight - player2.size.y){
        player2.position.y += player2.velocity;
    }
    else if(ball.position.y < player2.position.y +  player2.hitbox.height/2 && (player2.position.y >= 0)){
        player2.position.y -= player2.velocity;
    }

    player2.hitbox.y = player2.position.y;

}
void reflectBallFromPlayer(Block* player){
    Vector2 clamped = Vector2Clamp(ball.position,
                        (Vector2){player->position.x, player->position.y}, 
                        (Vector2){player->position.x+ player->hitbox.width, player->position.y+ player->hitbox.height});
    
    Vector2 normalDir = Vector2Subtract(ball.position, clamped);

    if (Vector2LengthSqr(normalDir) > 0.001f) {
        Vector2 normal = Vector2Normalize(normalDir);
        ball.velocity = Vector2Reflect(ball.velocity, normal);
    } else {
        ball.velocity.x *= -1.0f;
    }
    ball.bounceCount++;


}
void reflectBallFromEdge(){
    ball.velocity.y *= -1;
    ball.bounceCount ++; 
}
void speedUpBall(){
    
    ball.velocity.x += 0.5f * ball.velocity.x / fabsf(ball.velocity.x);
    ball.velocity.y += 0.5f * ball.velocity.y / fabsf(ball.velocity.y);
    //printf("Ball vx: %f, Ball vy: %f\n", ball.velocity.x, ball.velocity.y);

    
}
void resetBallPos(){
    scoreTimer = 60;
    ball.position = (Vector2){screenWidth/2, screenHeight/2};
    ball.velocity = (Vector2){BALL_VELOCITY, BALL_VELOCITY};

}

// Draw game (one frame)
void DrawGame(){
    BeginDrawing();
        ClearBackground(BLACK);
        if(pause){
            
        }
        else{
            //Draw players
            DrawRectangleV(player1.position, player1.size, player1.color);
            DrawRectangleV(player2.position, player2.size, player2.color);

            //Draw ball
            DrawCircleV(ball.position, ball.radius, ball.color);

            //Draw score
            int scoreTextWidth = MeasureText(TextFormat("%i - %i", scoreP1, scoreP2), 40);
            DrawText(TextFormat("%i - %i", scoreP1, scoreP2), screenWidth/2 - scoreTextWidth/2, 10, 40, WHITE);
        }

        if(scoreTimer > 0) {
            scoreTimer--;
            DrawLineEx((Vector2){scoreLinePosX, 0}, (Vector2){scoreLinePosX, screenHeight}, 10.0, RED);
        }
    EndDrawing();
}

void UpdateEnding(){
    if(IsKeyDown(KEY_R)){
        currentScreen = GAMEPLAY;
        InitGame();
    }
    if(IsKeyDown(KEY_M)){
        currentScreen = TITLE;
        InitGame();
    }
}

void DrawEnding(){
    BeginDrawing();

    ClearBackground(BLACK);
    if(scoreP1 == 10){
        DrawText("PLAYER 1 WINS", screenWidth/2 - winp1TextWidth/2, screenHeight/2 - 80, 40, WHITE);
    }
    else{
        DrawText("PLAYER 2 WINS", screenWidth/2 - winp2TextWidth/2, screenHeight/2 - 80, 40, WHITE);
    }

    DrawText("Press [R] to play again", screenWidth/2 - restartTextWidth/2, screenHeight/2 + 40, 30, LIGHTGRAY);
    DrawText("Press [M] to go back to main menu", screenWidth/2 - backToMenuTextWidth/2, screenHeight/2 + 80, 30, LIGHTGRAY);


    EndDrawing();
}



void initBlock(Block* block, float posX){
    block->position = (Vector2){posX, screenHeight/2 - BLOCK_HEIGHT/2};

    block->size = (Vector2){BLOCK_WIDTH, BLOCK_HEIGHT};

    block->hitbox = (Rectangle){block->position.x, 
                                block->position.y, 
                                block->size.x, 
                                block->size.y};
    block->velocity = BLOCK_VELOCITY;
    
    block->color = WHITE;

}
void initBall(Ball* ball){
    ball->position = (Vector2){screenWidth/2, screenHeight/2};

    ball->velocity = (Vector2){BALL_VELOCITY, BALL_VELOCITY};

    ball->radius = BALL_RADIUS;

    ball->bounceCount = 0;
    
    ball->color = WHITE;
}
void initButton(Button* button, float posY){
    button->position = (Vector2) {screenWidth/2 - 96, posY};
    button->bounds = (Rectangle){screenWidth/2 - 96, posY, 192, 48};
    button->color = (Color) WHITE;
}