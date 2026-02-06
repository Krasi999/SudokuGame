#include "SudokuGUI.h"
#include <iostream>
#include <sstream>
#include <iomanip>

SudokuGame::SudokuGame()
    : window(sf::VideoMode(600, 800), "Sudoku"), state(GameState::MENU),
      difficulty(1), selectedRow(-1), selectedCol(-1), cellSelected(false),
      cellSize(60.f), offsetX(30.f), offsetY(50.f),
      hintsUsed(0), maxHints(5), checksUsed(0), maxChecks(5),
      highlightNumber(0), timerStopped(false) {  // Инициализиране на timerStopped

    // Зареждане на шрифт
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::wcerr << L"Не може да се зареди шрифт OpenSans-Regular.ttf\n";
    }

    // Начален език и локализация
    currentLanguage = Language::ENGLISH;
    currentText = &EN;
    boardLogic.setLocalization(currentText);

    // Инициализиране на checked cells
    resetCheckedCells();

    // Заглавие
    title.setFont(font);
    title.setString(currentText->title);
    title.setCharacterSize(70);
    title.setPosition(155, 70);
    title.setFillColor(sf::Color::Black);

    // Зареждане на изображението за езиков бутон
    if (!langTexture.loadFromFile("globe.png")) {
        std::wcerr << L"Не може да се зареди изображение globe.png\n";
    }
    langSprite.setTexture(langTexture);
    langSprite.setPosition(535, 20);

    // Трудности (Menu)
    sf::String names[4] = {currentText->easy, currentText->medium, currentText->hard, currentText->expert};
    for (int i = 0; i < 4; i++) {
        difficultyButtons[i].setSize(sf::Vector2f(200, 80));
        difficultyButtons[i].setFillColor(sf::Color(180,220,250));
        difficultyButtons[i].setPosition(200, 220 + i * 120);
        difficultyText[i].setFont(font);
        difficultyText[i].setString(names[i]);
        difficultyText[i].setCharacterSize(40);
        difficultyText[i].setPosition(220, 234 + i * 120);
        difficultyText[i].setFillColor(sf::Color::Black);
    }

    // Бутоните (Провери/Изтрий/Подсказка/Предай)
    controls.init(font, currentText);

    //Default стойности за многократна употреба при плазирането на Бутоните (Провери/Изтрий/Подсказка/Предай)
    float btnW = 130.f;
    float btnH = 45.f;
    float baseY = 675.f;
    float startX = 25.f;
    float gap = 10.f;

    // Check/Провери
    controls.checkCellButton.setSize(sf::Vector2f(btnW, btnH));
    controls.checkCellButton.setPosition(startX, baseY);
    controls.checkCellButton.setOutlineThickness(2.f);             // черна рамка
    controls.checkCellButton.setOutlineColor(sf::Color::Black);
    controls.checkCellText.setFont(font);
    controls.checkCellText.setCharacterSize(18);
    controls.checkCellText.setFillColor(sf::Color::Black);

    // Erase/Изтрий
    controls.eraseButton.setSize(sf::Vector2f(btnW, btnH));
    controls.eraseButton.setPosition(startX + (btnW + gap) * 1.0f, baseY);
    controls.eraseButton.setOutlineThickness(2.f);             // черна рамка
    controls.eraseButton.setOutlineColor(sf::Color::Black);
    controls.eraseText.setFont(font);
    controls.eraseText.setCharacterSize(18);
    controls.eraseText.setFillColor(sf::Color::Black);

    // Hint/Подсказка
    controls.hintButton.setSize(sf::Vector2f(btnW, btnH));
    controls.hintButton.setPosition(startX + (btnW + gap) * 2.0f, baseY);
    controls.hintButton.setOutlineThickness(2.f);             // черна рамка
    controls.hintButton.setOutlineColor(sf::Color::Black);
    controls.hintText.setFont(font);
    controls.hintText.setCharacterSize(18);
    controls.hintText.setFillColor(sf::Color::Black);

    // Submit/Предай
    controls.submitButton.setSize(sf::Vector2f(btnW, btnH));
    controls.submitButton.setPosition(startX + (btnW + gap) * 3.0f , baseY);
    controls.submitButton.setOutlineThickness(2.f);             // черна рамка
    controls.submitButton.setOutlineColor(sf::Color::Black);
    controls.submitText.setFont(font);
    controls.submitText.setCharacterSize(18);
    controls.submitText.setFillColor(sf::Color::Black);

    controls.updatePositions(); // Това позиционира текстовете на бутоните правилно - методът е описан в GameControls.cpp


    // Статус текст
    statusText.setFont(font);
    statusText.setCharacterSize(20);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(30, 740);
    statusText.setString(L"");

    // Бутоните за игра с мишката от 1..9
    float numbersY = offsetY + cellSize * 9 + 16.f;
    float startNumX = offsetX;
    float spacing = 61.f;
    for (int i = 1; i <= 9; i++) {
        numberButtons[i].setSize(sf::Vector2f(52, 52));
        numberButtons[i].setFillColor(sf::Color(230,230,230));
        numberButtons[i].setOutlineThickness(1.f);
        numberButtons[i].setOutlineColor(sf::Color::Black);
        numberButtons[i].setPosition(startNumX + (i-1) * spacing, numbersY);

        numberButtonText[i].setFont(font);
        numberButtonText[i].setString(std::to_string(i));
        numberButtonText[i].setCharacterSize(22);
        numberButtonText[i].setPosition(startNumX + (i-1) * spacing + 18, numbersY + 12);
        numberButtonText[i].setFillColor(sf::Color::Black);
    }

    // === Горен ред над дъската ===
    // BACK/Назад бутон с рамка(ляво)
    backButton.setSize(sf::Vector2f(88, 30));
    backButton.setFillColor(sf::Color::White);
    backButton.setOutlineThickness(2.f);             // черна рамка
    backButton.setOutlineColor(sf::Color::Black);
    backButton.setPosition(28, 12);

    backButtonText.setFont(font);
    backButtonText.setString(currentText->back);
    backButtonText.setCharacterSize(20);
    backButtonText.setPosition(backButton.getPosition().x + 6, backButton.getPosition().y + 2);
    backButtonText.setFillColor(sf::Color::Black);

    // Етикет за трудността (център)
    difficultyLabel.setFont(font);
    difficultyLabel.setCharacterSize(34);
    difficultyLabel.setFillColor(sf::Color::Blue);

    // Таймер (дясно) — показваме локализирания label + време в draw
    timerText.setFont(font);
    timerText.setCharacterSize(20);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(420, 25);

    // HINT/Подсказка counter text (малък)
    hintCounterText.setFont(font);
    hintCounterText.setCharacterSize(14);
    hintCounterText.setFillColor(sf::Color::Black);

    // CHECK/Провери counter text (малък)
    checkCounterText.setFont(font);
    checkCounterText.setCharacterSize(14);
    checkCounterText.setFillColor(sf::Color::Black);

    gameClock.restart();
}

void SudokuGame::resetCheckedCells() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cellChecked[i][j] = false;
        }
    }
}

void SudokuGame::updateDifficultyLabel() {
    sf::String text;
    switch(difficulty) {
        case 1: text = currentText->easy; break;
        case 2: text = currentText->medium; break;
        case 3: text = currentText->hard; break;
        case 4: text = currentText->expert; break;
        default: text = currentText->medium; break;
    }
    difficultyLabel.setString(text);

    sf::FloatRect bounds = difficultyLabel.getLocalBounds();
    difficultyLabel.setOrigin(bounds.left + bounds.width / 2.f, bounds.top);
    difficultyLabel.setPosition(window.getSize().x / 2.f, 10); // 10 пиксела отгоре
}

void SudokuGame::drawMenu() {
    window.clear(sf::Color(240,240,240));
    window.draw(title);
    window.draw(langSprite); // вместо Rectangle + текст
    for (int i = 0; i < 4; i++) {
        window.draw(difficultyButtons[i]);
        window.draw(difficultyText[i]);
    }
    window.display();
}

void SudokuGame::drawBoard() {
    window.clear(sf::Color(255,255,255));

    // === Горен ред ===
    window.draw(backButton);
    window.draw(backButtonText);

    updateDifficultyLabel();   // центриране на текста
    window.draw(difficultyLabel);

    // Таймер — ползваме локализиран етикет
    sf::Time elapsed = timerStopped ? finalTime : gameClock.getElapsedTime();
    int seconds = static_cast<int>(elapsed.asSeconds());
    int mins = seconds / 60;
    int secs = seconds % 60;

    // Билдването на таймера: пр. "TIME 04:23" или "ВРЕМЕ 04:23"
    std::wstringstream wss;
    wss << currentText->timeLabel << L" "
        << (mins < 10 ? L"0" : L"") << mins << L":" << (secs < 10 ? L"0" : L"") << secs;
    timerText.setString(wss.str());

    // Позициониране: timerText да е винаги отдясно на горния ред
    sf::FloatRect tb = timerText.getLocalBounds();
    float rightX = (float)window.getSize().x - 20.f;
    timerText.setOrigin(tb.left + tb.width, tb.top);
    timerText.setPosition(rightX - 10, 20);
    window.draw(timerText);

    // Рисуване на клетките
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            sf::RectangleShape &cell = cells[r][c];
            cell.setSize(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(offsetX + c * cellSize, offsetY + r * cellSize);
            cell.setOutlineThickness(1.f);
            cell.setOutlineColor(sf::Color::Black);

            int val = boardLogic.getValue(r, c);
            bool fixed = boardLogic.isCellFixed(r, c);

            // Highlight logic:
            if (r == selectedRow && c == selectedCol) {
                cell.setFillColor(sf::Color(120,180,255)); // selected stronger blue
            } else if (highlightNumber != 0 && val != 0 && val == highlightNumber) {
                cell.setFillColor(sf::Color(170,210,255)); // lighter highlight
            } else if (fixed) {
                cell.setFillColor(sf::Color(210,210,210));
            } else {
                cell.setFillColor(sf::Color::White);
            }

            window.draw(cell);

            if (val != 0) {
                sf::Text num;
                num.setFont(font);
                num.setString(std::to_string(val));
                num.setCharacterSize(28);
                num.setFillColor(fixed ? sf::Color::Black : sf::Color(10,50,120));
                num.setPosition(offsetX + c * cellSize + 20, offsetY + r * cellSize + 12);
                window.draw(num);
            }
        }
    }

    // Дебели линии за 3x3
    for (int i = 0; i <= 9; i++) {
        sf::RectangleShape lineH, lineV;
        lineH.setSize((i % 3 == 0 ? sf::Vector2f(cellSize*9+2,6) : sf::Vector2f(cellSize*9,1)));
        lineH.setPosition(offsetX-1, offsetY + i*cellSize - (i%3==0?2:0));
        lineH.setFillColor(sf::Color::Black);
        window.draw(lineH);

        lineV.setSize((i % 3 == 0 ? sf::Vector2f(6, cellSize*9+2) : sf::Vector2f(1, cellSize*9)));
        lineV.setPosition(offsetX + i*cellSize - (i%3==0?2:0), offsetY-1);
        lineV.setFillColor(sf::Color::Black);
        window.draw(lineV);
    }

    // Външна рамка
    sf::RectangleShape border(sf::Vector2f(cellSize*9+4, cellSize*9+4));
    border.setPosition(offsetX-2, offsetY-2);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Black);
    border.setOutlineThickness(3.f);
    window.draw(border);

    // Контроли (draw използва GameControls::draw която рисува фигури+текст)
    controls.draw(window);

    // Над бутона Hint показваме брояч used/max — променя се динамично от контролите
    {
        sf::Vector2f hintBtnPos = controls.hintButton.getPosition();
        float hintBtnW = controls.hintButton.getSize().x;
        float rightX = hintBtnPos.x + hintBtnW;
        std::wstringstream hc;
        hc << hintsUsed << L"/" << maxHints;
        hintCounterText.setString(hc.str());
        sf::FloatRect hcb = hintCounterText.getLocalBounds();
        hintCounterText.setOrigin(hcb.left + hcb.width, hcb.top);
        hintCounterText.setPosition(rightX - 6.f, hintBtnPos.y + 6.f);
        window.draw(hintCounterText);
    }

    // Над бутона Check показваме брояч used/max - променя се динамично от контролите
    {
        sf::Vector2f chkBtnPos = controls.checkCellButton.getPosition();
        float chkBtnW = controls.checkCellButton.getSize().x;
        float rightX = chkBtnPos.x + chkBtnW;
        std::wstringstream cc;
        cc << checksUsed << L"/" << maxChecks;
        checkCounterText.setString(cc.str());
        sf::FloatRect ccb = checkCounterText.getLocalBounds();
        checkCounterText.setOrigin(ccb.left + ccb.width, ccb.top);
        checkCounterText.setPosition(rightX - 6.f, chkBtnPos.y + 6.f);
        window.draw(checkCounterText);
    }

    // Бутони от 1 до 9
    for (int i = 1; i <= 9; i++) {
        window.draw(numberButtons[i]);
        window.draw(numberButtonText[i]);
    }

    // Статус
    window.draw(statusText);

    window.display();
}

void SudokuGame::handleMenuClick(sf::Vector2f mousePos) {
    // Проверка за езиков бутон
    if (langSprite.getGlobalBounds().contains(mousePos)) {
        if (currentLanguage == Language::BULGARIAN) {
            currentLanguage = Language::ENGLISH;
            currentText = &EN;
        } else {
            currentLanguage = Language::BULGARIAN;
            currentText = &BG;
        }

        title.setString(currentText->title);

        for (int i = 0; i < 4; i++){
            difficultyText[i].setString((i==0?currentText->easy:(i==1?currentText->medium:(i==2?currentText->hard:currentText->expert))));
        }

        controls.setLocalization(currentText);
        controls.updatePositions(); // Обновяване на позициите след смяна на езика

        backButtonText.setString(currentText->back);
        backButtonText.setPosition(backButton.getPosition().x + 12, backButton.getPosition().y + 6);
        statusText.setString(L"");

        boardLogic.setLocalization(currentText);

        return;
    }

    // избор на трудност
    for (int i = 0; i < 4; i++) {
        if (difficultyButtons[i].getGlobalBounds().contains(mousePos)) {
            difficulty = i+1;
            boardLogic.generateBoard(difficulty);
            state = GameState::PLAYING;
            selectedRow = selectedCol = -1;
            cellSelected = false;
            hintsUsed = 0; // нулираме броя подсказки при нова игра
            checksUsed = 0;
            highlightNumber = 0;
            statusText.setString(L"");
            timerStopped = false; // рестартираме таймера
            resetCheckedCells(); // нулираме проверените клетки
            gameClock.restart(); // рестартираме таймера
        }
    }
}

void SudokuGame::handleBoardClick(sf::Vector2f mousePos) {

    // BACK/Назад бутон
    if (backButton.getGlobalBounds().contains(mousePos)) {
        state = GameState::MENU;
        return;
    }

    //Submit/Предай (проверка на цялата дъска)
    if (controls.isSubmitPressed(mousePos)) {
        bool anyEmpty = false;
        for (int r = 0; r < 9 && !anyEmpty; r++)
            for (int c = 0; c < 9 && !anyEmpty; c++)
                if (boardLogic.getValue(r, c) == 0) anyEmpty = true;

        if (anyEmpty) {
            statusText.setString(currentText->notFilled);
        } else {
            if (boardLogic.checkCorrect()) {
                sf::Time elapsed = gameClock.getElapsedTime();
                finalTime = elapsed; // Запазваме финалното време
                timerStopped = true; // Спираме таймера

                int seconds = static_cast<int>(elapsed.asSeconds());
                int mins = seconds / 60;
                int secs = seconds % 60;
                std::wstringstream wss;
                wss << currentText->congratulations << L"  " << currentText->timeLabel << L" ";
                wss << (mins < 10 ? L"0" : L"") << mins << L":" << (secs < 10 ? L"0" : L"") << secs;
                statusText.setString(wss.str());

                state = GameState::FINISHED; // Променяме състоянието на FINISHED
            } else {
                statusText.setString(currentText->boardErrors);
            }
        }
        return;
    }

    //  С бутона Check/Провери проверяваме само една избрана от нас клетка
    if (controls.isCheckCellPressed(mousePos)) {
        if (!cellSelected) {
            statusText.setString(currentText->selectCell);
            return;
        }

        if (checksUsed >= maxChecks) {
            statusText.setString(currentText->noMoreChecks);
            return;
        }

        // Проверка дали клетката вече е била проверена
        if (cellChecked[selectedRow][selectedCol]) {
            statusText.setString(L"Това поле вече беше проверено!"); // или локализирано съобщение
            return;
        }

        int val = boardLogic.getValue(selectedRow, selectedCol);
        if (val == 0) {
            statusText.setString(currentText->emptyCell);
            return;
        }

        bool ok = boardLogic.checkCell(selectedRow, selectedCol);
        statusText.setString(ok ? currentText->cellCorrect : currentText->cellWrong);
        checksUsed++;

        // Маркираме клетката като проверена
        cellChecked[selectedRow][selectedCol] = true;
        return;
    }

    // ERASE/Изтрий
    if (controls.isErasePressed(mousePos) && cellSelected) {
        boardLogic.setCell(selectedRow, selectedCol, 0);
        // ако сме изтрили стойността на избраната клетка — премахваме highlight
        highlightNumber = 0;
        // При изтриване маркираме клетката като непроверена (ако е била проверена)
        cellChecked[selectedRow][selectedCol] = false;
        return;
    }

    // HINT/Подсказка -  избрана или първа празна, със лимит ===
    if (controls.isHintPressed(mousePos)) {
        if (hintsUsed >= maxHints) {
            statusText.setString(currentText->noHints);
            return;
        }

        bool added = false;
        if (cellSelected) {
            // ако вече има подсказка в избраната клетка — не харчим опит
            if (boardLogic.hasHintAt(selectedRow, selectedCol)) {
                statusText.setString(currentText->hintAlready);
                return;
            }

            added = boardLogic.giveHintForCell(selectedRow, selectedCol);
            if (added) {
                // обновяваме highlight към новата стойност на избраната клетка
                highlightNumber = boardLogic.getValue(selectedRow, selectedCol);
                // При добавяне на подсказка маркираме клетката като проверена
                cellChecked[selectedRow][selectedCol] = true;
            }
        } else {
            added = boardLogic.giveHint();
        }

        if (added) {
            hintsUsed++;
            statusText.setString(currentText->hintAdded);
        } else {
            statusText.setString(currentText->noHints);
        }
        return;
    }

    // избор на клетка (ако кликнеш върху някоя клетка)
    bool clickedOnCell = false;
    for (int r = 0; r < 9 && !clickedOnCell; r++) {
        for (int c = 0; c < 9; c++) {
            if (cells[r][c].getGlobalBounds().contains(mousePos)) {
                selectedRow = r;
                selectedCol = c;
                cellSelected = true;
                // когато избираме клетка — обновяваме highlightNumber според стойността в нея
                int val = boardLogic.getValue(r, c);
                highlightNumber = (val != 0) ? val : 0;

                std::wstringstream ss;
                ss << currentText->cellSelected << L" " << r+1 << L"," << c+1;
                statusText.setString(ss.str());
                clickedOnCell = true;
                break;
            }
        }
    }

    if (clickedOnCell) return;

    // ако кликът не е върху клетка, не е върху контролите и не е върху цифрите -> отмаркираме
    bool clickedOnNumber = false;
    for (int i = 1; i <= 9; i++)
        if (numberButtons[i].getGlobalBounds().contains(mousePos)) clickedOnNumber = true;

    if (!clickedOnNumber &&
        !controls.isCheckCellPressed(mousePos) &&
        !controls.isErasePressed(mousePos) &&
        !controls.isHintPressed(mousePos) &&
        !controls.isCheckCellPressed(mousePos) &&
        !backButton.getGlobalBounds().contains(mousePos) &&
        !langSprite.getGlobalBounds().contains(mousePos)) {
        selectedRow = -1;
        selectedCol = -1;
        cellSelected = false;
        highlightNumber = 0; // премахваме highlight при клик извън дъската
        statusText.setString(L"");
        return;
    }

    // избор на число (само ако има селекция)
    if (cellSelected) {
        for (int i = 1; i <= 9; i++) {
            if (numberButtons[i].getGlobalBounds().contains(mousePos)) {
                if (!boardLogic.isCellFixed(selectedRow, selectedCol)) {
                    boardLogic.setCell(selectedRow, selectedCol, i);
                    // обновяваме highlight според новата стойност
                    highlightNumber = i;
                    // При промяна на число маркираме клетката като непроверена
                    cellChecked[selectedRow][selectedCol] = false;
                } else {
                    statusText.setString(currentText->invalidMove);
                }
                return;
            }
        }
    }
}

void SudokuGame::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if (state == GameState::MENU) handleMenuClick(mousePos);
                else if (state == GameState::PLAYING || state == GameState::FINISHED) handleBoardClick(mousePos);
            }

            if ((state == GameState::PLAYING) && cellSelected && event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= '0' && event.text.unicode <= '9') {
                    int num = event.text.unicode - '0';
                    if (!boardLogic.isCellFixed(selectedRow, selectedCol)) {
                        boardLogic.setCell(selectedRow, selectedCol, num);
                        // При промяна на число маркираме клетката като непроверена
                        cellChecked[selectedRow][selectedCol] = false;
                    }
                    else
                        statusText.setString(currentText->invalidMove);

                    // Обновяваме highlight ако сме написали число от клавиатурата
                    if (!boardLogic.isCellFixed(selectedRow, selectedCol))
                        highlightNumber = num;
                }
            }
        }

        if (state == GameState::MENU) drawMenu();
        else if (state == GameState::PLAYING || state == GameState::FINISHED) drawBoard();
    }
}
