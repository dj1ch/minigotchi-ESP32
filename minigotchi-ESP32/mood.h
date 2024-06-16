/**
 * mood.cpp: header files for mood.cpp
 */

#ifndef MOOD_CPP
#define MOOD_CPP

#include "config.h"
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

    static void getMood();
    static void checkMood(String face);
private:

};

#endif // MOOD_CPP