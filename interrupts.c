/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <string.h>
#include <pic16f1615.h>

#include "user.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Note that some PIC16's 
 * are baseline devices.  Unfortunately the baseline detection macro is 
 * _PIC12 */
#ifndef _PIC12

static inline void __unhandled_isr(void);
static inline void __Timer5_isr(void);
static inline void __IOC_isr(void);

void interrupt isr(void)
{
    if(PIR5bits.TMR5IF) {
        __Timer5_isr();
    } else if(INTCONbits.IOCIF){
        __IOC_isr();
    } else {
        __unhandled_isr();
    }

}

static inline void __unhandled_isr(void)
{
    while(1) {
        NOP();
    }
}

/**
 * __Timer5_isr
 * @brief - Update video buffer
 */
static inline void __Timer5_isr(void)
{
    ASSERT_SS();
    SPI_SendByte(1 << video_buffer.digit);
    SPI_SendByte(video_buffer.buffer1[video_buffer.digit]);    
    DEASSERT_SS();
    
    video_buffer.digit = (uint8_t)((video_buffer.digit + 1) & 0x0F);
    
    /* If digit roll-over then enable transfer from buffer2 to buffer1 */
    if(!video_buffer.digit)
        memcpy(video_buffer.buffer1, video_buffer.buffer2, 4);
    
    TMR5H = 0xEF;
    TMR5L += 0x78;
    PIR5bits.TMR5IF = 0;
    
}

/**
 * __IOC_isr
 * @brief - Handle buttons interrupts
 */
static inline void __IOC_isr(void)
{
    if(IOCAFbits.IOCAF4) {
        gd.buttons.btn1 = !BUTTON1;
        IOCAFbits.IOCAF4 = 0;
    }
    
    if(IOCAFbits.IOCAF5) {
        gd.buttons.btn2 = !BUTTON2;
        IOCAFbits.IOCAF5 = 0;
    }
    
    /* Motor1 count */
    if(IOCAFbits.IOCAF0) {
        if(PORTAbits.RA0 == 0)
            *(gd.motors[0].speed) = 32;
        else
            *(gd.motors[0].speed) = 0;
        IOCAFbits.IOCAF0 = 0;
    }
    
    /* Motor2 count */
    if(IOCAFbits.IOCAF1) {
        if(PORTAbits.RA1 == 0)
            *(gd.motors[1].speed) = 127;
        else
            *(gd.motors[1].speed) = 0;
        IOCAFbits.IOCAF1 = 0;
    }
    
    INTCONbits.IOCIF = 0;
    
}
#endif


