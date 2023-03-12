#include <SPI.h>
#include <Ethernet.h>

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
/*
www.arduinopoint.com 
Fire Alarm System 
*/
EthernetClient client;

int    HTTP_PORT   = 80;
String HTTP_METHOD = "GET";
char   HOST_NAME[] = "192.168.1.7"; // change to your PC's IP address
String PATH_NAME   = "/php_fireandsmokedtct.php";
String queryString = "?temperature=29.1";

int redLed1 = 3;
int redLed2 = 4;
int greenLed = 8;
int buzzer1 = 5; //PWM (~) pin
int buzzer2 = 6; //PWM (~) pin
int gasPin = A0;
int flamePin = 2;
// Your threshold value
int gasSensorThres = 400;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:


void setup() {
  Serial.begin(9600);
  
  pinMode(redLed1, OUTPUT);  
  pinMode(redLed2, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(gasPin, INPUT);
  pinMode(flamePin, INPUT);

  
  // connect to web server on port 80:
  if(client.connect(HOST_NAME, HTTP_PORT)) {
    // if connected:
    Serial.println("Connected to server");
    // make a HTTP request:
    // send HTTP header
    client.println(HTTP_METHOD + " " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println(); // end HTTP header

    while(client.connected()) {
      if(client.available()){
        // read an incoming byte from the server and print it to serial monitor:
        char c = client.read();
        Serial.print(c);
      }
    }

    // the server's disconnected, stop the client:
    client.stop();
    Serial.println();
    Serial.println("disconnected");
  } else {// if not connected:
    Serial.println("connection failed");
  }
}
void loop() {
  int gasSensor = analogRead(gasPin);
  int flameSensor = digitalRead(flamePin);
  
  Serial.print("gasPin Value: ");
  Serial.println(gasSensor);
  Serial.print("flamePin Value: ");
  Serial.println(flameSensor);
  delay(1000);
  
  if (gasSensor > gasSensorThres  && flameSensor==LOW){
   digitalWrite(redLed1, HIGH);
    tone(buzzer1, 5000); //the buzzer sound frequency at 5000 Hz
    digitalWrite(redLed2, HIGH);
    tone(buzzer2, 5000); //the buzzer sound frequency at 5000 Hz
    digitalWrite(greenLed, LOW);
  }
   else if (gasSensor > gasSensorThres)
  {
    digitalWrite(redLed1, HIGH);
    tone(buzzer1, 4000); //the buzzer sound frequency at 5000 Hz
    digitalWrite(redLed2, LOW);
    noTone(buzzer2);
    digitalWrite(greenLed, LOW);
  }
  else if (flameSensor==LOW){ // HIGH MEANS NO FLAME
    digitalWrite(redLed1, LOW);
    noTone(buzzer1);
    digitalWrite(redLed2, HIGH);
    tone(buzzer2, 5000); //the buzzer sound frequency at 5000 Hz
    digitalWrite(greenLed, LOW);

    
    }
  
  else
  {
    digitalWrite(redLed1, LOW);
    digitalWrite(redLed2, LOW);
    noTone(buzzer1);
    noTone(buzzer2);
    digitalWrite(greenLed, HIGH);    
  }
}