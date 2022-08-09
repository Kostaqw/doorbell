//#include "doorbell-11khz.h"
//#include <PCM.h>
#include "pitches.h"
#define in A0
#define bell 11
#define button 2

int value;
int counter=0;
int res=0;
int melody[]={NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[]={4, 8, 8, 4, 4, 4, 4, 4};

int buttonState = 0; 

void setup() {
Serial.begin(9600);
pinMode(button, INPUT);
}

void loop() {
   buttonState = digitalRead(button);

   if (buttonState == HIGH) 
   {
    for (int thisNote=0; thisNote <8; thisNote++)
    {
     // для вычисления длительности ноты потребуется одна секунда
     int noteDuration = 1000 / noteDurations [thisNote];
     tone(8, melody [thisNote], noteDuration);
     // чтобы различать ноты, установим минимальное время между ними
     // длительность ноты +30% звучит хорошо
     int pauseBetweenNotes = noteDuration * 1.30;
     delay(pauseBetweenNotes);

     noTone(8);
    }

   }
}
