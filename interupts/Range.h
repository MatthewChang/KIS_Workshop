#ifndef _RANGE_H_
#define _RANGE_H_

const static int trigPin = 8; //This is a digital pin
const static int echoPin = 0; //This is a digial pin 

void rangeSetup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

 loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  //  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;
  if (distance >= 200 || distance <= 0) {
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }
}

#endif 
