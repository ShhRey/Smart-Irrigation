  int relayPin = 8;
  #include "DHT.h"
  #define DHTPIN 2 // what digital pin we're connected to
  const int sensor_pin = A1; /* Soil moisture sensor O/P pin */
  #define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
  DHT dht(DHTPIN, DHTTYPE);
  
  void setup() 
  {
    Serial.begin(9600); /* Define baud rate for serial communication */
    pinMode(relayPin, OUTPUT);
    Serial.println("DHT22 Test!");
    Serial.println("Soil Moisture Test!");
    dht.begin();
  }
  void loop() 
  {
    // Wait a few seconds between measurements.
    delay(2000);
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    
    float f = dht.readTemperature(true);
    float moisture_percentage;
    int sensor_analog;
    sensor_analog = analogRead(sensor_pin);
    moisture_percentage = ( 100 - ( (sensor_analog/1023.00) * 100 ) );
    Serial.print("Moisture Percentage = ");
    Serial.print(moisture_percentage);
    Serial.print("%\n\n");
    delay(1000); 
    
    if (isnan(h) || isnan(t) || isnan(f)) 
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(" \t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print("\n");
  if (moisture_percentage > 10) 
  {
    digitalWrite(relayPin, HIGH);
  }
  else {
    digitalWrite(relayPin, LOW); 
  }
 }
