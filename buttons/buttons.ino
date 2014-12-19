//So it works. Use high resistance so the capasitor takes a longer time to charge. we used 10M ohms to get these numbers. We might get a more
#include "./CapSense.h"

CapSense sensor(5, 4);
int test_led_pin = 13;
void setup()
{

  //cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(9600);
  analogReadResolution(12);
  analogWriteResolution(12);
  pinMode(4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(test_led_pin, OUTPUT);

}


void loop()
{
  if (sensor.buttonState() == 1) {
    Serial.println("on");
    digitalWrite(test_led_pin, HIGH);
  } else {
    digitalWrite(test_led_pin, LOW);
  }
}

