#include <Arduino.h>
#include <LowPower.h>

#define IS_BIT_SET(var, pos) ((var) & (1 << (pos)))
#define EXIT_FUNCTION_IF_NO_KEY() \
  if (!keyConnected)              \
  return

#define LED_1_PIN 5
#define LED_2_PIN 6
#define LED_4_PIN 4
#define LED_8_PIN 8
#define BTN_1_PIN 12
#define BTN_2_PIN 13
#define BUZZ_PIN 10
#define ADC_PIN 0
#define WAKE_UP_PIN 2

#define TEAMS_COUNT 2
#define DEBOUNCE_MILLIS 500
#define TWEET_DELAY_MILLIS 5000
#define DISARM_MILLIS 5000

int btn1State;
int btn2State;
int lastBtn1State;
int lastBtn2State;
unsigned long lastDebounceTimeBtn1 = 0;
unsigned long lastDebounceTimeBtn2 = 0;

int teams[] = {130, 205};

volatile int keyConnected = -1;

int tweetTimerMillis  = 0;
int disarmTimerMillis = 0;
int disarmed    = 0;
int serviceMode = 1;

void displayNumber(int n);
void readBtns();

void displayNumberEncoded(int n) 
{
  int num3 = (n / 1000) % 10;
  int num2 = (n / 100) % 10;
  int num1 = (n / 10) % 10;
  int num0 = n % 10;

  while (num0 > 0 || num1 > 0 || num2 > 0 || num3 > 0) {
    digitalWrite(LED_1_PIN, num0 > 0 ? HIGH : LOW);
    digitalWrite(LED_2_PIN, num1 > 0 ? HIGH : LOW);
    digitalWrite(LED_4_PIN, num2 > 0 ? HIGH : LOW);
    digitalWrite(LED_8_PIN, num3 > 0 ? HIGH : LOW);

    delay(300); displayNumber(0); delay(300);

    num0--; num1--; num2--; num3--;
  }
  displayNumber(0);
}

void serviceLoop()
{
  while (1) {
    displayNumberEncoded(1085);
    delay(2000);
  }
}

void disarmLoop()
{
  displayNumber(15);
  delay(10);
  displayNumber(0);
  delay(1000);

  readBtns();

  while (btn1State && btn2State && disarmTimerMillis <= DISARM_MILLIS)
  {
    unsigned long lastMillis = millis();
    delay(10);
    displayNumber(random(0, 15));
    disarmTimerMillis += (millis() - lastMillis);
    readBtns();

    EXIT_FUNCTION_IF_NO_KEY();
  }
  displayNumber(0);
  if (disarmTimerMillis > DISARM_MILLIS)
  {
    disarmed = 1;
    displayNumber(15);
  }
}

void tweet()
{
  tone(BUZZ_PIN, random(300, 1500));
  delay(30);
  noTone(BUZZ_PIN);
}

void welcome()
{
  for (int i = 0; i < 10; i++)
  {

    EXIT_FUNCTION_IF_NO_KEY();

    displayNumber(15);
    tone(BUZZ_PIN, random(300));
    delay(20);
    noTone(BUZZ_PIN);
    displayNumber(0);
    delay(20);
  }
}

void goToSleep()
{
  displayNumber(0);
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
}

void onKeyToggle()
{
  keyConnected = digitalRead(WAKE_UP_PIN);
}

int getConnectedKeyId()
{
  //Serial.print("millis ");
  //Serial.print(millis());
  //Serial.print("\n");

  int adcRaw = analogRead(ADC_PIN);

  if (adcRaw)
  {
    //Serial.print("ADC ");
    //Serial.println(adcRaw);

    for (int i = 0; i < TEAMS_COUNT; i++)
    {
      if (abs(adcRaw - teams[i]) < 10)
      {
        return i;
      }
    }
  }

  return -1;
}

void readBtns()
{
  int btn1 = digitalRead(BTN_1_PIN);
  int btn2 = digitalRead(BTN_2_PIN);

  if (btn1 != lastBtn1State)
  {
    lastDebounceTimeBtn1 = millis();
  }
  if (btn2 != lastBtn2State)
  {
    lastDebounceTimeBtn2 = millis();
  }

  if ((millis() - lastDebounceTimeBtn1) > DEBOUNCE_MILLIS)
  {
    if (btn1 != btn1State)
    {
      btn1State = btn1;
    }
  }
  if ((millis() - lastDebounceTimeBtn2) > DEBOUNCE_MILLIS)
  {
    if (btn2 != btn2State)
    {
      btn2State = btn2;
    }
  }

  lastBtn1State = btn1;
  lastBtn2State = btn2;
}

void displayNumber(int number)
{
  digitalWrite(LED_1_PIN, IS_BIT_SET(number, 0));
  digitalWrite(LED_2_PIN, IS_BIT_SET(number, 1));
  digitalWrite(LED_4_PIN, IS_BIT_SET(number, 2));
  digitalWrite(LED_8_PIN, IS_BIT_SET(number, 3));
}

void setup()
{
  Serial.begin(9600);

  pinMode(WAKE_UP_PIN, INPUT);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);
  pinMode(LED_8_PIN, OUTPUT);

  pinMode(BTN_1_PIN, INPUT);
  pinMode(BTN_2_PIN, INPUT);

  attachInterrupt(0, onKeyToggle, CHANGE);
}

void loop()
{

  onKeyToggle();

  if (keyConnected)
  {
    delay(500);
    welcome();

    while (keyConnected)
    {
      if (serviceMode)
      {
        serviceLoop();
      }
      if (!disarmed)
      {
        disarmTimerMillis = 0;
        disarmLoop();
      }
      else
      {
      }
    }
  }
  else if (tweetTimerMillis >= TWEET_DELAY_MILLIS)
  {
    tweet();
    tweetTimerMillis = 0;
  }

  goToSleep();
  tweetTimerMillis += 1000;

  //waken
}