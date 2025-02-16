#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

// Game states
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

GameState currentState = MENU;
int selectedOption = 0; // 0 for first option, 1 for second option

struct Vec2 {
    float x, y;
};

Vec2 operator+(const Vec2& a, const Vec2& b) {
    return {a.x + b.x, a.y + b.y};
}

Vec2 operator-(const Vec2& a, const Vec2& b) {
    return {a.x - b.x, a.y - b.y};
}

Vec2 operator*(const Vec2& a, float scalar) {
    return {a.x * scalar, a.y * scalar};
}

float dot(const Vec2& a, const Vec2& b) {
    return a.x * b.x + a.y * b.y;
}

bool operator==(const Vec2& a, const Vec2& b) {
    return a.x == b.x && a.y == b.y;
}

// Forward declarations
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void updateSnake();
void render();
void renderText(const std::string& text, float x, float y, float scale, bool selected);

GLFWwindow* window;
int windowWidth = 640;
int windowHeight = 480;
float aspectRatio = static_cast<float>(windowWidth) / windowHeight;
int baseGridSize = 20;
float gridSize;

struct Snake {
    std::vector<Vec2> body;
    Vec2 direction;
    bool alive = true;

    Snake() { reset(); }

    void reset() {
        // Mulai dari tengah grid, bukan tengah pixel
        float centerX = static_cast<float>(static_cast<int>((windowWidth / gridSize) / 2) * gridSize);
        float centerY = static_cast<float>(static_cast<int>((windowHeight / gridSize) / 2) * gridSize);
        body = { {centerX, centerY} };
        direction = Vec2{1, 0} * gridSize;
        alive = true;
    }
};

struct Food {
    Vec2 position;

    void respawn(const Snake& snake) {
        bool valid = false;
        while (!valid) {
            // Gunakan gridSize yang ter-update untuk menghitung posisi maksimum
            int maxX = static_cast<int>(windowWidth / gridSize) - 1;  // -1 untuk memastikan tidak keluar batas
            int maxY = static_cast<int>(windowHeight / gridSize) - 1;
            
            position.x = static_cast<float>((rand() % maxX) * gridSize);
            position.y = static_cast<float>((rand() % maxY) * gridSize);
            valid = true;
    
            for (const auto& seg : snake.body) {
                if (seg == position) {
                    valid = false;
                    break;
                }
            }
        }
    }
};

Snake snake;
Food food;
float lastUpdateTime = 0.0f;
int score = 0;

void renderText(const std::string& text, float x, float y, float scale, bool selected) {
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);
    
    if (selected) {
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow for selected text
    } else {
        glColor3f(1.0f, 1.0f, 1.0f); // White for unselected text
    }

    // Very simple rectangle-based font rendering
    for (char c : text) {
        glBegin(GL_LINES);
        // Simplified character rendering using lines
        switch (c) {
            case 'P':
                glVertex2f(0, 0); glVertex2f(0, 20);
                glVertex2f(0, 0); glVertex2f(10, 0);
                glVertex2f(10, 0); glVertex2f(10, 10);
                glVertex2f(0, 10); glVertex2f(10, 10);
                break;
            case 'L':
                glVertex2f(0, 0); glVertex2f(0, 20);
                glVertex2f(0, 20); glVertex2f(10, 20);
                break;
            case 'A':
                glVertex2f(0, 0); glVertex2f(5, 20);
                glVertex2f(5, 20); glVertex2f(10, 0);
                glVertex2f(2.5f, 10); glVertex2f(7.5f, 10);
                break;
            case 'Y':
                glVertex2f(0, 20); glVertex2f(5, 10);
                glVertex2f(10, 20); glVertex2f(5, 10);
                glVertex2f(5, 10); glVertex2f(5, 0);
                break;
            case 'E':
                glVertex2f(0, 0); glVertex2f(0, 20);
                glVertex2f(0, 0); glVertex2f(10, 0);
                glVertex2f(0, 10); glVertex2f(8, 10);
                glVertex2f(0, 20); glVertex2f(10, 20);
                break;
            case 'X':
                glVertex2f(0, 0); glVertex2f(10, 20);
                glVertex2f(0, 20); glVertex2f(10, 0);
                break;
            case 'I':
                glVertex2f(5, 0); glVertex2f(5, 20);
                break;
            case 'T':
                glVertex2f(0, 20); glVertex2f(10, 20);
                glVertex2f(5, 20); glVertex2f(5, 0);
                break;
            case 'R':
                glVertex2f(0, 0); glVertex2f(0, 20);
                glVertex2f(0, 20); glVertex2f(10, 20);
                glVertex2f(10, 20); glVertex2f(10, 10);
                glVertex2f(0, 10); glVertex2f(10, 10);
                glVertex2f(0, 10); glVertex2f(10, 0);
                break;
        }
        glEnd();
        glTranslatef(15, 0, 0); // Space between letters
    }
    glPopMatrix();
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
    aspectRatio = static_cast<float>(width) / height;
    gridSize = static_cast<float>(baseGridSize) * (static_cast<float>(width) / 640.0f);
    glViewport(0, 0, width, height);
    
    // Reset posisi snake dan food saat window diresize
    snake.reset();
    food.respawn(snake);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    Vec2 dir{0, 0};

    switch (currentState) {
        case MENU:
            if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
                selectedOption = 0;
            } else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
                selectedOption = 1;
            } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE) {
                if (selectedOption == 0) {
                    currentState = PLAYING;
                    snake.reset();
                    food.respawn(snake);
                    score = 0;
                } else {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            }
            break;

            case PLAYING:
            switch (key) {
                case GLFW_KEY_UP:
                case GLFW_KEY_W:
                    dir = Vec2{0, -1} * gridSize;
                    break;
                case GLFW_KEY_DOWN:
                case GLFW_KEY_S:
                    dir = Vec2{0, 1} * gridSize;
                    break;
                case GLFW_KEY_LEFT:
                case GLFW_KEY_A:
                    dir = Vec2{-1, 0} * gridSize;
                    break;
                case GLFW_KEY_RIGHT:
                case GLFW_KEY_D:
                    dir = Vec2{1, 0} * gridSize;
                    break;
            }
            if (dir.x != 0 || dir.y != 0) {
                if (dot(snake.direction, dir) != -gridSize * gridSize) {
                    snake.direction = dir;
                }
            }
            break;

        case GAME_OVER:
            if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
                selectedOption = 0;
            } else if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
                selectedOption = 1;
            } else if (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE) {
                if (selectedOption == 0) {
                    currentState = PLAYING;
                    snake.reset();
                    food.respawn(snake);
                    score = 0;
                } else {
                    glfwSetWindowShouldClose(window, GLFW_TRUE);
                }
            }
            break;
    }
}

void updateSnake() {
    if (currentState != PLAYING) return;

    float currentTime = static_cast<float>(glfwGetTime());
    float deltaTime = currentTime - lastUpdateTime;

    if (deltaTime < 0.2f || !snake.alive) {
        if (!snake.alive) {
            currentState = GAME_OVER;
            selectedOption = 0;
        }
        return;
    }

    lastUpdateTime = currentTime;

    Vec2 head = snake.body[0] + snake.direction;

    if (head.x < 0 || head.x >= windowWidth || head.y < 0 || head.y >= windowHeight) {
        snake.alive = false;
        return;
    }

    for (size_t i = 1; i < snake.body.size(); ++i) {
        if (snake.body[i] == head) {
            snake.alive = false;
            return;
        }
    }

    if (head == food.position) {
        score += 10;
        food.respawn(snake);
    } else {
        snake.body.pop_back();
    }

    snake.body.insert(snake.body.begin(), head);
}

void render() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, windowHeight, 0);
    glMatrixMode(GL_MODELVIEW);
    
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentState) {
        case MENU: {
            glColor3f(0.2f, 0.2f, 0.2f);
            glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(windowWidth, 0);
            glVertex2i(windowWidth, windowHeight);
            glVertex2i(0, windowHeight);
            glEnd();

            float scale = windowWidth / 640.0f;
            renderText("PLAY", windowWidth/2 - 50*scale, windowHeight/2 - 40*scale, scale, selectedOption == 0);
            renderText("EXIT", windowWidth/2 - 50*scale, windowHeight/2 + 40*scale, scale, selectedOption == 1);
            break;
        }

        case PLAYING: {
            glColor3f(0.2f, 0.2f, 0.2f);
            glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(windowWidth, 0);
            glVertex2i(windowWidth, windowHeight);
            glVertex2i(0, windowHeight);
            glEnd();

            glColor3f(0.0f, 0.8f, 0.0f);
            glBegin(GL_QUADS);
            for (const auto& seg : snake.body) {
                glVertex2f(seg.x, seg.y);
                glVertex2f(seg.x + gridSize, seg.y);
                glVertex2f(seg.x + gridSize, seg.y + gridSize);
                glVertex2f(seg.x, seg.y + gridSize);
            }
            glEnd();

            glColor3f(0.8f, 0.0f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(food.position.x, food.position.y);
            glVertex2f(food.position.x + gridSize, food.position.y);
            glVertex2f(food.position.x + gridSize, food.position.y + gridSize);
            glVertex2f(food.position.x, food.position.y + gridSize);
            glEnd();
            break;
        }

        case GAME_OVER: {
            glColor3f(0.5f, 0.1f, 0.1f);
            glBegin(GL_QUADS);
            glVertex2i(0, 0);
            glVertex2i(windowWidth, 0);
            glVertex2i(windowWidth, windowHeight);
            glVertex2i(0, windowHeight);
            glEnd();

            float scale = windowWidth / 640.0f;
            renderText("RETRY", windowWidth/2 - 60*scale, windowHeight/2 - 40*scale, scale, selectedOption == 0);
            renderText("EXIT", windowWidth/2 - 50*scale, windowHeight/2 + 40*scale, scale, selectedOption == 1);
            break;
        }
    }

    glfwSwapBuffers(window);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(windowWidth, windowHeight, "Snake Game", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    gridSize = static_cast<float>(baseGridSize);
    snake.reset();
    food.respawn(snake);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        updateSnake();
        render();
    }

    glfwTerminate();
    return 0;
}