// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2); // 0x20 is adresss for LCC 16x2

byte grad[8] = {
  B01110,
  B10001,
  B10001,
  B01110,
  B00000,
  B00000,
  B00000,
};

const int numar = 30;
float t[numar];
byte err[numar];
float temed = 0.;
float teok = 0.;
float dete = 0.1;
byte j = 0;
float tempC = 0.;
byte liniar = 1;

unsigned long citire;
unsigned long timp = 1000;

#define led 7

/*
 * The setup function. We only start the sensors here
 */
void setup(void)
{
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  
 // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  lcd.begin(); // initialise the lcd 
  lcd.createChar(0, grad); // create custom char
  lcd.backlight(); //backlight is now ON
  lcd.setCursor(1, 0);
  lcd.print("ArduinotehNiq");  
  lcd.setCursor(0, 1);
  lcd.print("made by niq_ro");
  delay (2500);
  lcd.clear();

  // Start up the library
  sensors.begin();

  // More on resolution: https://homautomation.org/2015/11/17/ds18b20-how-to-change-resolution-9-10-11-12-bits/
  sensors.setResolution(10); 
  /*
   *   9bits 0.5*C
   *  10bits 0.25*C
   *  11bits 0.125*C
   *  12bits 0.0625*C
   */

 for (byte i = 0; i <= numar; i++) 
 {
  t[i] = 0.;
  err[i] = 0;
 }
 
 }  // end setup loop

void loop(void)
{ 

if (millis() - citire > timp)
{
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
citire = millis();
 t[j] = tempC;
  Serial.print("t[");
  Serial.print(j);
  Serial.print("] = ");
  Serial.print(t[j]);
  Serial.println("*C");

  lcd.setCursor(9,0);
  if (t[j] < 10) lcd.print(" ");
  lcd.print(t[j]);
  lcd.write(byte(0));
  lcd.print("C ");
  
j = j + 1;
  lcd.setCursor(0,0);
  if (j < 10) lcd.print(" ");
  lcd.print(j);
if (j == numar) j = 0;
}

temed = 0;
 for (byte i = 0; i < numar; i++) 
 {
  temed = temed + t[i];
 }
temed = (float)temed/numar;
  Serial.print("temed");
  Serial.print(j);
  Serial.print(" = ");
  Serial.print(temed);
  Serial.println("*C");
  
 lcd.setCursor(0,1);
  if (temed < 10) lcd.print(" ");
  lcd.print(temed);
  lcd.write(byte(0));
  lcd.print("C ");

liniar = 0;
for (byte i = 0; i < numar; i++) 
 {
  if ((t[i] < temed - dete) or (t[i] > temed + dete))
  {
    err[i] = 1;  // temperature != medium temperature

  }
  else
  {
    err[i] = 0;
  }
  Serial.print("err[");
  Serial.print(i);
  Serial.print("] = ");
  Serial.println(err[i]);
liniar = liniar + err[i];
  }
  Serial.print("liniar = ");
  Serial.println(liniar);
if (liniar == 0)  // temperature is constant for long time
{
  teok = temed;
  digitalWrite(led, HIGH);
  lcd.setCursor(9,1);
  if (teok < 10) lcd.print(" ");
  lcd.print(teok);
  lcd.write(byte(0));
  lcd.print("C ");
}
else
{
  digitalWrite(led, LOW);
}

teok = 0;
  
 delay(500);

} // end main loop
