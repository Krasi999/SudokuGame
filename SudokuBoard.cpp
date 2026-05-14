#include "SudokuBoard.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

SudokuBoard::SudokuBoard() {
    srand((unsigned)time(0));
    text = &BG;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            board[i][j] = 0;
            solution[i][j] = 0;
            fixedCell[i][j] = false;
            hintPlaced[i][j] = false;
        }
}

bool SudokuBoard::isSafe(int row, int col, int num) {
    // Проверка за ред
    for (int x = 0; x < 9; x++)
        if (board[row][x] == num)
            return false;

    // Проверка за колона
    for (int x = 0; x < 9; x++)
        if (board[x][col] == num)
            return false;

    // Проверка за 3x3 квадрат
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i + startRow][j + startCol] == num)
                return false;

    return true;
}

bool SudokuBoard::solveBoard(int b[9][9]) {
    int row = -1, col = -1;
    bool empty = false;

    // Намираме първата празна клетка
    for (int i = 0; i < 9 && !empty; i++) {
        for (int j = 0; j < 9 && !empty; j++) {
            if (b[i][j] == 0) {
                row = i;
                col = j;
                empty = true;
            }
        }
    }

    if (!empty) return true;

    // Опитваме всички числа от 1 до 9 в случаен ред
    std::vector<int> numbers = {1,2,3,4,5,6,7,8,9};
    std::random_shuffle(numbers.begin(), numbers.end());

    for (int num : numbers) {
        // Проверяваме дали числото е безопасно
        bool safe = true;

        // Проверка за ред
        for (int x = 0; x < 9; x++)
            if (b[row][x] == num) {
                safe = false;
                break;
            }

        // Проверка за колона
        if (safe) {
            for (int x = 0; x < 9; x++)
                if (b[x][col] == num) {
                    safe = false;
                    break;
                }
        }

        // Проверка за 3x3 квадрат
        if (safe) {
            int startRow = row - row % 3;
            int startCol = col - col % 3;
            for (int i = 0; i < 3 && safe; i++) {
                for (int j = 0; j < 3 && safe; j++) {
                    if (b[i + startRow][j + startCol] == num) {
                        safe = false;
                    }
                }
            }
        }

        if (safe) {
            b[row][col] = num;
            if (solveBoard(b)) return true;
            b[row][col] = 0;
        }
    }
    return false;
}

bool SudokuBoard::solveBoardMultiple(int b[9][9], int& solutionCount) {
    int row = -1, col = -1;
    bool empty = false;

    // Намираме първата празна клетка
    for (int i = 0; i < 9 && !empty; i++) {
        for (int j = 0; j < 9 && !empty; j++) {
            if (b[i][j] == 0) {
                row = i;
                col = j;
                empty = true;
            }
        }
    }

    if (!empty) {
        solutionCount++;
        return (solutionCount > 1); // Спираме ако намерим повече от 1 решение
    }

    // Опитваме всички числа от 1 до 9
    for (int num = 1; num <= 9 && solutionCount <= 1; num++) {
        // Проверяваме дали числото е безопасно
        bool safe = true;

        // Проверка за ред
        for (int x = 0; x < 9; x++)
            if (b[row][x] == num) {
                safe = false;
                break;
            }

        // Проверка за колона
        if (safe) {
            for (int x = 0; x < 9; x++)
                if (b[x][col] == num) {
                    safe = false;
                    break;
                }
        }

        // Проверка за 3x3 квадрат
        if (safe) {
            int startRow = row - row % 3;
            int startCol = col - col % 3;
            for (int i = 0; i < 3 && safe; i++) {
                for (int j = 0; j < 3 && safe; j++) {
                    if (b[i + startRow][j + startCol] == num) {
                        safe = false;
                    }
                }
            }
        }

        if (safe) {
            b[row][col] = num;
            solveBoardMultiple(b, solutionCount);
            b[row][col] = 0;

            if (solutionCount > 1) return true;
        }
    }

    return false;
}

void SudokuBoard::copyBoard(int src[9][9], int dest[9][9]) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            dest[i][j] = src[i][j];
}

bool SudokuBoard::hasUniqueSolution() {
    int tempBoard[9][9];
    copyBoard(board, tempBoard);

    int solutionCount = 0;
    solveBoardMultiple(tempBoard, solutionCount);

    return solutionCount == 1;
}

void SudokuBoard::removeCells(int difficulty) {
    int cellsToRemove;
    switch (difficulty) {
        case 1: cellsToRemove = 33; break;  // Лесно
        case 2: cellsToRemove = 39; break;  // Средно
        case 3: cellsToRemove = 45; break;  // Трудно
        case 4: cellsToRemove = 51; break;  // Експерт
        default: cellsToRemove = 39;
    }

    // Минимален брой числа във всяка 3x3 кутия
    int minPerBox;
    switch (difficulty) {
        case 1: minPerBox = 4; break;
        case 2: minPerBox = 3; break;
        case 3: minPerBox = 2; break;
        case 4: minPerBox = 2; break;
        default: minPerBox = 3;
    }

    // Създаваме списък с всички позиции
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            positions.push_back(std::make_pair(i, j));
        }
    }

    // Разбъркваме позициите
    std::random_shuffle(positions.begin(), positions.end());

    int removed = 0;
    int attempts = 0;
    const int MAX_ATTEMPTS = 500; // Защита от безкраен цикъл

    // Премахваме клетки една по една, проверявайки за уникално решение
    while (removed < cellsToRemove && attempts < MAX_ATTEMPTS) {
        bool removedThisRound = false;

        for (auto& pos : positions) {
            if (removed >= cellsToRemove) break;

            int i = pos.first;
            int j = pos.second;

            if (board[i][j] != 0) {
                // Проверяваме дали можем да премахнем тази клетка
                int boxRow = i / 3;
                int boxCol = j / 3;
                int numbersInBox = 0;

                // Преброяваме числата в кутията
                for (int bi = 0; bi < 3; bi++) {
                    for (int bj = 0; bj < 3; bj++) {
                        int checkRow = boxRow * 3 + bi;
                        int checkCol = boxCol * 3 + bj;
                        if (board[checkRow][checkCol] != 0) {
                            numbersInBox++;
                        }
                    }
                }

                // Ако можем да премахнем клетката без да нарушим минималния брой
                if (numbersInBox > minPerBox) {
                    int backup = board[i][j];
                    board[i][j] = 0;

                    // Проверяваме дали все още има уникално решение
                    if (hasUniqueSolution()) {
                        removed++;
                        removedThisRound = true;
                    } else {
                        // Връщаме стойността ако няма уникално решение
                        board[i][j] = backup;
                    }
                }
            }
        }

        attempts++;
        if (!removedThisRound) {
            // Ако не сме премахнали нито една клетка в този кръг, спираме
            break;
        }
    }

    std::cout << "Removed " << removed << " cells after " << attempts << " attempts" << std::endl;
}

void SudokuBoard::generateBoard(int difficulty) {
    // Инициализиране на дъската
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            board[i][j] = 0;
            solution[i][j] = 0;
            fixedCell[i][j] = false;
            hintPlaced[i][j] = false;
        }

    // Стъпка 1: Попълване на диагоналните 3x3 квадрати
    for (int box = 0; box < 3; box++) {
        std::vector<int> nums = {1,2,3,4,5,6,7,8,9};
        std::random_shuffle(nums.begin(), nums.end());

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int row = box * 3 + i;
                int col = box * 3 + j;
                board[row][col] = nums[i * 3 + j];
            }
        }
    }

    // Стъпка 2: Решаване на пълната дъска
    solveBoard(board);

    // Стъпка 3: Запазване на решението
    copyBoard(board, solution);

    // Стъпка 4: Премахване на клетки с проверка за уникално решение
    removeCells(difficulty);

    // Стъпка 5: Маркиране на фиксираните клетки
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            fixedCell[i][j] = (board[i][j] != 0);

    // Преброяваме и показваме информация
    int emptyCells = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                emptyCells++;
            }
        }
    }

    std::cout << "Generated Sudoku (Difficulty: " << difficulty << "):" << std::endl;
    std::cout << "Empty cells: " << emptyCells << std::endl;

    // Проверка за уникално решение
    std::cout << "Has unique solution: " << (hasUniqueSolution() ? "YES" : "NO") << std::endl;

    printBoard();

    // Допълнителна информация за разпределението
    std::cout << "Distribution per 3x3 box:" << std::endl;
    for (int boxRow = 0; boxRow < 3; boxRow++) {
        for (int boxCol = 0; boxCol < 3; boxCol++) {
            int count = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (board[boxRow * 3 + i][boxCol * 3 + j] != 0) {
                        count++;
                    }
                }
            }
            std::cout << "Box (" << boxRow << "," << boxCol << "): " << count << " numbers" << std::endl;
        }
    }
    std::cout << std::endl;
}

// Останалите функции остават същите...
void SudokuBoard::setCell(int row, int col, int num) {
    if (row < 0 || row > 8 || col < 0 || col > 8) return;
    if (fixedCell[row][col]) return;
    board[row][col] = num;
}

bool SudokuBoard::checkCorrect() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] != 0 && board[i][j] != solution[i][j])
                return false;
    return true;
}

bool SudokuBoard::checkCell(int r, int c) {
    if (board[r][c] == 0) return false;
    return (board[r][c] == solution[r][c]);
}

bool SudokuBoard::giveHintForCell(int r, int c) {
    if (fixedCell[r][c]) return false;
    if (hintPlaced[r][c]) return false;
    board[r][c] = solution[r][c];
    hintPlaced[r][c] = true;
    return true;
}

bool SudokuBoard::giveHint() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == 0 && !fixedCell[i][j] && !hintPlaced[i][j]) {
                board[i][j] = solution[i][j];
                hintPlaced[i][j] = true;
                return true;
            }
    return false;
}

bool SudokuBoard::isSolved() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == 0 || board[i][j] != solution[i][j])
                return false;
    return true;
}

bool SudokuBoard::hasHintAt(int row, int col) const {
    if (row < 0 || row > 8 || col < 0 || col > 8) return false;
    return hintPlaced[row][col];
}

void SudokuBoard::printBoard() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << (board[i][j] == 0 ? "." : std::to_string(board[i][j])) << " ";
            if ((j + 1) % 3 == 0 && j < 8) std::cout << "| ";
        }
        std::cout << std::endl;
        if ((i + 1) % 3 == 0 && i < 8) std::cout << "------+-------+------" << std::endl;
    }
    std::cout << std::endl;
}
