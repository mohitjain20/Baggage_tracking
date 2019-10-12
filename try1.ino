#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
 #include <SPI.h>
#include <MFRC522.h>                      
//#define FIREBASE_HOST "test.firebaseio.com"                          // database URL 
//#define FIREBASE_AUTH "xxxxxxxxxx"             // secret key
//
//#define WIFI_SSID "xxxxxxx"                                           
//#define WIFI_PASSWORD "xxxxxxx"   

unsigned long t;
  
#define FIREBASE_HOST "prototype-b694d.firebaseio.com"
#define FIREBASE_AUTH "p3bG6vIoBiPwGfe9UL1dgW844HNZa8YpbCAh1Oeg"
#define WIFI_SSID "Tachyon"
#define WIFI_PASSWORD "bepa8139"
#define SS_PIN 4  //D2
#define RST_PIN 5 //D1



MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
int val=0, val3=1000;

void setup() {

  Serial.begin(9600);

  Serial.println("Serial communication started\n\n");  
           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);


  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();
  
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  delay(1000);
}

void loop() { 

// Firebase Error Handling ************************************************
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
  Serial.println();
  Serial.print(" UID tag :");
  String content= "ID: ";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  t = millis();
  content.concat(" Time: ");
  content.concat(String(t));
  if (Firebase.failed())
  { delay(500);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Serial.println(Firebase.error());
  Serial.println("Connection to fiebase failed. Reconnecting...");
  delay(500);
  }
  
 else { 
    Serial.println("Everything is ready!");
    delay(300); Serial.println("Everything is ready! \n \n \n");
    delay(300);

  
    Firebase.pushString("/P1",content.substring(1));
    Serial.println(content.substring(1));
      /*Firebase.setInt("/test/val3",val3);
   Serial.println(val3);
    delay(300); Serial.println("uploaded val3 to firebase \n \n \n");
  */

 } 
}
