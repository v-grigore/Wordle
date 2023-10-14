#include "UI.h"

#include <chrono>
#include <thread>
#include <array>
#include <algorithm>
#include <cmath>

#include "logic.h"

namespace UI {
    void start(const bool winOS) {
        initscr();
        curs_set(0);
        noecho();
        start_color();
        keypad(stdscr , TRUE);
        init_color(COLOR_GREEN , static_cast<short>(RGB * 83) , static_cast<short>(RGB * 141) ,
                   static_cast<short>(RGB * 78));
        init_color(COLOR_YELLOW , static_cast<short>(RGB * 181) , static_cast<short>(RGB * 159) ,
                   static_cast<short>(RGB * 59));
        init_color(COLOR_GRAY , static_cast<short>(RGB * 58) , static_cast<short>(RGB * 58) ,
                   static_cast<short>(RGB * 60));
        init_color(COLOR_WHITE , 1000 , 1000 , 1000);
        init_pair(1 , COLOR_RED , COLOR_BLACK);
        init_pair(2 , COLOR_GREEN , COLOR_BLACK);
        init_pair(10 , COLOR_WHITE , COLOR_GRAY);
        init_pair(11 , COLOR_BLACK , COLOR_WHITE);
        init_pair(12 , COLOR_WHITE , COLOR_YELLOW);
        init_pair(13 , COLOR_BLACK , COLOR_WHITE);
        init_pair(14 , COLOR_WHITE , COLOR_GREEN);
        init_pair(15 , COLOR_BLACK , COLOR_WHITE);
        init_pair(16 , COLOR_WHITE , COLOR_BLACK);
        init_pair(17 , COLOR_BLACK , COLOR_WHITE);
        init_pair(18 , COLOR_WHITE , COLOR_RED);
        init_pair(19 , COLOR_BLACK , COLOR_WHITE);

        if (winOS) {
            attron(COLOR_PAIR(1));
            attron(A_BOLD);
            mvprintw(LINES / 2 - 1 , COLS / 2 - 10 , "Windows OS detected.\n");
            mvprintw(LINES / 2 , COLS / 2 - 25 , "Resizing terminal window might break the interface.");
            refresh();
            getch();
            clear();
            attroff(COLOR_PAIR(1));
            attroff(A_BOLD);
        }
    }

    // Menu
    Menu::Menu() {
        wButtons = newwin(7 , 14 , LINES/2 - 4 , COLS/2 - 7);
        keyPressed = 0;
        buttonSelected = 0;
        tButtons = {"   > Play <   " ,
                    "  Statistics  " ,
                    "   Settings   " ,
                    "     Exit     "};
    }

    Menu::~Menu() {
        wclear(wButtons);
        wrefresh(wButtons);
        delwin(wButtons);
        refresh();
    }

    void Menu::changeButton(const int move) {
        switch (buttonSelected + move * 10) {
            case 10:
                tButtons[buttonSelected][3] = ' '; tButtons[buttonSelected][10] = ' ';
                tButtons[buttonSelected + 1][0] = '>'; tButtons[buttonSelected + 1][13] = '<';
                buttonSelected++;
                break;
            case 11:
                tButtons[buttonSelected][0] = ' '; tButtons[buttonSelected][13] = ' ';
                tButtons[buttonSelected + 1][1] = '>'; tButtons[buttonSelected + 1][12] = '<';
                buttonSelected++;
                break;
            case 12:
                tButtons[buttonSelected][1] = ' '; tButtons[buttonSelected][12] = ' ';
                tButtons[buttonSelected + 1][3] = '>'; tButtons[buttonSelected + 1][10] = '<';
                buttonSelected++;
                break;
            case 21:
                tButtons[buttonSelected][0] = ' '; tButtons[buttonSelected][13] = ' ';
                tButtons[buttonSelected - 1][3] = '>'; tButtons[buttonSelected - 1][10] = '<';
                buttonSelected--;
                break;
            case 22:
                tButtons[buttonSelected][1] = ' '; tButtons[buttonSelected][12] = ' ';
                tButtons[buttonSelected - 1][0] = '>'; tButtons[buttonSelected - 1][13] = '<';
                buttonSelected--;
                break;
            case 23:
                tButtons[buttonSelected][3] = ' '; tButtons[buttonSelected][10] = ' ';
                tButtons[buttonSelected - 1][1] = '>'; tButtons[buttonSelected - 1][12] = '<';
                buttonSelected--;
                break;
        }
    }

    void Menu::update() {
        werase(wButtons);
        for (const auto& tButton: tButtons)
            wprintw(wButtons, "%s\n", tButton.data());
        wrefresh(wButtons);
    }

    void Menu::loop() {
        refresh();
        update();

        while ((keyPressed = getch()) != '\n') {
            switch (keyPressed) {
                case KEY_DOWN:
                    changeButton(1);
                    update();
                    break;
                case KEY_UP:
                    changeButton(2);
                    update();
                    break;
                case KEY_RESIZE:
                    endwin();
                    clear();
                    refresh();
                    curs_set(1);
                    curs_set(0);
                    mvwin(wButtons , LINES/2 - 4 , COLS/2 - 7);
                    update();
                    break;
            }
        }
    }

    // Stats
    StatsMenu::StatsMenu() {
        wStats[0] = newwin(23 , 59 , LINES / 2 - 12 , COLS / 2 - 60);
        wStats[1] = newwin(23 , 59 , LINES / 2 - 12 , COLS / 2);
        backButton = newwin(1 , 8 , LINES / 5 , COLS / 5);
    }

    StatsMenu::~StatsMenu() {
        werase(wStats[0]);
        werase(wStats[1]);
        werase(backButton);
        wrefresh(wStats[0]);
        wrefresh(wStats[1]);
        wrefresh(backButton);
        delwin(wStats[0]);
        delwin(wStats[1]);
        delwin(backButton);
        refresh();
    }

    void StatsMenu::printStats(const Logic::Stats &stats , const int index) {
        int played , won , curStk , maxStk , guessMax = 1 , wonPc , guessPc;
        std::array<int , 6> guess{};
        if (!index) {
            played = stats.playedA; won = stats.wonA; curStk = stats.curStkA; maxStk = stats.maxStkA;
            for (int i = 0; i < 6; i++)
                guess[i] = stats.guessA[i];
        }
        else {
            played = stats.playedB; won = stats.wonB; curStk = stats.curStkB; maxStk = stats.maxStkB;
            for (int i = 0; i < 6; i++)
                guess[i] = stats.guessB[i];
        }
        if (!played)
            wonPc = 0;
        else
            wonPc = static_cast<int>(1. * won / played * 100);
        for (int i = 0; i < 6; i++)
            if (guess[i] > guessMax)
                guessMax = guess[i];

        wprintw(wStats[index] , "***********************************************************");
        wprintw(wStats[index] , "*   Solver ");
        if (index) {
            wattron(wStats[index], COLOR_PAIR(greenText));
            wprintw(wStats[index] , "ON ");
        }
        else {
            wattron(wStats[index], COLOR_PAIR(redText));
            wprintw(wStats[index] , "OFF");
        }
        wattron(wStats[index] , COLOR_PAIR(black));
        wprintw(wStats[index] , "             Stats                          *");
        wprintw(wStats[index] , "*                                                         *");
        wprintw(wStats[index] , "*   Played %-7d               Current streak %-7d   *" , played , curStk);
        wprintw(wStats[index] , "*   Win %3d%%                     Max streak %-7d       *" , wonPc , maxStk);
        wprintw(wStats[index] , "*                                                         *");
        wprintw(wStats[index] , "*                   Guess  Distribution                   *");
        wprintw(wStats[index] , "*                                                         *");

        for (int i = 0; i < 6; i++) {
            wprintw(wStats[index] , "*   %d " , i + 1);
            wattron(wStats[index] , COLOR_PAIR(green));
            guessPc = static_cast<int>(1. * guess[i] / guessMax * 48) + 1;
            int numCif = static_cast<int>(log10(guess[i]));
            wprintw(wStats[index] , "%*d" , guessPc , guess[i]);
            wattron(wStats[index] , COLOR_PAIR(black));
            wprintw(wStats[index] , "%*c  *" , 50 - (guessPc > numCif?guessPc:numCif + 1) , ' ');
            wprintw(wStats[index] , "*                                                         *");
        }

        wprintw(wStats[index] , "*                                                         *");
        wprintw(wStats[index] , "*                                                         *");
        wprintw(wStats[index] , "***********************************************************");
        box(wStats[index] , 0 , 0);
        wrefresh(wStats[index]);
    }

    void StatsMenu::loop(const Logic::Stats &stats) {
        refresh();
        printStats(stats , 0);
        printStats(stats , 1);
        wprintw(backButton , "> Back <");
        wrefresh(backButton);
        for (int keyPressed = getch(); keyPressed != '\n'; keyPressed = getch());
    }

    // Settings
    SettingsMenu::SettingsMenu(const Logic::Settings &settings) {
        wButtons = newwin(9 , 40 , LINES / 2 - 5 , COLS / 2 - 12);
        backButton = newwin(1 , 8 , LINES / 5 , COLS / 5);
        wConfirm = newwin(6 , 45 , LINES / 2 - 4 , COLS / 2 - 23);
        tButtons[0] = {"> Solver" ,
                       "  Solver version" ,
                       "  Answer choice" ,
                       "  Autoplay" ,
                       "  Clear Stats"};
        if (settings.solverON)
            tButtons[1][0] = "ON";
        else
            tButtons[1][0] = "OFF";
        if (settings.updatedVer)
            tButtons[1][1] = "2.0";
        else
            tButtons[1][1] = "1.0";
        if (settings.randomAnswer)
            tButtons[1][2] = "random";
        else
            tButtons[1][2] = "based on user input";
        if (settings.nrOfGames)
            tButtons[1][3] = std::to_string(settings.nrOfGames);
        else
            tButtons[1][3] = "OFF";
        tButtons[1][4] = " ";
        textBack = "  Back  ";
        tConfirm = {"*********************************************" ,
                    "* Are you sure you want to clear ALL stats? *" ,
                    "*                                           *" ,
                    "*         yes                  > no <       *" ,
                    "*                                           *" ,
                    "*********************************************"};
    }

    SettingsMenu::~SettingsMenu() {
        werase(wButtons);
        werase(wConfirm);
        werase(backButton);
        wrefresh(wButtons);
        wrefresh(wConfirm);
        wrefresh(backButton);
        delwin(wButtons);
        delwin(wConfirm);
        delwin(backButton);
        refresh();
    }

    void SettingsMenu::update(const Logic::Settings& settings) {
        werase(backButton);
        wprintw(backButton , "%s" , textBack.data());
        wrefresh(backButton);
        werase(wButtons);

        wprintw(wButtons , "%s %s\n\n" , tButtons[0][0].data() , tButtons[1][0].data());
        if (!settings.solverON)
            wattron(wButtons , COLOR_PAIR(redText));
        wprintw(wButtons , "%s %s\n\n" , tButtons[0][1].data() , tButtons[1][1].data());
        wprintw(wButtons , "%s %s\n\n" , tButtons[0][2].data() , tButtons[1][2].data());
        if (!settings.randomAnswer)
            wattron(wButtons , COLOR_PAIR(redText));
        wprintw(wButtons , "%s %s\n\n" , tButtons[0][3].data() , tButtons[1][3].data());
        wattron(wButtons , COLOR_PAIR(black));
        wprintw(wButtons , "%s %s\n\n" , tButtons[0][4].data() , tButtons[1][4].data());
        wrefresh(wButtons);
    }

    void SettingsMenu::printConfirm() {
        werase(wConfirm);
        for (const auto& confirmStr: tConfirm)
            wprintw(wConfirm, "%s", confirmStr.data());
        box(wConfirm , 0 , 0);
        wrefresh(wConfirm);
    }

    void SettingsMenu::changeSetting(Logic::Settings &settings , const bool leftKey) {
        switch (buttonSelected) {
            case 0:
                if (settings.solverON) {
                    settings.solverON = false;
                    tButtons[1][0] = "OFF";
                }
                else {
                    settings.solverON = true;
                    tButtons[1][0] = "ON";
                }
                update(settings);
                break;
            case 1:
                if (settings.updatedVer) {
                    settings.updatedVer = false;
                    tButtons[1][1] = "1.0";
                }
                else {
                    settings.updatedVer = true;
                    tButtons[1][1] = "2.0";
                }
                update(settings);
                break;
            case 2:
                if (settings.randomAnswer) {
                    settings.randomAnswer = false;
                    tButtons[1][2] = "based on user input";
                }
                else {
                    settings.randomAnswer = true;
                    tButtons[1][2] = "random";
                }
                update(settings);
                break;
            case 3:
                if (leftKey) {
                    settings.nrOfGames /= 10;
                }
                else {
                    if (!settings.nrOfGames)
                        settings.nrOfGames = 1;
                    else if (settings.nrOfGames < 1000000)
                        settings.nrOfGames *= 10;
                }
                if (settings.nrOfGames)
                    tButtons[1][3] = std::to_string(settings.nrOfGames);
                else
                    tButtons[1][3] = "OFF";
                update(settings);
                break;
        }
    }

    void SettingsMenu::delStats(Logic::Stats &stats , const Logic::Settings& settings) {
        bool confirmStatus = false;
        printConfirm();
        while ((keyPressed = getch()) != '\n') {
            if (keyPressed == KEY_LEFT && !confirmStatus) {
                confirmStatus = true;
                tConfirm[3][8] = '>';
                tConfirm[3][14] = '<';
                tConfirm[3][31] = ' ';
                tConfirm[3][36] = ' ';
                printConfirm();
            }
            if (keyPressed == KEY_RIGHT && confirmStatus) {
                confirmStatus = false;
                tConfirm[3][8] = ' ';
                tConfirm[3][14] = ' ';
                tConfirm[3][31] = '>';
                tConfirm[3][36] = '<';
                printConfirm();
            }
        }
        werase(wConfirm);
        wrefresh(wConfirm);
        update(settings);
        if (confirmStatus) {
            tConfirm[3][8] = ' ';
            tConfirm[3][14] = ' ';
            tConfirm[3][31] = '>';
            tConfirm[3][36] = '<';
            stats.playedA = 0; stats.playedB = 0;
            stats.wonA = 0; stats.wonB = 0;
            stats.curStkA = 0; stats.curStkB = 0;
            stats.maxStkA = 0; stats.maxStkB = 0;
            for (int i = 0; i < 6; i++) {
                stats.guessA[i] = 0;
                stats.guessB[i] = 0;
            }
            stats.save();
        }
    }

    void SettingsMenu::loop(Logic::Stats &stats, Logic::Settings &settings) {
        refresh();
        update(settings);

        while ((keyPressed = getch()) != '\n' || buttonSelected != -1) {
            if (keyPressed == KEY_DOWN || keyPressed == KEY_UP) {
                if (buttonSelected > -1)
                    tButtons[0][buttonSelected][0] = ' ';
                else {
                    textBack[0] = ' ';
                    textBack[7] = ' ';
                }
            }
            switch (keyPressed) {
                case KEY_DOWN:
                    if (buttonSelected < 4) {
                        buttonSelected++;
                        tButtons[0][buttonSelected][0] = '>';
                        update(settings);
                    }
                    break;
                case KEY_UP:
                    if (buttonSelected > -1) {
                        buttonSelected--;
                        if (buttonSelected > -1)
                            tButtons[0][buttonSelected][0] = '>';
                        else {
                            textBack[0] = '>';
                            textBack[7] = '<';
                        }
                        update(settings);
                    }
                    break;
                case KEY_LEFT:
                    changeSetting(settings , true);
                    break;
                case KEY_RIGHT:
                    changeSetting(settings , false);
                    break;
                case '\n':
                    if (buttonSelected == 4)
                        delStats(stats , settings);
                    break;
            }
        }

        settings.save();
    }

    // Game UI
    GameUI::GameUI() {
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 5; j++)
                boxes[i][j] = newwin(9 , 19 , i * 9 , COLS / 2 + j * 19 - 48);

        endScreen = newwin(23 , 59 , 10 , COLS / 2 - 30);
        ansBox = newwin(1 , 5 , 8 , COLS / 2 - 3);
    }

    GameUI::~GameUI() {
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 5; j++) {
                werase(boxes[i][j]);
                wrefresh(boxes[i][j]);
                delwin(boxes[i][j]);
            }
        delwin(endScreen);
        delwin(ansBox);
        refresh();
    }

    void GameUI::initBoxes() {
        refresh();
        for (int i = 0; i < 6; i++)
            for (int j = 0; j < 5; j++) {
                box(boxes[i][j], 0, 0);
                wrefresh(boxes[i][j]);
            }
        refresh();
    }

    void GameUI::printBox(const int box_I , const int box_J , const int boxColor , const char letter) {
        werase(boxes[box_I][box_J]);

        if (letter != ' ')
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 19; j++) {
                    wattron(boxes[box_I][box_J], COLOR_PAIR(boxColor + letters.key.at(letter)[i][j]));
                    wprintw(boxes[box_I][box_J], " ");
                }

        if (boxColor == black) {
            box(boxes[box_I][box_J], 0, 0);
        }

        wrefresh(boxes[box_I][box_J]);
    }

    void GameUI::updateLetters(const std::string& guess, const int guessNr , const int mode) {
        if (mode) {
            int i = static_cast<int>(guess.length()) - 1;
            printBox(guessNr , i , black , guess[i]);
        }
        else {
            int i = static_cast<int>(guess.length());
            printBox(guessNr , i , black , ' ');
        }
    }

    void GameUI::checkLetters(std::string guess , std::string answer , const int guessNr ,
                              const std::array<std::string , guessLen>& guesses , bool& validGuess) {
        std::string tempAnswer = answer;
        for (int i = 0; i < guessLen; i++) {
            if (guess == guesses[i]) {
                validGuess = true;
                break;
            }
        }
        if (validGuess) {
            for (int i = 0; i < 5; i++) {
                if (guess[i] == answer[i]) {
                    printBox(guessNr, i, green, guess[i]);
                    guess[i] = '0';
                    tempAnswer[i] = '0';
                }
            }
            for (int i = 0; i < 5; i++) {
                int boxColor = gray;
                if (guess[i] == '0')
                    continue;
                for (int j = 0; j < 5; j++) {
                    if (guess[i] == tempAnswer[j]) {
                        tempAnswer[j] = '0';
                        boxColor = yellow;
                        break;
                    }
                }
                printBox(guessNr, i, boxColor, guess[i]);
            }
        }
        else {
            for (int i = 0; i < 5; i++) {
                printBox(guessNr, i, red, guess[i]);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            for (int i = 0; i < 5; i++) {
                printBox(guessNr, i, black, guess[i]);
            }
        }
    }

    void GameUI::showEndScreen(const int solverStatus, const Logic::Stats &stats , const int guessNr) {
        int played , won , curStk , maxStk , guessMax = 1;
        std::array<int , 6> guess{};
        if (!solverStatus) {
            played = stats.playedA; won = stats.wonA; curStk = stats.curStkA; maxStk = stats.maxStkA;
            for (int i = 0; i < 6; i++)
                guess[i] = stats.guessA[i];
        }
        else {
            played = stats.playedB; won = stats.wonB; curStk = stats.curStkB; maxStk = stats.maxStkB;
            for (int i = 0; i < 6; i++)
                guess[i] = stats.guessB[i];
        }
        if (!played)
            played = 1;
        int wonPc = static_cast<int>(1. * won / played * 100) , guessPc;
        for (int i = 0; i < 6; i++)
            if (guess[i] > guessMax)
                guessMax = guess[i];

        wprintw(endScreen , "***********************************************************");
        wprintw(endScreen , "*   Solver ");
        if (solverStatus) {
            wattron(endScreen, COLOR_PAIR(greenText));
            wprintw(endScreen , "ON ");
        }
        else {
            wattron(endScreen, COLOR_PAIR(redText));
            wprintw(endScreen , "OFF");
        }
        wattron(endScreen , COLOR_PAIR(black));
        wprintw(endScreen , "             Stats                          *");
        wprintw(endScreen , "*                                                         *");
        wprintw(endScreen , "*   Played %-7d               Current streak %-7d   *" , played , curStk);
        wprintw(endScreen , "*   Win %3d%%                     Max streak %-7d       *" , wonPc , maxStk);
        wprintw(endScreen , "*                                                         *");
        wprintw(endScreen , "*                   Guess  Distribution                   *");
        wprintw(endScreen , "*                                                         *");

        for (int i = 0; i < 6; i++) {
            wprintw(endScreen , "*   %d " , i + 1);
            if (guessNr == i + 1)
                wattron(endScreen , COLOR_PAIR(green));
            else
                wattron(endScreen , COLOR_PAIR(gray));
            guessPc = static_cast<int>(1. * guess[i] / guessMax * 48) + 1;
            int numCif = static_cast<int>(log10(guess[i]));
            wprintw(endScreen , "%*d" , guessPc , guess[i]);
            wattron(endScreen , COLOR_PAIR(black));
            wprintw(endScreen , "%*c  *" , 50 - (guessPc > numCif?guessPc:numCif + 1) , ' ');
            wprintw(endScreen , "*                                                         *");
        }

        wprintw(endScreen , "*                Press any KEY to continue                *");
        wprintw(endScreen , "*                                                         *");
        wprintw(endScreen , "***********************************************************");
        box(endScreen , 0 , 0);
        wrefresh(endScreen);
    }

    void GameUI::showAnswer(std::string answer) {
        std::transform(answer.begin() , answer.end() , answer.begin() , ::toupper);
        wattron(ansBox , COLOR_PAIR(white));
        wprintw(ansBox , "%s" , answer.data());
        wrefresh(ansBox);
    }

    // END
    void end() {
        mvprintw(LINES/2 - 1 , COLS/2 - 9 , "Thanks for playing!");
        getch();
        endwin();
    }
}