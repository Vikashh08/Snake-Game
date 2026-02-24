#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>

using namespace std;

struct SnakePart {
    int x;
    int y;
    SnakePart* next;

    SnakePart(int posX, int posY) {
        x = posX;
        y = posY;
        next = nullptr;
    }
};

const int boardWidth = 20;
const int boardHeight = 15;

int foodX;
int foodY;
int score;

bool gameFinished;

enum Direction {
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

Direction currentDirection;

SnakePart* snakeHead;

int keyPressed() {
    struct termios oldSettings, newSettings;
    int character;
    int oldFlags;

    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags | O_NONBLOCK);

    character = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags);

    if (character != EOF) {
        ungetc(character, stdin);
        return 1;
    }

    return 0;
}

char readChar() {
    char ch;
    struct termios oldSettings, newSettings;

    tcgetattr(STDIN_FILENO, &oldSettings);
    newSettings = oldSettings;

    newSettings.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);

    return ch;
}

void generateFood() {
    bool foodOnSnake;

    do {
        foodOnSnake = false;

        foodX = rand() % boardWidth;
        foodY = rand() % boardHeight;

        SnakePart* temp = snakeHead;

        while (temp != nullptr) {
            if (temp->x == foodX && temp->y == foodY) {
                foodOnSnake = true;
                break;
            }
            temp = temp->next;
        }

    } while (foodOnSnake);
}

void initializeGame() {
    srand(time(0));

    gameFinished = false;
    currentDirection = NONE;

    snakeHead = new SnakePart(boardWidth / 2, boardHeight / 2);

    score = 0;

    generateFood();
}

void drawGame() {
    system("clear");

    for (int i = 0; i < boardWidth + 2; i++)
        cout << "#";
    cout << endl;

    for (int row = 0; row < boardHeight; row++) {

        for (int col = 0; col < boardWidth; col++) {

            if (col == 0)
                cout << "#";

            if (snakeHead->x == col && snakeHead->y == row)
                cout << "O";

            else if (foodX == col && foodY == row)
                cout << "*";

            else {
                bool isBody = false;

                SnakePart* temp = snakeHead->next;

                while (temp != nullptr) {
                    if (temp->x == col && temp->y == row) {
                        cout << "o";
                        isBody = true;
                        break;
                    }
                    temp = temp->next;
                }

                if (!isBody)
                    cout << " ";
            }

            if (col == boardWidth - 1)
                cout << "#";
        }

        cout << endl;
    }

    for (int i = 0; i < boardWidth + 2; i++)
        cout << "#";

    cout << endl;

    cout << "Score: " << score << endl;
    cout << "Controls: W A S D | Press X to exit" << endl;
}

void readInput() {

    if (keyPressed()) {

        char key = readChar();

        while (keyPressed())
            readChar();

        if (key == 'a' || key == 'A') {
            if (currentDirection != RIGHT)
                currentDirection = LEFT;
        }

        else if (key == 'd' || key == 'D') {
            if (currentDirection != LEFT)
                currentDirection = RIGHT;
        }

        else if (key == 'w' || key == 'W') {
            if (currentDirection != DOWN)
                currentDirection = UP;
        }

        else if (key == 's' || key == 'S') {
            if (currentDirection != UP)
                currentDirection = DOWN;
        }

        else if (key == 'x' || key == 'X') {
            gameFinished = true;
        }
    }
}

void updateGame() {

    if (currentDirection == NONE)
        return;

    int newHeadX = snakeHead->x;
    int newHeadY = snakeHead->y;

    if (currentDirection == LEFT)
        newHeadX--;

    else if (currentDirection == RIGHT)
        newHeadX++;

    else if (currentDirection == UP)
        newHeadY--;

    else if (currentDirection == DOWN)
        newHeadY++;

    if (newHeadX < 0 || newHeadX >= boardWidth ||
        newHeadY < 0 || newHeadY >= boardHeight) {

        gameFinished = true;
        return;
    }

    SnakePart* temp = snakeHead;

    while (temp != nullptr) {
        if (temp->x == newHeadX && temp->y == newHeadY) {
            gameFinished = true;
            return;
        }
        temp = temp->next;
    }

    SnakePart* newHead = new SnakePart(newHeadX, newHeadY);

    newHead->next = snakeHead;

    snakeHead = newHead;

    if (newHeadX == foodX && newHeadY == foodY) {

        score += 10;

        generateFood();
    }
    else {

        SnakePart* temp = snakeHead;

        while (temp->next->next != nullptr)
            temp = temp->next;

        delete temp->next;

        temp->next = nullptr;
    }
}

int main() {

    initializeGame();

    while (!gameFinished) {

        drawGame();

        readInput();

        updateGame();

        usleep(200000);
    }

    drawGame();

    cout << endl;
    cout << "Game Over!" << endl;
    cout << "Final Score: " << score << endl;

    SnakePart* temp = snakeHead;

    while (temp != nullptr) {
        SnakePart* next = temp->next;
        delete temp;
        temp = next;
    }

    return 0;
}