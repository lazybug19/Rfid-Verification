#include <SPI.h>
#include <MFRC522.h>
#include <LedControl.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>
#define WIFI_SSID "oppo"
#define WIFI_PASSWORD "rahulabc"

#define SMTP_server "smtp.gmail.com"
#define SMTP_Port 465


#define sender_email "f20213103@hyderabad.bits-pilani.ac.in"
#define sender_password "Password@f2021"

#define Recipient_email "avasthirahul60@gmail.com"


SMTPSession smtp;

int DIN=16;
int CS=5;
int CLK=4;
LedControl lc=LedControl(DIN,CLK,CS,0);
#define SS_PIN 2
#define RST_PIN 0
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  smtp.debug(1);

}
void printByte(byte character[])
{
  int i=0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
void loop() 
{
  byte smile[8]=   {0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
  byte neutral[8]= {0x3C,0x42,0xA5,0x81,0xBD,0x81,0x42,0x3C};
  byte sad[8]=   {0x3C,0x42,0xA5,0x81,0x99,0xA5,0x42,0x3C};
  int l=0;
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "2B E6 05 85") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    l=1;
    Serial.println();
    delay(3000);
  }
 
 else   {
    Serial.println(" Access denied");
    delay(3000);
    l=0;
  } 
  if (l==1)
  {
    for(;;)
    {
    printByte(smile);
    delay(1000);
    }
    ESP_Mail_Session session;


  session.server.host_name = SMTP_server ;
  session.server.port = SMTP_Port;
  session.login.email = sender_email;
  session.login.password = sender_password;
  session.login.user_domain = "";
  
  SMTP_Message message;


  message.sender.name = "Doctor";
  message.sender.email = sender_email;
  message.subject = "covid vaccination status";
  message.addRecipient("rahul",Recipient_email);

   //Send HTML message
  String htmlMsg = "<div style=\"color:#0000FF;\"><h1>Hello Microcontrollerslab!</h1><p> You are vaccinated!!!</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit; 

  if (!smtp.connect(&session))
    return;


  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
  }
  else
  {
    printByte(sad);
    delay(1000);
  }
}
