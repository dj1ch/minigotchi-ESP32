/**
 * mood.cpp: handles and sets mood on the minigotchi
 */

#include "mood.h"

// set moods to be config's
String Mood::happy = Config::happy;
String Mood::sad = Config::sad;
String Mood::broken = Config::broken;
String Mood::intense = Config::intense;
String Mood::looking1 = Config::looking1;
String Mood::looking2 = Config::looking2;
String Mood::neutral = Config::neutral;
String Mood::sleeping = Config::sleeping;

String Mood::getMood() {
  return currentMood;
}

void Mood::setMood(String face) {
  if (face == Mood::happy) {
    currentMood = "happy";
  } else if (face == Mood::sad)  {
    currentMood = "sad";
  } else if (face == Mood::broken) {
    currentMood = "broken"
  } else if (face == Mood::intense) {
    currentMood = "intense";
  } else if (face == Mood::looking1 || face == Mood::looking2) {
    currentMood = "looking";
  } else if (face == Mood::neutral) {
    currentMood = "neutral";
  } else if (face == Mood::sleeping) {
    currentMood = "sleeping";
  } else {
    currentMood = " ";
  }
}