volatile bool toneLoc = false;
volatile bool preambFlag = false;
volatile bool postambFlag = false;
volatile bool u1 = false;
volatile bool b1 = false;
volatile bool u2 = false;
volatile bool ok = false;
volatile bool kon = 1;
String so = "";
#define kn1 7
#define kn2 8
#define kn3 9
#define kn4 10
unsigned long previousMillis = 0;  // глобальная переменная для хранения предыдущего времени
unsigned long previousMillis2 = 0;
const unsigned long interval = 20000;  // интервал в миллисекундах между циклами
const unsigned long interval2 = 20000;
#include <synth.h>

synth pwmSynth;
int onTime;
int offTime;
String message;
void setup() {
  attachInterrupt(digitalPinToInterrupt(2), newDTMF, RISING);
  Serial.begin(9600);
  pinMode(kn1, INPUT);
  pinMode(kn2, INPUT);
  pinMode(kn3, INPUT);
  pinMode(kn4, INPUT);
  pwmSynth.begin();
  pwmSynth.setupVoice(0, SINE, 60, ENVELOPE1, 127, 64);
  pwmSynth.setupVoice(1, SINE, 60, ENVELOPE1, 127, 64);
}
void loop() {

  if ((digitalRead(kn1) == 1 or digitalRead(kn2) == 1 or digitalRead(kn3) == 1)) {
    if (ok == false) {
      unsigned long currentMillis2 = millis();
      if (b1 == false) {
        if (currentMillis2 - previousMillis2 >= interval2) {
          previousMillis2 = currentMillis2;
          per();
        }
      }
    }
  }
  if (ok = true and b1 == false) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      per();
    }
  }  ///////////////////////////////}

  //////////////////////////// прием сигнала на устройстве
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

    if (preambFlag && postambFlag) {  // здесь будет обработчик каждого устройства в зависимости от первого символа после обрезки преамбулы.

      preambFlag = false;
      postambFlag = false;
      so = so.substring(1, so.length());
      ///if(so.length()!=4){so="";}
      Serial.println(so);
      if (so == "8000") {
        
        per();
        b1 = false;
      }
      if (so == "8001") { ok = true; }
      if (so == "8002") { b1 = true; }
    }
    toneLoc = false;

  }  //////////////////////////////////////



  ////////////////////////
}

void newDTMF() {
  toneLoc = true;
}
void per() {
  kon = false;
  if (digitalRead(kn1) == 1) {

    obr("#A011*", 40, 100);
  } else {

    obr("#A010*", 40, 100);
  }
  if (digitalRead(kn2) == 1) {

    obr("#A021*", 40, 100);
  } else {

    obr("#A020*", 40, 100);
  }
  if (digitalRead(kn3) == 1) {

    obr("#A031*", 40, 100);
  } else {

    obr("#A030*", 40, 100);
  }
  kon = true;
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
void obr(String message, int onTime, int offTime) {
  for (int i = 0; i < message.length(); i++) {
    char c = message.charAt(i);
    switch (c) {
      case '0':
        keypadTone(0, onTime);
        delay(offTime);
        break;
      case '1':
        keypadTone(1, onTime);
        delay(offTime);
        break;
      case '2':
        keypadTone(2, onTime);
        delay(offTime);
        break;
      case '3':
        keypadTone(3, onTime);
        delay(offTime);
        break;
      case '4':
        keypadTone(4, onTime);
        delay(offTime);
        break;
      case '5':
        keypadTone(5, onTime);
        delay(offTime);
        break;
      case '6':
        keypadTone(6, onTime);
        delay(offTime);
        break;
      case '7':
        keypadTone(7, onTime);
        delay(offTime);
        break;
      case '8':
        keypadTone(8, onTime);
        delay(offTime);
        break;
      case '9':
        keypadTone(9, onTime);
        delay(offTime);
        break;
      case '*':
        keypadTone(10, onTime);
        delay(offTime);
        break;
      case '#':
        keypadTone(11, onTime);
        delay(offTime);
        break;
      case 'A':
        keypadTone(12, onTime);
        delay(offTime);
        break;
      case 'B':
        keypadTone(13, onTime);
        delay(offTime);
        break;
      case 'C':
        keypadTone(14, onTime);
        delay(offTime);
        break;
      case 'D':
        keypadTone(15, onTime);
        delay(offTime);
        break;
      default: break;
    }
  }
}
void keypadTone(int n, unsigned int onTime) {
  const unsigned int ColumnFrequencies[] = { 1209, 1336, 1477, 1633 };
  const unsigned int RowFrequencies[] = { 697, 770, 852, 941 };

  //no tone beyond D
  if (n >= 16) {
    return;
  }


  if (n == 0) {
    n = 10;
  } else if (n <= 10) {
    n -= 1;
  }


  if (n < 12) {
    pwmSynth.setFrequency(0, (float)ColumnFrequencies[n % 3]);
    pwmSynth.setFrequency(1, (float)RowFrequencies[n / 3]);
  }

  else {
    pwmSynth.setFrequency(0, (float)ColumnFrequencies[3]);
    pwmSynth.setFrequency(1, (float)RowFrequencies[n - 12]);
  }

  pwmSynth.trigger(0);
  pwmSynth.trigger(1);


  if (onTime > 0) {
    delay(onTime);
    pwmSynth.setFrequency(0, 0);
    pwmSynth.setFrequency(1, 0);
  }
}
void keypadTone(char n, unsigned int onTime) {

  if (n >= 'a') {
    n -= 32;
  }

  if (n == '*') {
    keypadTone(10, onTime);
  } else if (n == '#') {
    keypadTone(11, onTime);
  } else if ((n >= 'A') && (n <= 'D')) {
    keypadTone((byte)(n - 'A' + 12), onTime);
  }
}
