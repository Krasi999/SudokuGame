#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <SFML/Graphics.hpp>
#include "SudokuBoard.h"
#include "GameControls.h"

enum class GameState {
    MENU,
    PLAYING,
    FINISHED
};

class SudokuGame {
private:
    sf::RenderWindow window;
    GameState state;
    SudokuBoard boardLogic;
    int difficulty;

    Language currentLanguage;
    const Localization* currentText;

    sf::Font font;
    sf::Text title;
    sf::RectangleShape difficultyButtons[4];
    sf::Text difficultyText[4];

    // Бутонът за смяна на езика - при натискане да сменя динамично езика
    sf::RectangleShape langButton;

    // Texture / Sprite  за изображението за смяна на езика
    sf::Texture langTexture;
    sf::Sprite langSprite;

    // Дъската
    sf::RectangleShape cells[9][9];
    int selectedRow, selectedCol;
    bool cellSelected;

    // Бутони на числата(от 1 до 9)
    sf::RectangleShape numberButtons[10]; // Индексите от 1 до 9
    sf::Text numberButtonText[10];

    // Бутони (check/erase/hint/submit)
    GameControls controls;

    // Статус текст
    sf::Text statusText;

    // Константи нужни за оформлението
    float cellSize;
    float offsetX, offsetY;

    // Горен ред над дъската
    sf::RectangleShape backButton;
    sf::Text backButtonText;

    sf::Text difficultyLabel;
    sf::Text timerText;
    sf::Clock gameClock;
    bool timerStopped;  // Контрол за спрян таймер
    sf::Time finalTime; // Финално време след успешно предаване на Судокуто

    int hintsUsed;
    int maxHints;
    sf::Text hintCounterText; // Показва колко пъти е използван бутонът Hint/Подсказка -  малко над него разположено

    int checksUsed;
    int maxChecks;
    sf::Text checkCounterText; // Показва колко пъти е използван бутонът Check/Провери -  малко над него разположено

    int highlightNumber; // Когато > 0 — оцветява всички клетки с това число

    bool cellChecked[9][9]; // Проследява дали клетка е била проверена

    void drawMenu();
    void drawBoard();
    void handleMenuClick(sf::Vector2f mousePos);
    void handleBoardClick(sf::Vector2f mousePos);
    void resetCheckedCells(); // Нулиране на проверените клетки

public:
    SudokuGame();
    void run();
    void updateDifficultyLabel();
};

#endif // SUDOKUGUI_H
