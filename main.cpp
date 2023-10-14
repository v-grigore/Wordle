#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <random>
#include <algorithm>
#include <list>
#include <map>
#include <chrono>

#include <curses.h>

#include "logic.h"
#include "UI.h"
#include "constants.h"
#include "solver.h"

#ifdef _WIN32
#include <windows.h>
#endif

class Main {
public:
    void main(bool winOS) {
        Logic::readAns(answers);
        Logic::readGuess(guesses , candidates);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> ansDistr(0, Constants::ansLen - 1);

        UI::start(winOS);
        bool isRunning = true;
        while (isRunning) {
            auto *menu = new UI::Menu;
            menu->loop();
            switch (menu->buttonSelected) {
                case 0: {
                    delete (menu);
                    gameLoop(rd , ansDistr);
                    break;
                }
                case 1: {
                    delete (menu);
                    UI::StatsMenu statsMenu;
                    statsMenu.loop(stats);
                    break;
                }
                case 2: {
                    delete (menu);
                    UI::SettingsMenu settingsMenu{settings};
                    settingsMenu.loop(stats , settings);
                    break;
                }
                case 3:
                    delete(menu);
                    isRunning = false;
                    break;
            }
        }
        UI::end();
    }
private:
    std::array<std::string , Constants::ansLen> answers;
    std::array<std::string , Constants::guessLen> guesses;
    std::list<std::string> candidates;
    std::map<double , int> entropies;
    Logic::Stats stats;
    Logic::Settings settings;
    std::string guess , answer;
    int guessNr{};

    static bool isLetter(int ch) {
        if (ch >= 'a' && ch <= 'z')
            return true;
        return false;
    }

    void gameLoop(std::random_device& rd , std::uniform_int_distribution<>& ansDistr) {
        if (!settings.solverON)
            stats.playedA++;
        else
            stats.playedB++;
        answer = answers[ansDistr(rd)];

        UI::GameUI game;

        game.initBoxes();
        getGuess(game);

        if (guessNr < 6) {
            if (!settings.solverON) {
                stats.wonA++;
                stats.curStkA++;
                stats.maxStkA = std::max(stats.curStkA, stats.maxStkA);
                stats.guessA[guessNr]++;
            }
            else {
                stats.wonB++;
                stats.curStkB++;
                stats.maxStkB = std::max(stats.curStkB, stats.maxStkB);
                stats.guessB[guessNr]++;
            }
        }
        else {
            if (!settings.solverON)
                stats.curStkA = 0;
            else
                stats.curStkB = 0;
        }
        stats.save();

        game.showEndScreen(settings.solverON , stats , guessNr + 1);
        if (guessNr == 6)
            game.showAnswer(answer);
        getch();
    }

    void getGuess(UI::GameUI& game) {
        for (guessNr = 0; guessNr < 6; guessNr++) {
            guess = "";
            bool validGuess = false;
            while (!validGuess) {
                int ch = 0;
                while (ch != '\n') {
                    ch = getch();
                    if (isLetter(ch) && guess.length() < 5) {
                        guess.push_back(static_cast<char>(ch));
                        game.updateLetters(guess, guessNr, 1);
                    } else if ((ch == KEY_BACKSPACE || ch == '\b') && guess.length()) {
                        guess.pop_back();
                        game.updateLetters(guess, guessNr, 0);
                    }
                }
                if (guess.length() == 5)
                    game.checkLetters(guess, answer, guessNr, guesses, validGuess);
            }
            if (answer == guess)
                break;
        }
    }
};

/*const int ansLen = Constants::ansLen;
const int guessLen = Constants::guessLen;*/

/*bool isLetter(int ch) {
    if (ch >= 'a' && ch <= 'z')
        return true;
    return false;
}*/

/*void getGuess(std::string& guess , UI::GameUI& game , int& guessNr , const std::array<std::string , ansLen>& answers ,
              const std::array<std::string , guessLen>& guesses , const std::string& answer) {

    for (guessNr = 0; guessNr < 6; guessNr++) {
        guess = "";
        bool validGuess = false;
        while (!validGuess) {
            int ch = 0;
            while (ch != '\n') {
                ch = getch();
                if (isLetter(ch) && guess.length() < 5) {
                    guess.push_back(static_cast<char>(ch));
                    game.updateLetters(guess, guessNr, 1);
                } else if ((ch == KEY_BACKSPACE || ch == '\b') && guess.length()) {
                    guess.pop_back();
                    game.updateLetters(guess, guessNr, 0);
                }
            }
            if (guess.length() == 5)
                game.checkLetters(guess, answer, guessNr, guesses, validGuess);
        }
        if (answer == guess)
            break;
    }
}*/

int main() {
    bool winOS = false;

#ifdef _WIN32
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, 0);
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    ShowScrollBar(GetConsoleWindow(), SB_BOTH, FALSE);
    winOS = true;
#endif

    Main main;
    main.main(winOS);




    /* Solver testing
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::ofstream fout("solvertest.txt");

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[seconds]" << std::endl;
    */



    /*bool winOS = false;

#ifdef _WIN32
    SetWindowLong(GetConsoleWindow(), GWL_STYLE, 0);
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
    ShowScrollBar(GetConsoleWindow(), SB_BOTH, FALSE);
    winOS = true;
#endif

    std::array<std::string , ansLen> answers;
    std::array<std::string , guessLen> guesses;
    Logic::readAns(answers);
    Logic::readGuess(guesses);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ansDistr(0, ansLen - 1);
    Logic::Stats stats;
    Logic::Settings settings;

    UI::start(winOS);
    bool isRunning = true;
    while (isRunning) {
        auto *menu = new UI::Menu;
        menu->loop();
        switch (menu->buttonSelected) {
            case 0: {
                delete (menu);
                stats.playedA++;

                std::string guess , answer = answers[ansDistr(rd)];
                int guessNr;
                UI::GameUI game;

                game.initBoxes();
                getGuess(guess , game , guessNr , answers , guesses , answer);

                if (guessNr < 6) {
                    stats.wonA++;
                    stats.curStkA++;
                    stats.maxStkA = std::max(stats.curStkA , stats.maxStkA);
                    stats.guessA[guessNr]++;
                }
                else {
                    stats.curStkA = 0;
                }
                stats.save();

                game.showEndScreen(0 , stats , guessNr + 1);
                if (guessNr == 6)
                    game.showAnswer(answer);
                getch();
                break;
            }
            case 1: {
                delete (menu);
                UI::StatsMenu statsMenu;
                statsMenu.loop(stats);
                break;
            }
            case 2: {
                delete (menu);
                UI::SettingsMenu settingsMenu{settings};
                settingsMenu.loop(stats , settings);
                break;
            }
            case 3:
                delete(menu);
                isRunning = false;
                break;
        }
    }
    UI::end();*/
/*
    std::array<std::string , ansLen> answers;
    std::array<std::string , guessLen> guesses;
    Logic::readAns(answers);
    Logic::readGuess(guesses);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> ansDistr(0, ansLen - 1);
    std::string guess , answer;
    Logic::Stats stats;

    UI::start(winOS);
    int ch = 0;
    bool isRunning = true;
    while (isRunning) {
        auto *menu = new UI::Menu;
        menu->loop();
        switch (menu->buttonSelected) {
            case 0: {
                delete (menu);
                answer = answers[ansDistr(rd)];
                UI::GameUI game;
                game.initBoxes();
                for (int guessNr = 0; guessNr < 6; guessNr++) {
                    guess = "";
                    bool validGuess = false;
                    while (!validGuess) {
                        ch = 0;
                        while (ch != '\n') {
                            ch = getch();
                            if (isLetter(ch) && guess.length() < 5) {
                                guess.push_back(static_cast<char>(ch));
                                game.updateLetters(guess, guessNr, 1);
                            } else if ((ch == KEY_BACKSPACE || ch == '\b') && guess.length()) {
                                guess.pop_back();
                                game.updateLetters(guess, guessNr, 0);
                            }
                        }
                        if (guess.length() == 5)
                            game.checkLetters(guess, answer, guessNr, guesses, validGuess);
                    }
                    if (answer == guess) {
                        game.showEndScreen(0 , stats , guessNr + 1);
                        break;
                    }
                }
                break;
            }
            case 1:
            case 2:
                delete(menu);
                break;
            case 3:
                delete(menu);
                isRunning = false;
                break;
        }
    }
    stats.save();
    UI::end();
*/
    /*
    UI::start(winOS);
    std::string guess , answer = "avert";
    int ch = 0;
    {
        UI::GameUI gameUi;
        gameUi.initBoxes();
        for (int guessNr = 0; guessNr < 6; guessNr++) {
            guess = "";
            while ((guess.length() < 5) || (ch != '\n')) {
                ch = getch();
                if (isLetter(ch) && guess.length() < 5) {
                    guess.push_back(static_cast<char>(ch));
                    gameUi.updateLetters(guess , guessNr , 1);
                }
                else if (ch == KEY_BACKSPACE && guess.length()) {
                    guess.pop_back();
                    gameUi.updateLetters(guess , guessNr , 0);
                }
            }
            gameUi.checkLetters(guess , answer , guessNr);
        }
        getch();
    }
    UI::end();
*/
    /*UI::start(winOS);
    {
        UI::GameUI gameUi;
        gameUi.initBoxes();
        getch();
        gameUi.checkLetters("water" , "amber" , 0);
        getch();
    }
    UI::end();*/

    /*UI::start(winOS);
    {
        UI::Menu menu;
        menu.loop();
    }
    UI::end();*/


    /*initscr();
    curs_set(0);
    noecho();
    keypad(stdscr , TRUE);
    char mat[4][15] = {"   > Play <   " , "  Scoreboard  " , "   Settings   " , "     Exit     "};
    WINDOW *win1 = newwin(7 , 14 , LINES/2 - 4 , COLS/2 - 7);
    for (char *i:mat) {
        wprintw(win1 , "%s\n" , i);
    }
    refresh();
    wrefresh(win1);
    int ch = 0 , pos = 0;
    while (ch != '\n') {
        ch = getch();
        switch (ch) {
            case KEY_UP:
                changeButton(mat , pos , 2);
                update(win1 , mat);
                break;
            case KEY_DOWN:
                changeButton(mat , pos , 1);
                update(win1 , mat);
                break;
        }
    }
    endwin();*/


    /*initscr();
    start_color();
    curs_set(0);
    noecho();
    init_color(COLOR_GREEN , 83 * RGB , 141 * RGB , 78 * RGB);
    init_color(COLOR_YELLOW , 181 * RGB , 159 * RGB , 59 * RGB);
    init_color(COLOR_GRAY , 58 * RGB , 58 * RGB , 60 * RGB);
    init_color(COLOR_WHITE , 1000 , 1000 , 1000);
    Letters letters;
    letters.init();
    WINDOW *win = newwin(9 , 19 , 1 , 1);
    WINDOW *l2 = newwin(9 , 19 , 1 , 21);
    WINDOW *l3 = newwin(9 , 19 , 1 , 41);
    WINDOW *l4 = newwin(9 , 19 , 1 , 61);
    WINDOW *l5 = newwin(9 , 19 , 1 , 81);
    box(l2 , 0 , 0);
    box(l3 , 0 , 0);
    box(l4 , 0 , 0);
    box(l5 , 0 , 0);
    refresh();
    wrefresh(l2);
    wrefresh(l3);
    wrefresh(l4);
    wrefresh(l5);
    init_pair(1 , COLOR_BLACK , COLOR_YELLOW);
    init_pair(2 , COLOR_BLACK , COLOR_WHITE);
    init_pair(3 , COLOR_BLACK , COLOR_GREEN);
    init_pair(4 , COLOR_BLACK , COLOR_WHITE);
    int k = 3;
    for (char ch = 'a'; ch <= 'z'; ch++) {
        werase(win);
        if (k == 3)
            k = 1;
        else
            k = 3;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 19; j++) {
                wattron(win , COLOR_PAIR(letters.key[ch][i][j] + k));
                wprintw(win , " ");
            }
        }
        refresh();
        wrefresh(win);
        getch();
    }
    refresh();
    wrefresh(win);
    int ch = 0;
    while ((ch = getch()) != '\n');
    endwin();*/

    /*WINDOW *win1 = newwin(9 , 19 , 0 , 0) , *win2 = newwin(9 , 19 , 0 , 19) , *u = subwin(win1 , 7 , 17 , 1 , 1);
    WINDOW *v = subwin(win2 , 7 , 17 , 1 , 20);
    WINDOW *colors = newwin(1 , 50 , 10 , 0);
    WINDOW *win3 = newwin(9 , 19 , 0 , 38) , *win4 = newwin(9 , 19 , 0 , 57);
    WINDOW *u2 = subwin(win3 , 7 , 17 , 1 , 39) , *v2 = subwin(win4 , 7 , 17 , 1 , 58);
    for (int i = 0; i <= 15; i++) {
        init_pair(i+10 , COLOR_BLACK , i);
        wattron(colors , COLOR_PAIR(i+10));
        wprintw(colors , "%d" , i);
    }
    init_pair(1 , COLOR_BLACK , COLOR_GREEN);
    init_pair(2 , COLOR_BLACK , COLOR_WHITE);
    init_pair(3 , COLOR_BLACK , COLOR_YELLOW);
    init_pair(4 , COLOR_BLACK , COLOR_WHITE);
    init_pair(5 , COLOR_GRAY , COLOR_BLACK);
    init_pair(6 , COLOR_BLACK , COLOR_WHITE);
    init_pair(7 , COLOR_BLACK , COLOR_GRAY);
    init_pair(8 , COLOR_BLACK , COLOR_WHITE);
    //box(win1 , 0 , 0);
    //box(win2 , 0 , 0);
    //box(win3 , 0 , 0);
    //box(win4 , 0 , 0);
    wbkgdset(u , COLOR_PAIR(2));
    wbkgdset(v , COLOR_PAIR(2));
    int m[7][17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                    0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
                    0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
                    0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,
                    0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
                    0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,
                    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 17; j++) {
            wattron(u , COLOR_PAIR(m[i][j]+1));
            wprintw(u , " ");
            wattron(v , COLOR_PAIR(m[i][j]+3));
            wprintw(v , " ");
            wattron(u2 , COLOR_PAIR(m[i][j]+5));
            wprintw(u2 , " ");
            wattron(v2 , COLOR_PAIR(m[i][j]+7));
            wprintw(v2 , " ");
        }
        //wprintw(u , "\n");
    }
    box(u2 , 0 , 0);
    refresh();
    wrefresh(win1);
    wrefresh(win2);
    wrefresh(win3);
    wrefresh(win4);
    wrefresh(u);
    wrefresh(v);
    wrefresh(u2);
    wrefresh(v2);
    wrefresh(colors);
    getch();
    endwin();*/
}

/*void changeButton(char mat[4][15] , int &pos , const int move) {
    switch (pos + move * 10) {
        case 10:
            mat[pos][3] = ' '; mat[pos][10] = ' ';
            mat[pos + 1][0] = '>'; mat[pos + 1][13] = '<';
            pos++;
            break;
        case 11:
            mat[pos][0] = ' '; mat[pos][13] = ' ';
            mat[pos + 1][1] = '>'; mat[pos + 1][12] = '<';
            pos++;
            break;
        case 12:
            mat[pos][1] = ' '; mat[pos][12] = ' ';
            mat[pos + 1][3] = '>'; mat[pos + 1][10] = '<';
            pos++;
            break;
        case 21:
            mat[pos][0] = ' '; mat[pos][13] = ' ';
            mat[pos - 1][3] = '>'; mat[pos - 1][10] = '<';
            pos--;
            break;
        case 22:
            mat[pos][1] = ' '; mat[pos][12] = ' ';
            mat[pos - 1][0] = '>'; mat[pos - 1][13] = '<';
            pos--;
            break;
        case 23:
            mat[pos][3] = ' '; mat[pos][10] = ' ';
            mat[pos - 1][1] = '>'; mat[pos - 1][12] = '<';
            pos--;
            break;
    }
}

void update(WINDOW *win , char mat[4][15]) {
    werase(win);
    for (int i = 0; i < 4; i++) {
        wprintw(win , "%s\n" , mat[i]);
    }
    refresh();
    wrefresh(win);
}*/
