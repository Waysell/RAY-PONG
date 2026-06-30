#include "raylib.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define BLOCK_VELOCITY  3.0f
#define BLOCK_WIDTH     15
#define BLOCK_HEIGHT    80



#define BALL_VELOCITY   5.0f       
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
    Color color;
} Ball;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static const int screenWidth = 800;
static const int screenHeight = 450;

Block player1 = {0};
Block player2 = {0};

Ball ball = {0};

static int scoreP1 = 0;
static int scoreP2 = 0;



bool gameEnds = false;
bool onePlayer = false;



//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void initBlocks(Block*, float); //initialize players

static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{

    InitWindow(800, 450, "Ray-Pong");
    SetTargetFPS(60);              

    InitGame();

    while (!WindowShouldClose()){
        
        UpdateDrawFrame();

    }
    
    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(){

    initBlocks(&player1, 30);
    initBlocks(&player2, screenWidth-40);

    



}
void initBlocks(Block* block, float posX){
    block->position = (Vector2){posX, screenHeight/2 - BLOCK_HEIGHT/2};

    block->size = (Vector2){BLOCK_WIDTH, BLOCK_HEIGHT};

    block->hitbox = (Rectangle){block->position.x, 
                                block->position.y, 
                                block->position.x+ block->size.x, 
                                block->position.y+ block->size.y};
    block->velocity = BLOCK_VELOCITY;
    
    block->color = WHITE;

}


// Update game (one frame)
void UpdateGame(){


}
// Draw game (one frame)
void DrawGame(){
    BeginDrawing();
        ClearBackground(BLACK);
        //DrawRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, WHITE);

        DrawRectangleV(player1.position, player1.size, player1.color);
        DrawRectangleV(player2.position, player2.size, player2.color);
    EndDrawing();
}

// Update and Draw (one frame)
void UpdateDrawFrame(){
    UpdateGame();
    DrawGame();
}
