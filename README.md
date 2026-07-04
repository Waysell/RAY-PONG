# Ray-Pong

A lightweight, 2D implementation of the classic Pong game, built from scratch in **C** using the **Raylib** library[cite: 1].

## Features
*   **Game Modes:** Play against a friend (2-player) or challenge the AI (1-player)[cite: 1].
*   **Menu System:** Intuitive main menu to select game modes[cite: 1].
*   **Dynamic Gameplay:** Includes score tracking, ball speed acceleration, and win/loss states[cite: 1].
*   **Simple Controls:** Easy-to-use keyboard inputs for both players[cite: 1].

## Prerequisites
To compile and run this game, you need the **Raylib** library installed on your system[cite: 1, 2].

## Controls
*   **Player 1:** `W` (Up) / `S` (Down)[cite: 1]
*   **Player 2:** `Up Arrow` / `Down Arrow`[cite: 1]
*   **Game Management:** 
    *   `R` to restart during the ending screen[cite: 1]
    *   `M` to return to the main menu[cite: 1]

## How to Build and Run
This project includes a `Makefile` for easy compilation[cite: 2].

1. **Clone the repository** and navigate to the project folder.
2. **Build the game:**
   ```bash
   make build
   ```
   *Note: This command uses the default Linux configuration in the Makefile[cite: 2].*
3. **Run the game:**
   ```bash
   make run
   ```
4. **Cleanup:** To remove the executable, use:
   ```bash
   make clean
   ```

### Note for Non-Linux Users
The provided `Makefile` is configured for Linux by default[cite: 2]. If you are on **Windows (MinGW)** or **macOS**, open the `Makefile` in a text editor and uncomment the appropriate `LDFLAGS` line for your operating system, while commenting out the Linux line[cite: 2].
