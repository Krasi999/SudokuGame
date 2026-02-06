#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "SudokuGUI.h"

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT); // Unicode конзола
    SudokuGame game;
    game.run();
    return 0;
}
