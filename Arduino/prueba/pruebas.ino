/*
 PRUEBAS
 Fade de Led

 This example shows how to fade an LED on pin 9
 using the analogWrite() function.

 This example code is in the public domain.
 */

byte led = 9;           // the pin that the LED is attached to
byte brightMin = 0;    // how bright the LED is
int brightness = (int)brightMin;    // how bright the LED is
int fadeAmount = 10;    // how many points to fade the LED by
unsigned long ya = millis();
byte estPin[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long tiePin[13] = {ya,ya,ya,ya,ya,ya,ya,ya,ya,ya,ya,ya,ya};
unsigned int dlPin[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};

void fadeLed(byte pinLed,byte brilloSalto,byte brilloMin,byte brilloMax,unsigned int dlMin,unsigned int dlMax) 
{
  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= brightMin || brightness >= 255) {
    fadeAmount = -fadeAmount ;
    if (brilloMax >= 255 && dlMax > 0) {
      delay(dlMax);
    }else{
      delay(dlMin);
    }
  }
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // set the brightness of pin 9:
  analogWrite(pinLed, brightness);
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  ya = millis();
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.print("YA: ");
  ya = millis();
  Serial.print(ya);  
  Serial.print("\tBRILLO: ");
  Serial.println(brightness);
  Serial.println(fadeAmount);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= brightMin || brightness >= 255) {
    fadeAmount = -fadeAmount ;
    if (brightness >= 255) {
      brightness = 255;
    }else{
      brightness = brightMin;
    }
  }
    
  // set the brightness of pin 9:
  analogWrite(led, brightness);
  // wait for 30 milliseconds to see the dimming effect
  delay(1);
}
