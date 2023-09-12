#include <WiFi.h>
#include <HTTPClient.h>
int area=0;
// Set our wifi name and password
const char* ssid = "AndroidShare_5544";
const char* password = "123456789";
String serverName = "https://api.thingspeak.com/update?api_key=1O0QAQHPUF36PWH0";
unsigned long lastTime = 0;
unsigned long timerDelay = 60000;

#include<ESP32Servo.h>
Servo servo_up;
Servo servo_down;

#define echoPin 25
#define trigPin 33


long duration;
int distance,cntr=0; 

int l,ll,lr,w,wl,wr,h,m;

void setup() {
 // pinMode(34,INPUT);
  Serial.begin(9600);
  // servo
  servo_up.attach(13);
  servo_down.attach(12);
  //ultrasonic
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  
  // Opens up the serial port with a baudrate of 9600 bits per second
  WiFi.begin(ssid, password); // Attempt to connect to wifi with our password
  Serial.println("Connecting"); // Print our status to the serial monitor
  // Wait for wifi to connect
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
void ultrasonic()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2; //formula to calculate the distance for ultrasonic sensor
    delay(500);
}
void loop() {
  for(int i=0;i<=181;i++)
{
   if(i==26)
   {
    servo_down.write(26);
    ultrasonic();
    ll=distance;
   }
   else if(i==100)
   {
    servo_down.write(100);
    ultrasonic();
    wl=distance;
    Serial.print("w: ");
    Serial.println(wl);
   }
    if(i==180)
   {
    servo_up.write(37);
    delay(10);
    servo_down.write(180);
    ultrasonic();
    lr=distance;
      l=ll+lr;
      Serial.print("L: ");
      Serial.println(l);
   }
  else if(i==143){
   servo_up.write(143);
   ultrasonic();
   h=distance;
   Serial.print("h: ");
   Serial.println(h);
  }
  
  delay(30);
}

  area= l*(wl*2)*h;
   area=area*0.01;
  Serial.print("area in m: ");
  Serial.println(area);
  if ((millis() - lastTime) > timerDelay) { // Check if its been a minute
    if(WiFi.status()== WL_CONNECTED){ // Check to make sure wifi is still connected
     
      sendData(area); // Call the sendData function defined below
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
void sendData(int y){
  HTTPClient http; // Initialize our HTTP client
Serial.println(y);

  String url = serverName + "&field1=" + y; // Define our entire url
      
  http.begin(url.c_str()); // Initialize our HTTP request
      
  int httpResponseCode = http.GET(); // Send HTTP request
      
  if (httpResponseCode > 0){ // Check for good HTTP status code
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }else{
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
