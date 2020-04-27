/*

  Arduino Full Home Automation Project

  Created by Tudor Cernat

  Status: Work in progress / devtesting

*/


#include <SPI.h>
#include <Ethernet.h>

// Network mask, IP and port
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
//IPAddress ip(192, 168, 100, 250); Use this for manual IP Address

EthernetServer server(80);
EthernetClient client;

const int passCount = 20;
unsigned long passIDstart[passCount];  // array to record start time of user sessions
unsigned long passIDlast[passCount];   // array to record last time of user sessions
unsigned long passIDvalid = 0;         // set default as passID : 0 = missing or invalid
unsigned long passExpireMil = 60000;   // expiry time for automatic expiry of valid passID
long uptime;

String readString;                     // used to clear away incoming data that is not required
char myPass[] = "valentin123";         // Valid password -----------------------------------------------------------------PASSWORD HERE <<<
char doaction[] = "doaction";          // keyword for DoAction commands
char buffer[256];                      // buffer for debugging
char xx1[20];                          // temp char for the long converted to char
char JoinedChar[100];                  // char to hold the joined strings
String LastAction;

/*----------------------------------------------------------------------------*/
/* incoming data : Get a single client char                                   */
/*----------------------------------------------------------------------------*/
char gchr(void) {
  while (!client.available());         /* Await data from client           */
  return client.read();               /* Return input character            */
}                                      /* end: gchr()                        */

/*----------------------------------------------------------------------------*/
/* incoming data : Get an entire line from the client                         */
/*----------------------------------------------------------------------------*/
char *glin(char *buf) {
  char c, *p = buf;                   /* Input char, input buffer pointer   */
  while (' ' > (c = gchr()));         /* Discard (leading) control chars    */
  do *p++ = c;                        /* Move input char to line buffer     */
  while (' ' <= (c = gchr()));        /* Until control char encountered     */
  *p = '\0';                          /* Terminate line in buffer           */
  return buf;                         /* Return pointer to input string     */
}                                       /* end: glin()                        */

#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 5, 6, 7, 8);

#include <EduIntro.h>
DHT11 dht11(9);



int releu1 = 49; // Pini pentru relee
int releu2 = 48;
int releu3 = 47;
int releu4 = 46;
int releu5 = 45;
int releu6 = 44;
int releu7 = 43;
int releu8 = 42;
int releu9 = 41;
int releu10 = 40;
int releu11 = 39;
int releu12 = 38;
int releu13 = 37;
int releu14 = 36;
int releu15 = 35;
int releu16 = 34;

int temperatura, umid;
String readString1;
String r1;




void setup() //                        --- VOID SETUP ---
{
  pinMode(4, OUTPUT); // SD Card kill switch
  digitalWrite(4, HIGH);

  pinMode(releu1, OUTPUT); // Definirea pinilor digitali
  pinMode(releu2, OUTPUT);
  pinMode(releu3, OUTPUT);
  pinMode(releu4, OUTPUT);
  pinMode(releu5, OUTPUT);
  pinMode(releu6, OUTPUT);
  pinMode(releu7, OUTPUT);
  pinMode(releu8, OUTPUT);
  pinMode(releu9, OUTPUT);
  pinMode(releu10, OUTPUT);
  pinMode(releu11, OUTPUT);
  pinMode(releu12, OUTPUT);
  pinMode(releu13, OUTPUT);
  pinMode(releu14, OUTPUT);
  pinMode(releu15, OUTPUT);
  pinMode(releu16, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT); // Initializare LED onboard

  // Default relay state

  digitalWrite(releu1, HIGH);  // 1
  digitalWrite(releu2, HIGH);  // 2
  digitalWrite(releu3, HIGH);  // 3
  digitalWrite(releu4, HIGH);  // 4
  digitalWrite(releu5, HIGH);  // 5
  digitalWrite(releu6, HIGH);  // 6
  digitalWrite(releu7, HIGH);  // 7
  digitalWrite(releu8, HIGH);  // 8
  digitalWrite(releu9, HIGH);  // 9
  digitalWrite(releu10, HIGH); // 10
  digitalWrite(releu11, HIGH); // 11
  digitalWrite(releu12, HIGH); // 12
  digitalWrite(releu13, HIGH); // 13
  digitalWrite(releu14, HIGH); // 14
  digitalWrite(releu15, HIGH); // 15
  digitalWrite(releu16, HIGH); // 16

  digitalWrite(LED_BUILTIN, HIGH); // Pornire LED Galben

  lcd.begin(20, 4);
  lcd.clear();

  Ethernet.begin(mac);
  //Ethernet.begin(mac, ip); Use this when using an automatic IP address
  server.begin();
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());
  lcd.setCursor(0, 1);
  lcd.print("Initializare");

  delay(2000); // 2 secunde

  lcd.clear();


  digitalWrite(LED_BUILTIN, LOW);
}

//                                                                                                        VOID LOOP
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//

void loop()
{
  unsigned long currentMillis = millis(); // Nu stiu ce fac, dar sunt si eu aici

  digitalWrite(LED_BUILTIN, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());

  dht11.update();

  temperatura = dht11.readCelsius() - 2;
  umid = dht11.readHumidity();

  lcd.setCursor(0, 2);
  lcd.print("T ");
  lcd.print(temperatura);
  lcd.print("'C  - U ");
  lcd.print(umid);
  lcd.print("%");

  if (readString1.indexOf("on1") > 0) //checks for on
  {
    digitalWrite(releu1, LOW);  // 1
    lcd.setCursor(0, 3);
    r1 = "Releu 1 pornit";
    lcd.print(r1);
  }
  if (readString1.indexOf("off") > 0) //checks for off
  {
    digitalWrite(releu1, HIGH);  // 1
    lcd.setCursor(0, 3);
    r1 = "Releu 1 oprit";
    lcd.print(r1);
  }
  //clearing string for next read
  readString1 = "";

  if (client = server.available()) {    // Request client connection
    while (client.connected()) {      // Is there client data available?
      glin(buffer);                 // Get HTTP request line by line
      Serial.print("The current millis is : ");
      Serial.println(millis());
      Serial.print("Received: '");  // Show what we received from
      Serial.print(buffer);         // the current client
      Serial.println("'");

      passIDvalid = 0;              // set the passIDvalid to 0
      LastAction = "";


      if (strstr(buffer, "logform")) {   /* Is logform keyword present?         */
        Serial.println("Found logform");
        if (strstr(buffer, myPass)) {   /* Is password present?                */
          Serial.println("Found myPass");

          // clear the rest of the incoming buffer
          char c = client.read();
          if (readString.length() < 100) {         // read char by char HTTP request
            readString += c;                       // store characters to string
          }
          if (c == '\n') {                         // if HTTP request has ended - blank line received
            Serial.println("Header Flushed");
          }

          /* Login form processing */

          // get the first open passID storage space and create a new session ID.
          for (int i = 0; i < passCount; i++) {
            if (passIDvalid == 0 && passIDstart[i] == 0) {
              passIDstart[i] = currentMillis;  // set the variable to use as the passID
              passIDlast[i] = currentMillis;   // set the last time the passID was used
              passIDvalid = passIDstart[i];
              Serial.print("New alocated passID = ");
              Serial.print(passIDvalid);
              Serial.print(" in array space ");
              Serial.println(i);
              LastAction = "Parola corecta";
            }
          }                     // end of loop for get the first open passID storage space
        }                         // end: if password is present
      }                             // end: if logform in line

      Serial.println("Processing input");

      // clear expired passIDs from array - housekeeping each time a server connection is made
      for (int i = 0; i < passCount; i++) {
        if (passIDstart[i] != 0) {
          if (currentMillis >= passExpireMil) {
            if (currentMillis - passIDlast[i] >= passExpireMil) {
              passIDstart[i] = 0;
              Serial.print("Cleared passIDstart ");
              Serial.println(i);
            }
          }
        }
      }

      unsigned int GotAction = 0;   // shows that no DoAction command received

      // look for DoAction commands in the input buffer
      if (strstr(buffer, "doaction")) {   /* If DoAction keyword is present         */
        Serial.println("Found DoAction keyword");
        Serial.println("Looking for valid passID");

        //see if a valid passID exists in the DoAction line
        for (int i = 0; i < passCount; i++) {

          if (GotAction == 0)
          {

            ltoa(passIDstart[i], xx1, 10); // convert the long to char

            //check for the passID + DoAction keyword + action code in the buffer

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "1on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 1;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "1off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 2;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "2on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 3;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "2off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 4;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "3on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 5;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "3off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 6;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "4on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 7;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "4off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 8;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "5on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 9;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "5off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 10;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "6on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 11;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "6off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 12;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "7on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 13;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "7off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 14;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "8on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 15;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "8off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 16;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "9on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 17;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "9off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 18;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "10on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 19;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "10off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 20;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "11on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 21;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "11off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 22;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "12on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 23;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "12off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 24;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "13on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 25;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "13off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 26;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "14on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 27;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "14off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 28;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "15on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 29;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "15off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 30;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "16on");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 31;

            JoinedChar[0] = '\0';               // clear the destination array
            strcat(JoinedChar, xx1);            //add the passID to the array
            strcat(JoinedChar, doaction);       //add the keyword to the array
            strcat(JoinedChar, "16off");        //add the action code to the array
            if (strstr(buffer, JoinedChar)) GotAction = 32;

            if (GotAction != 0) {
              Serial.print("Found DoAction ");
              Serial.print(GotAction);
              Serial.print(" for passID : ");
              Serial.println(passIDstart[i]);
              passIDlast[i] = currentMillis;   //renew the last time that the passID was used
              passIDvalid = passIDstart[i];
            }
          }
        }

        if (GotAction != 0) {

          // clear the rest of the incoming buffer
          char c = client.read();
          if (readString.length() < 100) {         // read char by char HTTP request
            readString += c;                       // store characters to string
          }
          if (c == '\n') {                         // if HTTP request has ended - blank line received
            Serial.println("Header Flushed");
          }

          /* DoAction processing */

          if (GotAction == 1) {
            Serial.println("Received Web Server command on001");
            // do tasks in response to command 1 received
            digitalWrite(releu1, LOW);  // 1 pornit
            LastAction = "Command on001 processed";
          }
          if (GotAction == 2) {
            Serial.println("Received Web Server command on002");
            // do tasks in response to command 2 received
            digitalWrite(releu1, HIGH);  // 1 oprit
            LastAction = "Command on002 processed";
          }
          if (GotAction == 3) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu2, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 4) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu2, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 5) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu3, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 6) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu3, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 7) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu4, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 8) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu4, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 9) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu5, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 10) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu5, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 11) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu6, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 12) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu6, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 13) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu7, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 14) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu7, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 15) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu8, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 16) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu8, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 17) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu9, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 18) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu9, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 19) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu10, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 20) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu10, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 21) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu11, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 22) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu11, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 23) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu12, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 24) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu12, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 25) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu13, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 26) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu13, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 27) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu14, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 28) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu14, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 29) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu15, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 30) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu15, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
          if (GotAction == 31) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu16, LOW);  // 2 pornit
            LastAction = "Releu actionat";
          }
          if (GotAction == 32) {
            Serial.println("Received Web Server command @ Releu");
            digitalWrite(releu16, HIGH);  // 2 oprit
            LastAction = "Releu actionat";
          }
        }
      }


      //display the web page

      if (passIDvalid == 0)

      { // this is not a valid passID - ask for the password

        client.println("HTTP/1.1 200 OK"); //send new page
        client.println("Content-Type: text/html");
        client.println();
        client.println("<HTML>");
        client.println("<HEAD>");
        client.println("<STYLE TYPE=\"text/css\"><!--");     //set the style for the page to Courier New
        client.println("BODY   {   font-family:Courier New;   }");
        client.println("--></STYLE>");
        client.println("<TITLE>Resedinta Cernat</TITLE>");       //browser tab title
        client.println("</HEAD>");
        client.println("<BODY>");                            //start of body section
        client.println("Resedinta Cernat<br><br>");
        client.print("Uptime = ");
        client.print(uptime / 600);
        client.print(" minute.");
        client.println("<br><br>");
        client.print("Introdu parola: ");
        client.print(LastAction);
        client.println("<br><br>");

        client.print ("<form method=get>");
        client.print ("<input type=password name=logform size=10>");
        client.print (" <input type=submit value=Login>");
        client.print ("</form>");

      }

      if (passIDvalid != 0) { // this IS a valid passID - display the web page

        client.println("HTTP/1.1 200 OK"); //send new page
        client.println("Content-Type: text/html");
        client.println();
        client.println("<HTML>");
        client.println("<HEAD>");
        client.println("<STYLE TYPE=\"text/css\"><!--");     //set the style for the page to Courier New
        client.println("BODY   {   font-family:Courier New;   }");
        client.println("--></STYLE>");
        client.println("<TITLE>Resedinta Cernat</TITLE>");       //browser tab title
        client.println("</HEAD>");
        client.println("<BODY>");                            //start of body section
        client.println("Resedinta Cernat<br><br>");
        client.print("Uptime = ");
        client.print(uptime / 600);
        client.print(" minute.");
        client.println("<br><br>");

        client.print("Temperatura: ");
        client.print(temperatura);
        client.print(" 'C");
        client.println("<br />");
        client.print("Umiditate: " + String(umid));
        client.print("%");
        client.println("<br />");
        client.println("<br><br>");

        // ------------------------------------------- Releu 1
        client.print("Releu 1 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction1on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction1off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu1) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 2
        client.print("Releu 2 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction2on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction2off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu2) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 3
        client.print("Releu 3 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction3on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction3off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu3) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 4
        client.print("Releu 4 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction4on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction4off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu4) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 5
        client.print("Releu 5 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction5on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction5off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu5) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 6
        client.print("Releu 6 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction6on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction6off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu6) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 7
        client.print("Releu 7 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction7on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction7off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu7) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 8
        client.print("Releu 8 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction8on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction8off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu8) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 9
        client.print("Releu 9 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction9on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction9off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu9) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 10
        client.print("Releu 10 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction10on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction10off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu10) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 11
        client.print("Releu 11 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction11on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction11off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu11) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 12
        client.print("Releu 12 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction12on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction12off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu12) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 13
        client.print("Releu 13 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction13on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction13off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu13) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 14
        client.print("Releu 14 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction14on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction14off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu14) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 15
        client.print("Releu 15 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction15on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction15off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu15) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------
        // ------------------------------------------- Releu 16
        client.print("Releu 16 : <a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction16on");
        client.println("\"\">[Pornit]</a>");
        client.print("<a href=\"./?");
        client.print(passIDvalid);
        client.print("doaction16off");
        client.println("\"\">[Oprit]</a>");
        if (digitalRead (releu16) == 1)
        {
          client.print("Oprit");
        }
        else
        {
          client.print("Pornit");
        }
        client.println("<br><br>");
        // --------------------------------------------------


      }

      client.print ("<head>");
      client.print ("<title>Absolute Positioning</title>");
      client.print ("<style rel='stylesheet' type='text/css'>");
      client.print ("body {<!--  www  . j  av  a 2  s. co  m-->");
      client.print ("}");
      client.print ("div {");
      client.print ("position: absolute;");
      client.print ("padding: 5px;");
      client.print ("width: 75px;");
      client.print ("height: 35px;");
      client.print ("}");
      client.print ("div#bottom-right {");
      client.print ("bottom: 0;");
      client.print ("right: 0;");
      client.print ("}");
      client.print ("</style>");
      client.print ("</head>");
      client.print ("<body>");
      client.print ("<div id='bottom-right'>");
      client.print ("v.1.0");
      client.print ("</div>");
      client.print ("</body>");

      //delay(1);

      client.stop();                /* Disconnect from the server          */
    }                                /*  end: while client connected        */
  }                                   /*  end: if client connection          */

  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);

  uptime++;

} // final void loop
