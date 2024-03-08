/*
 * File:   new_main.c
 * Author: Logan
 */

// 'C' source line config statements
// CONFIG
#pragma config FOSC = HS   // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include <pic.h>
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#define uchar unsigned char
#define uint unsigned int
/*THESE DEFINE ARE FOR THE KEYBOARD*/
#define WI1 RA0
#define WI2 RA1
#define WI3 RA2
#define WI4 RA3
#define HI1 RB0
#define HI2 RB1
#define HI3 RB2
#define HI4 RB4
/*********************************/
/*THESE DEFINE ARE FOR BUZ*/
#define BUZ RB3
#define L1 1
#define L1_ 2
#define L2 3
#define L2_ 4
#define L3 5
#define L4 6
#define L4_ 7
#define L5 8
#define L5_ 9
#define L6 10
#define L6_ 11
#define L7 12
#define M1 13
#define M1_ 14
#define M2 15
#define M2_ 16
#define M3 17
#define M4 18
#define M4_ 19
#define M5 20
#define M5_ 21
#define M6 22
#define M6_ 23
#define M7 24
#define H1 25
#define H1_ 26
#define H2 27
#define H2_ 28
#define H3 29
#define H4 30
#define H4_ 31
#define H5 32
#define H5_ 33
#define H6 34
#define H6_ 35
#define H7 36

#define ClockSpeed 4000000 // Hz
#define SongSpeed 330      // ms
/******************************************/
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625
/******************************************/
// frequency
unsigned int freq_table[] = {
    0, 61714, 61928, 62131, 62322, 62502, 62673, 62833, 62985, 63128, 63263, 63391, 63511 // low
    ,
    63628, 63731, 63835, 63928, 64021, 64103, 64185, 64260, 64331, 64400, 64463, 64524 // midum
    ,
    64580, 64633, 64684, 64732, 64777, 64820, 64860, 64898, 64934, 64968, 65000, 65030 // high
};
// molihua
unsigned char song[] = {
    //
    17, 2, 17, 1,
    20, 1, 22, 1, 25, 1, 25, 1, 22, 1, 20, 2, 20, 1, 22, 1, 20, 4,
    M3, 2, M3, 1, M5, 1, M6, 1, H1, 1, H1, 1, M6, 1, M5, 2, M5, 1, M6, 1, M5, 4,
    M5, 2, M5, 2, M5, 2, M3, 1, M5, 1, M6, 2, M6, 2, M5, 4,
    M3, 2, M2, 1, M3, 1, M5, 2, M3, 1, M2, 1, M1, 2, M1, 1, M2, 1, M1, 4,
    M3, 1, M2, 1, M1, 1, M3, 1,
    // M2, 2, 0, 1, M3, 1, M5, 2, M6, 1, H1, 1, M5, 4,
    // M2, 2, M3, 1, M5, 1, M2, 1, M3, 1, M1, 1, L6, 1, L5, 4, L6, 2, L6, 1, L1, 1,
    // M2, 3, M3, 1, M1, 1, M2, 1, M1, 1, L6, 1, L5, 8, 40
};
unsigned char freq_select;
int song_Length = sizeof(freq_table) / sizeof(freq_table[0]);
/*********************************/
//** one command cost 1us**//
/*These are the const*/
const uchar LED_code[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
const uint keyboard_1[] = {1, 4, 7, '*'};
const uint keyboard_2[] = {2, 5, 8, 0};
const uint keyboard_3[] = {3, 6, 9, '#'};
const uint keyboard_4[] = {'A', 'B', 'C', 'D'};
/* the key board is this martix*/
// 1 2 3 A
// 4 5 6 B
// 7 8 9 C
// * 0 # D
/*********************/
// A stand for 校时/定闹 B stand for 时 C stand for 分 D stand for 秒
// # stand for 清定闹 * stand for 回车
/*These are the variable*/
// 0x80 hour 1 0x40 hour 2
// 0x20 fen 1 0x10 fen 2
// 0x08 miao 1 0x04 miao 2
// 0x02 mmiao 1 0x01 mmiao 2

// 0x08 hour 1 0x04 hour 2
// 0x02 fen 1 0x01 fen 2
// 0x80 miao 1 0x40 miao 2
// 0x20 mmiao1 1 0x10 mmiao2
uint count = 0;                     // setup the counter to correct the TIMER0
int Clock_set[] = {0, 0, 0, 0};     // Hour,Minute,second,msecond
int Clock_set_now[] = {0, 0, 0, 0}; // Hour,Minute,second,msecond
char function = 0;
int gain_time_delay = 20;
// 1 stand for 校时 2 stand for 定时
/*********************/
void initialize(); // initialize the port
// void testLED();    // this is the code for testing the LED
void delay_LED(); // just for delay_LED
/*these codes are for showing time*/
void showhour(int);                       // show the hours
void showmin(int);                        // show the minutes
void showsec(int);                        // show the secconds
void showmsec(int);                       // show the mseconds
void showLED(int *, int *, int *, int *); // begin to show the LED//success!
void buzzer();
int buzzer_time_cal = -1;
void delay_BUZ(uint);
void delay(uint); // delay us
void Timer1Init_BUZ();
void Timer0Init_Clock();
int get_function();
void correct_time();
void clock_function();
char gain_clock_unit();
void gain_hour();
void gain_minute();
void gain_second();
int delay_gain_time_hour_1();
int delay_gain_time_hour_2();
int delay_gain_time_minute_1();
int delay_gain_time_minute_2();
int delay_gain_time_second_1();
int delay_gain_time_second_2();
// void __interrupt(high_priority) ISR();
/*these codes are for gaining the input*/
int gain(); // gain the input number
/***************************************/
void main(void)
{
    initialize();
    function = get_function();
    if (function == 2)
    {
        correct_time();
    }
    else if (function == 1)
    {
        clock_function();
    }
    /****************************/
}
void delay_LED()
{
    for (int i = 0; i < 30; i++)
    {
    }
}
void Timer0Init_Clock() // initialize the interrupt
{
    count = 0;
    T0CS = 0;    // TMR0时钟源选择位   如果T0CS=0，TMR0的时钟源 选择 内部指令周期（fosc/4）
    PSA = 1;     // 预分频器分配给WDT（看门狗），也就是1:1分频  此时PS0~PS2无效
    TMR0 = 0x0b; // 8位计数寄存器赋初值
    T0IF = 0;    // TMR0的溢出中断标志位 清零，从而计数寄存器满的时候，T0IF产生一个上升沿。
    T0IE = 1;    // TMR0IE 中断允许控制位 置一
    GIE = 1;     // 总中断允许
    PEIE = 1;
}
//
void showLED(int *Hour, int *Minute, int *Seconds, int *Mseconds) // begin to show the LED
{
    for (;;)
    {
        TRISC = 0; // out
        TRISD = 0; // out
        showhour(*Hour);
        showmin(*Minute);
        showsec(*Seconds);
        showmsec(*Mseconds);
        if (*Hour == Clock_set[0] && *Minute == Clock_set[1] && *Seconds == Clock_set[2] && *Mseconds == Clock_set[3])
            break;
    }
}
void showhour(int Hour) // show the hours //success
{
    // PORTD = 0x01;
    PORTC = 0xff;
    delay(10);
    PORTD = 0x08;
    PORTC = LED_code[Hour / 10];
    delay_LED();
    PORTC = 0xff;
    delay(10);
    PORTD = 0x04;
    PORTC = LED_code[Hour % 10];
    delay_LED();
}
void showmin(int Minute) // show the minutes
{
    // PORTD = 0x04;
    PORTC = 0xff;
    delay(10);
    PORTD = 0x02;
    PORTC = LED_code[Minute / 10];
    delay_LED();
    PORTC = 0xff;
    delay(10);
    PORTD = 0x01;
    PORTC = LED_code[Minute % 10];
    delay_LED();
}
void showsec(int Seconds) // show the secconds
{
    // PORTD = 0x01;
    PORTC = 0xff;
    delay(10);
    PORTD = 0x80;
    PORTC = LED_code[Seconds / 10];
    delay_LED();
    PORTC = 0xff;
    delay(10);
    PORTD = 0x40;
    PORTC = LED_code[Seconds % 10];
    delay_LED();
}
void showmsec(int Mseconds) // show the mseconds
{
    PORTC = 0x00;
    delay(10);
    PORTD = 0x20;
    PORTC = LED_code[Mseconds / 10];
    delay_LED();
    PORTC = 0x00;
    delay(10);
    PORTD = 0x10;
    PORTC = LED_code[Mseconds % 10];
    delay_LED();
}
/********************************************/
/**
 * @boxw987
 * 2024.3.3
 */
/********************************************/
int gain()
{
    for (;;)
    {
        // change the way to realize 2.29//
        // four way
        // first The WI1 output is zero
        TRISA = 0x00;         // set WI is output
        TRISB = TRISB | 0x17; // set the keyboard HI is input
        PORTA = 0xFE;         // 1110
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_4[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_4[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_4[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_4[3];
        }
        PORTA = 0xFD; // 1101
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_3[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_3[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_3[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_3[3];
        }
        PORTA = 0xFB; // 1011
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_2[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_2[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_2[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_2[3];
        }
        PORTA = 0xF7; // 0111
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_1[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_1[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_1[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_1[3];
        }
    }
}
void buzzer()
{
    PORTC = 0x00;
    PORTD = 0x00;
    TRISA = 0x00;         // set WI is output
    TRISB = TRISB | 0x17; // set the keyboard HI is input
    PORTA = 0xFD;         // 1101
    buzzer_time_cal = 1;
    Clock_set_now[0] = 0;
    Clock_set_now[1] = 0;
    Clock_set_now[2] = 0;
    Clock_set_now[3] = 0;
    Clock_set[0] = 0;
    Clock_set[1] = 0;
    Clock_set[2] = 10;
    Clock_set[3] = 0;
    Timer0Init_Clock();
    for (; HI4 == 1 && buzzer_time_cal == 1;)
    {
        for (int select = 0; select < song_Length && HI4 == 1 && buzzer_time_cal == 1;)
        // when the song isn't end and the button isn't pressed
        {
            Timer1Init_BUZ();
            freq_select = song[select];
            if (freq_select)
            {
                select++;
                delay_BUZ(song[select] * SongSpeed);
                TMR1IE = 0;
                delay_BUZ(10);
                TMR1IE = 1;
                select++;
            }
            else
            {
                TMR1IE = 0;
                select++;
                delay_BUZ(song[select] * SongSpeed);
                TMR1IE = 1;
                select++;
            }
        }
    }
    TMR0IE = 0;
    TMR0IF = 0;
    count = 0;
    TMR1IE = 0;
    BUZ = 0;
}
void Timer1Init_BUZ() // Timer 1 is for the song
{
    T1CKPS0 = 0; // 1:1
    T1CKPS1 = 0;

    TMR1CS = 0; // 定时器模式
    TMR1IE = 1;

    TMR1H = 0xfb; // 10ms@4MHz
    TMR1L = 0x09;

    TMR1ON = 1;
    PEIE = 1;
    GIE = 1;
}
void __interrupt(high_priority) ISR()
{
    if (TMR1IE == 1 && TMR1IF == 1)
    {
        BUZ = !BUZ;
        TMR1H = freq_table[freq_select] / 256;
        TMR1L = freq_table[freq_select] % 256;
        TMR1IF = 0;
    }
    // Timer has not been defined

    if (TMR0IE == 1 && TMR0IF == 1)
    {
        if (count >= 35)
        {
            //    Clock_set = {0, 0, 0}; // Hour,Minute,second
            T0IE = 0;
            T0IF = 0;
            count = 0;
            TMR0 = 0x0b;
            if (Clock_set[0] == Clock_set_now[0] && Clock_set[1] == Clock_set_now[1] && Clock_set[2] == Clock_set_now[2] && Clock_set[3] == Clock_set_now[3])
            {
                if (buzzer_time_cal == -1)
                {
                    return;
                }
                else if (buzzer_time_cal == 1)
                {
                    buzzer_time_cal = 2;
                    return;
                }
            }
            else
            {
                T0IE = 1;
                if (Clock_set_now[3] == 99)
                {
                    Clock_set_now[3] = 0;
                    if (Clock_set_now[2] == 59)
                    {
                        Clock_set_now[2] = 0;
                        if (Clock_set_now[1] == 59)
                        {
                            Clock_set_now[1] = 0;
                            Clock_set_now[0]++;
                        }
                        else
                        {
                            Clock_set_now[1]++;
                        }
                    }
                    else
                    {
                        Clock_set_now[2]++;
                    }
                }
                else
                {
                    Clock_set_now[3]++;
                }
            }
        }
        else
        {
            T0IF = 0;
            TMR0 = 0x1b;
            T0IE = 1;
            count++;
        }
    }
}
void delay_BUZ(uint duration)
{
    duration--;
    for (int i = 0; i < 3000; i += 1)
    {
    }
}
void initialize()
{
    TRISA = 0xFF; // set the keyboard WI is input
    TRISB = 0x17; // set the keyboard HI is input
    BUZ = 0;
    TRISC = 0; // out
    TRISD = 0; // out
    PORTC = 0x00;
    PORTD = 0x00;
}
void delay(uint us)
{
    for (int i = 0; i < us; i++)
    {
    }
}
int get_function()
{
    PORTD = 0x08;
    PORTC = 0xF7;
    TRISA = 0x00;
    TRISB = TRISB | 0x17;
    PORTA = 0xFE;
    for (;;)
    {
        if (RB0 == 0)
        {
            PORTC = 0xFF;
            for (; RB0 == 0;)
            {
            }
            delay(100);
            for (;;)
            {
                TRISA = 0x00;
                TRISB = TRISB | 0x17;
                PORTA = 0xFE;
                if (RB0 == 0)
                {
                    for (; RB0 == 0;)
                    {
                    }
                    return 1;
                }
                PORTA = 0xF7;
                if (RB4 == 0)
                {
                    for (; RB4 == 0;)
                    {
                    }
                    return 2;
                }
            }
        }
    }
}
void correct_time()
{
    Clock_set[0] = 0;
    Clock_set[1] = 0;
    Clock_set[2] = 10;
    Clock_set[3] = 0;
    Timer0Init_Clock();
    showLED(&Clock_set_now[0], &Clock_set_now[1], &Clock_set_now[2], &Clock_set_now[3]);
    buzzer();
}
void clock_function()
{
    char choice = -1;
    for (; choice != '*';)
    {
        choice = gain_clock_unit();
        if (choice == 'B')
        {
            gain_hour();
        }
        else if (choice == 'C')
        {
            gain_minute();
        }
        else if (choice == 'D')
        {
            gain_second();
        }
    }
    Timer0Init_Clock();
    showLED(&Clock_set_now[0], &Clock_set_now[1], &Clock_set_now[2], &Clock_set_now[3]);
    buzzer();
}
void gain_hour()
{
    int temp1 = 0, temp2 = 0;
    PORTD = 0x08;
    PORTC = 0xF7;
    temp1 = delay_gain_time_hour_1();
    Clock_set[0] = 10 * temp1;
    temp2 = delay_gain_time_hour_2();
    Clock_set[0] += temp2;
    temp1 = delay_gain_time_hour_1();
    Clock_set_now[0] = 10 * temp1;
    temp2 = delay_gain_time_hour_2();
    Clock_set_now[0] += temp2;
}
void gain_minute()
{
    int temp1 = 0, temp2 = 0;
    PORTD = 0x02;
    PORTC = 0xF7;
    temp1 = delay_gain_time_minute_1();
    Clock_set[1] = 10 * temp1;
    temp2 = delay_gain_time_minute_2();
    Clock_set[1] += temp2;
    temp1 = delay_gain_time_minute_1();
    Clock_set_now[1] = 10 * temp1;
    temp2 = delay_gain_time_minute_2();
    Clock_set_now[1] += temp2;
}
void gain_second()
{
    int temp1 = 0, temp2 = 0;
    PORTD = 0x80;
    PORTC = 0xF7;
    temp1 = delay_gain_time_second_1();
    Clock_set[2] = 10 * temp1;
    temp2 = delay_gain_time_second_2();
    Clock_set[2] += temp2;
    temp1 = delay_gain_time_second_1();
    Clock_set_now[2] = 10 * temp1;
    temp2 = delay_gain_time_second_2();
    Clock_set_now[2] += temp2;
}
int delay_gain_time_hour_1()
{
    int cal = 1;
    for (;;)
    {
        showmin(Clock_set[1]);
        showsec(Clock_set[2]);
        showmsec(Clock_set[3]);
        PORTD = 0x08;
        if (cal == 1)
        {
            cal = 0;
            PORTC = 0xF7;
        }
        else
        {
            cal = 1;
            PORTC = 0xFF;
        }
        for (int i = 0; i < gain_time_delay; i++)
        {
            TRISA = 0x00;         // set WI is output
            TRISB = TRISB | 0x17; // set the keyboard HI is input
            PORTA = 0xFE;         // 1110
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_4[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_4[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_4[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_4[3];
            }
            PORTA = 0xFD; // 1101
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_3[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_3[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_3[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_3[3];
            }
            PORTA = 0xFB; // 1011
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_2[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_2[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_2[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_2[3];
            }
            PORTA = 0xF7; // 0111
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_1[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_1[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_1[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_1[3];
            }
        }
    }
}
int delay_gain_time_hour_2()
{
    int j = 1;
    for (;;)
    {
        showmin(Clock_set[1]);
        showsec(Clock_set[2]);
        showmsec(Clock_set[3]);
        if (j == 1)
        {
            j = 0;
        }
        if (j == 0)
        {
            j = 1;
        }
        for (int i = 0; i < gain_time_delay; i++)
        {
            if (i > gain_time_delay / 2)
            {
                PORTD = 0x08;
                PORTC = LED_code[Clock_set[0] / 10];
            }
            if (i < gain_time_delay / 2)
            {
                if (j == 1)
                {
                    PORTD = 0x04;
                    PORTC = 0xF7;
                }
                if (j == 0)
                {
                    PORTD = 0x04;
                    PORTC = 0xFF;
                }
            }
            TRISA = 0x00;         // set WI is output
            TRISB = TRISB | 0x17; // set the keyboard HI is input
            PORTA = 0xFE;         // 1110
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_4[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_4[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_4[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_4[3];
            }
            PORTA = 0xFD; // 1101
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_3[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_3[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_3[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_3[3];
            }
            PORTA = 0xFB; // 1011
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_2[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_2[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_2[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_2[3];
            }
            PORTA = 0xF7; // 0111
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_1[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_1[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_1[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_1[3];
            }
        }
    }
}
int delay_gain_time_minute_1()
{
    int cal = 1;
    for (;;)
    {
        showhour(Clock_set[0]);
        showsec(Clock_set[2]);
        showmsec(Clock_set[3]);
        PORTD = 0x02;
        if (cal == 1)
        {
            cal = 0;
            PORTC = 0xF7;
        }
        else
        {
            cal = 1;
            PORTC = 0xFF;
        }
        for (int i = 0; i < gain_time_delay; i++)
        {
            TRISA = 0x00;         // set WI is output
            TRISB = TRISB | 0x17; // set the keyboard HI is input
            PORTA = 0xFE;         // 1110
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_4[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_4[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_4[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_4[3];
            }
            PORTA = 0xFD; // 1101
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_3[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_3[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_3[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_3[3];
            }
            PORTA = 0xFB; // 1011
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_2[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_2[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_2[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_2[3];
            }
            PORTA = 0xF7; // 0111
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_1[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_1[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_1[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_1[3];
            }
        }
    }
}
int delay_gain_time_minute_2()
{
    int j = 1;
    for (;;)
    {
        showhour(Clock_set[0]);
        showsec(Clock_set[2]);
        showmsec(Clock_set[3]);
        if (j == 1)
        {
            j = 0;
        }
        if (j == 0)
        {
            j = 1;
        }
        for (int i = 0; i < gain_time_delay; i++)
        {
            if (i > gain_time_delay / 2)
            {
                PORTD = 0x02;
                PORTC = LED_code[Clock_set[1] / 10];
            }
            if (i < gain_time_delay / 2)
            {
                if (j == 1)
                {
                    PORTD = 0x01;
                    PORTC = 0xF7;
                }
                if (j == 0)
                {
                    PORTD = 0x01;
                    PORTC = 0xFF;
                }
            }
            TRISA = 0x00;         // set WI is output
            TRISB = TRISB | 0x17; // set the keyboard HI is input
            PORTA = 0xFE;         // 1110
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_4[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_4[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_4[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_4[3];
            }
            PORTA = 0xFD; // 1101
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_3[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_3[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_3[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_3[3];
            }
            PORTA = 0xFB; // 1011
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_2[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_2[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_2[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_2[3];
            }
            PORTA = 0xF7; // 0111
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_1[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_1[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_1[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_1[3];
            }
        }
    }
}
int delay_gain_time_second_1()
{
    int cal = 1;
    for (;;)
    {
        showhour(Clock_set[0]);
        showmin(Clock_set[1]);
        showmsec(Clock_set[3]);
        PORTD = 0x80;
        if (cal == 1)
        {
            cal = 0;
            PORTC = 0xF7;
        }
        else
        {
            cal = 1;
            PORTC = 0xFF;
        }
        for (int i = 0; i < gain_time_delay; i++)
        {
            TRISA = 0x00;         // set WI is output
            TRISB = TRISB | 0x17; // set the keyboard HI is input
            PORTA = 0xFE;         // 1110
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_4[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_4[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_4[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_4[3];
            }
            PORTA = 0xFD; // 1101
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_3[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_3[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_3[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_3[3];
            }
            PORTA = 0xFB; // 1011
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_2[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_2[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_2[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_2[3];
            }
            PORTA = 0xF7; // 0111
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_1[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_1[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_1[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_1[3];
            }
        }
    }
}
int delay_gain_time_second_2()
{
    int j = 1;
    for (;;)
    {
        showhour(Clock_set[0]);
        showmin(Clock_set[1]);
        showmsec(Clock_set[3]);
        if (j == 1)
        {
            j = 0;
        }
        if (j == 0)
        {
            j = 1;
        }
        for (int i = 0; i < gain_time_delay; i++)
        {
            if (i > gain_time_delay / 2)
            {
                PORTD = 0x80;
                PORTC = LED_code[Clock_set[2] / 10];
            }
            if (i < gain_time_delay / 2)
            {
                if (j == 1)
                {
                    PORTD = 0x40;
                    PORTC = 0xF7;
                }
                if (j == 0)
                {
                    PORTD = 0x40;
                    PORTC = 0xFF;
                }
            }
            TRISA = 0x00;         // set WI is output
            TRISB = TRISB | 0x17; // set the keyboard HI is input
            PORTA = 0xFE;         // 1110
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_4[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_4[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_4[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_4[3];
            }
            PORTA = 0xFD; // 1101
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_3[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_3[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_3[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_3[3];
            }
            PORTA = 0xFB; // 1011
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_2[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_2[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_2[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_2[3];
            }
            PORTA = 0xF7; // 0111
            if (HI1 == 0)
            {
                for (; HI1 == 0;)
                {
                }
                return keyboard_1[0];
            }
            if (HI2 == 0)
            {
                for (; HI2 == 0;)
                {
                }
                return keyboard_1[1];
            }
            if (HI3 == 0)
            {
                for (; HI3 == 0;)
                {
                }
                return keyboard_1[2];
            }
            if (HI4 == 0)
            {
                for (; HI4 == 0;)
                {
                }
                return keyboard_1[3];
            }
        }
    }
}
char gain_clock_unit()
{
    for (;;)
    {
        showhour(Clock_set[0]);
        showmin(Clock_set[1]);
        showsec(Clock_set[2]);
        showmsec(Clock_set[3]);
        // change the way to realize 2.29//
        // four way
        // first The WI1 output is zero
        TRISA = 0x00;         // set WI is output
        TRISB = TRISB | 0x17; // set the keyboard HI is input
        PORTA = 0xFE;         // 1110
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_4[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_4[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_4[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_4[3];
        }
        PORTA = 0xFD; // 1101
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_3[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_3[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_3[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_3[3];
        }
        PORTA = 0xFB; // 1011
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_2[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_2[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_2[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_2[3];
        }
        PORTA = 0xF7; // 0111
        if (HI1 == 0)
        {
            for (; HI1 == 0;)
            {
            }
            return keyboard_1[0];
        }
        if (HI2 == 0)
        {
            for (; HI2 == 0;)
            {
            }
            return keyboard_1[1];
        }
        if (HI3 == 0)
        {
            for (; HI3 == 0;)
            {
            }
            return keyboard_1[2];
        }
        if (HI4 == 0)
        {
            for (; HI4 == 0;)
            {
            }
            return keyboard_1[3];
        }
    }
}