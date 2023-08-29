#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "board.h"
#include "solver.h"
using namespace std;

void printMatrix(vector<vector<string>> m) {
    for (size_t i = 0; i < m.size(); i++) {
        for (size_t j = 0; j < m[0].size(); j++) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int boardSize = 16;
    Board board = Board(boardSize);

    string line;
    while (1) {
        line = "";
        int success = 0;
        printf("\x1B[2J");
        printf("\x1B[H");
        board.printMaskedBoard();
        cout << "Type \"solve\" to solve, \"exit\" to exit" << endl;
        getline(cin, line);
        if (line == "exit") {
            break;
        }
        if (line == "solve") {
            Solver solver = Solver(&board);
            success = solver.solve();
        } else {
            stringstream ss(line);
            int x = -1, y = -1;
            ss >> x >> y;
            success = board.revealTile(x, y);
        }
        if (board.didWin()) {
            cout << "You won!" << endl;
            break;
        }
        if (!success) {
            printf("\x1B[2J");
            printf("\x1B[H");
            board.printMaskedBoard();
            cout << "You lose!" << endl;
            break;
        }
    }

    return 0;
}