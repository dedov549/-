
volatile bool u1 = false;
volatile bool u2 = false;

unsigned long previousMillis = 0;      // глобальная переменная для хранения предыдущего времени
const unsigned long interval = 15000;  // интервал в миллисекундах между циклами
#include <synth.h>

synth pwmSynth;  // create pwmSynth object
int onTime;      // time in mS to keep playing DTMF tones
int offTime;     // time in mS to stay silent after playing a DTMF tone
String message;
void setup() {
  pinMode(8,INPUT);
  pinMode(6,INPUT);
  
  Serial.begin(9600);
  pwmSynth.begin();
  pwmSynth.setupVoice(0, SINE, 60, ENVELOPE1, 127, 64);  //-Set up voice 0
  pwmSynth.setupVoice(1, SINE, 60, ENVELOPE1, 127, 64);  //-Set up voice 1
}
void loop() {


     if (digitalRead(8)==1){
       Serial.print("отправка");
  
       obr("#8000*",60,60);
       
     }
     if (digitalRead(6)==1){
      Serial.print("принял");
       obr("#8001*",60,60);
       obr("#8002*",60,60);
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
      default: break;  // ignore invalid characters
    }
  }
}
void keypadTone(int n, unsigned int onTime) {
  const unsigned int ColumnFrequencies[] = {1209, 1336, 1477, 1633};
  const unsigned int RowFrequencies[] = {697, 770, 852, 941};

  //no tone beyond D
  if (n >= 16) {
    return;
  }

  //shift a bit to get the order 123456789*0# for the first three columns
  if (n == 0) {
    n = 10;
  }
  else if (n <= 10) {
    n -= 1;
  }

  //1 to # (first three columns)
  if (n < 12) {
    pwmSynth.setFrequency(0, (float)ColumnFrequencies[n % 3]);
    pwmSynth.setFrequency(1, (float)RowFrequencies[n / 3]);
  }
  //A to D (4th column)
  else {
    pwmSynth.setFrequency(0, (float)ColumnFrequencies[3]);
    pwmSynth.setFrequency(1, (float)RowFrequencies[n - 12]);
  }

  pwmSynth.trigger(0);
  pwmSynth.trigger(1);

  // if onTime=0 the sound continuously plays until another sound is commanded elsewhere
  // if onTime>0 the sound plays for the duration in mS
  if (onTime > 0) {
    delay(onTime);
    pwmSynth.setFrequency(0, 0);
    pwmSynth.setFrequency(1, 0);
  }
}

// *can't get working yet
// play a tone requested by a char by using a char: 0123456789*#ABCDabcd
void keypadTone(char n, unsigned int onTime) {
  //if small letter, make capital
  if (n >= 'a') {
    n -= 32;
  }

  if (n == '*') {
    keypadTone(10, onTime);
  }
  else if (n == '#') {
    keypadTone(11, onTime);
  }
  else if ((n >= 'A') && (n <= 'D')) {
    keypadTone((byte)(n - 'A' + 12), onTime);
  }
}

