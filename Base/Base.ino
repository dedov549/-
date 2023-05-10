volatile bool toneLoc = false;
volatile bool preambFlag = false;
volatile bool postambFlag = false;
#include <Wire.h>
volatile bool ok = true;
volatile bool god = true;
#include <synth.h>
#define kn1 22
String so = "";
synth pwmSynth;
int offTime;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(8, OUTPUT);

  pinMode(10, INPUT);
  pinMode(9, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), newDTMF, RISING);
  pinMode(6, INPUT);
  pinMode(kn1, INPUT);
  /////'//////дисплей сток
  lcd.clear();

  lcd.setCursor(0, 0);  // установка курсора на первую строку, первый символ
  lcd.print("UNIT 1: ");
  lcd.setCursor(0, 1);  // установка курсора на первую строку, первый символ
  lcd.print("UNIT 2: ");
  lcd.setCursor(0, 2);  // установка курсора на первую строку, первый символ
  lcd.print("UNIT 3: ");
}

void loop() {
  //Serial.print(digitalRead(24),digitalRead(24));
  if (digitalRead(10) == 1) {
    digitalWrite(9, 0);
  }






  if (toneLoc == true) {



    char keySymbol;
    bool Q1 = digitalRead(6);
    bool Q2 = digitalRead(5);
    bool Q3 = digitalRead(4);
    bool Q4 = digitalRead(3);
    byte toneData = (0x00 | Q1 | (Q2 << 1) | (Q3 << 2) | (Q4 << 3));
    keySymbol = dtmf_decode(toneData);

    if (keySymbol == '#' && !preambFlag && !postambFlag) {
      preambFlag = true;
      so = "";
    } else if (keySymbol == '*' && preambFlag) {
      postambFlag = true;
    }

    if (preambFlag && !postambFlag) {
      so += keySymbol;
    }

    if (preambFlag && postambFlag) {

      preambFlag = false;
      postambFlag = false;
      so = so.substring(1, so.length());
      ///if(so.length()!=4){so="";}
      Serial.println(so);



      if (so == "A010") {
        lcd.setCursor(8, 0);  // установка курсора на первую строку, первый символ
        lcd.print(" + ");     // вывод сообщения о состоянии кнопки 1
      } else if (so == "A011") {
        god = 0;  // установка курсора на первую строку, первый символ
        lcd.setCursor(8, 0);
        lcd.print(" -  ");  // вывод сообщения о состоянии кнопки 1
        digitalWrite(9, 1);
      }

      // вывод информации о состоянии кнопки 2
      else if (so == "A020") {
        lcd.setCursor(8, 1);  // установка курсора на вторую строку, первый символ
        lcd.print(" +  ");    // вывод сообщения о состоянии кнопки 2
      } else if (so == "A021") {
        god = 0;
        lcd.setCursor(8, 1);  // установка курсора на вторую строку, первый символ
        digitalWrite(9, 1);
        lcd.print(" -  ");  // вывод сообщения о состоянии кнопки 2
      }

      // вывод информации о состоянии кнопки 3
      else if (so == "A030") {
        lcd.setCursor(8, 2);  // установка курсора на первую строку, десятый символ
        lcd.print(" + ");     // вывод сообщения о состоянии кнопки 3
      } else if (so == "A031") {
        god = 0;
        lcd.setCursor(8, 2);  // установка курсора на первую строку, десятый символ
        lcd.print(" - ");     // вывод сообщения о состоянии кнопки 3
        digitalWrite(9, 1);
      }
    }
  }
  toneLoc = false;
}
void newDTMF() {
  toneLoc = true;
}
char dtmf_decode(byte recv_data) {
  switch (recv_data) {
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return '0';
    case 11: return '*';
    case 12: return '#';
    case 13: return 'A';
    case 14: return 'B';
    case 15: return 'C';
    case 0: return 'D';
    default: return '?';
  }
}
///////////////////////////