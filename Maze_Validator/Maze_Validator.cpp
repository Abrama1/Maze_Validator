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

int main()
{

}