// Libraries
#include <DHT.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 
SoftwareSerial SIM900(2,3); // RX, TX
// Constants
#define RAIN_SENSOR_PIN 4
#define FIRE_SENSOR_PIN 6
#define DHT_PIN 5
#define DHT_TYPE DHT11
#define MOISTURE_SENSOR_PIN A2
const int Waterlevel_Pin = A1;

#define RelayFan 9
#define RelayPump 10
#define buzerPin1 11
#define buzerPin2 12

#define DTH_INTERVAL 0 // Read sensors every 1000ms
#define LCD_INTERVAL 50
#define Rain_INTERVAL 10
#define Fire_INTERVAL 0
#define Mois_INTERVAL 5
#define Water_INTERVAL 0
#define READ_INTERVAL 500
#define Ultra_INTERVAL 0
#define sendSms_INTERVAL 1

#define ledPin 13
// Objects
DHT dht(DHT_PIN, DHT_TYPE);
unsigned long previousMillis = 50;  // Keep track of time since last sensor reading
unsigned long previousMillis_Rain = 0;
unsigned long previousMillis_Fire = 0;
unsigned long previousMillis_DTH = 0;
unsigned long previousMillis_Mois =0;
unsigned long previousMillis_Water =0;
unsigned long previousMillis_Lcd =0;
unsigned long previousMillis_Ultra =0;
unsigned long previousMillis_sendSms =0;
// Sensor values
int rainValue = 0;
int fireValue = 0;
float temp = 0.0;
float humidity = 0.0;
int moistureValue = 0;
float Water =0;
const int trigPin = 7;
const int echoPin = 8;


long duration;
int distance;

void setup() {
  
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);

  
   pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(RelayFan,OUTPUT);
  pinMode(RelayPump,OUTPUT);
pinMode(buzerPin1,OUTPUT);
pinMode(buzerPin2,OUTPUT);
  
pinMode(RelayFan,OUTPUT);
  
  dht.begin();

  lcd.init();               
  lcd.init();
  lcd.backlight();

  pinMode(RAIN_SENSOR_PIN, INPUT);
  SIM900.begin(19200);
  delay(1000);
  SIM900.print("AT+CMGF=1\r"); // Set SMS mode to text
  delay(100);
  SIM900.println("AT+CNMI=2,2,0,0,0\r"); // Configure module to send SMS data to serial out upon receipt 
  delay(100);
}

void loop() {
  unsigned long currentMillis = millis();  // Get the current time
 // if (currentMillis - previousMillis >= READ_INTERVAL) {
    // Update the previous reading time
  //  previousMillis = currentMillis;

    // Call the functions to read the senso


//}

  // Check if it's time to read the sensors
 


if (currentMillis - previousMillis_Mois >= Mois_INTERVAL) 
  {
    previousMillis_Mois = currentMillis;
    
  moistureValue = analogRead(MOISTURE_SENSOR_PIN);
Serial.println(moistureValue) ; //Serial.println(moistureValue);
 if (moistureValue > 900)
 {  
    digitalWrite(RelayPump,HIGH); 
    Serial.print("The soil is DRY : ");
    Serial.println("Pump is ON      ");
 }
 
if (moistureValue <350)
{
digitalWrite(RelayPump,LOW);   
Serial.println("Pump is OFF      ");
Serial.println("The soil is WET ");
}
}


if (currentMillis - previousMillis_Rain >= Rain_INTERVAL) 
  {
    previousMillis_Rain = currentMillis; 
 int rainValue = digitalRead(RAIN_SENSOR_PIN);

  if (rainValue == LOW) {
    // Turn on the LED
    digitalWrite(ledPin, LOW);
    Serial.println("Rainning");
   sendSMS("+918768795298", "Rainning!");
   delay(10000);
  } else {
    // Turn off the LED
    digitalWrite(ledPin, LOW);
  }
}
if (currentMillis - previousMillis_Fire >= Fire_INTERVAL) 
  {
    previousMillis_Fire = currentMillis; 
  fireValue = digitalRead(FIRE_SENSOR_PIN);
  //Serial.println(fireValue);
  if(fireValue == LOW)
  {
 //for(int i = 0; i < 5; i++) {
   // digitalWrite(buzerPin1, HIGH); // turn on the LED
    //delay(500);             // wait for 500 milliseconds
    //digitalWrite(buzerPin1, LOW);  // turn off the LED
    //delay(500);        
 
  //Serial.print("Fire sensor value: ");
//Serial.println("Firing");
//for(int j = 0; j < 2; j++)
//{
Serial.println("Fire Detected"); 
 sendSMS("+918768795298", "Fire Detected!");
delay(5000);
//}
  }
}

if (currentMillis - previousMillis_Water >= Water_INTERVAL) 
  {
    previousMillis_Water = currentMillis;


  int sensorValue = analogRead(Waterlevel_Pin );
  int percent = map(sensorValue, 0, 1023, 0, 100);
  Serial.print("Water level: ");
  Serial.print(percent);
  Serial.println("%");
  if (percent >= 60) {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzerPin2,HIGH); // turn on the LED
       Serial.println("Tank is Full");
  } 
  else if(percent <= 50){
    digitalWrite(buzerPin2,LOW);

   //for(int k = 0;k < 1; k++) {
//sendSMS("+919064111712", "Tank is Full, Please Turn Off the Pump");
  // delay(1000);
    ///}   
  }else {
    //digitalWrite(ledPin, LOW);
    digitalWrite(buzerPin2, LOW);    
  }
  //delay(500);  
  

}

if (currentMillis - previousMillis_DTH >= DTH_INTERVAL) 
  {
    previousMillis_DTH = currentMillis;
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println("℃\t");
 //Serial.print("Humidity: ");
//Serial.print(humidity);
  //Serial.println("%");
if (temp >30)
{
digitalWrite(RelayFan,HIGH);  
Serial.println("Fan is On");
//sendSMS("+919064111712", "Fan is on");
  // delay(1000);
}
if (temp < 30)
{
  digitalWrite(RelayFan,LOW); 
Serial.println("Fan is Off");
//sendSMS("+919064111712", "Fan is off");
  // delay(1000);
}
}

if (currentMillis - previousMillis_Ultra >= Ultra_INTERVAL) 
  {
    previousMillis_Ultra = currentMillis;
{
digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 20) {
    digitalWrite(buzerPin1, HIGH);
for(int i = 0; i < 5; i++) {
   digitalWrite(buzerPin1, HIGH); // turn on the LED
   delay(500);             // wait for 500 milliseconds
digitalWrite(buzerPin1, LOW);  // turn off the LED
   delay(500); 
digitalWrite(buzerPin1, HIGH);   
}     
    sendSMS("+918768795298", "Movement detected");
   delay(5000);
  }
  
  else {
    digitalWrite(buzerPin1, LOW);
  }
  //delay(500);
}

  }
 if (currentMillis - previousMillis_Lcd >= LCD_INTERVAL) {
       previousMillis_Lcd = currentMillis;
 // lcd.setCursor(0,0);
//lcd.print(rainValue);
//delay (1000);


lcd.clear();
lcd.setCursor(0,0);
lcd.print("Temperature=");
lcd.print(temp);
lcd.setCursor(0,1);
lcd.print("Humidity=");
lcd.print(humidity);
delay(3000);
lcd.clear();


  int rainValue = digitalRead(RAIN_SENSOR_PIN);
  if (rainValue == LOW) {
    lcd.setCursor(0,0);
    lcd.print("Rainning");
  } 
  else 
  {
     lcd.setCursor(0,0);
    lcd.print(" Not Rainning");
  }  
  delay(3000);
  
if (fireValue == LOW)
{
  lcd.setCursor(0,0);
lcd.print("Fire Ditected");
delay(3000);
}

int sensorValue = analogRead(Waterlevel_Pin );
  int percent = map(sensorValue, 0, 1023, 0, 100);
lcd.setCursor(0,0);
 lcd.print("Water level: ");
  lcd.print(percent);
  lcd.print("%");
delay(3000);

if (moistureValue > 900)
 {  
   lcd.setCursor(0,0); 
    lcd.print("The soil is DRY : ");
    lcd.setCursor(0,1);    
    lcd.print("Pump IS ON     ");
 }
if (moistureValue <300)
{ 
lcd.setCursor(0,0);
lcd.print("The soil is WET ");
lcd.setCursor(0,1);
lcd.print("Pump IS OFF");
}
delay(3000);
 }

}

void sendSMS(String phone_number, String message){
//if (currentMillis - previousMillis_sendSms >= sendSms_INTERVAL) 
  //{
   // previousMillis_sendSms = currentMillis;
  SIM900.print("AT+CMGS=\"");
  SIM900.print(phone_number);
  SIM900.println("\"");
  delay(100);
  SIM900.print(message);
  delay(100);
  SIM900.write(0x1A);
  delay(100); 

}