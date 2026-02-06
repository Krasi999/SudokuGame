#include "GameControls.h"

void GameControls::init(sf::Font& font, const Localization* loc) {
    this->loc = loc;

    // Задаване на цветове на бутоните
    checkCellButton.setFillColor(sf::Color(200, 230, 200));  // зелен
    eraseButton.setFillColor(sf::Color(250, 180, 180));      // червен
    hintButton.setFillColor(sf::Color(180, 220, 250));       // син
    submitButton.setFillColor(sf::Color(180, 250, 180));     // зелен

    setLocalization(loc); // задаваме текстовете веднага
    updatePositions();
}

void GameControls::setLocalization(const Localization* loc) {
    this->loc = loc;
    if (!loc) return;

    submitText.setString(loc->check);           // Submit / Предай
    eraseText.setString(loc->erase);            // Erase / Изтрий
    hintText.setString(loc->hint);              // Hint / Подсказка
    checkCellText.setString(loc->checkCell);    // Check / Провери

    updatePositions();
}

void GameControls::updatePositions() {
    // Задаваме позициите на текстовете в нашите 4 бутона под Судоку дъската

    checkCellText.setPosition(30, 692);
    eraseText.setPosition(170, 692);
    hintText.setPosition(310, 692);
    submitText.setPosition(450, 692);
}

void GameControls::draw(sf::RenderWindow& window) {
    window.draw(submitButton);
    window.draw(checkCellButton);
    window.draw(eraseButton);
    window.draw(hintButton);

    window.draw(submitText);
    window.draw(checkCellText);
    window.draw(eraseText);
    window.draw(hintText);
}
