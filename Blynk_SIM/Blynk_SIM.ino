#define BLYNK_PRINT SwSerial
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX
    
#include <BlynkSimpleStream.h>
#include <DHT.h>

int relayPin = 8;
int sensor_analog;
float moisture_percentage;
const int sensor_pin = A1; /* Soil moisture sensor O/P pin */

char auth[] = "af4fe45d97a24d1c92bc16e07778cb66";

#define DHTPIN 2          // What digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
DHT dht(DHTPIN, DHTTYPE);

BlynkTimer timer;

void sendSensor()
{
  sensor_analog = analogRead(sensor_pin);
  moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit  
  if (isnan(h) || isnan(t)) 
  {
    SwSerial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V7, moisture_percentage);
}

WidgetTerminal terminal(V1);
BLYNK_WRITE(V1)
{
  if (String("Soil Moisture Test!") == param.asStr()) 
  {
    terminal.println("You said: 'Soil Moisture Test!'") ;
    terminal.println("I said: 'Look for the values under Soil Moisture GAUGE.'") ;
  } 
  else 
  {
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  
  if (String("DHT22 Test!") == param.asStr()) 
  {
    terminal.println("You said: 'DHT22 Test!'") ;
    terminal.println("I said: 'Look for the Temperature and Humidity GAUGE above.'") ;
  } 
  else 
  {
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }
  
  terminal.flush();
}


void setup()
{
  SwSerial.begin(9600);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);

  pinMode(relayPin, OUTPUT);

  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("--------------------"));
  terminal.println(F("Welcome To the Demonstration of SIM"));
  terminal.println(F("Here we perform various tests like :"));
  terminal.println(F("Soil Moisture Test!"));
  terminal.println(F("DHT22 Test!"));
  terminal.println(F("Relay Module Operation Test!"));
  terminal.flush();

  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop()
{ 
  Blynk.run();
  timer.run();
}

