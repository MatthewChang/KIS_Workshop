#ifndef _RANGE_H_
#define _RANGE_H_

#include "arduino.h"

const static int RANGE_TRIGGER_PIN = 8; //This is a digital pin
const static int RANGE_ECHO_PIN = 5; //This is a digial pin

static void rangeSetup() {
  pinMode(RANGE_TRIGGER_PIN, OUTPUT);
  pinMode(RANGE_ECHO_PIN, INPUT);
}

static float getRange() {
  float duration, distance;
  digitalWrite(RANGE_TRIGGER_PIN, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(RANGE_TRIGGER_PIN, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(RANGE_TRIGGER_PIN, LOW);
  duration = pulseIn(RANGE_ECHO_PIN, HIGH);
  //distance = (duration / 2) / 29.1;
  distance = (duration / 2) / 2.91;
  if (distance >= 200 || distance <= 0) {
    return -1;
  } else {
    return distance;
  }
}

#endif
