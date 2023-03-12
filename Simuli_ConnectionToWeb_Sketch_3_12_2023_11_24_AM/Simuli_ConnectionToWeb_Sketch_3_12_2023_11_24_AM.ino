#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {

  Serial.begin(9600);

  IPAddress meineIP(192, 168, 1, 7);

  Ethernet.begin(mac, meineIP);
  Serial.println(meineIP);
  Serial.println(Ethernet.localIP());
}

void loop() {

}