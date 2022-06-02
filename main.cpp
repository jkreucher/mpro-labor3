#include "mbed.h"
#include <cstdint>

#define LED_NUCLEO  PA_5
#define SW_NUCLEO   PC_13
// Velleman STEM Shield I/O
#define SegA     PB_0
#define SegB     PC_1
#define SegC     PA_10
#define SegD     PB_3
#define SegE     PB_5
#define SegF     PB_4
#define SegG     PB_10
#define SegDot   PA_8


#define SW_1    PA_9
#define SW_2    PC_7
#define SW_3    PB_6
#define SW_4    PA_7
#define SW_5    PA_6

// button inputs
DigitalIn buttonWuerfel(SW_1);
DigitalIn buttonCheatSix(SW_2);
DigitalIn buttonDiagnosemode(SW_3);
DigitalIn buttonCheatOne(SW_5);

DigitalOut DOt(SegDot);

//Interrupt
Ticker stSysTick;
volatile char bStandby;

//BUS
BusOut bus7Seg{SegA,SegB,SegC,SegD,SegE,SegF,SegG};


//Blink pattern
const uint8_t bPatternSeg[] = {0x79, 0x24, 0x30, 0x19,0x12,0x02};
const uint8_t bPatternOFF = 0x7F;

//4 Würfelreihenfolgen
const uint8_t   aPatternWuerfeln1[] ={3,5,0,2,1,4};
const uint8_t   aPatternWuerfeln2[] ={1,4,2,5,0,3};
const uint8_t   aPatternWuerfeln3[] ={0,2,5,3,1,4};
const uint8_t   aPatternWuerfeln4[] ={4,5,2,1,3,0};
//Adressen der 4 Würfelreihenfolgen
const uint8_t *tWuerfelPattern[]={aPatternWuerfeln1,aPatternWuerfeln2,aPatternWuerfeln3,aPatternWuerfeln4};


void vTimer();
void vWechsel();
void vTestButton();
void vAusrollen();

enum bWuerfelenum{enAusrollen,enWuerfeln};
bWuerfelenum bWuerfelState;
enum bAusrollmusterenum{Ausrollmuster1,Ausrollmuster2,Ausrollmuster3,Ausrollmuster4};
bAusrollmusterenum bAusrollmuster=Ausrollmuster1;

uint8_t bWurdegewuerfelt=0;
uint8_t bWuerfelStelle=0,bWuerfelpatternWahl=0; //Würfelstelle= Stelle im patternarray 
uint8_t bZeitscheibenCounter=9;
uint8_t bAusrolldauer=0;
uint8_t bCheatSIX=0;
uint8_t bCheatONE=0;
uint16_t bCheatTimerOne=0,bCheatTimerSix=0;
uint8_t bDiagnosemode=0;


// main() runs in its own thread in the OS
int main(){
    stSysTick.attach(&vTimer,5ms);

    bStandby=0;
    uint8_t bWuerfelCount=0;

    if (buttonDiagnosemode.read()==1){
        bDiagnosemode=1;
    }
    

    while (true) {
        
           
        if (bWuerfelCount>bZeitscheibenCounter) {                      //Alle 10 durchgänge also 10 X 5 ms = 50 ms
            bWuerfelCount=0;
            bWuerfelStelle++;
            if(bWuerfelStelle>5){                   //Würfelstelle ist immer kleiner 6
                bWuerfelStelle=0;
            }
            bWuerfelpatternWahl=0;                  //bWuerfelStelle % 4; //Zufällies Pattern wird gewählt
            vTestButton();                          //Teste Input
            vWechsel();                             //Schalte den würfel
        }
        bWuerfelCount++;
        /*bWuerfelStelle++;
        if(bWuerfelStelle>5){                       //Würfelstelle ist immer kleiner 6
            bWuerfelStelle=0;
        }*/
        
        if (bCheatTimerOne>0) {
            bCheatTimerOne--;
            if (bDiagnosemode==1) {
                DOt=0;
            }
        }else if (bCheatTimerSix>0){
            bCheatTimerSix--;
            if (bDiagnosemode==1) {
                DOt=0;
            }
            
        }else {
            DOt=1;
        }
        
        

        while (bStandby==0){
        }
        bStandby=0;
    }
}

void vTimer(){
    bStandby=1;
}

void vWechsel(){

    switch (bWuerfelState){                                     
    case enAusrollen:
        if (bWurdegewuerfelt==1) {              //
            vAusrollen();
        }else {                                     
            bus7Seg=bPatternOFF;
        }
      
        break;
    case enWuerfeln:
        bus7Seg=bPatternSeg[*(tWuerfelPattern[bWuerfelpatternWahl]+bWuerfelStelle)];
        break;
    } 
    
}

void vTestButton(){
    if (buttonWuerfel.read()==1) {
        bWuerfelState=enWuerfeln;           //Setzt den Würfelsatus
        bWurdegewuerfelt=1;                 //entscheidet ob ausgrollt wird
        bZeitscheibenCounter=9;             //Setzt die würfeldauer wieder au 50 ms 
        bAusrolldauer=0;                    //Setzt die Ausrolldauer zurück auf 0
        if (bCheatTimerOne>0) {
            bCheatONE=1;
        }else if (bCheatTimerSix>0){
            bCheatSIX=1;
        }
    }else{
        bWuerfelState=enAusrollen;
    }

    if (buttonCheatOne.read()==1) {
     bCheatTimerOne=300;
    }else if (buttonCheatSix.read()==1){
        bCheatTimerSix=300;
    }
}

void vAusrollen(){
    
    if (bAusrolldauer >8) {
       bCheatSIX=0;
       bCheatONE=0;
    }else if (bAusrolldauer==8) {
        if (bCheatONE==1){                      //hier muss 50/50 hin
            bus7Seg=bPatternSeg[0];
        }else if (bCheatSIX==1){
            bus7Seg=bPatternSeg[5];
        }
        bAusrolldauer++;
    }else{
        switch (bAusrollmuster) {                       /////hier muss random hin
            case Ausrollmuster1:
                bZeitscheibenCounter+=10;
                break;
            case Ausrollmuster2:
                bZeitscheibenCounter+=(5*bAusrolldauer);
                break;
            case Ausrollmuster3:
                break;
            case Ausrollmuster4:
                break;
            }
            bus7Seg=bPatternSeg[*(tWuerfelPattern[bWuerfelpatternWahl]+bWuerfelStelle)];
           
            bAusrolldauer++;
    }

    
}
