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

int main()
{

}