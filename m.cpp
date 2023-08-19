#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iterator>
#include <set>
#include <tuple>
#include <sstream>
using namespace std;

void printMatrix(vector<vector<string>> m)
{
    for (size_t i = 0; i < m.size(); i++)
    {
        for (size_t j = 0; j < m[0].size(); j++)
        {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<string>> randomSwapXItemsTo(vector<vector<string>> m, int numberOfItems, string swapTo)
{
    vector<string> aux((m.size() - 2) * (m[0].size() - 2), " ");
    for (size_t i = 0; i < numberOfItems; i++)
    {
        aux[i] = swapTo;
    }
    random_shuffle(aux.begin(), aux.end());
    for (size_t i = 0; i < aux.size(); i++)
    {
        m[1 + i / (m.size() - 2)][1 + i % (m.size() - 2)] = aux[i];
    }
    return m;
}

vector<vector<string>> calculateNumberOfBombsAroundTiles(vector<vector<string>> m)
{
    for (size_t i = 1; i < m.size() - 1; i++)
    {
        for (size_t j = 1; j < m[0].size() - 1; j++)
        {
            if (m[i][j] == " ")
            {
                int bombCount = 0;
                for (int k = -1; k < 2; k++)
                {
                    for (int h = -1; h < 2; h++)
                    {
                        if (!(k == 0 && h == 0) && m[i + k][j + h] == "B")
                        {
                            bombCount++;
                        }
                    }
                }

                if (bombCount > 0)
                {
                    m[i][j] = to_string(bombCount);
                }
            }
        }
    }
    return m;
}

vector<vector<string>> expandEmptySpaces(int x, int y, vector<vector<string>> board, vector<vector<string>> maskedBoard)
{
    maskedBoard[x][y] = board[x + 1][y + 1];
    if(maskedBoard[x][y] != " "){
        return maskedBoard;
    }
    printMatrix(maskedBoard);
    for (int k = -1; k < 2; k++)
    {
        for (int h = -1; h < 2; h++)
        {
            if (!(k == 0 && h == 0) && k != 2 && h != 2 && board[1 + x + k][1 + y + h] != "B" && board[1 + x + k][1 + y + h] != "0" && maskedBoard[x + k][y + h] == "■")
            {
                maskedBoard = expandEmptySpaces(x + k, y + h, board, maskedBoard);
            }
        }
    }
    return maskedBoard;
}

vector<vector<string>> revealInMaskedBoard(int x, int y, vector<vector<string>> board, vector<vector<string>> maskedBoard)
{
    maskedBoard[x][y] = board[x + 1][y + 1];
    if (maskedBoard[x][y] == " ")
    {
        maskedBoard = expandEmptySpaces(x, y, board, maskedBoard);
    }
    return maskedBoard;
}

vector<vector<string>> revealAllBombsInMaskedBoard(vector<vector<string>> board, vector<vector<string>> maskedBoard)
{
    for (size_t i = 1; i < board.size() - 1; i++)
    {
        for (size_t j = 1; j < board[0].size() - 1; j++)
        {
            if (board[i][j] == "B")
            {
                maskedBoard[i - 1][j - 1] = board[i][j];
            }
        }
    }
    return maskedBoard;
}

int main()
{
    int boardSize = 16;
    vector<vector<string>> board(boardSize + 2, vector<string>(boardSize + 2, "0"));

    // ███
    board = randomSwapXItemsTo(board, boardSize * boardSize / 6, "B");
    board = calculateNumberOfBombsAroundTiles(board);

    vector<vector<string>> maskedBoard(boardSize, vector<string>(boardSize, "■"));

    string line;
    while (1)
    {
        printMatrix(maskedBoard);
        getline(cin, line);
        if (line == "exit")
        {
            break;
        }
        stringstream ss(line);
        int x = -1, y = -1;
        ss >> x >> y;
        if (x != -1 && y != -1)
        {
            maskedBoard = revealInMaskedBoard(x, y, board, maskedBoard);
            if (maskedBoard[x][y] == "B")
            {
                maskedBoard = revealAllBombsInMaskedBoard(board, maskedBoard);
                printMatrix(maskedBoard);
                cout << "You lose!" << endl;
                break;
            }
        }
    }

    return 0;
}