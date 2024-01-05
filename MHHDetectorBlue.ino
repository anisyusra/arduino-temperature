/*Plant watering system with new blynk update
   https://srituhobby.com
*/
//Include the library files
#define BLYNK_TEMPLATE_ID "TMPLf0O972jM"
#define BLYNK_DEVICE_NAME "Temperature"

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

//Initialize the LCD display
LiquidCrystal_I2C lcd(0x27, 16, 2);

DHT dht(D4, DHT11); //(sensor pin,sensor type)

char auth[] = "bxk5NWRI7M292ZGTl6ihK9dh5vaAhs3L";//Enter your Auth token
char ssid[] = "AY";//Enter your WIFI name
char pass[] = "anisyusra01";//Enter your WIFI password

BlynkTimer timer;
bool Relay = 0;

//Define component pins
#define sensor A0
#define waterPump D3

void setup() {
  Serial.begin(9600);
  pinMode(waterPump, OUTPUT);
  digitalWrite(waterPump, HIGH);
  lcd.init();
  lcd.backlight();

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  lcd.setCursor(1, 0);
  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(500);
  }
  lcd.clear();

  //Call the function
  timer.setInterval(100L, soilMoistureSensor);

  Wire.begin(D2, D1);
  lcd.init();
  lcd.backlight();
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(100L, sendSensor);

}

//Get the soil moisture values
void soilMoistureSensor() {
  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;

  Blynk.virtualWrite(V0, value);
  lcd.setCursor(0, 0);
  lcd.print("Moisture :");
  lcd.print(value);
  lcd.print(" ");


}

void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    Blynk.logEvent("Alert","Warning! please water your plant.");
    return;
  }

  // lcd.setCursor(0, 0);
  // lcd.print("Temp : ");
  // lcd.print(t);
  // lcd.setCursor(0, 1);
  // lcd.print("Humi : ");
  // lcd.print(h);

  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V2, h);
}

void loop() {

  Blynk.run();

  int value = analogRead(sensor);
  value = map(value, 0, 1024, 0, 100);
  value = (value - 100) * -1;
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if(value < 30)
  {
    digitalWrite(waterPump, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Motor is ON");
  }

  else
  {
    digitalWrite(waterPump, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Motor is OFF");
    Blynk.email("2020482782@student.uitm.edu.my", "Must", "Nice");

  }

  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}