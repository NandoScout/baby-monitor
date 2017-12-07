/*
* Example 02: Ex_02_MultiBlink.ino
* By: D. Aaron Wisner
*
* In this example we are creating a "BlinkProcess" Class that will toggle a pin at a constant period
* We create 3 BlinkProcess objects that will blink at a diff. period and add them to the scheduler:
* - blink250 (pin 13)
* - blink500 (pin 12)
* - blink1000 (pin 11)
* Connect an LED to each of these pins and watch them blink
*/


#ifndef FADE_PROCESS_H
#define FADE_PROCESS_H

#include <ProcessScheduler.h>

typedef enum CicleMode
{
    ONE_WAY_ONE_TIME = 0,
    TWO_WAY_ONE_TIME,
    ONE_WAY_CICLE,
    TWO_WAY_CICLE
} CicleMode;

// Fade Process
class FadeProcess : public Process
{
public:

    FadeProcess(Scheduler &manager, ProcPriority pr, unsigned int period, int pin, 
              byte startValue, byte valueMin, byte valueMax, byte valueStep, 
              CicleMode mode, unsigned int delayMin, unsigned int delayMax, 
              Process * nextProcess) 
        :  Process(manager, pr, period, RUNTIME_FOREVER)
        {
          _period = period;
          _pin = pin; // Store the pin number
          _startValue = (startValue > valueMin ? (startValue < valueMax ? startValue : valueMax) : valueMin);
          _value = _startValue;
          _stopValue = _startValue;
          _stopping= false;
          _valueMin = valueMin;
          _valueMax = valueMax;
          _valueStep = (int)valueStep;
          _mode = mode;
          _delayMin = delayMin;
          _delayMax = delayMax;
          _nextProcess = nextProcess;
        }
        
    FadeProcess(Scheduler &manager, ProcPriority pr, unsigned int period, int pin, byte valueStep, CicleMode mode)
        :  FadeProcess(manager, pr, period, pin, 0, 0, 255, valueStep, mode, 0, 0, NULL)
        {
          //constructor simplificado
        }

    FadeProcess(Scheduler &manager, ProcPriority pr, unsigned int period, int pin)
        :  FadeProcess(manager, pr, period, pin, 0, 0, 255, 5, TWO_WAY_CICLE, 0, 0, NULL)
        {
          //constructor simplificado
        }

    void FadeProcess::setNextProcess(Process *nextProcess)
    {
          _nextProcess = nextProcess;
    }

protected:
    //setup the pins
    virtual void setup()
    {
      setupStopValue();
      pinMode(_pin, OUTPUT);
      analogWrite(_pin, _value);
    }

     // Undo setup()
    virtual void cleanup()
    {
      pinMode(_pin, INPUT);
      //_pinState = LOW;
    }
    
    //LEDs should be off when disabled
    virtual void onDisable()
    {
      //_pinState = LOW;
      //analogWrite(_pin, _pinState);
      _stopping = true;
    }
    
    //Start the LEDs on
    virtual void onEnable()
    {
      //_pinState = HIGH;
      _stopping = false;
      analogWrite(_pin, _value);
    }

    // Create our service routine
    virtual void service()
    {
        Serial.println(String("Process: ") + getID() + "-A Valor(" + _value + ") Step(" + _valueStep + ") Period(" + this->getPeriod() + ")" );
        // restablecer periodo
        if (_value == _valueMin || _value == _valueMax){
            this->setPeriod(_period);
        }
        // variar un Step
        _value = _valueStep + (int)_value;
        // evaluar limites -> igualar valor extremo + cambio de sentido Step
        if (evaluateLimit()){
            // es un limite
            Serial.println(String("Process: ") + getID() + "------------------------------LIMITE");
        } else {
            // no es un limite
        }
        // variacion del led
        analogWrite(_pin, _value);
        Serial.println(String("Process: ") + getID() + "-B Valor(" + _value + ") Step(" + _valueStep + ") Stopping(" + _stopping + ")" );
        // evaluar detencion segun modo
        evaluateMode();
        // deshabilitar en condicion de stop
        evaluateStop();
    }

private:
    int _value; //the Current value of the pin
    int _pin; // The pin the LED is on
    byte _startValue;
    byte _stopValue;
    bool _stopping;
    byte _valueMin;
    byte _valueMax;
    int _valueStep;
    CicleMode _mode;
    unsigned int _period;
    unsigned int _delayMin;
    unsigned int _delayMax;

    Process *_nextProcess;

    void setupStopValue()
    {      
        switch(_mode) {
          case ONE_WAY_ONE_TIME:
          case ONE_WAY_CICLE:
              _stopValue = _valueMax;
              break;
          case TWO_WAY_ONE_TIME:
          case TWO_WAY_CICLE:
          default:
              _stopValue = _valueMin;
              break;
        }
    }

    int evaluateLimit()
    {
      // alcanza un valor extremo
        if (_value <= _valueMin || _value >= _valueMax) {
            _valueStep = -(_valueStep);
            if (_value >= _valueMax) {
                _value = _valueMax;
                if (_delayMax > 0 ) {
                    this->setPeriod(_delayMax);
                }
            } else if (_value <= _valueMin) {
                _value = _valueMin;
                if (_delayMin > 0 ) {
                    this->setPeriod(_delayMin);
                }
            }
            return true;
        } else
            return false;
    }

    void evaluateMode()
    {      
        switch(_mode) {
          case ONE_WAY_ONE_TIME:
              if (_value == _valueMax)
                  _stopping =  true;
              break;
          case TWO_WAY_ONE_TIME:
              if (_value == _valueMin)
                  _stopping =  true;
              break;
        }
    }

    void evaluateStop()
    {
        // stopping y valor en stopValue -> detener
        if (_stopping == true && (_value > _stopValue - abs(_valueStep) && _value < _stopValue + abs(_valueStep))){
            _value = _stopValue;
            this->disable();
            if (_nextProcess != NULL){
                _nextProcess->enable();
            }
        }
    }
};

#endif

Scheduler sched; // Create a global Scheduler object

#define PIN_LED_BLAN 9
#define PIN_LED_ROJO 11
#define PIN_LED_AMAR 12
#define PIN_LED_VERD 13


// Create our blink processes

FadeProcess fadeV(sched, HIGH_PRIORITY, 1000, PIN_LED_VERD); 
FadeProcess fadeA(sched, HIGH_PRIORITY, 1000, PIN_LED_AMAR); 
FadeProcess fadeB(sched, HIGH_PRIORITY, 40, PIN_LED_BLAN, 0, 0, 250, 2, TWO_WAY_ONE_TIME, 3000, 5000, NULL); 
//  byte startValue, byte valueMin, byte valueMax, byte valueStep, CicleMode mode, unsigned int delayMin, unsigned int delayMax,
FadeProcess fadeR(sched, HIGH_PRIORITY, 30, PIN_LED_ROJO, 0, 0, 250, 2, TWO_WAY_ONE_TIME, 3000, 5000, &fadeB);

void setup()
{
  Serial.begin(9600);
  // Add and enable our blink processes
  fadeB.setNextProcess(&fadeR);
  //fadeV.add(); // .add(true)= Same as calling blink250.add() and blink250.enable();
  //fadeA.add();
  fadeR.add();
  fadeB.add();
        //fadeV.enable();
        //fadeA.enable();
        fadeR.enable();
}

void loop()
{
    unsigned int ya = millis();   
    /*
    if (ya < 300){
        fadeV.enable();
    }else if (ya < 600){
        fadeA.enable();
    }else if (ya < 900){
        fadeR.enable();
    }
    */
    sched.run();
}
