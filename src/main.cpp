#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic1(38, 37); // TrigpiN and Echopin

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C Address, Cols, Rows

const int GreenLed  = 41;   // Assign pin D0 to the Green Led
const int YellowLed = 40;   // Assign pin D4 to the Yellow Led
const int RedLed    = 39;   // Assign pin D3 to the Red Led

const byte Full  = 8;    
const byte Empty = 30;

void loop();
void GreenLight();
void YellowLight();
void RedLightandBuzz();
void printpercent(int percent);
int  ReadWaterLevel();

void setup()
{
  Serial.begin(115200);

  lcd.init();        // Initialize lcd screen
  lcd.backlight();   // Turn on backlight on lcd
  lcd.clear();       // Clear the lcd

  pinMode(GreenLed, OUTPUT);    // Set Green Led as Output
  pinMode(YellowLed, OUTPUT);   // Set Yellow Led as Output
  pinMode(RedLed, OUTPUT);      // Set Red Led as Output
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
}

void GreenLight()
{
  digitalWrite(YellowLed, LOW);
  digitalWrite(RedLed, LOW);

  digitalWrite(GreenLed, HIGH);
  delay(400);
}

void YellowLight()
{
  digitalWrite(GreenLed, LOW);
  digitalWrite(RedLed, LOW);

  digitalWrite(YellowLed, HIGH);
  delay(400);
}

void RedLightandBuzz()
{
  digitalWrite(GreenLed, LOW);
  digitalWrite(YellowLed, LOW);

  digitalWrite(RedLed, HIGH);
  delay(300);
  digitalWrite(RedLed, LOW);
  delay(300);
}

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

int ReadWaterLevel()
{
  unsigned int distance = 0;
  int percent = 0;

  distance = ultrasonic1.read(CM);
  if (distance > Empty) {
  distance = Empty;
  } else if(distance < Full){
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