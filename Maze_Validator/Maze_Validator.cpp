#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <cmath>
#include <cctype>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;

const int MAX_DIM = 20; // Maximum maze dimensions

// ANSI color codes
const string RESET = "\033[0m";
const string PLAYER_COLOR = "\033[32m"; // Green
const string END_COLOR = "\033[31m";    // Red
const string PATH_COLOR = "\033[34m";   // Blue

void printMaze(const vector<vector<char>>& maze) {
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            if (cell == 'P') {
                cout << PLAYER_COLOR << cell << RESET << ' ';
            }
            else if (cell == 'E') {
                cout << END_COLOR << cell << RESET << ' ';
            }
            else if (cell == '*') {
                cout << PATH_COLOR << cell << RESET << ' ';
            }
            else {
                cout << cell << ' ';
            }
        }
        cout << endl;
    }
}

struct Node {
    int x, y;
    int g, h; // g = cost from start, h = heuristic cost to end
    Node* parent;

    Node(int x, int y, int g, int h, Node* parent = nullptr)
        : x(x), y(y), g(g), h(h), parent(parent) {}

    int f() const { return g + h; } // Total cost

    bool operator>(const Node& other) const { return f() > other.f(); }
};

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2); // Manhattan distance
}

bool isValidMove(const vector<vector<char>>& maze, int x, int y) {
    return x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && maze[x][y] != '1';
}

bool solveMazeForValidation(vector<vector<char>>& maze, int startX, int startY, int endX, int endY) {
    priority_queue<Node, vector<Node>, greater<Node>> openList;
    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

    openList.emplace(startX, startY, 0, heuristic(startX, startY, endX, endY));

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        // If the end is reached
        if (current.x == endX && current.y == endY) {
            return true; // Valid path exists
        }

        if (visited[current.x][current.y]) continue;
        visited[current.x][current.y] = true;

        // Generate neighbors
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (const auto& [dx, dy] : directions) {
            int newX = current.x + dx;
            int newY = current.y + dy;

            if (isValidMove(maze, newX, newY) && !visited[newX][newY]) {
                openList.emplace(newX, newY, current.g + 1, heuristic(newX, newY, endX, endY), nullptr);
            }
        }
    }

    return false; // No valid path found
}

void generateMaze(vector<vector<char>>& maze, int rows, int cols, int& startX, int& startY, int& endX, int& endY) {
    do {
        maze = vector<vector<char>>(rows, vector<char>(cols, '0')); // Initialize with paths
        srand(time(nullptr));

        // Add random walls
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (rand() % 4 == 0) { // 25% chance to place a wall
                    maze[i][j] = '1';
                }
            }
        }

        // Place start ('P') and end ('E')
        startX = rand() % rows;
        startY = rand() % cols;
        endX = rand() % rows;
        endY = rand() % cols;

        maze[startX][startY] = 'P'; // Starting position
        maze[endX][endY] = 'E';     // Ending position
    } while (!solveMazeForValidation(maze, startX, startY, endX, endY));
}

void solveMazeAStar(vector<vector<char>>& maze, int startX, int startY, int endX, int endY) {
    priority_queue<Node, vector<Node>, greater<Node>> openList;
    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

    openList.emplace(startX, startY, 0, heuristic(startX, startY, endX, endY));

    while (!openList.empty()) {
        Node current = openList.top();
        openList.pop();

        // If the end is reached
        if (current.x == endX && current.y == endY) {
            // Backtrack to mark the path
            Node* path = &current;
            while (path->parent != nullptr) {
                if (maze[path->x][path->y] != 'E') {
                    maze[path->x][path->y] = '*';
                }
                path = path->parent;

                system("cls");
                printMaze(maze);
                this_thread::sleep_for(chrono::milliseconds(200));
            }
            cout << "\nSolved the maze using A*!\n";
            return;
        }

        if (visited[current.x][current.y]) continue;
        visited[current.x][current.y] = true;

        // Generate neighbors
        vector<pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
        for (const auto& [dx, dy] : directions) {
            int newX = current.x + dx;
            int newY = current.y + dy;

            if (isValidMove(maze, newX, newY) && !visited[newX][newY]) {
                openList.emplace(newX, newY, current.g + 1, heuristic(newX, newY, endX, endY), new Node(current));
            }
        }
    }

    cout << "No solution found for the maze!\n";
}

void updatePlayerPosition(vector<vector<char>>& maze, int& playerX, int& playerY, int newX, int newY, int& moves) {
    maze[playerX][playerY] = '0'; // Reset current position to path
    playerX = newX;
    playerY = newY;
    maze[playerX][playerY] = 'P'; // Mark new position
    moves++; // Increment move count

    system("cls"); // Clear console (use "clear" on Unix-like systems)
    printMaze(maze);
    cout << "Moves made: " << moves << endl;
}

void validatePath(vector<vector<char>>& maze, int& playerX, int& playerY, int endX, int endY, int& moves) {
    cout << "Enter your moves (W: up, A: left, S: down, D: right). Enter Q to stop: ";
    char move;
    while (cin >> move) {
        move = tolower(move); // Normalize input to lowercase
        if (move == 'q') break;

        int newX = playerX, newY = playerY;
        switch (move) {
        case 'w': newX--; break; // Up
        case 'a': newY--; break; // Left
        case 's': newX++; break; // Down
        case 'd': newY++; break; // Right
        default:
            cout << "Invalid input! Use W/A/S/D to move or Q to quit.\n";
            continue;
        }

        if (newX >= 0 && newX < maze.size() && newY >= 0 && newY < maze[0].size() && maze[newX][newY] != '1') {
            updatePlayerPosition(maze, playerX, playerY, newX, newY, moves);
            if (playerX == endX && playerY == endY) {
                cout << "\nCongratulations! You've successfully reached the end in " << moves << " moves!\n";
                break;
            }
        }
        else {
            cout << "Invalid move: Wall or boundary ahead!\n";
        }
    }
}

int main() {
    vector<vector<char>> maze;
    int rows = 10, cols = 10; // Default dimensions
    int startX, startY, endX, endY;
    int playerX, playerY;
    int moves = 0;

    while (true) {
        generateMaze(maze, rows, cols, startX, startY, endX, endY);

        playerX = startX;
        playerY = startY;

        int choice;
        do {
            system("cls");
            cout << "--- Maze Path Validator ---\n\n";
            printMaze(maze);
            cout << "\n1. Generate new maze\n";
            cout << "2. Change maze size\n";
            cout << "3. Start maze\n";
            cout << "4. Solve automatically\n";
            cout << "5. Quit\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear(); // Clear error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                cout << "Invalid input! Please enter a number.\n";
                continue;
            }

            switch (choice) {
            case 1:
                generateMaze(maze, rows, cols, startX, startY, endX, endY);
                playerX = startX;
                playerY = startY;
                moves = 0;
                break;
            case 2:
                cout << "Enter new dimensions (max " << MAX_DIM << "x" << MAX_DIM << "): ";
                if (!(cin >> rows >> cols) || rows <= 0 || cols <= 0) {
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Invalid dimensions! Please enter positive integers.\n";
                    rows = 10; cols = 10; // Reset to default
                    break;
                }
                rows = min(rows, MAX_DIM);
                cols = min(cols, MAX_DIM);
                generateMaze(maze, rows, cols, startX, startY, endX, endY);
                playerX = startX;
                playerY = startY;
                moves = 0;
                break;
            case 3:
                validatePath(maze, playerX, playerY, endX, endY, moves);
                if (playerX == endX && playerY == endY) {
                    generateMaze(maze, rows, cols, startX, startY, endX, endY);
                    playerX = startX;
                    playerY = startY;
                    moves = 0;
                    char playAgain;
                    cout << "\nDo you want to play again? (Y/N): ";
                    cin >> playAgain;
                    playAgain = tolower(playAgain);
                    if (playAgain == 'y') {
                        choice = 0; // Return to main menu
                    }
                    else {
                        choice = 5; // Exit
                    }
                }
                break;
            case 4:
                solveMazeAStar(maze, startX, startY, endX, endY);
                cout << "Press Enter to return to the main menu...";
                cin.ignore(); cin.get();
                generateMaze(maze, rows, cols, startX, startY, endX, endY);
                playerX = startX;
                playerY = startY;
                moves = 0;
                break;
                break;
            case 5:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number corresponding to a menu option.\n";
            }

        } while (choice != 5);

        if (choice == 5) break;
    }

    return 0;
}