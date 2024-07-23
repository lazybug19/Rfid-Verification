#include <WiFi101.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TelegramBot.h>
#include <LedControl.h>

int DIN=7;
int CS=6;
int CLK=5;
LedControl lc = LedControl(DIN,CLK,CS,0);
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // MFRC522 instance.

//Bot initialising & network connection
const char* BotToken = "5300438669:AAFMiu1xCgrhBnGHuAYJFy0vR1a756rNiTI";

char ssid[] = "lazybug" ;  // network SSID
char pass[] = "8249422424";  // network password

WiFiSSLClient client;
TelegramBot bot (BotToken, client);

void setup() {

  Serial.begin(9600);

  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  
  Serial.print("Waiting for WiFi connection");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected!");

  SPI.begin();      
  mfrc522.PCD_Init();
  bot.begin();
  message M = bot.getUpdates(); 
  bot.sendMessage(M.chat_id, "Yo");
}

void loop() {

  bot.sendMessage(m.chat_id, "Yo");


  //Dot Matrix
  byte smile[8]=   {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
  byte neutral[8]= {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};
  byte sad[8]=   {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};
  int l = 0;

  //Card reading
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  Serial.print("UID tag :");
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println("Status : ");
  content.toUpperCase();
  if (content.substring(1) == " ") //UID to be granted access 
  {
    Serial.println("Authorized access");
    l = 1;
    bot.sendMessage(m.chat_id, "Authorized access "); //Fill in patient's details
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    l = 0;
    bot.sendMessage(m.chat_id, "Access denied");
    delay(3000);
  }

  if (l==1)
  {
    printByte(smile);
    delay(1000);
  }
  else
  {
    printByte(sad);
    delay(1000);
  }

 message M = bot.getUpdates(); 

  if ( M.text.equals("Allow")) {
    digitalWrite(); //Dot Matrix
  }
  else if ( M.text.equals("Wait")) {
    digitalWrite(); //Dot Matrix
  }


//for figure on dot matrix
  void printByte(byte character[])
{
  int i=0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
}
