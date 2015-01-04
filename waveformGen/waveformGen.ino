//the variable PI is already defined by the arduino base code
//PI = PI 3.1415926535897932384626433832795

void printSinValues(int num_samples) {
 Serial.print("{");
 for(int i = 0; i < num_samples; i++) {
   
   double sample_val = 0;
   //the variable PI is already defined by the arduino base code
   sample_val = sin(2*PI*i/num_samples);
   sample_val += 1;
   sample_val /= 2;
   sample_val *= 4095;
   
   int outval = floor(sample_val);
   if(outval > 0xfff || outval < 0) {
     Serial.println("Invalid Sample Value");
   }
   Serial.print(outval);
   if(i != num_samples-1)
     Serial.print(",");
 }
 Serial.print("}");
}

void printSquareValues(int num_samples) {
 Serial.print("{");
 for(int i = 0; i < num_samples; i++) {
   
   double sample_val = 0;
   if(i < num_samples/2)
     sample_val = 0xfff;
     
   int outval = floor(sample_val);
   if(outval > 0xfff || outval < 0) {
     Serial.println("Invalid Sample Value");
   }
   Serial.print(outval);
   if(i != num_samples-1)
     Serial.print(",");
 }
 Serial.print("}");
}

void setup()
{  
 Serial.begin(9600);
 printSquareValues(512);
}

void loop()
{
 
}


