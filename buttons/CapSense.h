/*Capacitive sensor class:
using a digital output pin and analog input pin determine if
there is a capacitance between the two pins. This is designed to
be used for ad-hoc buttons using the users finger to form a capacitor.
Designed around a serial circuit: out_pin -> 10 M ohm resistor -> copper plate -> 10 M ohm resistor -> in_pin
Using higher resistances cause the capacitor to charge and discharge slower. Default constants were chosen with
the above specified resistances in mind.

Written by Matthew Chang for the 2015 MISTI GTL Korea Workshops*/

#include <arduino.h>

class CapSense {
  private:
    int out_pin;
    int in_pin;
    int voltage_threshold;
    int ticks_threshold;
    const int tick_timeout = 10000;
    const int low_threshold = 5;
  public:
    /*Constructor for sensor, 2000 is a value I found to be good
    emperically for resolution 12, 50 is a number I emperically
    found to be good with 2, 10 Mohm resistors and our*/
    CapSense(int output_pin, int read_pin, int v_threshold = 2000, int t_threshold = 1000) {
      out_pin = output_pin;
      in_pin = read_pin;
      voltage_threshold = v_threshold;
      ticks_threshold = t_threshold;
      pinMode(out_pin, OUTPUT);
      pinMode(in_pin, INPUT);
    }

    void setVoltageThreshold(int v) {
      voltage_threshold = v;
    }

    void setTickThreshold(int t) {
      ticks_threshold = t;
    }

    /*returns the number of ticks for the input pin to detect a
    rise in voltage from the output pin
    if the component appears disconnected (i.e. no change in voltage)
    sampleTicks() returns -1*/
    int sampleTicks() {
      digitalWrite(out_pin, LOW);
      int t = 0;
      while (analogRead(in_pin) > low_threshold) {
        t++;
        if (t > tick_timeout) {
          return -1;
        }
        delay(1);
      }
      t = 0;
      digitalWrite(out_pin, HIGH);
      while (analogRead(in_pin) < voltage_threshold) {
        t++;
        if (t > tick_timeout) {
          return -1;
        }
      }
      return t;
    }

    /*returns the state of the button formed by the sensor
    1 if on (as determined by ticks threshold)
    0 if off
    -1 if disconnected (i.e. timeout)*/
    int buttonState() {
      int ticks = sampleTicks();
      if (ticks > ticks_threshold)
        return 1;
      else if (ticks == -1)
        return 0;
      else
        return 0;
    }
};
