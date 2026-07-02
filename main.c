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


bool pause = false;
bool gameEnds = false;
bool onePlayer = false;
int framesCounter = 0;
int scoreTimer = 0;
float scoreLinePosX = 0;


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);             // Initialize game
static void initBlock(Block* block, float posX);   //initialize player
static void initBall(Ball* ball);     //initialize ball
static void initButton(Button* button, float posY);

static void DrawLogo(void);

static void UpdateDrawTitle(void);

static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
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

    InitWindow(800, 450, "Ray-Pong");
    SetTargetFPS(60);              


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




    initBlock(&player1, 30);
    initBlock(&player2, screenWidth-40);
    initBall(&ball);

    initButton(&btn1p, screenHeight/2);
    initButton(&btn2p, screenHeight/2 + 64);
  

}


void DrawLogo(){
    BeginDrawing();

    ClearBackground(LIGHTGRAY);
    DrawText("WAYSELL", screenWidth/2 -100, screenHeight/2 -80, 50, BLACK);


    EndDrawing();

}


void UpdateDrawTitle(){
    BeginDrawing();

    ClearBackground(BLACK);
    //Draw title
    DrawText("RAY-PONG", screenWidth/2 -100, screenHeight/2 -80, 40, WHITE);
            
    //Draw 1 player button
    DrawRectangleV(btn1p.position, (Vector2){btn1p.bounds.width, btn1p.bounds.height}, WHITE);
    DrawText("1 Player", btn1p.position.x+26, btn1p.position.y+9, 35, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), btn1p.bounds))
    {
        DrawRectangleRec(btn1p.bounds, Fade(RED, 0.2f));
        DrawRectangleLinesEx(btn1p.bounds, 3.0f, RED);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onePlayer = true;
            currentScreen = GAMEPLAY;
        }
    }else{
        DrawRectangleRec(btn1p.bounds, Fade(DARKGRAY, 0.2f));
        DrawRectangleLinesEx(btn1p.bounds, 3.0f, DARKGRAY);
    }  
     

    //Draw 2 player button
    DrawRectangleV(btn2p.position, (Vector2){btn2p.bounds.width, btn2p.bounds.height}, WHITE);
    DrawText("2 Player", btn2p.position.x+26, btn2p.position.y+9, 35, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), btn2p.bounds))
    {
        DrawRectangleRec(btn2p.bounds, Fade(RED, 0.2f));
        DrawRectangleLinesEx(btn2p.bounds, 3.0f, RED);
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            onePlayer = false;
            currentScreen = GAMEPLAY;
        }
    }else{
        DrawRectangleRec(btn2p.bounds, Fade(DARKGRAY, 0.2f));
        DrawRectangleLinesEx(btn2p.bounds, 3.0f, DARKGRAY);
    }

    EndDrawing();
}


// Update game (one frame)
void UpdateGame(){
    if(IsKeyDown(KEY_W) && player1.position.y >= 0) player1.position.y -= player1.velocity;
    if(IsKeyDown(KEY_S) && player1.position.y <= screenHeight - player1.size.y) player1.position.y += player1.velocity;

    player1.hitbox.y = player1.position.y;
    player2.hitbox.y = player2.position.y;

    if(onePlayer){
        if((ball.position.y > player2.position.y +  player2.hitbox.height/2) && player2.position.y <= screenHeight - player2.size.y){
            player2.position.y += player2.velocity;
        }
        else if(ball.position.y < player2.position.y +  player2.hitbox.height/2 && (player2.position.y >= 0)){
            player2.position.y -= player2.velocity;
        }
    }
    else{

        if(IsKeyDown(KEY_UP) && player2.position.y >= 0) player2.position.y -= player2.velocity;
        if(IsKeyDown(KEY_DOWN) && player2.position.y <= screenHeight - player2.size.y) player2.position.y += player2.velocity;

    }

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
    printf("Ball vx: %f, Ball vy: %f\n", ball.velocity.x, ball.velocity.y);

    
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
            DrawText(TextFormat("%i - %i", scoreP1, scoreP2), screenWidth/2 -50, 10, 40, WHITE);
        }

        if(scoreTimer > 0) {
            scoreTimer--;
            DrawLineEx((Vector2){scoreLinePosX, 0}, (Vector2){scoreLinePosX, screenHeight}, 10.0, RED);
        }
    EndDrawing();
}

void UpdateEnding(){

}

void DrawEnding(){
    BeginDrawing();

    ClearBackground(BLACK);
    if(scoreP1 == 10){
        DrawText("PLAYER 1 WINS", screenWidth/2, screenHeight/2, 40, WHITE);
    }
    else{
        DrawText("PLAYER 2 WINS", screenWidth/2, screenHeight/2, 40, WHITE);

    }
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
    button->position = (Vector2) {screenWidth/2-88, posY};
    button->bounds = (Rectangle){screenWidth/2-88, posY, 192, 48};
    button->color = (Color) WHITE;
}