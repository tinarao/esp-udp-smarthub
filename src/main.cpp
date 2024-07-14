#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define UDP_PORT 1053

WiFiUDP UDP;
char packet[255];
char reply[] = "Packet recieved";

const char *ssid = "TP-Link_B358";
const char *password = "42096641";
const char *serverhost = "127.0.0.1";

const byte yellowPin = 13;
const byte greenPin = 15;

void shutLed(byte p_pin);
void glowLed(byte p_pin);
void blinkLed(byte p_pin, unsigned long p_delaytime);

void sendUdpPacket();
void recieveUdpPackets();

void setup()
{
    Serial.begin(115200);
    pinMode(yellowPin, OUTPUT);
    pinMode(greenPin, OUTPUT);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        blinkLed(150, yellowPin);
        delay(1000);
        Serial.println("Connecting..");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Could not connect to Wi-Fi");
    }

    shutLed(yellowPin);
    shutLed(greenPin);

    Serial.printf("Connected! Local IP: ");
    Serial.print(WiFi.localIP());
    Serial.print("\n");

    UDP.begin(UDP_PORT);
    Serial.printf("Listening UDP Port at %i\n", UDP_PORT);
}

void loop()
{
    shutLed(yellowPin);
    blinkLed(greenPin, 500);

    sendUdpPacket();
    recieveUdpPackets();
}

void sendUdpPacket()
{
    UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
    UDP.write("hello from esp8266!");
    UDP.endPacket();
}

void recieveUdpPackets()
{
    int packetSize = UDP.parsePacket();
    if (packetSize)
    {
        Serial.print("Recieved a packet!\n");
        Serial.printf("Size: %i\n", packetSize);
        int pktLen = UDP.read(packet, 255);
        if (pktLen > 0)
        {
            packet[pktLen] = '\0';
        }

        Serial.print("Packet: ");
        Serial.println(packet);
    }
}

void blinkLed(byte p_pin, unsigned long p_delaytime)
{
    digitalWrite(p_pin, HIGH);
    delay(750);
    digitalWrite(p_pin, LOW);
    delay(750);
}

void shutLed(byte p_pin)
{
    digitalWrite(p_pin, LOW);
}

void glowLed(byte p_pin)
{
    digitalWrite(p_pin, HIGH);
}