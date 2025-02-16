# OpenGL Snake Game

A simple Snake game implementation using OpenGL, GLEW, and GLFW for VSCode.

## Prerequisites

Before running the game, ensure you have installed:
1. Visual Studio Code
2. C/C++ extension for VSCode
3. Code Runner extension for VSCode
4. MinGW (for Windows) or GCC (for Linux/macOS)
5. OpenGL libraries:
   - GLEW
   - GLFW
   - FreeGLUT

### Windows Setup
1. Install required libraries using a package manager like vcpkg:
```bash
vcpkg install glew:x64-windows
vcpkg install glfw3:x64-windows
vcpkg install freeglut:x64-windows
```

### Linux (Ubuntu/Debian) Setup
```bash
sudo apt-get update
sudo apt-get install libglew-dev libglfw3-dev freeglut3-dev
```

### macOS Setup
```bash
brew install glew glfw freeglut
```

## VSCode Setup

1. Install the required extensions:
   - C/C++ Extension (ms-vscode.cpptools)
   - Code Runner (formulahendry.code-runner)

2. Enable "Run In Terminal" for Code Runner:
   - Open VSCode Settings (Ctrl+,)
   - Search for "Code-runner: Run In Terminal"
   - Check the box to enable it
   ![image](https://github.com/user-attachments/assets/dfbe8eb3-8f98-485b-ace0-673fec8eca1a)


## Running the Game

1. Clone the repository:
```bash
git clone https://github.com/Rehandra145/GLUT-GLFW-simple-snake-games.git
cd snake-game
```

2. Open the project in VSCode:
```bash
code .
```

3. Open `main.cpp`

4. Run the code:
   - Click the play button in the top right corner
   - Or use the shortcut: Ctrl+Alt+N

## Game Controls
- Arrow keys or WASD to move the snake
- Enter/Space to select menu options
- Up/Down arrows or W/S to navigate menus

## Troubleshooting

If you encounter any issues:

1. **Compilation errors about missing libraries**
   - Make sure all OpenGL libraries are properly installed
   - For Windows users, ensure MinGW is in your system PATH

2. **Code Runner not executing**
   - Verify "Run In Terminal" is enabled in VSCode settings
   - Check if your C++ compiler is properly installed and in PATH

3. **Game window not appearing**
   - Ensure your graphics drivers are up to date
   - Verify OpenGL libraries are properly installed

## Project Structure
```
snake-game/
│
└── main.cpp           # Main game code
```

For any other issues, please open an issue on the project repository.


made by : Rehandra with ❤️
