#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>  // For srand()

using namespace std;

bool gameOver;
const int height = 20;
const int width = 20;

int x, y, fruitX, fruitY, score;
int nTail;  // Track snake tail length
int tailX[100], tailY[100];  // Store tail positions

enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
eDirection dir;

void Setup() {
    gameOver = false;
    dir = STOP;  // Initialize direction
    x = width / 2;
    y = height / 2;
    
    // Ensure fruit doesn't spawn on snake head
    do {
        fruitX = rand() % width;
        fruitY = rand() % height;
    } while (fruitX == x && fruitY == y);
    
    score = 0;
    nTail = 0;
}

void Draw() {
    system("cls");  // Clear screen

    // Draw top border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Draw game area
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Draw borders
            if (j == 0 || j == width - 1)
                cout << "#";
            // Draw snake head
            else if (i == y && j == x)
                cout << "O";
            // Draw fruit
            else if (i == fruitY && j == fruitX)
                cout << "F";
            else {
                bool printTail = false;
                // Draw tail segments
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printTail = true;
                    }
                }
                if (!printTail)
                    cout << " ";
            }
        }
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Display score
    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'a':
            if (dir != RIGHT)  // Prevent 180° turns
                dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            case 'X':
            case 27:  // ESC key
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    // Update tail positions
    int prevX = tailX[0];
    int prevY = tailY[0];
    tailX[0] = x;
    tailY[0] = y;
    
    for (int i = 1; i < nTail; i++) {
        int tempX = tailX[i];
        int tempY = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    // Move snake head
    switch (dir) {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }

    // Wall collision (teleport to opposite side)
    if (x >= width) x = 0;
    else if (x < 0) x = width - 1;
    if (y >= height) y = 0;
    else if (y < 0) y = height - 1;

    // Self-collision check
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;
    }

    // Fruit collision
    if (x == fruitX && y == fruitY) {
        score += 10;
        nTail++;
        
        // Spawn new fruit (not on snake)
        bool validFruit;
        do {
            validFruit = true;
            fruitX = rand() % width;
            fruitY = rand() % height;
            
            // Check if fruit spawns on snake
            if (fruitX == x && fruitY == y)
                validFruit = false;
            for (int i = 0; i < nTail; i++) {
                if (fruitX == tailX[i] && fruitY == tailY[i])
                    validFruit = false;
            }
        } while (!validFruit);
    }
}

int main() {
    srand(time(0));  // Seed random number generator
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(50);  // Adjust game speed (lower = faster)
    }
    cout << "Game Over! Final Score: " << score << endl;
    system("pause");
    return 0;
}