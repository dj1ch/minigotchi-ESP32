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

/*
String Mood::happy = "";
String Mood::sad = "";
String Mood::broken = "";
String Mood::intense = "";
String Mood::looking1 = "";
String Mood::looking2 = "";
String Mood::neutral = "";
String Mood::sleeping = "";
*/

Mood *Mood::instance = nullptr;

/**
 * Sets faces according to configuration at least
 * @param happy Happy face! (not me after making this constructor)
 * @param sad Sad face (me after making this constructor)
 * @param broken Broken face (also me after making this constructor)
 * @param intense Intense face
 * @param looking1 Face looking left
 * @param looking2 Face looking right
 * @param neutral Normal
 * @param sleeping Sleeping face (i wish i was sleeping)
 */
Mood::Mood(String happy, String sad, String broken, String intense,
           String looking1, String looking2, String neutral, String sleeping)
    : happy(happy), sad(sad), broken(broken), intense(intense),
      looking1(looking1), looking2(looking2), neutral(neutral),
      sleeping(sleeping) {}

/**
 * Initializes class in the singleton pattern thingy
 * @param happy Happy face! (not me after doing all of this)
 * @param sad Sad face (me after doing all of this)
 * @param broken Broken face (also me after doing this)
 * @param intense Intense face
 * @param looking1 Face looking left
 * @param looking2 Face looking right
 * @param neutral Normal
 * @param sleeping Sleeping face (i wish i was sleeping)
 */
void Mood::init(const String happy, const String sad, const String broken,
                const String intense, const String looking1,
                const String looking2, const String neutral,
                const String sleeping) {
  if (instance == nullptr) {
    instance = new Mood(happy, sad, broken, intense, looking1, looking2,
                        neutral, sleeping);
  }

  Mood &instance = getInstance();
  instance.happy = happy;
  instance.sad = sad;
  instance.broken = broken;
  instance.intense = intense;
  instance.looking1 = looking1;
  instance.looking2 = looking2;
  instance.neutral = neutral;
  instance.sleeping = sleeping;

  /* debugging
  Serial.println("Faces:");
  Serial.println("Happy: " + instance.happy);
  Serial.println("Sad: " + instance.sad);
  Serial.println("Broken: " + instance.broken);
  Serial.println("Intense: " + instance.intense);
  Serial.println("Looking1: " + instance.looking1);
  Serial.println("Looking2: " + instance.looking2);
  Serial.println("Neutral: " + instance.neutral);
  Serial.println("Sleeping: " + instance.sleeping);
  */
}

/**
 * Gets instance of class so it is only used once
 */
Mood &Mood::getInstance() { return *instance; }

/** developer note:
 *
 * these checking procedures will only really be used for faces,
 * since they change often and the minigotchi is intended to be fast...
 *
 * obviously, this is all a WIP!
 *
 */

/**
 * Returns the current mood and writes it to a variable
 * @param face Face to get mood off of
 */
String Mood::getFull(String face) { Mood::getFace(Mood::getMood(face)); }

/**
 * Returns current mood
 */
String Mood::getCurrentMood() { return currentMood; }

/**
 * Gets current mood off of face
 * @param face Current face
 */
String Mood::getMood(String face) {
  if (face == happy) {
    currentMood = "happy";
  } else if (face == sad) {
    currentMood = "sad";
  } else if (face == broken) {
    currentMood = "broken";
  } else if (face == intense) {
    currentMood = "intense";
  } else if (face == looking1) {
    currentMood = "looking1";
  } else if (face == looking2) {
    currentMood = "looking2";
  } else if (face == neutral) {
    currentMood = "neutral";
  } else if (face == sleeping) {
    currentMood = "sleeping";
  } else {
    currentMood = " ";
  }

  return currentMood;
}

/**
 * Returns the current face
 */
String Mood::getCurrentFace() { return currentFace; }

/**
 * Gets face off of the mood
 * @param mood Mood to use
 */
String Mood::getFace(String mood) {
  if (mood == "happy") {
    currentFace = happy;
  } else if (mood == "sad") {
    currentFace = sad;
  } else if (mood == "broken") {
    currentFace = broken;
  } else if (mood == "looking1") {
    currentFace = looking1;
  } else if (mood == "looking2") {
    currentFace = looking2;
  } else if (mood == "neutral") {
    currentFace = neutral;
  } else if (mood == "sleeping") {
    currentFace = sleeping;
  } else {
    currentFace = " ";
  }

  return currentFace;
}

/**
 * Checks if moods match one another.
 * If they're similar, return true.
 * If they aren't similar, return false.
 * @param previous Previous face
 * @param current Current face
 */
bool Mood::checkMood(String previous, String current) {
  if (previous == current) {
    return true;
  } else if (previous != current) {
    return false;
  } else {
    return false;
  }
}

/**
 * Getter for happy mood
 */
String Mood::getHappy() { return instance->happy; }

/**
 * Getter for sad mood
 */
String Mood::getSad() { return instance->sad; }

/**
 * Getter for broken mood
 */
String Mood::getBroken() { return instance->broken; }

/**
 * Getter for intense mood
 */
String Mood::getIntense() { return instance->intense; }

/**
 * Getter for looking1 mood
 */
String Mood::getLooking1() { return instance->looking1; }

/**
 * Getter for looking2 mood
 */
String Mood::getLooking2() { return instance->looking2; }

/**
 * Getter for neutral mood
 */
String Mood::getNeutral() { return instance->neutral; }

/**
 * Getter for sleeping mood
 */
String Mood::getSleeping() { return instance->sleeping; }
