/*
 * Minigotchi: An even smaller Pwnagotchi
 * Copyright (C) 2024 dj1ch
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

/** developer note:
 *
 * these checking procedures will only really be used for faces,
 * since they change often and the minigotchi is intended to be fast...
 *
 */

// not only are we returning the current mood,
// but we are also writing the current mood to the private variable
String Mood::getFull(String face) { Mood::getFace(Mood::getMood(face)); }

String Mood::getCurrentMood() { return currentMood; }

String Mood::getMood(String face) {
  if (face == Mood::happy) {
    currentMood = "happy";
  } else if (face == Mood::sad) {
    currentMood = "sad";
  } else if (face == Mood::broken) {
    currentMood = "broken";
  } else if (face == Mood::intense) {
    currentMood = "intense";
  } else if (face == Mood::looking1) {
    currentMood = "looking1";
  } else if (face == Mood::looking2) {
    currentMood = "looking2";
  } else if (face == Mood::neutral) {
    currentMood = "neutral";
  } else if (face == Mood::sleeping) {
    currentMood = "sleeping";
  } else {
    currentMood = " ";
  }

  return currentMood;
}

String Mood::getCurrentFace() { return currentFace; }

String Mood::getFace(String mood) {
  if (mood == "happy") {
    currentFace = Mood::happy;
  } else if (mood == "sad") {
    currentFace = Mood::sad;
  } else if (mood == "broken") {
    currentFace = Mood::broken;
  } else if (mood == "looking1") {
    currentFace = Mood::looking1;
  } else if (mood == "looking2") {
    currentFace = Mood::looking2;
  } else if (mood == "neutral") {
    currentFace = Mood::neutral;
  } else if (mood == "sleeping") {
    currentFace = Mood::sleeping;
  } else {
    currentFace = " ";
  }

  return currentFace;
}

bool Mood::checkMood(String previous, String current) {
  if (previous == current) {
    return true;
  } else if (previous != current) {
    return false;
  } else {
    return false;
  }
}
