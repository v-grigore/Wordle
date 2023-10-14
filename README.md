# Wordle
C++ CLI implementation of the popular game Wordle
# Setting up the game
```
sudo apt install g++
sudo apt install libncurses-dev
g++ -Wall main.cpp logic.cpp solver.cpp UI.cpp -lncurses -o wordle
```
Run using
```
./wordle
```
# Game features
- Classic Wordle gameplay, with over 2000 words to guess
- Fully functioning menu
- Stats tracker easily accesssible from the game's menu
- Settings tab
# Upcoming features
- Wordle solver, accessible from the settings tab
