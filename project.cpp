#include <iostream>
using namespace std;
#include <fstream>
#include <stack>
#include <ctype.h>

const int ROWS = 6;
const int COLS = 7;

typedef struct player
{
    string name;
    char s;
} PLAYER;

class ConnectFourBoard
{
private:
    char board[ROWS][COLS];
    PLAYER player1;
    PLAYER player2;
    char currentPlayer;
    stack<int> moves;

public:
    ConnectFourBoard()
    {
        initializeBoard();
        setPlayers();
        string n;
        do
        {
            cout << "Who will start?: ";
            cin >> n;
            if (player1.name == n || player2.name == n)
                break;
            cout << "invalid player name try again " << endl;
        } while (1);
        if (player1.name == n)
            currentPlayer = player1.s;
        else
            currentPlayer = player2.s;
    }

    void initializeBoard()
    {
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                board[i][j] = ' ';
            }
        }
    }

    void setPlayers()
    {
        cout << "Enter the name for Player 1: ";
        cin >> player1.name;

        do
        {
            cout << "Enter the name for Player 2: ";
            cin >> player2.name;
            if (player1.name != player2.name)
                break;
            cout << "Try again, 2 players can't have the same name" << endl;
        } while (true);

        do
        {
            cout << "Choose a symbol for " << player1.name << " (X or O): ";
            cin >> player1.s;
            if (player1.s == 'X' || player1.s == 'O')
                break;
            cout << "Invalid input, try again" << endl;
        } while (true);

        if (player1.s == 'X')
            player2.s = 'O';
        else
            player2.s = 'X';
    }
    void displayBoard()
    {
        for (int i = 0; i < COLS; i++)
        {
            cout << "----";
        }
        cout << "-" << endl;
        for (int i = 0; i < ROWS; i++)
        {
            cout << "|";
            for (int j = 0; j < COLS; j++)
            {
                cout << " " << board[i][j] << " |";
            }
            cout << " " << i + 1 << endl;
            for (int j = 0; j < COLS; j++)
            {
                cout << "----";
            }
            cout << "-" << endl;
        }
        cout << "  1   2   3   4   5   6   7 " << endl;
    }

    bool isColumnFull(int col)
    {
        return board[0][col] != ' ';
    }

    bool dropPiece(int col)
    {
        for (int i = ROWS - 1; i >= 0; i--)
        {
            if (board[i][col] == ' ')
            {
                board[i][col] = currentPlayer;
                moves.push(col);
                return true;
            }
        }
        return false;
    }

    bool isWinner()
    {
        char piece = currentPlayer;

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (j + 3 < COLS && board[i][j] == piece && board[i][j + 1] == piece && board[i][j + 2] == piece && board[i][j + 3] == piece)
                {
                    return true;
                }
                if (i + 3 < ROWS)
                {
                    if (board[i][j] == piece && board[i + 1][j] == piece && board[i + 2][j] == piece && board[i + 3][j] == piece)
                    {
                        return true;
                    }
                    if (j + 3 < COLS && board[i][j] == piece && board[i + 1][j + 1] == piece && board[i + 2][j + 2] == piece && board[i + 3][j + 3] == piece)
                    {
                        return true;
                    }
                    if (j - 3 >= 0 && board[i][j] == piece && board[i + 1][j - 1] == piece && board[i + 2][j - 2] == piece && board[i + 3][j - 3] == piece)
                    {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isBoardFull()
    {
        for (int i = 0; i < COLS; i++)
        {
            if (!isColumnFull(i))
            {
                return false;
            }
        }
        return true;
    }

    void undoMove()
    {
        if (!moves.empty())
        {
            int lastMove = moves.top();
            moves.pop();
            for (int i = 0; i < ROWS; i++)
            {
                if (board[i][lastMove] != ' ')
                {
                    board[i][lastMove] = ' ';
                    break;
                }
            }
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
        else
        {
            cout << "No moves to undo." << endl;
        }
    }
    void saveGameToFile(string filename)
    {
        try
        {
            ofstream file(filename, ios::app);

            if (!file.is_open())
            {
                throw "Unable to open file for saving.";
            }

            for (int i = 0; i < ROWS; ++i)
            {
                for (int j = 0; j < COLS; ++j)
                {
                    file << board[i][j] << " ";
                }
                file << " " << i + 1 << "\n";
            }
            file << "1 2 3 4 5 6 7 "
                 << "\n";
            file << "\n";
            file << "player 1:" << player1.name << " (" << player1.s << ")"
                 << "\n";
            file << "player 2:" << player2.name << " (" << player2.s << ")"
                 << "\n";
            if (currentPlayer == player1.s)
                file << "winner:" << player1.name << endl;
            else
                file << "winner:" << player2.name << endl;

            file.close();
        }
        catch (string e)
        {
            cerr << "Error: " << e << endl;
        }
    }

    void printFileContents(string filename)
    {
        try
        {
            ifstream file(filename);

            if (!file.is_open())
            {
                throw "Unable to open file for printing.";
            }

            char ch;
            while (file.get(ch))
            {
                cout << ch;
            }

            file.close();
        }
        catch (string e)
        {
            cerr << "Error: " << e << endl;
        }
    }

    void playGame()
    {
        bool gameWon = false;
        char choice;

        do
        {
            displayBoard();
            int column;

            cout << "Player " << currentPlayer << " (" << (currentPlayer == player1.s ? player1.name : player2.name) << "), enter a column (1-7), or enter 'U' to undo a move: ";
            cin >> choice;

            if (toupper(choice) == 'U')
            {
                undoMove();
                continue;
            }

            column = choice - '0';

            if (column < 1 || column > COLS || isColumnFull(column - 1))
            {
                cout << "Invalid move. Try again." << endl;
                continue;
            }

            column--;

            if (dropPiece(column))
            {
                if (isWinner())
                {
                    gameWon = true;
                    displayBoard();
                    cout << endl
                         << "Player " << (currentPlayer == player1.s ? player1.name : player2.name) << " (" << currentPlayer << ") wins!" << endl
                         << endl;
                }
                else if (isBoardFull())
                {
                    gameWon = true;
                    displayBoard();
                    cout << "It's a tie!" << endl;
                }
                else
                {
                    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                }
            }
            else
            {
                cout << "Invalid move. Try again." << endl;
            }

        } while (!gameWon);
    }
};

int main()
{
    ConnectFourBoard game;
    char loadChoice;
    do
    {
        cout << "Do you want to load a saved game (Y/N)? ";
        cin >> loadChoice;
        if (loadChoice == 'Y' || loadChoice == 'y')
        {
            game.printFileContents("2.txt");
            break;
        }
        else if (loadChoice == 'N' || loadChoice == 'n')
        {
            game.playGame();
            game.saveGameToFile("2.txt");
            break;
        }
        cout << "invalid input try again" << endl;
    } while (1);
    return 0;
} /*

 contents of 2.txt after one execution
                1
                2
 X              3
 X O            4
 X O            5
 X O            6
 1 2 3 4 5 6 7

 player 1:a (O)
 player 2:b (X)
 winner:b

 TARUN KUMAR DORA
 K SECTION
 PES1UG22CS648
 C++ PROJECT
 BRANCH-CSE
 BOARD GAME- CONNECT FOUR
 */