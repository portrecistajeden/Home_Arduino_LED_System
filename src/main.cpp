#include <Arduino.h>
#include <Encoder.h>
#include <Bounce2.h>

#define LED1 10
#define LED2 9
#define LED3 6

#define ENCODER1_CLK 8
#define ENCODER1_DT 7
#define ENCODER2_CLK 5
#define ENCODER2_DT 4

#define BUTTON1 15
#define BUTTON2 16
#define BUTTON3 17
#define BUTTON4 18

void getValueFromEncoder(Encoder encoder, int &LED_BRIGHTNESS, int &OLD_POSITION);

Encoder Encoder1(ENCODER1_CLK, ENCODER1_DT);
Encoder Encoder2(ENCODER2_CLK, ENCODER2_DT);
int ENCODER1_OLD_POSITION = 0;
int ENCODER2_OLD_POSITION = 0;
Bounce SelectButton1 = Bounce();
Bounce SelectButton2 = Bounce();
Bounce SelectButton3 = Bounce();
Bounce SelectButton4 = Bounce();

byte LEDS_STATE = 0x00;

// INITIAL BRIGHTNESS
int LED1_BRIGHTNESS = 100;
int LED2_BRIGHTNESS = 50;
int LED3_BRIGHTNESS = 150;

int LED1_TEMP_BRIGHTNESS = 0;
int LED2_TEMP_BRIGHTNESS = 0;
int LED3_TEMP_BRIGHTNESS = 0;

bool LED1_ON = true;
bool LED1_TURN_ON = false;
bool LED1_TURN_OFF = false;
bool LED2_ON = true;
bool LED2_TURN_ON = false;
bool LED2_TURN_OFF = false;
bool LED3_ON = true;
bool LED3_TURN_ON = false;
bool LED3_TURN_OFF = false;

void setup()
{
  Serial.begin(9600);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  pinMode(BUTTON1, INPUT);
  SelectButton1.attach(BUTTON1, INPUT);
  SelectButton1.interval(5);
  pinMode(BUTTON2, INPUT);
  SelectButton2.attach(BUTTON2, INPUT);
  SelectButton2.interval(5);
  pinMode(BUTTON3, INPUT);
  SelectButton3.attach(BUTTON3, INPUT);
  SelectButton3.interval(5);
  pinMode(BUTTON4, INPUT);
  SelectButton4.attach(BUTTON4, INPUT);
  SelectButton4.interval(5);
}

void loop()
{
  SelectButton1.update();
  if (SelectButton1.rose())
  {
    LED1_ON = !LED1_ON;
    if (LED1_ON)
    {
      LED1_TURN_ON = true;
      LED1_TURN_OFF = false;
    }
    else
    {
      LED1_TEMP_BRIGHTNESS = LED1_BRIGHTNESS;
      LED1_TURN_ON = false;
      LED1_TURN_OFF = true;
    }
  }

  SelectButton2.update();
  if (SelectButton2.rose())
  {
    LED2_ON = !LED2_ON;
    if (LED2_ON)
    {
      LED2_TURN_ON = true;
      LED2_TURN_OFF = false;
    }
    else
    {
      LED2_TEMP_BRIGHTNESS = LED2_BRIGHTNESS;
      LED2_TURN_ON = false;
      LED2_TURN_OFF = true;
    }
  }

  SelectButton3.update();
  if (SelectButton3.rose())
  {
    LED3_ON = !LED3_ON;
    if (LED3_ON)
    {
      LED3_TURN_ON = true;
      LED3_TURN_OFF = false;
    }
    else
    {
      LED3_TEMP_BRIGHTNESS = LED3_BRIGHTNESS;
      LED3_TURN_ON = false;
      LED3_TURN_OFF = true;
    }
  }

  SelectButton4.update();
  if (SelectButton4.rose())
  {
    if (LED1_ON || LED2_ON || LED3_ON)
    {

      if (LED1_ON)
      {
        LED1_TEMP_BRIGHTNESS = LED1_BRIGHTNESS;
        LED1_TURN_ON = false;
        LED1_TURN_OFF = true;
        LED1_ON = false;
      }

      if (LED2_ON)
      {
        LED2_TEMP_BRIGHTNESS = LED2_BRIGHTNESS;
        LED2_TURN_ON = false;
        LED2_TURN_OFF = true;
        LED2_ON = false;
      }

      if (LED3_ON)
      {
        LED3_TEMP_BRIGHTNESS = LED3_BRIGHTNESS;
        LED3_TURN_ON = false;
        LED3_TURN_OFF = true;
        LED3_ON = false;
      }
    }
    else if (!LED1_ON && !LED2_ON && !LED3_ON)
    {
      LED1_ON = true;
      LED2_ON = true;
      LED3_ON = true;

      LED1_BRIGHTNESS = 100;
      LED2_BRIGHTNESS = 50;
      LED3_BRIGHTNESS = 100;

      LED1_TURN_ON = true;
      LED1_TURN_OFF = false;

      LED2_TURN_ON = true;
      LED2_TURN_OFF = false;

      LED3_TURN_ON = true;
      LED3_TURN_OFF = false;
    }
  }

  // LED1
  if (LED1_ON)
  {
    if (LED1_TURN_ON)
    {
      if (LED1_TEMP_BRIGHTNESS < LED1_BRIGHTNESS)
      {
        LED1_TEMP_BRIGHTNESS++;
        analogWrite(LED1, LED1_TEMP_BRIGHTNESS);
        delay(5);
      }
      else
        LED1_TURN_ON = false;
    }
    else
    {
      analogWrite(LED1, LED1_BRIGHTNESS);
      getValueFromEncoder(Encoder1, LED1_BRIGHTNESS, ENCODER1_OLD_POSITION);
    }
  }
  else
  {
    if (LED1_TURN_OFF)
    {
      if (LED1_TEMP_BRIGHTNESS > 0)
      {
        LED1_TEMP_BRIGHTNESS--;
        analogWrite(LED1, LED1_TEMP_BRIGHTNESS);
        delay(5);
      }
      else
        LED1_TURN_OFF = false;
    }
    else
      analogWrite(LED1, 0);
  }
  // LED1 END

  // LED2
  if (LED2_ON)
  {
    if (LED2_TURN_ON)
    {
      if (LED2_TEMP_BRIGHTNESS < LED2_BRIGHTNESS)
      {
        LED2_TEMP_BRIGHTNESS++;
        analogWrite(LED2, LED2_TEMP_BRIGHTNESS);
        delay(5);
      }
      else
        LED2_TURN_ON = false;
    }
    else
    {
      analogWrite(LED2, LED2_BRIGHTNESS);
      getValueFromEncoder(Encoder2, LED2_BRIGHTNESS, ENCODER2_OLD_POSITION);
    }
  }
  else
  {
    if (LED2_TURN_OFF)
    {
      if (LED2_TEMP_BRIGHTNESS > 0)
      {
        LED2_TEMP_BRIGHTNESS--;
        analogWrite(LED2, LED2_TEMP_BRIGHTNESS);
        delay(5);
      }
      else
        LED2_TURN_OFF = false;
    }
    else
      analogWrite(LED2, 0);
  }
  // LED2 END

  // LED3
  if (LED3_ON)
  {
    if (LED3_TURN_ON)
    {
      if (LED3_TEMP_BRIGHTNESS < LED3_BRIGHTNESS)
      {
        LED3_TEMP_BRIGHTNESS++;
        analogWrite(LED3, LED3_TEMP_BRIGHTNESS);
        delay(5);
      }
      else
        LED3_TURN_ON = false;
    }
    else
    {
      analogWrite(LED3, LED3_BRIGHTNESS);
    }
  }
  else
  {
    if (LED3_TURN_OFF)
    {
      if (LED3_TEMP_BRIGHTNESS > 0)
      {
        LED3_TEMP_BRIGHTNESS--;
        analogWrite(LED3, LED3_TEMP_BRIGHTNESS);
        delay(5);
      }
      else
        LED3_TURN_OFF = false;
    }
    else
      analogWrite(LED3, 0);
  }
  // LED3 END
}

void getValueFromEncoder(Encoder encoder, int &LED_BRIGHTNESS, int &OLD_POSITION)
{
  int NEW_POSITION = encoder.read();
  if (NEW_POSITION > OLD_POSITION && LED_BRIGHTNESS < 255)
    LED_BRIGHTNESS++;
  else if (NEW_POSITION < OLD_POSITION && LED_BRIGHTNESS > 1)
    LED_BRIGHTNESS--;

  OLD_POSITION = NEW_POSITION;
}
