#include "raylib.h"

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------


//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
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
        




    }
    
    return 0;
}

//------------------------------------------------------------------------------------
// Module Functions Definitions (local)
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(){

}


// Update game (one frame)
void UpdateGame(){

}
// Draw game (one frame)
void DrawGame(){

}

// Update and Draw (one frame)
void UpdateDrawFrame(){
    UpdateGame();
    DrawGame();
}
