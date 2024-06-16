/**
 * mood.cpp: header files for mood.cpp
 */

#ifndef MOOD_CPP
#define MOOD_CPP

#include "config.h"
#include "display.h"
#include <Arduino.h>
#include <string>

class Mood {
public:
    static String happy;
    static String sad;
    static String broken;
    static String intense;
    static String looking1;
    static String looking2;
    static String neutral;
    static String sleeping;

    static String getMood();
    static void setMood(String face);
private:
    static String currentMood;
};

#endif // MOOD_CPP