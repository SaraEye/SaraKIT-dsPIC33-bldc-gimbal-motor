/**
  SCCP2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    sccp2.c

  @Summary
    This is the generated driver implementation file for the SCCP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for SCCP2. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  dsPIC33CK32MP503
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
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

/**
  Section: Included Files
*/

#include "sccp2_capture.h"


void SCCP2_CAPTURE_Initialize(void)
{
    // CCPON enabled; MOD Every falling edge; CCSEL enabled; CCPSIDL disabled; T32 16 Bit; CCPSLP disabled; TMRPS 1:1; CLKSEL FOSC/2; TMRSYNC disabled; 
    CCP2CON1L = (0x8012 & 0x7FFF); //Disabling CCPON bit
    //RTRGEN disabled; ALTSYNC disabled; ONESHOT disabled; TRIGEN disabled; OPS Each IC Event; SYNC None; OPSSRC Timer Interrupt Event; 
    CCP2CON1H = 0x00;
    //ASDGM disabled; SSDG disabled; ASDG 0; PWMRSEN disabled; 
    CCP2CON2L = 0x00;
    //ICGSM Level-Sensitive mode; ICSEL IC2; AUXOUT Disabled; OCAEN disabled; OENSYNC disabled; 
    CCP2CON2H = 0x00;
    //OETRIG disabled; OSCNT None; POLACE disabled; PSSACE Tri-state; 
    CCP2CON3H = 0x00;
    //ICDIS disabled; SCEVT disabled; TRSET disabled; ICOV disabled; ASEVT disabled; ICGARM disabled; TRCLR disabled; 
    CCP2STATL = 0x00;
    //TMR 0; 
    CCP2TMRL = 0x00;
    //TMR 0; 
    CCP2TMRH = 0x00;
    //PR 0; 
    CCP2PRL = 0x00;
    //PR 0; 
    CCP2PRH = 0x00;
    //CMP 0; 
    CCP2RA = 0x00;
    //CMP 0; 
    CCP2RB = 0x00;
    //BUF 0; 
    CCP2BUFL = 0x00;
    //BUF 0; 
    CCP2BUFH = 0x00;

    CCP2CON1Lbits.CCPON = 0x1; //Enabling CCP

    // Clearing IF flag before enabling the interrupt.
    IFS1bits.CCP2IF = 0;
    // Enabling SCCP2 interrupt.
    IEC1bits.CCP2IE = 1;

    // Clearing IF flag before enabling the interrupt.
    IFS1bits.CCT2IF = 0;
    // Enabling SCCP2 interrupt.
    IEC1bits.CCT2IE = 1;
}

void SCCP2_CAPTURE_Start( void )
{
    /* Start the Timer */
    CCP2CON1Lbits.CCPON = true;
}

void SCCP2_CAPTURE_Stop( void )
{
    /* Stop the Timer */
    CCP2CON1Lbits.CCPON = false;
}

void getEnc2Val(uint16_t *enc){
    *enc=enc2;
}
void __attribute__ ((weak)) SCCP2_CAPTURE_CallBack(void)
{    
        uint16_t rtime=0;
        if(SCCP4_CAPTURE_IsBufferEmpty()||SCCP2_CAPTURE_IsBufferEmpty()){
            return;
        }
        while(!SCCP4_CAPTURE_IsBufferEmpty()){
            rtime=SCCP4_CAPTURE_Data16Read();
        }
        uint16_t ftime=0;
        while(!SCCP2_CAPTURE_IsBufferEmpty()){
            ftime=SCCP2_CAPTURE_Data16Read();
        }
        if(ftime>rtime){
            enc2 =ftime-rtime;
        }               
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCP2Interrupt ( void )
{
    /* Check if the Timer Interrupt/Status is set */
    if(IFS1bits.CCP2IF)
    {
		// SCCP2 CAPTURE callback function 
		SCCP2_CAPTURE_CallBack();
        
        IFS1bits.CCP2IF = 0;
    }
}

void __attribute__ ((weak)) SCCP2_CAPTURE_TimerCallBack(void)
{
    // Add your custom callback code here
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _CCT2Interrupt ( void )
{
    if(IFS1bits.CCT2IF)
    {
		// SCCP2 CAPTURE Timer callback function 
		SCCP2_CAPTURE_TimerCallBack();

        IFS1bits.CCT2IF = 0;
    }
}

uint16_t SCCP2_CAPTURE_Data16Read( void )
{
    /* Return the captured timer data */
    return(CCP2BUFL);

}


bool SCCP2_CAPTURE_HasBufferOverflowed( void )
{
    return( CCP2STATLbits.ICOV );
}

bool SCCP2_CAPTURE_IsBufferEmpty( void )
{
    return( ! CCP2STATLbits.ICBNE );
}

void SCCP2_CAPTURE_OverflowFlagReset( void )
{
    CCP2STATLbits.ICOV = 0;
}
/**
 End of File
*/