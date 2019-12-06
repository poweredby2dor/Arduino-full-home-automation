/*

Arduino Full Home Automation Project

Created by Tudor Cernat
 
 */




#include<LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 5, 6, 7, 8);


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




void setup() //                        --- VOID SETUP ---
{
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

  lcd.print("Setup");
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
  lcd.clear();
  digitalWrite(LED_BUILTIN, HIGH);
  lcd.print("Loop");
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
 
} // final void loop
