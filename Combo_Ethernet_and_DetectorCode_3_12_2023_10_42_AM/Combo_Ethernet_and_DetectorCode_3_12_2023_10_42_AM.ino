#include <SPI.h>
#include <Ethernet.h>

// replace the MAC address below by the MAC address printed on a sticker on the Arduino Shield 2
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

char server[] = "www.google.com";
//static ip address:
IPAddress ip (192, 168, 1, 7);
IPAddress myDns (8,8,8,8);
/*
www.arduinopoint.com 
Fire Alarm System 
*/

EthernetClient client;
/*

*/
int redLed1 = 3;
int redLed2 = 4;
int greenLed = 8;
int buzzer1 = 5; //PWM (~) pin
int buzzer2 = 6; //PWM (~) pin
int gasPin = A0;
int flamePin = 2;
// Your threshold value
int gasSensorThres = 400;
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;
bool printWebData = true;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:


void setup() {
  Ethernet.init(10);

  Serial.begin(9600);
  Serial.println("Ethernet Demo");
  
  //if connection == true, report back to serial
  if (client.connect(server, 80)){
    Serial.println("connected to");
    Serial.println(client.remoteIP());
    //Make http req:
  client.println("GET /search?q=arduino HTTP/1.1");
   client.println("Host: www.google.com");
   client.println("Connection: close");
   client.println("");

  }
  else{
    Serial.println("connection failed");
  }
  beginMicros = micros();

  pinMode(redLed1, OUTPUT);  
  pinMode(redLed2, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(gasPin, INPUT);
  pinMode(flamePin, INPUT);

  Ethernet.begin(mac, ip, myDns);
  delay(1000);
  Serial.print("connecting to");
  Serial.print("server");
  Serial.println("...");

/*
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
  }*/
}
void loop() {
  int gasSensor = analogRead(gasPin);
  int flameSensor = digitalRead(flamePin);

  int len = client.available();

  Serial.print("gasPin Value: ");
  Serial.println(gasSensor);
  Serial.print("flamePin Value: ");
  Serial.println(flameSensor);
  delay(1000);
  
  if(len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    if(printWebData){
      Serial.write(buffer, len);
    }
    byteCount = byteCount + len;
  }
  //if server is disconnected, stop client
  if (!client.connected()){
    endMicros = micros();
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    Serial.print("Received");
    Serial.print(byteCount);
    Serial.print(" bytes in ");
    float seconds = (float)(endMicros - beginMicros) / 1000000.0;
    Serial.print(seconds, 4);
    float rate = (float)byteCount / seconds / 1000.0;
    Serial.print(", rate = ");
    Serial.print(rate);
    Serial.print(" kbytes/second");
    Serial.println();

    // do nothing forevermore:
    while (true) {
      delay(1);
    }
  }

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