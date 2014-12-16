// These are the clock frequencies available to the timers /2,/8,/32,/128
// 84Mhz/2 = 42.000 MHz
// 84Mhz/8 = 10.500 MHz
// 84Mhz/32 = 2.625 MHz
// 84Mhz/128 = 656.250 KHz
//
// 42Mhz/44.1Khz = 952.38
// 10.5Mhz/44.1Khz = 238.09
// 2.625Hmz/44.1Khz = 59.5
// 656Khz/44.1Khz = 14.88 // 131200 / 656000 = .2 (.2 seconds)

// 84Mhz/44.1Khz = 1904 instructions per tick

#include "./Waveforms.h"

const int led_pin = 13;
int state = false;
int interruptCtr = 1;
int S = 0;
int inputPin = 5;
int frequency = 1;
int sample_rate = 44100;
int clocks_per_sample = 656250 / sample_rate;
int analogOut = 11;
int sample_number = 0;
int sample_period = sample_rate/frequency;

void setup()
{
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(analogOut, OUTPUT);
  pinMode(DAC1,OUTPUT);
  pmc_set_writeprotect(false);		 // disable write protection for pmc registers
  pmc_enable_periph_clk(ID_TC7);	 // enable peripheral clock TC7

  // we want wavesel 01 with RC
  TC_Configure(/* clock */TC2,/* channel */1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
  TC_SetRC(TC2, 1, clocks_per_sample);
  TC_Start(TC2, 1);

  // enable timer interrupts on the timer
  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS; // IER = interrupt enable register
  TC2->TC_CHANNEL[1].TC_IDR = ~TC_IER_CPCS; // IDR = interrupt disable register

  /* Enable the interrupt in the nested vector interrupt controller */
  /* TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number (=(1*3)+1) for timer1 channel1 */
  NVIC_EnableIRQ(TC7_IRQn);
  analogWriteResolution(12);
  analogReadResolution(12);
  analogWrite(DAC1, 4095);
}

void loop()
{
  int in = analogRead(inputPin);
  //analogWrite(analogOut,map(in,0,4096,0,255));
  //analogWrite(DAC1,map(in,0,4096,0,4096));
  //dacc_write_conversion_data(DACC_INTERFACE, map(in,0,4096,0,256));
  int n_frequency = map(in, 0, 4096, 0, 20000);
  n_frequency = constrain(n_frequency, 1, 20000);
  frequency = frequency*0.99 + n_frequency*0.01; //frequency stabalization
  sample_period = sample_rate/frequency;
  Serial.print(sample_period);
  Serial.print(" ");
  Serial.println(frequency);
  //Serial.println(sample_number);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT HANDLERS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void TC7_Handler()
{
  // We need to get the status to clear it and allow the interrupt to fire again
  TC_GetStatus(TC2, 1);
  state = !state;
  Serial.write("callback");
  if(state) {
    analogWrite(DAC0,255);
  } else {
    analogWrite(DAC0,0);
  }
  //TC_SetRC(TC2,1,interupt_delay);
}*/

static inline int SinSample(int sample_number, int sample_period) {
  return waveformsTable[0][WAVEFORM_SAMPLES*sample_number/sample_period];
}

static inline int TriangleSample(int sample_number, int sample_period) {
  return waveformsTable[1][WAVEFORM_SAMPLES*sample_number/sample_period];
}

static inline int SawSample(int sample_number, int sample_period) {
  return waveformsTable[2][WAVEFORM_SAMPLES*sample_number/sample_period];
}
/*int SquareSample(int sample_number, int sample_period) {
  if(sample_number > sample_period/2) {
    return 4096;
  } else {
    return 0;
  }
}*/

static inline int SquareSample(int sample_number, int sample_period) {
  return waveformsTable[3][WAVEFORM_SAMPLES*sample_number/sample_period];
}


void TC7_Handler()
{
  // We need to get the status to clear it and allow the interrupt to fire again
  TC_GetStatus(TC2, 1);
  sample_number++;
  if(sample_number>sample_period) {
    sample_number = 0;
  }
  
  int sample = SinSample(sample_number,sample_period);
  sample = constrain(sample,0,4095);
  //analogWrite(analogOut,sample);
  //analogWrite(DAC1,sample);
  dacc_write_conversion_data(DACC_INTERFACE, sample);
}
