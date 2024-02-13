#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <HCSR04.h>

float distance = 0;

HCSR04 ultrasonicSensor(36, 35, 22, 40); // Trigpin, Echopin, Temperature, Max distance

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C Address, Cols, Rows

const int GreenLed = 42;  // Assign pin D0 to the Green Led
const int YellowLed = 2; // Assign pin D4 to the Yellow Led
const int RedLed = 1;   // Assign pin D3 to the Red Led

void RedLight();
void printpercent();

int percent = map(distance, 0, 40, 0, 100); // Change from distance to percent 0-100

void setup()
{
  Serial.begin(9600);

  ultrasonicSensor.begin();              // Initialize Trigpin and Echopin
  ultrasonicSensor.setTemperature(22.5); // Set temp for ultrasonic sensor

  lcd.init();      // Initialize lcd screen
  lcd.backlight(); // Turn on backlight on lcd
  lcd.clear();     // Clear the lcd

  pinMode(GreenLed, OUTPUT);  // Set Green Led as Output
  pinMode(YellowLed, OUTPUT); // Set Yellow Led as Output
  pinMode(RedLed, OUTPUT);    // Set Red Led as Output
}

void loop()
{
  int distance = ultrasonicSensor.getDistance();
  Serial.print(distance);
  Serial.println("cm");

  if (distance >= 32)
  {
    digitalWrite(GreenLed, LOW);
    digitalWrite(YellowLed, LOW);

    RedLight();
    lcd.setCursor(2, 0);
    lcd.print("Empty! Fill!");

    printpercent();
  }
  else if ((distance > 20) && (distance < 32))
  {
    digitalWrite(GreenLed, LOW);
    digitalWrite(RedLed, LOW);

    digitalWrite(YellowLed, HIGH);
  }
  else if (distance > 32)
  {
    digitalWrite(YellowLed, LOW);
    digitalWrite(RedLed, LOW);

    digitalWrite(GreenLed, HIGH);
  }
}

void RedLight()
{
  digitalWrite(RedLed, HIGH);
  delay(200);
  digitalWrite(RedLed, LOW);
  delay(200);
  digitalWrite(RedLed, HIGH);
  delay(200);
  digitalWrite(RedLed, LOW);
  delay(200);
}

void printpercent()
{
  if (percent = 100)
  {
    lcd.setCursor(5, 1);
    lcd.print(percent);
    lcd.print("%");
  }
  else if ((percent > 10) && (percent < 100))
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
