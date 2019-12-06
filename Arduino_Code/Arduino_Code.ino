
/*

  Arduino Full Home Automation Project

  Created by Tudor Cernat

*/


#include <SPI.h>
#include <Ethernet.h>

// Network mask, IP and port
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 250);
EthernetServer server(80);

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
String readString;
String r1;




void setup() //                        --- VOID SETUP ---
{
  //pinMode(4, OUTPUT); // SD Card kill switch
  //digitalWrite(4, HIGH);

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

  Ethernet.begin(mac, ip);
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
  digitalWrite(LED_BUILTIN, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(Ethernet.localIP());

  dht11.update();

  temperatura = dht11.readCelsius() - 2;
  umid = dht11.readHumidity();

  listenForEthernetClients();

  lcd.setCursor(0, 2);
  lcd.print("T ");
  lcd.print(temperatura);
  lcd.print("'C  - U ");
  lcd.print(umid);
  lcd.print("%");

  if (readString.indexOf("on1") > 0) //checks for on
  {
    digitalWrite(releu1, LOW);  // 1
    lcd.setCursor(0, 3);
    r1 = "Releu 1 pornit";
    lcd.print(r1);
  }
  if (readString.indexOf("off") > 0) //checks for off
  {
    digitalWrite(releu1, HIGH);  // 1
    lcd.setCursor(0, 3);
    r1 = "Releu 1 oprit";
    lcd.print(r1);
  }
  //clearing string for next read
  readString = "";

  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

} // final void loop

void listenForEthernetClients()
{
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    lcd.setCursor(0, 1);
    lcd.print("Client conectat");
    // an http request ends with a blank line
    bool currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        readString += c;
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<HTML>");
          client.print("<HEAD>");
          client.print("<meta http-equiv=\"refresh\" content=\"5\">");
          client.print("<TITLE />Resedinta Cernat</title>");
          client.print("</head>");

          client.println("<BODY>");
          client.print("Temperatura: ");
          client.print(temperatura);
          client.print(" 'C");
          client.println("<br />");
          client.print("Umiditate: " + String(umid));
          client.print("%");
          client.println("<br />");

          client.print("Releu 1: ");
          client.print("<a href='/?on1' target='inlineframe'><button type='button'>ON</button></a>");
          client.print("<a href='/?off' target='inlineframe'><button type='button'>OFF</button></a>");
          client.print("   ");
          client.print(r1);

          client.println("<IFRAME name=inlineframe style='display:none'>");
          client.println("</IFRAME>");

          client.println("</BODY>");
          client.println("</HTML>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
