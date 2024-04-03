// Navn: Kasper Fjelldalen Lurås
// Klide 1: https://projecthub.arduino.cc/Manusha_Ramanayake/wireless-water-tank-level-meter-with-alarm-ce92f6

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic1(38, 37);     // Trigpin and Echopin

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C Address, Cols, Rows

const int GreenLed  = 41;   // Assign pin 41 to the Green Led
const int YellowLed = 40;   // Assign pin 40 to the Yellow Led
const int RedLed    = 39;   // Assign pin 39 to the Red Led
const int ButtonPin = 36;   // Assign Pin 36 to the Button
const int BuzzerPin = 35;   // Assign Pin 35 to the Buzzer

const byte Full  = 8;    
const byte Empty = 30;

const unsigned long BacklightTimeout = 10000;
unsigned long       BacklightOnTime  = 0;

void loop();
void GreenLight();
void YellowLight();
void RedLightandBuzz();
void printpercent(int percent);
void Backlight();
int  ReadWaterLevel();

void setup()
{
  Serial.begin(115200);

  lcd.init();        // Initialize lcd screen
  lcd.clear();       // Clear the lcd

  pinMode(GreenLed,  OUTPUT);   // Set Green Led as Output
  pinMode(YellowLed, OUTPUT);   // Set Yellow Led as Output
  pinMode(RedLed,    OUTPUT);   // Set Red Led as Output
  pinMode(BuzzerPin, OUTPUT);   // Set Buzzer as Output
  pinMode(ButtonPin, INPUT_PULLUP);   // Set Button as Output (Pullup)
}

void loop()
{
  int WaterLevel = 0;

  WaterLevel = ReadWaterLevel();  // Read WaterLevel in percent

  if (WaterLevel <= 20)
  {
    RedLightandBuzz();
    printpercent(WaterLevel);
    lcd.setCursor(2, 0);
    lcd.print("Empty! Fill!");
  }
  else if ((WaterLevel > 20) && (WaterLevel < 60))
  {
    YellowLight();
    printpercent(WaterLevel);
  }
  else if (WaterLevel >= 60)
  {
    GreenLight();
    printpercent(WaterLevel);
  }
   
  Backlight();
}

// Reads water level and maps distance to perecent
int ReadWaterLevel()
{
  unsigned int distance = 0;
  int percent = 0;

  distance = ultrasonic1.read(CM);
  if (distance > Empty)
  {
    distance = Empty;
  }
  else if(distance < Full)
  {
    distance = Full;
  }
  percent = map(distance, Full, Empty, 100, 0); // Change from distance to percent 0-100
  
  Serial.print(distance);
  Serial.print("cm");

  Serial.print(" ");

  Serial.print(percent);
  Serial.println("%");

  return(percent);
}

// Turns on green led and turns off all other leds
void GreenLight()
{
  digitalWrite(YellowLed, LOW);
  digitalWrite(RedLed, LOW);

  digitalWrite(GreenLed, HIGH);
  delay(25);
}

// Turns on yellow led and turn off all other leds
void YellowLight()
{
  digitalWrite(GreenLed, LOW);
  digitalWrite(RedLed, LOW);

  digitalWrite(YellowLed, HIGH);
  delay(25);
}

// Start red led sequence and buzzer also turns off all other leds
void RedLightandBuzz()
{
  digitalWrite(GreenLed, LOW);
  digitalWrite(YellowLed, LOW);

  digitalWrite(RedLed, HIGH);
  tone(BuzzerPin, 250);
  delay(200);
  digitalWrite(RedLed, LOW);
  noTone(BuzzerPin);
  delay(200);
}

// Prints the water precentage
void printpercent(int percent)
{
  lcd.clear();

  if (percent == 100)
  {
    lcd.setCursor(5, 1);
    lcd.print(percent);
    lcd.print("%");
  }
  else if ((percent >= 10) && (percent < 100))
  {
    lcd.setCursor(6, 1);
    lcd.print(percent);
    lcd.print("%");
  }
  else if (percent < 10)
  {
    lcd.setCursor(7, 1);
    lcd.print(percent);
    lcd.print("%");
    
  }
}

// Turn on LCD backlight if button pressed, also turn off backlight after timeout
void Backlight()
{
  if (digitalRead(ButtonPin) == LOW)
  {
    lcd.display();
    lcd.backlight();
    BacklightOnTime = millis();
  }

  if ((millis() - BacklightOnTime) >= BacklightTimeout)
  {
    lcd.noBacklight();
    lcd.noDisplay();
  }  
}