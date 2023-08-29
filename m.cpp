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

    Solver solver = Solver(board);

    solver.solve();

    // string line;

    // while (1) {
    //     board.printMaskedBoard();
    //     getline(cin, line);
    //     if (line == "exit") {
    //         break;
    //     }
    //     stringstream ss(line);
    //     int x = -1, y = -1;
    //     ss >> x >> y;
    //     int sucess = board.revealTile(x, y);
    //     if(!sucess){
    //         board.printMaskedBoard();
    //         cout << "You lose!" << endl;
    //         break;
    //     }
    // }

    return 0;
}