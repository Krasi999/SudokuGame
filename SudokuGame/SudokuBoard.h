#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Localization.h"
using namespace std;

class SudokuBoard {
private:
    int board[9][9];
    int solution[9][9];
    bool fixedCell[9][9];
    bool hintPlaced[9][9];
    const Localization* text;

    bool isSafe(int row, int col, int num);
    bool solveBoard(int b[9][9]);
    bool solveBoardMultiple(int b[9][9], int& solutionCount);
    void copyBoard(int src[9][9], int dest[9][9]);
    void removeCells(int difficulty);
    bool hasUniqueSolution();

public:
    SudokuBoard();
    void generateBoard(int difficulty);
    void printBoard();
    void setCell(int row, int col, int num);
    int getValue(int row, int col) const { return board[row][col]; }
    bool isCellFixed(int row, int col) const { return fixedCell[row][col]; }
    void setLocalization(const Localization* loc) { text = loc; }

    bool checkCorrect();
    bool checkCell(int r, int c);
    bool giveHintForCell(int r, int c);
    bool giveHint();
    bool isSolved();
    bool hasHintAt(int row, int col) const;
};

#endif // SUDOKUBOARD_H
