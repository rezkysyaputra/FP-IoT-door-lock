
#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 
#define SS_PIN 4  // sda
#define RST_PIN 2



int lock = D1;
 
MFRC522 mfrc522(RST_PIN, SS_PIN);        // Create MFRC522 instance.
char auth[] ="jpLI9iEBWm8ifGn9IP4ZIYEz4B3xEcpO";    //Blynk Authentication Token -- sent via Email from Blynk
 
char ssid[] = "Redmi";   //Enter WiFi Name
char pass[] = "cobajadulu";   //Enter Wifi Password
 
SimpleTimer timer; 
int eflag = 0; 
int openDoor = 0;
WidgetLCD lcd(V5);
WidgetTerminal terminal(V2);
 
void setup() {
    Serial.begin(9600);        // Initialize serial communications with the PC
    Blynk.begin(auth, ssid, pass, "iot.amikom.ac.id", 8080);
    pinMode(lock,OUTPUT);
    digitalWrite(lock, LOW);
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();        // Init MFRC522 card
        //Serial.println("Scan a MIFARE Classic PICC to demonstrate Value Blocks.");
    timer.setInterval(1000L, iot_rfid);
    pinMode(D0, OUTPUT);

    lcd.clear();
    lcd.print(0,0, "SMARTDOOR LOCK");
    lcd.print(0,1, "< DOOR CLOSE >");

    SPI.begin();
    Serial.println("CLEARSHEET");
    Serial.println("LABEL,Data,Tanggal,Waktu"); 
}
 
void loop() {
        
  timer.run(); // Initiates SimpleTimer
  Blynk.run();
          // Open Door button blynk
  if(openDoor == 1){
    // LCD
    lcd.clear();
    lcd.print(0,0, "SMARTDOOR LOCK");
    lcd.print(0,1, "< DOOR OPEN >");

    String openBlynk = "BLYNK";
    // Excel
    Serial.print("DATA,");
    Serial.print(openBlynk);
    Serial.print(",");
    Serial.print("DATE");
    Serial.print(",");
    Serial.print("TIME");
    Serial.print(",");
    Serial.println("");

    Serial.println(openBlynk);
    Blynk.virtualWrite(V2, openBlynk);

    // LOCK
    digitalWrite(lock, HIGH); 
    digitalWrite(D0, HIGH); 
    delay(3000); 
    digitalWrite(lock, LOW); 
    digitalWrite(D0, LOW);

    // LCD
    lcd.clear();
    lcd.print(0,0, "SMARTDOOR LOCK");
    lcd.print(0,1, "< DOOR CLOSE >");
  }
       
}
 
void iot_rfid()
{
 
        // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
        MFRC522::MIFARE_Key key;
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;
        }
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
                return;
        }
 
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
                return;
        }
        // Now a card is selected. The UID and SAK is in mfrc522.uid.
        
        // Dump UID
        Serial.print("Card UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
               Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
               Serial.print(mfrc522.uid.uidByte[i], DEC);
        } 
        Serial.println();
 
        // Dump PICC type 
        byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    //    Serial.print("PICC type: ");
//Serial.println(mfrc522.PICC_GetTypeName(piccType));
        if (        piccType != MFRC522::PICC_TYPE_MIFARE_MINI 
                &&        piccType != MFRC522::PICC_TYPE_MIFARE_1K
                &&        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
                //Serial.println("This sample only works with MIFARE Classic cards.");
                return;
        }


        // Enter RFID Tag ID here
        if(( (mfrc522.uid.uidByte[0] == 03) && (mfrc522.uid.uidByte[1] == 199) && (mfrc522.uid.uidByte[2] == 140) && (mfrc522.uid.uidByte[3] == 246)) && (eflag == 1) )
        {
          lcd.clear();
          lcd.print(0,0, "SMARTDOOR LOCK");
          lcd.print(0,1, "< DOOR OPEN >");

          String kartu1 = "KARTU AKSES 1";
          Serial.print("DATA,");
          Serial.print(kartu1);
          Serial.print(",");
          Serial.print("DATE");
          Serial.print(",");
          Serial.print("TIME");
          Serial.print(",");
          Serial.println("");

         Serial.println(kartu1);
         Blynk.virtualWrite(V2, kartu1 );
         digitalWrite(lock, HIGH); 
         digitalWrite(D0, HIGH); 
         delay(3000); 
         digitalWrite(lock, LOW); 
         digitalWrite(D0, LOW);
          lcd.clear();
          lcd.print(0,0, "SMARTDOOR LOCK");
          lcd.print(0,1, "< DOOR CLOSE >"); 
        }
       else 
        Serial.println("Unregistered User");  
  
}
 
 
BLYNK_WRITE(V3)
{
   openDoor = param.asInt();
}

BLYNK_WRITE(V4)
{
   eflag = param.asInt(); // assigning incoming value from pin V4 to a variable
}

 
