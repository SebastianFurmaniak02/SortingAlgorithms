#pragma once

#include <random>

int getRandomNumber(int low, int high) {
    srand(time(NULL));
    int random = low + rand() % (high - low);
    return random;
}