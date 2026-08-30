#include <iostream>

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[H" << flush; //clear screen and move to top left
};