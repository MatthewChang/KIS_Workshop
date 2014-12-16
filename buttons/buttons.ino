//So it works. Use high resistance so the capasitor takes a longer time to charge. we used 10M ohms to get these numbers. We might get a more

void setup()
{

  //cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  Serial.begin(9600);
  pinMode(4,INPUT);
  pinMode(5,OUTPUT);
}

void loop()
{
  
  long start = millis();
  digitalWrite(5,LOW);
  delay(10);
  digitalWrite(5,HIGH);
  
  long total2 = analogRead(4);
  int ticks = 0;
  while(analogRead(4) < 300) {
    ticks++;
  }
  Serial.print(millis() - start);        // check on performance in milliseconds
  Serial.print("\t");
  Serial.print(total2);        // check on performance in milliseconds
  Serial.print("\t");     // tab character for debug window spacing

  Serial.println(ticks);                  // print sensor output 2

  delay(101);                             // arbitrary delay to limit data to serial port
}

