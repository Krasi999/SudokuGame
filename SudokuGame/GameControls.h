#pragma once
#include <SFML/Graphics.hpp>
#include "Localization.h"

class GameControls {
private:
    const Localization* loc = nullptr;

public:
    // Submit/Предай
    sf::RectangleShape submitButton;
    sf::Text submitText;

    // Check/Провери
    sf::RectangleShape checkCellButton;
    sf::Text checkCellText;

    // Erase/Изтрий
    sf::RectangleShape eraseButton;
    sf::Text eraseText;

    // Hint/Подсказка
    sf::RectangleShape hintButton;
    sf::Text hintText;

    void init(sf::Font& font, const Localization* loc);
    void setLocalization(const Localization* loc);

    bool isSubmitPressed(sf::Vector2f pos) const { return submitButton.getGlobalBounds().contains(pos); }
    bool isCheckCellPressed(sf::Vector2f pos) const { return checkCellButton.getGlobalBounds().contains(pos); }
    bool isErasePressed(sf::Vector2f pos) const { return eraseButton.getGlobalBounds().contains(pos); }
    bool isHintPressed(sf::Vector2f pos) const { return hintButton.getGlobalBounds().contains(pos); }

    void updatePositions(); // Само за разположение на текстовете
    void draw(sf::RenderWindow& window);
};
