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
#include "./Scales.h"
#include "./Range.h"

const int led_pin = 13;
int state = false;
int interruptCtr = 1;
int S = 0;
int inputPin = 5;
double frequency = 1.0;
//int sample_rate = 44100; //samples per second
int sample_rate = 46875; //samples per second, adjusted to get an integer clocks_per_second

//clock cycles between each audio sample (this must be an integer so your sample rate effectively gets truncated if it doesn't divide evenly)
int clocks_per_sample = 656250 / sample_rate;

int analogOut = 11;
double sample_position = 0;
double sample_period = sample_rate / frequency;

const int window_size=5;
int window_position = 0;
float average_window[window_size];

void setup()
{  
  //Ignore this stuff
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
  //stop ignoring this
  
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(inputPin, INPUT);
  pinMode(analogOut, OUTPUT);
  pinMode(DAC1, OUTPUT);
  analogWriteResolution(12);
  analogReadResolution(12);
  analogWrite(DAC1, 4095);
  rangeSetup();
}

//Should return the frequency mapped from a an arbitrary range onto a scale array
double frequencyFromRange(double in, double low, double high, double* scale, int scale_size) {
  //TODO: FILL ME IN
  return 0;
}

//Returns the frequency mapped from a voltage range of 0-4095 onto a scale array
double frequencyFromVoltage(int voltage, double* scale, int scale_size) {
  return frequencyFromRange(voltage,0,4095,scale,scale_size);
}



void loop()
{
  frequency = 440.0;
  
  //TODO: Fill me in for frequency control;
  
  sample_period = sample_rate / frequency;
}

static inline int SinSample(double pos) {
  double sample_pos = WAVEFORM_SAMPLES * pos;
  int sample_floor = floor(sample_pos);
  double frac = sample_pos - sample_floor;
  return (int)(waveformsTable[0][sample_floor]);
}

static inline int sampleWaveform(int * waveform, int waveform_samples, double pos) {
  double sample_pos = waveform_samples * pos;
  int sample_floor = floor(sample_pos);
  double frac = sample_pos - sample_floor;
  
  return (int)(waveform[sample_floor]);
}


static inline int nextSample() {
  sample_position++;
  if (sample_position >= sample_period) {
    sample_position -= sample_period;
  }

  int sample = sampleWaveform(waveformsTable[3], WAVEFORM_SAMPLES, sample_position / sample_period);
  return sample;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT HANDLERS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TC7_Handler()
{
  // We need to get the status to clear it and allow the interrupt to fire again
  TC_GetStatus(TC2, 1);
  dacc_write_conversion_data(DACC_INTERFACE, nextSample());
}
