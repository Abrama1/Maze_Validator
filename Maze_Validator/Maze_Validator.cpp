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

int main()
{

}