/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CK32MP503
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.50
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

//raspi-gpio set 17 ip pn //(before PIC programming)

/**
  Section: Included Files
 */
#include <stdio.h>
#include <string.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/i2c1.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "mcc_generated_files/pwm.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/sccp1_tmr.h"
#include "mcc_generated_files/interrupt_manager.h"
#include <p33CK32MP503.h>
#include "dsp.h"
/*
                         Main application
 */
#define ACC_I2C_ADDRESS ((uint8_t)0b0011001)
/*
 *  rxbuf[0]:
 *      b7-b4: typ wiadomo?ci:
 *          0000 - set the absolute position (0-3600, i.e. in degrees/10, i.e. with the accuracy to 1/10 of a degree) (position, direction and speed)
 *          0001 - set the absolute position with automatic direction (0-3600, i.e. in degrees/10, i.e. with the accuracy to 1/10 of a degree) (position and speed)
 *          0010 - ustaw pozycje wzgledna (-3600 do 3600, dokladnosc 1/10 stopnia) (pozycja,kierunek i szybkosc)
 *          0011 - ruch ciagly (szybkosc,kierunek)
 *          0100 - ustaw moc silnika w ruchu (0-100)
 *          0101 - ustaw moc i czas utrzymania mocy po ruchu (w spoczynku)
 *          0110 - zatrzymaj silnik
 *          0111 - pobierz i odeslij info
 *          1000 - reboot device - go to bootloader
 *          1001 - odczyt danych z akcelerometru
 *          1010 - zmien domyslny czas przerwania dla regulacji predkosci dzielnikiem
 *          1011 - ustaw liczbe par magnesow 7 dla 12N14P lub 11 dla 24N22P
 * 
 *              0 b3: kierunek '+', 1 - kierunek '-' dojscia do zadanego kata
 *               000 b2-b0: numer silnika (8 silników)
 * 
 *  rxbuf[1]:
 *      dla 0000:   b7-b4: pozycja MSB
 *                  b3-b0: szybkosc MSB
 *      dla 0001:   b7-b4: pozycja MSB
 *                  b3-b0: szybkosc MSB
 *      dla 0010:   b7-b4: pozycja MSB
 *                  b3-b0: szybkosc MSB
 *      dla 0011:   b7-b4: nie uzywane
 *                  b3-b0: szybkosc MSB
 *      dla 0100:   b7-b0: moc silnika 0-100
 *      dla 0101:   b7-b0: moc silnika w spoczynku 0-100
 *      dla 0110:   nie uzywane
 *      dla 1000:   0 - electrical position
 *                  1 - aktualneTorque
 *                  2 - lastMotionTime - odczyt czasu od ostatniego ruchu silników
 *                  3 - motor encoder (PWM)
 *      dla 1001:   nie uzywane 
 *      dla 1010: 
 *      dla 1011:   liczba par magnesow
 *      
 *  rxbuf[2]:
 *      dla 0000:   b7-b0: pozycja LSB (razem 4095, ale uzywamy stopni wiec 3600, czyli 360,0 czyli stopnie do 1/10 stopnia dokladnosci)
 *      dla 0001:   b7-b0: pozycja LSB (razem 4095, ale uzywamy stopni wiec 3600, czyli 360,0 czyli stopnie do 1/10 stopnia dokladnosci)
 *      dla 0010:   b7-b0: pozycja LSB (razem 4095, ale uzywamy stopni wiec 3600, czyli 360,0 czyli stopnie do 1/10 stopnia dokladnosci)
 *      dla 0011:   nie uzywane                
 *      dla 0100:   nie uzywane
 *      dla 0101:   b7-b0: czas IdleTime LSB 0-65535ms
 *      dla 0110:   nie uzywane
 *      dla 1000:   nie uzywane
 *      dla 1001:   nie uzywane 
 *      dla 1010: 
 *      dla 1011:   nie uzywane 
 * 
 *  rxbuf[3]:
 *      dla 0000:   b7-b0: szybkosc LSB (razem 4095, gdzie 4095=100...
 *      dla 0001:   b7-b0: szybkosc LSB (razem 4095, gdzie 4095=100...
 *      dla 0010:   b7-b0: szybkosc LSB (razem 4095, gdzie 4095=100...
 *      dla 0011:   b7-b0: szybkosc LSB (razem 4095, gdzie 4095=100...
 *      dla 0100:   nie uzywane
 *      dla 0101:   b7-b0: czas IdleTime MSB 0-65535ms
 *      dla 0110:   nie uzywane
 *      dla 1000:   nie uzywane
 *      dla 1001:   nie uzywane 
 *      dla 1010: 
 *      dla 1011:   nie uzywane 
 *  
*/


volatile uint8_t i2cBufferTX[8] = {0};
volatile uint8_t i2cBufferRX[8] = {0};
volatile I2C1_MESSAGE_STATUS i2c_status;

volatile uint16_t rxbuf[4] = {0}; //bufor SPI

bool accReadEnable = false;
volatile uint8_t accBuffer[6] = {0};

volatile float currentPositionA = 0, currentPositionB = 0; //obecna pozycja silnikow (7x360)/2 => -1260-0-1260
volatile float targetPositionA = 0, targetPositionB = 0; //zmiena do zadawania nowej pozycji silnika -1260-0-1260
volatile float electricalPositionA = 0, electricalPositionB = 0; //obecna pozycja w tablicy generujacej kolowe pole magnetyczne 0-360
volatile int16_t breakingZoneA = 100, breakingZoneB = 100; //strefa hamowania 0-500 
volatile int16_t StartA = 0, StartB = 0; //startuje, wi?c pozycja gdy ruch np. 360 stopni nie zako?czony lecz wystartowa? 
volatile float SpeedA = 0.5, SpeedB = 0.5; //pr?dko?? maksymalna 0.01-10 
volatile int16_t PolePairsA = 7, PolePairsB = 7; //7 dla 12N14P lub 11 dla 24N22P
volatile uint8_t lastMotionA=0, lastMotionB=0; //jak dawno ostatni ruch, zero gdy nadal trwa
volatile float lastSpeedA=0,lastSpeedB=0;
volatile bool newData = false;
volatile float torqueA = 0.0f, torqueB = 0.0f; //obecny moment silnikow
volatile float idleTorqueA = 0.0f, idleTorqueB = 0.0f;
volatile float curTorqueA = 0.0f, curTorqueB =0.0f;
volatile uint16_t keepTorqueTimeA = 300, keepTorqueTimeB = 300;
volatile uint8_t MoveContinuousA=0, MoveContinuousB=0;//ruch ci?g?y =1
volatile float dirA=1, dirB=1;//kierunek ruchu
volatile uint16_t defaultTimerPeriod = 0x0C34; //0x30D2 2ms, 0x1869 1ms, 0x0C34 0.5ms, 0x61a 0.25ms, 0x2a 0.01ms, 0xcd 0.034ms

static void Reset(void){
    asm ("reset");
}

void SCCP1_TMR_Timer32CallBack(){
    if (!accReadEnable){
        return;
    }
    memcpy((uint8_t*) accBuffer, (uint8_t*) i2cBufferRX, sizeof (accBuffer));
    i2cBufferTX[0] = 0x28 + 0x80; //0x80 - address auto increment
    I2C1_MasterWrite((uint8_t*) i2cBufferTX, 1, ACC_I2C_ADDRESS, (I2C1_MESSAGE_STATUS*) & i2c_status);
    I2C1_MasterRead((uint8_t*) i2cBufferRX, 6, ACC_I2C_ADDRESS, (I2C1_MESSAGE_STATUS*) & i2c_status);
}

float absf(float v) {
    if (v>=0)
        return v;
    else
        return -v;
}

float diffAngle(float anglecur, float angledest, uint8_t dir) {
    if (dir==1){
        if (angledest>anglecur)
            return angledest-anglecur;
        else
            return 360-(anglecur-angledest);
    } else {
        if (angledest<anglecur)
            return anglecur-angledest;
        else
            return 360-(angledest-anglecur);
    }        
}

float checkAngle(float angle) {    
    if (angle>=360) {
        int16_t a=angle/360;
        angle=(angle-a*360);
    }
    if (angle<0) {
        int16_t a=(-angle+360)/360;
        angle=(angle+a*360);
    }
    return angle;
}

void computeVoltageVector(float rotorPosition, float vectorAmplitude, float* voltageVector, float polePairs)
{
    //thx for https://github.com/ViktorAnchutin/motorControl
    static float initialRotorPosition=0;
    /// Compute d-q axis position
    /// compute the electric angle of the rotor with respect to the initial position. Initial position theoretically represents q axis alignment with an A stator axis.
    float theta_elec_degrees = ((rotorPosition - initialRotorPosition)*polePairs + 90 ); // 11 - pole pairs (22P). + 90 because at initial position theta = 90
	float theta = theta_elec_degrees*3.14159265358979323846/180;//Pi/180; // translating into radians

    /// Compute phase voltages
    /// compute a projection of the voltage vector in the q axis onto the stator's axis
    float Va_1 = cosf(theta);//cos(theta);
	float Vb_1 = cosf(theta - 2.0943951023931954923084289221863);//cos(theta - 2*Pi/3);
	float Vc_1 = cosf(theta + 2.0943951023931954923084289221863);//cos(theta + 2*Pi/3);

	float Va = Va_1 * vectorAmplitude; // projection calculation of Vq into A phase
	float Vb = Vb_1 * vectorAmplitude; // projection calculation of Vq into B phase
	float Vc = Vc_1 * vectorAmplitude; // projection calculation of Vq into C phase

	/// Compute invertor voltages
	/// Obtaining value for invertor, +50 because Vinv relates with V_phase as Vinv = Vphase + Vdc/2 in order to avoid negative values for invertor voltage. Vinv value should be 0-100%
	/// should also be taken into account that Vphase(max) = Vdc/2 (using sinusoidal commutation)
	voltageVector[0] = Va/2 + 31246;
	voltageVector[1] = Vb/2 + 31246;
	voltageVector[2] = Vc/2 + 31246;
}

/*
    Funkcja wywolywana co 0.5 ms.
    Czestotliwosc mozna dostosowac przez lub przez CodeConfigurator timera lub recznie w pliku tmr1.c
*/
void timer_interrupt(void){    
    static int16_t time_counterA = 0, time_counterB = 0; //liczniki czasu
    static float diffA = 0, diffB =0;
   
    breakingZoneA=(SpeedA*100);
    breakingZoneB=(SpeedB*100);       
    
//==============Driver A (Poziomy)    
    if(MoveContinuousA){
        currentPositionA=checkAngle(currentPositionA+SpeedA*dirA);
        electricalPositionA=currentPositionA;
        lastMotionA=0;                
        curTorqueA=torqueA;
    } else {
        if (currentPositionA==targetPositionA && StartA==0) {
            if(lastMotionA<255)
                lastMotionA++;
            if (time_counterA > 0) {
                //jest w pozycji target ale nie odpuszczaj momentu obrotowego przez jakis czas po zakonczeniu ruchu
                time_counterA--;
            } else { //daj moment na spoczynkowy
                curTorqueA = idleTorqueA;
            }
        } else {
            StartA=0;
            time_counterA = keepTorqueTimeA; //zadaj opoznienie po jakim zostanie zmniejszony moment trzymajacy silnika

            float spd=0;
            float step=0;
            diffA=diffAngle(currentPositionA,targetPositionA,dirA);
            if (diffA<=breakingZoneA){
                spd=SpeedA/(breakingZoneA/diffA+1);
                if(diffA>spd)
                    step=spd;
            }else 
            if (lastSpeedA<SpeedA){
                spd=lastSpeedA+(SpeedA/breakingZoneA);
                if(diffA>spd)
                    step=spd;
            }else{
                spd=SpeedA;
                if(abs(diffA)>spd){
                    step=spd;
                }
                else{
                    step=diffA;
                }
            } 
            lastSpeedA=spd;
                
            currentPositionA=checkAngle(currentPositionA+step*dirA);
            if (diffAngle(currentPositionA,targetPositionA,dirA)<0.01)
                currentPositionA=targetPositionA;

            curTorqueA=torqueA;
            electricalPositionA=checkAngle(currentPositionA);
            lastMotionA=0;        
        }
    }
    float vectorAmplitudeA = 6200*curTorqueA;
    float voltageVectorA[3] = {0};
    computeVoltageVector(electricalPositionA, vectorAmplitudeA, voltageVectorA, PolePairsA); /// compute the voltage vector based on the rotor's position   
    PWM_DutyCycleSet(PWM_GENERATOR_6, ((int16_t) voltageVectorA[0]));
    PWM_DutyCycleSet(PWM_GENERATOR_5, ((int16_t) voltageVectorA[1]));
    PWM_DutyCycleSet(PWM_GENERATOR_3, ((int16_t) voltageVectorA[2]));    
   
    /*
    Zadanie sygnalow do modulu PWM
    Modul jest skonfigurowanie do przyjmowania wartosci z zakresu 0-62492
    W ustawieniach peryferium PWM mozna modyfikowac powyzsza wartosc.
    Zwi?kszenie rozdzielczosci zmniejsza czestotliwosc pracy i na odwrot.
    Zmian mozna dokonac recznie w pwm.c lub przez CodeConfigurator
    */  
/*
    PWM_DutyCycleSet(PWM_GENERATOR_6, ((int16_t) (motor_table[electricalPositionA][0] * torqueA)) + 31246);
    PWM_DutyCycleSet(PWM_GENERATOR_5, ((int16_t) (motor_table[electricalPositionA][1] * torqueA)) + 31246);
    PWM_DutyCycleSet(PWM_GENERATOR_3, ((int16_t) (motor_table[electricalPositionA][2] * torqueA)) + 31246);
*/
//==============Driver B (Pionowy)
    if(MoveContinuousB){
        currentPositionB=checkAngle(currentPositionB+SpeedB*dirB);
        electricalPositionB=currentPositionB;
        lastMotionB=0;                
        curTorqueB=torqueB;
    } else {
        if (currentPositionB==targetPositionB && StartB==0) {
            if(lastMotionB<255)
                lastMotionB++;
            if (time_counterB > 0) {
                //jest w pozycji target ale nie odpuszczaj momentu obrotowego przez jakis czas po zakonczeniu ruchu
                time_counterB--;
            } else { //daj moment na spoczynkowy
                curTorqueB = idleTorqueB;
            }
        } else {
            StartB=0;
            time_counterB = keepTorqueTimeB; //zadaj opoznienie po jakim zostanie zmniejszony moment trzymajacy silnika

            float spd=0;
            float step=0;
            diffB=diffAngle(currentPositionB,targetPositionB,dirB);
            if (diffB<=breakingZoneB){
                spd=SpeedB/(breakingZoneB/diffB+1);
                if(diffB>spd)
                    step=spd;
            }else 
            if (lastSpeedB<SpeedB){
                spd=lastSpeedB+(SpeedB/breakingZoneB);
                if(diffB>spd)
                    step=spd;
            }else{
                spd=SpeedB;
                if(abs(diffB)>spd){
                    step=spd;
                }
                else{
                    step=diffB;
                }
            } 
            lastSpeedB=spd;
                
            currentPositionB=checkAngle(currentPositionB+step*dirB);
            if (step<0.01)
                currentPositionB=targetPositionB;

            curTorqueB=torqueB;
            electricalPositionB=checkAngle(currentPositionB);
            lastMotionB=0;        
        }
    }
    float vectorAmplitudeB = 6200*curTorqueB;
    float voltageVectorB[3] = {0};
    computeVoltageVector(electricalPositionB, vectorAmplitudeB, voltageVectorB, PolePairsB); /// compute the voltage vector based on the rotor's position
    PWM_DutyCycleSet(PWM_GENERATOR_2, ((int16_t) voltageVectorB[0]));
    PWM_DutyCycleSet(PWM_GENERATOR_1, ((int16_t) voltageVectorB[1]));
    PWM_DutyCycleSet(PWM_GENERATOR_4, ((int16_t) voltageVectorB[2]));    
}

/*
    Przerwanie peryferium SPI1. Odczytuje dane z bufora do tablicy.
    Domyslny rozmiar ramki to 4 bajty.
*/
void __attribute__ ( ( interrupt, no_auto_psv ) ) _SPI1RXInterrupt () {
//    uint16_t w=0;
//    while ( SPI1STATLbits.SPIRBE == true||++w<9999)
//    {}
    rxbuf[3] = 0xFF & SPI1BUFL;
    rxbuf[2] = 0xFF & (SPI1BUFL >> 8);
    rxbuf[1] = 0xFF & SPI1BUFH;
    rxbuf[0] = 0xFF & (SPI1BUFH >> 8);
    newData = true;
    IFS0bits.SPI1RXIF = false;
}
void spiBufReset(){
    SPI1BUFL=0;
    SPI1BUFH=0;
}

void parseMotorTimerPeriod() {
    defaultTimerPeriod = (uint16_t) rxbuf[1] << 8 | rxbuf[2]; //todo - sprawdzic czy dziala
//    if (rxbuf[3] & 0x80) {
//        /*
//         * rxbuf[3] == 128 odpowiada czasowi przerwania ok 62,5us
//         * rxbuf[3] == 128 + 8 odpowiada czasowi przerwania ok 0,5ms
//         * rxbuf[3] == 128 + 19 odpowiada czasowi przerwania ok 1ms
//         * rxbuf[3] == 128 + 100 odpowiada czasowi przerwania ok 5ms
//         *///
//        uint16_t timerPeriod = rxbuf[3] & 0x7F;
//        timerPeriod = (timerPeriod * 0x134) + 0x186;
//        TMR1_Period16BitSet(timerPeriod);
//        motorSpeedA = maxMotorParameterVal;
//        motorSpeedB = maxMotorParameterVal;
//    } else {
        TMR1_Period16BitSet(defaultTimerPeriod);
//    }   
}

void parseMotorMoveTorque() {
    float tmpTrq=0;
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B
        tmpTrq = (float) (rxbuf[1]);
        if(tmpTrq>100)
            tmpTrq=100;
        torqueB=tmpTrq/10;
    } else {
        tmpTrq = (float) (rxbuf[1]);
        if(tmpTrq>100)
            tmpTrq=100;
        torqueA=tmpTrq/10;
    }
}

void parseMotorIdleTorque() {
    float tmpTrq=0;
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B  
        tmpTrq = (float) (rxbuf[1]);
        if(tmpTrq>100)
            tmpTrq=100;
        idleTorqueB=tmpTrq/10;
        keepTorqueTimeB = (((uint16_t) (rxbuf[2]) << 8) | rxbuf[3]);
    } else {
        //sterowanie silnikiem A
        tmpTrq = (float) (rxbuf[1]);
        if(tmpTrq>100)
            tmpTrq=100;
        idleTorqueA=tmpTrq/10;
        keepTorqueTimeA = (((uint16_t) (rxbuf[2]) << 8) | rxbuf[3]);
    }   
}

void parseMotorMoveToAngle(bool autoCalcDir) {
    float tp=(uint16_t) (((rxbuf[1] & 0b11110000) << 4) | rxbuf[2]);
    tp=tp/10;
    float sp=(uint16_t) (((rxbuf[1] & 0b00001111) << 8) | rxbuf[3]);
    sp=sp/1000;
    
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B
        MoveContinuousB=0;
        if(autoCalcDir){
            float diff=currentPositionB-tp;
            dirB=diff>0?-1:1;
            if(abs(diff)>180.0)
                dirB*=-1;
        }else{
            dirB=(rxbuf[0] & 0b00001000)==0?-1:1;
        }
        SpeedB=sp;
        if (absf(tp-targetPositionB)>0.01){
            targetPositionB = tp;
            StartB=1;
        }
    } else {
        //sterowanie silnikiem A
        MoveContinuousA=0;
        if(autoCalcDir){
            float diff=currentPositionA-tp;
            dirA=diff>0?-1:1;
            if(abs(diff)>180.0)
                dirA*=-1;
        }else{
            dirA=(rxbuf[0] & 0b00001000)==0?-1:1;
        }
        SpeedA=sp;
        if (absf(tp-targetPositionA)>0.01){
            targetPositionA = tp;
            StartA=1;
        }
    }
}

void parseMotorMoveByAngle() {
    float tp=(uint16_t) (((rxbuf[1] & 0b11110000) << 4) | rxbuf[2]);
    tp=tp/10;    
    float sp=(uint16_t) (((rxbuf[1] & 0b00001111) << 8) | rxbuf[3]);
    sp=sp/1000;
    
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B
        MoveContinuousB=0;
        dirB=(rxbuf[0] & 0b00001000)==0?-1:1;
        if (dirB>0) {
            targetPositionB = checkAngle(electricalPositionB+tp);
        } else {
            targetPositionB = checkAngle(electricalPositionB-tp);
        }
        SpeedB=sp;
        StartB=1;
    } else {
        //sterowanie silnikiem A
        MoveContinuousA=0;        
        dirA=(rxbuf[0] & 0b00001000)==0?-1:1;
        if (dirA>0) {
            targetPositionA = checkAngle(electricalPositionA+tp);
        } else {
            targetPositionA = checkAngle(electricalPositionA-tp);
        }
        SpeedA=sp;
        StartA=1;
    }
}

void parseMotorMoveContinuous() {
    float sp=(uint16_t) (((rxbuf[1] & 0b00001111) << 8) | rxbuf[3]);

    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B
        dirB=(rxbuf[0] & 0b00001000)==0?-1:1;
        SpeedB=sp/1000;
        StartB=1;
        MoveContinuousB=1;        
    } else {
        //sterowanie silnikiem A
        dirA=(rxbuf[0] & 0b00001000)==0?-1:1;
        SpeedA=sp/1000;
        StartA=1;
        MoveContinuousA=1;
    }
}

void parseMotorStop() {
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B
        MoveContinuousB=0;
        targetPositionB = currentPositionB;
    } else {
        MoveContinuousA=0;
        targetPositionA = currentPositionA;
    }
}

void parseMotorGetInfo() {
    uint8_t info = rxbuf[1];
   
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        if (info==0){
            SPI1BUFL = electricalPositionB*10;
            SPI1BUFH = 0;
        } else 
        if (info==1){
            SPI1BUFL = curTorqueB*10;
            SPI1BUFH = 0;
        } else 
        if (info==2){
            SPI1BUFL = lastMotionB;
            SPI1BUFH = 0;
        }
        if (info==3){
            uint16_t encVal=0;
            getEnc2Val(&encVal);
            SPI1BUFL = encVal;
            SPI1BUFH = 0;
        }
    } else {
        if (info==0){
            SPI1BUFL = electricalPositionA*10;
            SPI1BUFH = 0;
        } else 
        if (info==1){
            SPI1BUFL = curTorqueA*10;
            SPI1BUFH = 0;
        } else 
        if (info==2){
            SPI1BUFL = lastMotionA;
            SPI1BUFH = 0;
        }       
        if (info==3){
            uint16_t encVal=0;
            getEnc1Val(&encVal);
            SPI1BUFL = encVal;
            SPI1BUFH =0;
        }
    }
}

void parseMotorPolePairs() {
    if (rxbuf[0] & 0b00000001) { //todo silniki nie tylko 2 ale 8 max
        //sterowanie silnikiem B
        PolePairsB = (float) (rxbuf[1]);
    } else {
        PolePairsA = (float) (rxbuf[1]);
    }
}


void Delay(uint16_t cycles){
    uint16_t i;
    for (i=0; i<cycles; i++){
        asm("nop");
    } 
}

void ACC_init() {    
    const uint8_t CTRL_REG1_ADDRESS = 0x20;
    const uint8_t CTRL_REG4_ADDRESS = 0x23;
//    const uint8_t CTRL_REG1_VAL = 0b10010111; //Normal mode 1.344kHz, axis X,Y,Z enabled
    const uint8_t CTRL_REG1_VAL = 0b01010111; //Normal mode 100Hz, axis X,Y,Z enabled
    const uint8_t CTRL_REG4_VAL = 0b00010000; //4g scale limit
     
    i2cBufferTX[0] = CTRL_REG1_ADDRESS; 
    i2cBufferTX[1] = CTRL_REG1_VAL;
    I2C1_MasterWrite((uint8_t*) i2cBufferTX, 2, ACC_I2C_ADDRESS, (I2C1_MESSAGE_STATUS*) & i2c_status);
    uint16_t w=0;
    while(i2c_status != I2C1_MESSAGE_COMPLETE && w<50000) w++;
    Delay(30000); 
    i2cBufferTX[0] = CTRL_REG4_ADDRESS; 
    i2cBufferTX[1] = CTRL_REG4_VAL;
    I2C1_MasterWrite((uint8_t*) i2cBufferTX, 2, ACC_I2C_ADDRESS, (I2C1_MESSAGE_STATUS*) & i2c_status);
    w=0;
    while(i2c_status != I2C1_MESSAGE_COMPLETE && w<50000) w++;
    Delay(30000); 
    
    accReadEnable = true;
}

// void SPI1_SendBuffer32bit(uint32_t *pTransmitBuffer, uint8_t buffSize) {
//    //wait until previous data will be sent
//    uint8_t dataSent = 0;
//    while (dataSent < buffSize) {
//        if (SPI1STATLbits.SPITBF != true) {
//            SPI1BUFL = 0xFFFF & pTransmitBuffer[dataSent];
//            SPI1BUFH = 0xFFFF & (pTransmitBuffer[dataSent] >> 16);
//            dataSent++;
//        }
//    }
// }

void readACCdata() {
    static uint8_t localBuffer[6];
//    static uint8_t mode=0;
//    if (mode == 0){
        //Inicjalizacja. Zapisz dane do bufora pomocniczego. Skopiuj pierwsze 4 bajty do bufora SPI
            IEC0bits.CCT1IE = 0;
            memcpy(localBuffer, (uint8_t*) accBuffer, sizeof (localBuffer));
            IEC0bits.CCT1IE = 1;
            SPI1BUFL = localBuffer[2] | (uint16_t)localBuffer[3]<<8;
            SPI1BUFH = localBuffer[4] | (uint16_t)localBuffer[5]<<8;
            if(SPI1BUFL==0&&SPI1BUFH==0&&accReadEnable){
                 ACC_init();
            }
/*            
            mode=1;
    } else if (mode == 1){
            SPI1BUFL = localBuffer[0] | (uint16_t)localBuffer[1]<<8;
            SPI1BUFH = localBuffer[2] | (uint16_t)localBuffer[3]<<8;
            mode=2;
    } else {
            SPI1BUFL = localBuffer[4] | (uint16_t)localBuffer[5]<<8;
            SPI1BUFH = 0;
            mode=0;
    }
*/    
}

int main(void)
{
    //Inicjalizacja peryferiow
    SYSTEM_Initialize();
    spiBufReset();
    //Wlaczenie kanalow wyjsiowych drivera A
    IO_RB7_SetHigh(); //EN1
    IO_RC5_SetHigh(); //EN2
    IO_RB5_SetHigh(); //EN3

    //Wlaczenie kanalow wyjsiowych drivera B
    IO_RB13_SetHigh(); //EN1
    IO_RB15_SetHigh(); //EN2
    IO_RB11_SetHigh(); //EN3

    //Uruchomienie driverow (wyjscie z resetu)
    IO_RA1_SetHigh(); //enable driver A
    IO_RA3_SetHigh(); //enable driver A

    //Inicjalizacja modulu PWM dopiero po skonfigurowaniu driverow                                                                     
    PWM_Initialize();

    //Inicjalizacja glownego przerwania programowego i uruchomienie sygnalow PWM
    TMR1_SetInterruptHandler(&timer_interrupt);
    TMR1_Start();
    PWM_Enable();
    
    SCCP1_TMR_Start();
    ACC_init();
    
    TMR1_Period16BitSet(defaultTimerPeriod);
    //glowna petla programu interpretujaca dane odczytane po SPI (od RPI)
    while (1) {   
       // uint32_t encVal=getEnc2Val();
        if (newData) {
            //Wylacz przerwania na czas modyfikacji parametrow
            //IEC0bits.SPI1RXIE = false;
            //IEC0bits.T1IE = false;
            newData = false;
            uint8_t msgType = rxbuf[0] >> 4;


            switch (msgType) {
                case 0:
                    parseMotorMoveToAngle(false);
                    break;
                case 1:
                    parseMotorMoveToAngle(true);
                    break;
                case 2:
                    IEC0bits.SPI1RXIE = false;//to do - czy to potrzebne?
                    IEC0bits.T1IE = false;
                    parseMotorMoveByAngle();
                    IEC0bits.SPI1RXIE = true;
                    IEC0bits.T1IE = true;
                    break;

                case 3:
                    parseMotorMoveContinuous();
                    break;
                    
                case 4:
                    parseMotorMoveTorque();
                    break;
                    
                case 5:
                    parseMotorIdleTorque();
                    break;

                case 6: 
                    parseMotorStop();
                    break;

                case 7:
                    parseMotorGetInfo();
                    break;
                    
                case 8:
                    Reset();                    
                    break;
                    
                case 9:
                    readACCdata();
                    break;
                    
                case 10:
                    IEC0bits.SPI1RXIE = false;
                    IEC0bits.T1IE = false;
                    parseMotorTimerPeriod();
                    IEC0bits.SPI1RXIE = true;
                    IEC0bits.T1IE = true;                    
                    break;
                    
                case 11:
                    parseMotorPolePairs();
                    break;

                case 12:
                    break;

                case 13:
                    break;
           
                default:
                    break;
            }
            //Wlacz ponownie przerwania
            //IEC0bits.SPI1RXIE = true;
            //IEC0bits.T1IE = true;
        }
    }
    return -1;
}
/**
 End of File
*/
