/*
-------------------------
EVOTest by Silvio Marano
-------------------------
Test olive oil quality using a <5mW laser diode with wavelength in 405nm-445nm range to check absorbance properties of chlorophyll and elicit its fluorescence and a TCS34725 color sensor to analyze color spectrum, considering that chlorophyll fluorescence emission peaks around 680nm-700nm wavelength and chlorophyl absorbance peaks around 400nm-450nm wavelength.
In EVO oils the chlorophyll isn't destroyed by chemical processing and heat treatments, so it gives a clue of oil quality.
It is also important to note that this test cannot directly measure the oil acidity, however if the oil has high acidity level the chlorophyll will degrade faster over the time. 


NOTES:
- The project model and firmware code are just preliminary concept versions that could be improved refining the current analysis method and adding additional modules and test routines to evaluate other aspects.
- This code uses customized libraries so they aren’t available in standard repositories. Use the libraries provided e.g., copying them from libs folder to your Arduino\libraries folder (or configuring them properly for whatever IDE you use).
- This code has been optimized for the use with STM32F030F4P6 and has been tested on STM32F030F4 Demo Board, in order to fit in small flash of this board the default Arduino USART serial support must be disabled.
- Check documentation for more details.


Version: 0.1a

Website:
www.silviomarano.tk

*/
#include <miniOled.h>
#include <Micro_TCS34725.h>

Micro_TCS34725 tcs = Micro_TCS34725(TCS34725_ADDRESS, TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_60X);

bool calibrationMode = false;             // manually set to true for testing
bool sensorWorks = false;                 // sensor initialization finished without errors
bool detecting = false;                   // true if sensor is detecting light
bool detected = false;                    // detection cycle is completed
int waitCounter = 0;                      // to count the number of cycles without anything sensed
int sampleCounter = 0;                    // to count the samples used to calculate the average values for the sample
bool displayOff = false;                  // display standby status
int score = -1;                           // olive oil quality score, -1 means it isn't not olive oil
uint16_t red, green, blue, clear;         // sensor RGBC channels output
int redRes, greenRes, blueRes, clearRes;  // average RGBC channels values used for result calc
const int maxSamples = 100;               // the max number of reads used for estimate the average RGB values of the sample
uint16_t oilSample[4][maxSamples];        // the oil sample matrix with the RGB reads of the sensor


void setup() {

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  if (tcs.begin()) {
    //Serial.println("Found sensor");
    sensorWorks = true;
  }
  // initialize digital pin PA1 attached to TCS34725 color sensor LED pin as an output.
  pinMode(D1, OUTPUT);
  // turn off TCS34725 color sensor LED
  digitalWrite(D1, LOW);

  Oled.init();  //initialze OLED display
  Oled.wideFont = false;
  Oled.smallFont = true;
  Oled.chrSpace = 1;

  if (sensorWorks) {
    Oled.printString("EVOTest by S. Marano ", 0, 0);
    Oled.drawLine(1, 0xa0);
    //Oled.printString("Carica un campione e  clicca il pulsante.", 0, 2);
    Oled.printString("Load a sample, and  click the button.", 0, 2);
  } else {
    Oled.printString("Sensor Error", 0, 0);
  }
}

void loop() {
  tcs.getRGBC(&red, &green, &blue, &clear);
  // if all color channels are at 0 it means that color sensor is detecting nothing so there isn't any light in the detection chambers of the device, so we are in a waiting status
  if (red > 0 | green > 0 | blue > 0) {
    // if the display is in standby it must be powered on again
    if (displayOff) {
      Oled.setPowerOn();
    }
    detecting = true;
    if (!detected) {
      Oled.clearDisplay();
      //Oled.printString("Analisi...", 2, 2);
      Oled.printString("Analyzing...", 2, 2);
    }
    // till the button is pressed and the counter hasn't reached max, the oil sample matrix is populated with a new read each cycle
    if (sampleCounter < maxSamples) {
      oilSample[sampleCounter][0] = red;
      oilSample[sampleCounter][1] = green;
      oilSample[sampleCounter][2] = blue;
      oilSample[sampleCounter][3] = clear;
      sampleCounter++;
    } else {
      // if we have reached max number of samples the matrix can hold, the second half of the matrix is overwritten
      sampleCounter = maxSamples / 2;
    }

    // blinking on board LED to indicate the activity
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);

    detected = true;
    waitCounter = 0;
  } else {
    detecting = false;
    // it isn't healthy to have the display always on, so if it isn't detected any activity for long, the display is switched off
    if (waitCounter > 5000) {
      if (!displayOff) {
        Oled.clearDisplay();
        Oled.setPowerOff();
        displayOff = true;
      }
    } else {
      waitCounter++;
    }
  }
  if (detected && !detecting) {
    Oled.clearDisplay();
    //Oled.printString("Valutazione...", 2, 2);
    Oled.printString("Evaluating...", 2, 2);

    // now we calculate the average value and then use this value to determine the score
    int redsum = 0, greensum = 0, bluensum = 0, clearSum = 0;
    for (int i = 0; i < sampleCounter; ++i) {
      redsum += oilSample[i][0];
      greensum += oilSample[i][1];
      bluensum += oilSample[i][2];
      clearSum += oilSample[i][3];
    }
    redRes = redsum / sampleCounter;
    greenRes = greensum / sampleCounter;
    blueRes = bluensum / sampleCounter;
    clearRes = clearSum / sampleCounter;
    sampleCounter = 0;
    score = evoTest(redRes, greenRes, blueRes);

    Oled.clearDisplay();
    if (calibrationMode) {
      sampleDetails();
    } else {
      if (score > 95) {
        //Oled.printString("Ottimo olio d'oliva  extravergine!", 0, 0);
        Oled.printString("Excellent extra virgin olive oil!", 0, 0);
      } else if (score > 65) {
        //Oled.printString("Buon olio d'oliva  extravergine!", 0, 0);
        Oled.printString("Good extra virgin  olive oil!", 0, 0);
      } else if (score > 58) {
        //Oled.printString("Olio d'oliva di  media qualita'!", 0, 0);
        Oled.printString("Medium quality olive oil!", 0, 0);
      } else if (score > 45) {
        //Oled.printString("Olio d'oliva di  bassa qualita'!", 0, 0);
        Oled.printString("Low quality olive oil!", 0, 0);
      } else if (score > -1) {
        //Oled.printString("Olio d'oliva di  pessima qualita'!", 0, 0);
        Oled.printString("Bad quality olive oil!", 0, 0);
      } else {
        //Oled.printString("Non e' olio d'oliva!", 0, 0);
        Oled.printString("It's definitely not  olive oil!", 0, 0);
      }
      Oled.drawLine(2, 0xe0);
      //Oled.printString("PUNTEGGIO: ", 3, 3);
      Oled.printString("SCORE: ", 3, 3);
      Oled.printInt(score, 4, 4);
    }
    detected = false;
  }
}

void sampleDetails() {
  Oled.printString(" R: ", 0, 0);
  Oled.printInt(redRes, 4, 0);
  Oled.printString(" G: ", 0, 1);
  Oled.printInt(greenRes, 4, 1);
  Oled.printString(" B: ", 0, 2);
  Oled.printInt(blueRes, 4, 2);
  Oled.printString(" C: ", 0, 3);
  Oled.printInt(clearRes, 4, 3);
  Oled.printString(" Score: ", 0, 4);
  Oled.printInt(score, 4, 4);
}

// assign a score to the oil sample according the color spectrum detected in reaction to violet laser
int evoTest(uint16_t redSensor, uint16_t greenSensor, uint16_t blueSensor) {
  int result = 0;
  int greenBlue = greenSensor + blueSensor;
  int redMargin = redSensor - greenBlue;
  if (redMargin > 0) {
    // good absorbance and red fluorescence, likely extravirgin olive oil
    if (redSensor > 2 * greenBlue) {
      // very good absorbance and significant red fluorescence, likely a good quality extravirgin olive oil
      result += 20;
      if (greenBlue < 20) {
        // excellent absorbance and significant red fluorescence, likely a top quality extravirgin olive oil
        result += 20;
      }
    }
    result += 58 + (redMargin / 20);
  } else if (greenBlue <= 2 * redSensor) {
    // absorbance low for an evo and red fluorescence weak, likely a cheap olive oil
    result += 48 + (redMargin / 20);
  } else {
    // absorbance too low and red fluorescence insignificant if any, it's not olive oil
    result = -1;
  }
  return result;
}
