#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#pragma once
#include "board.h"
using namespace std;

class Solver {
    Board board;
    vector<vector<string>> workingBoard;
    vector<string> safeMoves;

   public:
    Solver(Board board) : board(board) {
        this->workingBoard = board.maskedBoard;
    }

    void solve() {
        solveLoop();
    }

   private:
    vector<vector<string>> copyMatrix(vector<vector<string>> m) {
        vector<vector<string>> copy(m.size(), vector<string>());
        for (size_t i = 0; i < m.size(); i++) {
            copy[i] = m[i];
        }
        return copy;
    }

    bool compareMatrix(vector<vector<string>> m1, vector<vector<string>> m2) {
        for (size_t i = 0; i < m1.size(); i++) {
            if (m1[i] != m2[i]) {
                return false;
            }
        }
        return true;
    }

    bool updateWorkingBoard(vector<vector<string>> maskedBoard) {
        for (size_t i = 0; i < workingBoard.size(); i++) {
            for (size_t j = 0; j < workingBoard.size(); j++) {
                if ((maskedBoard[i][j] >= "1" && maskedBoard[i][j] <= "8") || maskedBoard[i][j] >= " ") {
                    workingBoard[i][j] = maskedBoard[i][j];
                }
            }
        }
        return true;
    }

    void solveLoop() {
        while (1) {
            printf("\x1B[2J");
            printf("\x1B[H");
            board.printMaskedBoard();
            usleep(300000);
            int success = 0;

            vector<vector<string>> n = copyMatrix(workingBoard);
            updateWorkingBoard(board.maskedBoard);
            while (1) {
                vector<vector<string>> m = copyMatrix(workingBoard);
                markBombs();
                decreaseNumbersFromMarkedBombs();
                if (compareMatrix(m, workingBoard)) {
                    break;
                }
            }
            for (string move : safeMoves) {
                stringstream ss(move);
                int x = -1, y = -1;
                ss >> x >> y;
                success = board.revealTile(x, y);
            }
            updateWorkingBoard(board.maskedBoard);

            string unsafeMove = pickRandomMaskedTile();
            stringstream ss(unsafeMove);
            int x = -1, y = -1;
            ss >> x >> y;
            success = board.revealTile(x, y);

            if (!success) {
                printf("\x1B[2J");
                printf("\x1B[H");
                board.printMaskedBoard();
                cout << "You lose!" << endl;
                break;
            }
            
        }
    }

    string pickRandomMaskedTile() {
        vector<string> unsafeMoves;
        for (size_t i = 0; i < workingBoard.size(); i++) {
            for (size_t j = 0; j < workingBoard.size(); j++) {
                if (workingBoard[i][j] == "■") {
                    unsafeMoves.push_back(to_string(i) + " " + to_string(j));
                }
            }
        }
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, unsafeMoves.size());
        string move = unsafeMoves[distr(gen)];
        return move;
    }

    void markBombs() {
        vector<vector<string>> copiedBoard = copyMatrix(workingBoard);
        for (size_t i = 0; i < workingBoard.size(); i++) {
            for (size_t j = 0; j < workingBoard[0].size(); j++) {
                if (workingBoard[i][j] >= "1" && workingBoard[i][j] <= "8") {
                    int maskedTileCount = 0;

                    for (int h = -1; h < 2; h++) {
                        for (int k = -1; k < 2; k++) {
                            if (i + h >= 0 && i + h < workingBoard.size() &&
                                j + k >= 0 && j + k < workingBoard[0].size() &&
                                workingBoard[i + h][j + k] == "■") {
                                maskedTileCount++;
                            }
                        }
                    }
                    if (maskedTileCount == stoi(workingBoard[i][j])) {
                        for (int h = -1; h < 2; h++) {
                            for (int k = -1; k < 2; k++) {
                                if (i + h >= 0 && i + h < workingBoard.size() &&
                                    j + k >= 0 && j + k < workingBoard[0].size() &&
                                    workingBoard[i + h][j + k] == "■") {
                                    copiedBoard[i + h][j + k] = "b";
                                }
                            }
                        };
                    }
                }
            }
        }
        workingBoard = copiedBoard;
    }

    void decreaseNumbersFromMarkedBombs() {
        vector<vector<string>> copiedBoard = copyMatrix(workingBoard);
        for (size_t i = 0; i < workingBoard.size(); i++) {
            for (size_t j = 0; j < workingBoard[0].size(); j++) {
                if (workingBoard[i][j] >= "1" && workingBoard[i][j] <= "8") {
                    int markedBombTileCount = 0;
                    for (int h = -1; h < 2; h++) {
                        for (int k = -1; k < 2; k++) {
                            if (i + h >= 0 && i + h < workingBoard.size() &&
                                j + k >= 0 && j + k < workingBoard[0].size()) {
                                if (workingBoard[i + h][j + k] == "b") {
                                    markedBombTileCount++;
                                }
                            }
                        }
                    }
                    int newTileValue = stoi(board.maskedBoard[i][j]) - markedBombTileCount;
                    if (newTileValue == 0) {
                        for (int h = -1; h < 2; h++) {
                            for (int k = -1; k < 2; k++) {
                                if (i + h >= 0 && i + h < workingBoard.size() &&
                                    j + k >= 0 && j + k < workingBoard[0].size()) {
                                    if (i + h == 4 && j + k == 9) {
                                    }
                                    if (workingBoard[i + h][j + k] == "■") {
                                        safeMoves.push_back(to_string(i + h) + " " + to_string(j + k));
                                    }
                                }
                            }
                        }
                    }
                    copiedBoard[i][j] = to_string(newTileValue);
                }
            }
        }
        workingBoard = copiedBoard;
    }
};
