// LOESS-LABS.NET
// Dylan Barry, 2025, GNU GPLv3

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>
#include <math.h> 

#define CONTROL_RATE 128

// Analog Input Pins
const byte AUDIO_FREQ_PIN = 2;
const byte LFO_RATE_A_PIN = 3;
const byte LFO_DIVISOR_PIN = 1;     // Division ratio for sub-LFO
const byte FIVE_STEP_GATE_PIN = 7;  // Clock in for 5 stepper
const byte FLIP_GATE_PIN = 5;       // Clock in for 5 step flip
const byte THREE_STEP_GATE_PIN = 6; // Clock in for 3 stepper
const byte THREE_STEP_FLIP_PIN = 4; // Clock in for 3 step flip

// Digital Output Pins
const byte LFO_PIN_A = 11;
const byte LFO_PIN_B = 12;

// Sequencer Pin Arrays
const byte FIVE_STEP_PINS[] = {2, 4, 13, 3, 5};
const int FIVE_STEP_COUNT = sizeof(FIVE_STEP_PINS) / sizeof(FIVE_STEP_PINS[0]);
const byte THREE_STEP_PINS[] = {6, 8, 7};
const int THREE_STEP_COUNT = sizeof(THREE_STEP_PINS) / sizeof(THREE_STEP_PINS[0]);

// SENSITIVITY CONSTANTS

// Lowering the threshold makes it trigger on weaker signals.
const int GATE_THRESHOLD = 800; 

// Define the fixed HIGH time (pulse width) in milliseconds for LFO A and LFO B.
// This makes the pulse a stable trigger signal, independent of the overall frequency.
const unsigned long LFO_A_FIXED_PULSE_MS = 10UL; // 10ms pulse width

// Define the min/max range for the main audio carrier (Sine LFO) frequency
const float CARRIER_FREQ_MIN = 0.01f;
const float CARRIER_FREQ_MAX = 18.0f;

// Define the min/max range for the LFO A Period (in milliseconds)
const float LFO_PERIOD_MIN_MS = 20.0f; // Max speed (50 Hz)
const float LFO_PERIOD_MAX_MS = 6000.0f; // Min speed (0.166 Hz)


// The array of possible divisors for LFO B
const int DIVISORS[] = {2, 3, 4, 5, 6};
const int DIVISOR_COUNT = sizeof(DIVISORS) / sizeof(DIVISORS[0]);

// STATE VARIABLES
Oscil<SIN2048_NUM_CELLS, MOZZI_AUDIO_RATE> aCarrier(SIN2048_DATA);

// Timers
unsigned long lfoAPreviousMillis = 0; // LFO A Pulse Timer
unsigned long lfoBPreviousMillis = 0; // LFO B Pulse Timer
unsigned long cPreviousMillis = 0; // 5 Stepper Gate Clock
unsigned long dPreviousMillis = 0; // 5 Stepper Flip Gate Clock
unsigned long ePreviousMillis = 0; // 3 Stepper Gate Clock
unsigned long fPreviousMillis = 0; // 3 Stepper Flip Gate Clock

// LFO State
int lfoACount = 0;     // Counts full cycles (toggles) of LFO A
int lfoBDivisor = 2;   // Selected division (2, 3, 4, 5, or 6)
bool lfoAIsHigh = false; // Internal state tracking for LFO A
bool lfoBIsHigh = false; // Internal state tracking for LFO B

// Sequencer State
int aPreviousGate = 0; // For 5-Step clock rising edge detection
int bPreviousGate = 0; // For 5-Step Flip rising edge detection
int cPreviousGate = 0; // For 3-Step clock rising edge detection
int dPreviousGate = 0; // For 3-Step Flip rising edge detection
int fivStep = 2;       // Current step for 5-step sequencer
int triStep = 0;       // Current step for 3-step sequencer
int flip = 0;          // Direction control for 5-step sequencer (0=up, 1=down)
int triFlip = 0;       // Direction control for 3-step sequencer (0=up, 1=down)

// Exponential mapping for pot response
float mapExponentialFloat(int analogValue, float minOutput, float maxOutput, bool invert) {
    float normalizedValue = (float)analogValue / 1023.0f;

    if (invert) {
        normalizedValue = 1.0f - normalizedValue;
    }

    float logMin = log(minOutput);
    float logMax = log(maxOutput);

    float logResult = logMin + normalizedValue * (logMax - logMin);

    float result = exp(logResult);

    return max(minOutput, min(result, maxOutput));
}


void setup() {
  // Start Mozzi
  startMozzi(CONTROL_RATE);
  
  // Set all sequencer pins as outputs
  for (int i = 0; i < FIVE_STEP_COUNT; i++) {
    pinMode(FIVE_STEP_PINS[i], OUTPUT);
  }
  for (int i = 0; i < THREE_STEP_COUNT; i++) {
    pinMode(THREE_STEP_PINS[i], OUTPUT);
  }
  
  // Set LFO output pins
  pinMode(LFO_PIN_A, OUTPUT);
  pinMode(LFO_PIN_B, OUTPUT);
}

// SEQUENCER/CONTROL LOGIC FUNCTIONS

// Output logic for a sequencer using an array.
// Only the pin corresponding to the current step is set HIGH.

void updateSequencerOutputs(const byte pins[], int count, int currentStep) {
  for (int i = 0; i < count; i++) {
    // Check if the current pin index matches the current step
    if (i == currentStep) {
      digitalWrite(pins[i], HIGH);
    } else {
      digitalWrite(pins[i], LOW);
    }
  }
}

void updateControl() {
  unsigned long currentMillis = millis();

  // LFO Rate and Division Configuration
  
  // Exponential Mapping for LFO A Period: 
  int lfoAPeriodBase = (int)mapExponentialFloat(
      mozziAnalogRead(LFO_RATE_A_PIN), 
      LFO_PERIOD_MIN_MS, 
      LFO_PERIOD_MAX_MS, 
      true
  );
  
  // Calculate ON time (pulse width) and OFF time (space).
  unsigned long lfoAOnTime = LFO_A_FIXED_PULSE_MS;
  
  // OFF time is the total period minus the fixed ON time.
  unsigned long lfoAOffTime;
  if (lfoAPeriodBase <= lfoAOnTime) {
      lfoAOffTime = LFO_A_FIXED_PULSE_MS; 
  } else {
      lfoAOffTime = (unsigned long)lfoAPeriodBase - lfoAOnTime;
  }
  lfoAOffTime = max(lfoAOffTime, 1UL); 
  
  // LFO B Divisor: 
  int rawDivisorRead = mozziAnalogRead(LFO_DIVISOR_PIN);
  int divisorIndex = map(rawDivisorRead, 1023, 0, 0, DIVISOR_COUNT - 1); 
  lfoBDivisor = DIVISORS[divisorIndex];

  // Determine the time limit based on the current internal LFO state
  unsigned long timeLimit = lfoAIsHigh ? lfoAOnTime : lfoAOffTime;

  // LFO A Logic (Master Clock)
  if (currentMillis - lfoAPreviousMillis > timeLimit){
    lfoAPreviousMillis = currentMillis;
    lfoAIsHigh = !lfoAIsHigh;
    digitalWrite(LFO_PIN_A, lfoAIsHigh);
    if (lfoAIsHigh) { 
      lfoACount++;
    }
  }

  // LFO B Logic
  
  if (lfoACount >= lfoBDivisor) {
      lfoACount = 0;
      
      if (lfoBIsHigh == false) {
          lfoBIsHigh = true;
          lfoBPreviousMillis = currentMillis;
      }
  }

  if (lfoBIsHigh == true && (currentMillis - lfoBPreviousMillis >= LFO_A_FIXED_PULSE_MS)) {
      lfoBIsHigh = false;
  }

  digitalWrite(LFO_PIN_B, lfoBIsHigh);
  
  // Gate/Control Input Readings
  int aCurrentGate = mozziAnalogRead(FIVE_STEP_GATE_PIN); 
  int bCurrentGate = mozziAnalogRead(FLIP_GATE_PIN);                  
  int cCurrentRawGate = mozziAnalogRead(THREE_STEP_GATE_PIN);
  int dCurrentGate = mozziAnalogRead(THREE_STEP_FLIP_PIN);

  // 5-Step Sequencer (Clocked by A7)
  // Rising edge detection check: Trigger when signal crosses threshold AND is rising
  if (currentMillis - cPreviousMillis > 10 && aCurrentGate > GATE_THRESHOLD && aPreviousGate < aCurrentGate){
    cPreviousMillis = currentMillis;
    
    if (flip == 1){
      fivStep--;
    } else {
      fivStep++;
    }
    
    if (fivStep >= FIVE_STEP_COUNT) {
      fivStep = 0;
    } else if (fivStep < 0) {
      fivStep = FIVE_STEP_COUNT - 1; 
    }
  }
  aPreviousGate = aCurrentGate;

  // 5-Step Flip Control
  // Rising edge detection check: Trigger when signal crosses threshold AND is rising
  if (currentMillis - dPreviousMillis > 10 && bCurrentGate > GATE_THRESHOLD && bPreviousGate < bCurrentGate){
    dPreviousMillis = currentMillis;
    flip = 1 - flip;
  }
  bPreviousGate = bCurrentGate;
  
  // 3-Step Sequencer
  // Rising edge detection for 3-step clock for reliable triggering
  if (currentMillis - ePreviousMillis > 10 && cCurrentRawGate > GATE_THRESHOLD && cPreviousGate < cCurrentRawGate){
    ePreviousMillis = currentMillis;
    
    if (triFlip == 1){
      triStep--;
    } else {
      triStep++;
    }
    
    if (triStep >= THREE_STEP_COUNT){
      triStep = 0;
    } else if (triStep < 0) {
      triStep = THREE_STEP_COUNT - 1; 
    }
  }
  cPreviousGate = cCurrentRawGate; 

  // 3-Step Flip Control
  // Rising edge detection check: Trigger when signal crosses threshold AND is rising
  if (currentMillis - fPreviousMillis > 10 && dCurrentGate > GATE_THRESHOLD && dPreviousGate < dCurrentGate){
    fPreviousMillis = currentMillis;
    triFlip = 1 - triFlip;
  }
  dPreviousGate = dCurrentGate;
  
  // Frequency Control
  int sensorValue = mozziAnalogRead(AUDIO_FREQ_PIN);
  
  // Exponential mapping for sine LFO frequency
  float newFreq = mapExponentialFloat(
      sensorValue, 
      CARRIER_FREQ_MIN, 
      CARRIER_FREQ_MAX, 
      false
  );
  
  aCarrier.setFreq(newFreq);

  // SEQUENCER OUTPUTS
  updateSequencerOutputs(FIVE_STEP_PINS, FIVE_STEP_COUNT, fivStep);
  updateSequencerOutputs(THREE_STEP_PINS, THREE_STEP_COUNT, triStep);
}

AudioOutput_t updateAudio() {
  return MonoOutput::from8Bit(aCarrier.next());
}

void loop() {
  audioHook();
}
