#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>

enum class Language {
    BULGARIAN,
    ENGLISH
};

struct Localization {
    // Меню / Заглавия
    std::wstring title;
    std::wstring easy;
    std::wstring medium;
    std::wstring hard;
    std::wstring expert;

    // Контроли
    std::wstring check;        // Submit / Предай
    std::wstring erase;        // Erase / Изтрий
    std::wstring hint;         // Hint / Подсказка
    std::wstring checkCell;    // Check / Провери

    // Горен ред
    std::wstring back;
    std::wstring timeLabel;

    // Съобщения
    std::wstring invalidMove;
    std::wstring congratulations;
    std::wstring boardErrors;
    std::wstring cellCorrect;
    std::wstring cellWrong;
    std::wstring selectCell;
    std::wstring hintAdded;
    std::wstring noHints;
    std::wstring cellSelected;
    std::wstring emptyCell;
    std::wstring notFilled;

    std::wstring hintAlready;
    std::wstring noMoreChecks;

    // EN/BG Бутон(Изображение)
    std::wstring enbg;
};

const Localization BG = {
    L"СУДОКУ",
    L"ЛЕСНО", L"СРЕДНО", L"ТРУДНО", L"ЕКСПЕРТ",
    L"ПРЕДАЙ", L"ИЗТРИЙ", L"ПОДСКАЗКА", L"ПРОВЕРИ",
    L"НАЗАД", L"ВРЕМЕ",
    L"Невалиден ход!",
    L"Поздравления! Решихте Судоку!",
    L"Имаш допуснати грешки!",
    L"Това число е на ПРАВИЛНОТО място!",
    L"Това число е на ГРЕШНОТО място!",
    L"Не е избрана клетка!",
    L"Подсказка добавена!",
    L"Няма повече подсказки!",
    L"Избрана клетка:",
    L"Клетката е празна!",
    L"Все още има непопълнени полета!",
    L"В това поле вече беше добавена подсказка!",
    L"Нямаш право на повече проверки!",
    L"EN / BG"
};

const Localization EN = {
    L"SUDOKU",
    L"EASY", L"MEDIUM", L"HARD", L"EXPERT",
    L"SUBMIT", L"ERASE", L"HINT", L"CHECK",
    L"BACK", L"TIME",
    L"Invalid move!",
    L"Congratulations! You solved the Sudoku!",
    L"There are errors on the board!",
    L"This number IS in the right place!",
    L"This number IS NOT in the right place!",
    L"No cell selected!",
    L"Hint added!",
    L"No more hints!",
    L"Selected cell:",
    L"The cell is empty!",
    L"There are still empty cells!",
    L"This cell already has a hint!",
    L"No more checks left!",
    L"EN / BG"
};

#endif // LOCALIZATION_H
