# Rfid based report Verification & Notification system 
 
RFID sensor based verification system for patients in healthcare systems and sending their earlier health reports or health/vaccination status to doctors via IoT (Telegram notification via Telegram bot). <br>
	
# Components involved:

- RFID Tag : Integrated circuit with a unique radio frequency identification key whose signal is detected by the RFID Reader; input voltage:3.3V (not 5V); response range: close contact-50mm; communicates with microcontroller via serial peripheral interface (SPI) communication protocol

> RFID-RC522 Pins:

- PowerInput-Reset(power down ard.) -> Ground -> Interrupt(not connected; used to wake up arduino when rfid tag is present) -> SPI Communication (MOSI+MISO) -> SerialClock -> SDA(I2C comm)

- RFID Reader : Scans/Reads ID from RFID tag via radio waves data transmission from the tag to reader
- Dot Matrix : Programmed to generate figures like happy face and sad face for acceptance or rejection of ID card respectively.
- ESP8266 Wifi Module : SOC that provides WiFi access via an integrated TCP/IP protocol stack 
- Smartphone : End-point IoT application
- SMTP communication protocols for communicating with end-point IoT  application
