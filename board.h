#include <algorithm>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>
#pragma once
using namespace std;

class Board {
   public:
    vector<vector<string>> maskedBoard;
    int boardSize;
    Board(int boardSize) {
        this->boardSize = boardSize;
        this->board = vector<vector<string>>(boardSize + 2, vector<string>(boardSize + 2, "0"));
        this->maskedBoard = vector<vector<string>>(boardSize, vector<string>(boardSize, "■"));
        randomSwapXItemsTo(boardSize * boardSize / 6, "B");
        calculateNumberOfBombsAroundTiles();
    }

    void printMaskedBoard() {
        printMatrix(maskedBoard);
    }

    int revealTile(int x, int y) {
        if (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            revealInMaskedBoard(x, y);
            if (maskedBoard[x][y] == "B") {
                maskedBoard[x][y] = "X";
                board[x + 1][y + 1] = "X";
                revealAllBombsInMaskedBoard();
                return 0;
            }
        }
        return 1;
    }

    void printMatrix(vector<vector<string>> m) {
        for (size_t i = 0; i < m.size(); i++) {
            for (size_t j = 0; j < m[0].size(); j++) {
                cout << m[i][j] << " ";
            }
            cout << endl;
        }
    }

   private:
    vector<vector<string>> board;

    void randomSwapXItemsTo(int numberOfItems, string swapTo) {
        vector<string> aux((board.size() - 2) * (board[0].size() - 2), " ");
        for (int i = 0; i < numberOfItems; i++) {
            aux[i] = swapTo;
        }
        std::random_device rd;
        std::mt19937 g(rd());
        shuffle(aux.begin(), aux.end(), g);
        for (size_t i = 0; i < aux.size(); i++) {
            board[1 + i / (board.size() - 2)][1 + i % (board.size() - 2)] = aux[i];
        }
    }

    void calculateNumberOfBombsAroundTiles() {
        for (size_t i = 1; i < board.size() - 1; i++) {
            for (size_t j = 1; j < board[0].size() - 1; j++) {
                if (board[i][j] == " ") {
                    int bombCount = 0;
                    for (int k = -1; k < 2; k++) {
                        for (int h = -1; h < 2; h++) {
                            if (!(k == 0 && h == 0) && board[i + k][j + h] == "B") {
                                bombCount++;
                            }
                        }
                    }

                    if (bombCount > 0) {
                        board[i][j] = to_string(bombCount);
                    }
                }
            }
        }
    }

    void expandEmptySpaces(int x, int y) {
        maskedBoard[x][y] = board[x + 1][y + 1];
        if (maskedBoard[x][y] != " ") {
            return;
        }
        for (int k = -1; k < 2; k++) {
            for (int h = -1; h < 2; h++) {
                if (!(k == 0 && h == 0) && k != 2 && h != 2 && board[1 + x + k][1 + y + h] != "B" && board[1 + x + k][1 + y + h] != "0" && maskedBoard[x + k][y + h] == "■") {
                    expandEmptySpaces(x + k, y + h);
                }
            }
        }
    }

    void revealInMaskedBoard(int x, int y) {
        maskedBoard[x][y] = board[x + 1][y + 1];
        if (maskedBoard[x][y] == " ") {
            expandEmptySpaces(x, y);
        }
    }

    void revealAllBombsInMaskedBoard() {
        for (size_t i = 1; i < board.size() - 1; i++) {
            for (size_t j = 1; j < board[0].size() - 1; j++) {
                if (board[i][j] == "B") {
                    maskedBoard[i - 1][j - 1] = board[i][j];
                }
            }
        }
    }
};