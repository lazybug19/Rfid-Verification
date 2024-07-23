#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h> 
#include <SPI.h>
#include <MFRC522.h>
#include <LedControl.h>
#include <WiFiClientSecure.h> 

int DIN = 16; //D0
int CS  = 5; //D1
int CLK = 4; // D2
LedControl lc = LedControl(DIN,CLK,CS,0);

int SS_PIN = 2; //D4
int RST_PIN = 0; //D3
int l = 1;

MFRC522 mfrc522(SS_PIN, RST_PIN);

const char* ssid = "lazybug"; 
const char* password = "8249422424"; 
const char BotToken[] = "5300438669:AAFMiu1xCgrhBnGHuAYJFy0vR1a756rNiTI"; 
const char chat_id[] = "5467581701";

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure client;
UniversalTelegramBot bot(BotToken, client);

void setup() {

  Serial.begin(115200);

  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);

  Serial.println("Waiting for WiFi connection");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("WiFi connected!");

  SPI.begin();      
  mfrc522.PCD_Init();
  bot.begin();
}

//for figures on dot matrix
void printByte(byte character[])
{
  int i=0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}

//Dot Matrix
byte smile[8] = {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
byte sad[8] =  {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};
byte tick[8] = {0x00,0x40,0x20,0x10,0x0a,0x04,0x00,0x00};
byte cross[8] = {0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00};

void loop() {

  //Card reading
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  Serial.print("UID tag : ");
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Status : ");
  content.toUpperCase();
  if (content.substring(1) == "C2 28 F3 1C") //UID to be granted access 
  {
    message M = bot.getUpdates();
    Serial.print("Authorized access");
    bot.sendMessage(M.chat_id, "Authorized access: Patient has cancer"); //Fill in patient's details
    l = 1;
  }
 
 else   
  {
    Serial.print("Access denied");
    l = 0;
  }

 if(l == 1)
  {
    printByte(smile);
    delay(3000);

    message M = bot.getUpdates();
    if ( M.text.equals("Allow")) 
      {
        printByte(tick);
        delay(3000); 
      }
    else if ( M.text.equals("Wait")) 
      {
        printByte(cross);
        delay(3000); 
      }
  }
 else if(l == 0)
 {
    printByte(sad);
    delay(3000);
 }
}
