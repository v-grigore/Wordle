#ifndef UI_H
#define UI_H

#include <array>
#include <string>

#include <curses.h>

#include "logic.h"
#include "constants.h"

#define COLOR_GRAY 77

namespace UI {
    const double RGB = Constants::RGB;
    const int gray = Constants::gray ,
              yellow = Constants::yellow ,
              green = Constants::green ,
              black = Constants::black ,
              red = Constants::red ,
              white = Constants::white ,
              redText = Constants::redText ,
              greenText = Constants::greenText;
    const int guessLen = Constants::guessLen;

    void start(bool winOS);

    class Menu {
    public:
        Menu();
        ~Menu();

        int buttonSelected;

        void loop();

    private:
        WINDOW *wButtons;
        std::array<std::string , 4> tButtons;
        int keyPressed;

        void changeButton(int move);
        void update();
    };

    class StatsMenu {
    public:
        StatsMenu();
        ~StatsMenu();

        void loop(const Logic::Stats& stats);

    private:
        std::array<WINDOW* , 2> wStats;
        WINDOW *backButton;

        void printStats(const Logic::Stats& stats , int index);
    };

    class SettingsMenu {
    public:
        explicit SettingsMenu(const Logic::Settings& settings);
        ~SettingsMenu();

        void loop(Logic::Stats& stats , Logic::Settings& settings);

    private:
        WINDOW *wButtons , *backButton , *wConfirm;
        std::array<std::array<std::string , 5> , 2> tButtons;
        std::string textBack;
        std::array<std::string , 6> tConfirm;
        int keyPressed{} , buttonSelected{};

        void update(const Logic::Settings& settings);
        void printConfirm();
        void changeSetting(Logic::Settings& settings , bool leftKey);
        void delStats(Logic::Stats& stats , const Logic::Settings& settings);
    };

    class GameUI {
    public:
        GameUI();
        ~GameUI();

        void initBoxes();

        void updateLetters(const std::string& guess , int guessNr , int mode);

        void checkLetters(std::string guess , std::string answer , int guessNr ,
                          const std::array<std::string , guessLen>& guesses , bool& validGuess);

        void showEndScreen(int solverStatus , const Logic::Stats& stats , int guessNr);

        void showAnswer(std::string answer);

    private:
        const Logic::Letters letters;
        std::array<std::array<WINDOW* , 5> , 6> boxes;
        WINDOW *endScreen , *ansBox;

        void printBox(int box_I , int box_J , int boxColor , char letter);
    };

    void end();
}

#endif
