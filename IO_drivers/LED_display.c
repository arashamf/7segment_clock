//Includes ------------------------------------------------------------------//
#include "LED_display.h"
#include "gpio.h"
#include "tim.h"
#include "typedef.h"

//Defines------------------------------------------------------------------------------------//
#define DIGIT_CODE_SIZE 0x0B

//Variables------------------------------------------------------------------//
const TPortPin LEDpins[ DIGIT_CODE_SIZE] = 
{
    { SA_GPIO_Port, SA_Pin},
    { SB_GPIO_Port, SB_Pin},
	{ SC_GPIO_Port, SC_Pin },
    { SD_GPIO_Port, SD_Pin},
	{ SE_GPIO_Port, SE_Pin},
	{ SF_GPIO_Port, SF_Pin },
    { SG_GPIO_Port, SG_Pin},
    { SDP_GPIO_Port, SDP_Pin }
};

 //коды чисел от 0 до 9, последний элемент (0x00) - код гашения всех сегментов
const uint8_t digit_code[] = 
{
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00
};

//segment_numb - количество сегментов каждого разряда табло
uint8_t segment_numb = sizeof (LEDpins)/ sizeof (LEDpins[0]);

//Prototypes-----------------------------------------------------------------//
static void reset_number (uint8_t );
static void reset_all_number (void);
static void select_number (uint8_t );
static void reset_all_digit (void);
static void digit_activating (uint8_t digit);

//---------------------------------------------------------------------------//
static void reset_number (uint8_t number)
{
 switch (number)
    {
        case 0:
            NUMBER_0 (OFF);
            break;

        case 1:
            NUMBER_1 (OFF);
            break;

        case 2:
            NUMBER_2 (OFF);
            break;
      
        case 3:
            NUMBER_3 (OFF);
            break;

        case 4:
            NUMBER_4 (OFF);
            break;
      
        case 5:
            NUMBER_5 (OFF);
            break;
      
        default:
            break;     
    }
}

//---------------------------------------------------------------------------//
static void reset_all_number (void)
{
    NUMBER_0 (OFF);
    NUMBER_1 (OFF);
    NUMBER_2 (OFF);
    NUMBER_3 (OFF);
    NUMBER_4 (OFF);
    NUMBER_5 (OFF);
}

//---------------------------------------------------------------------------//
static void select_number (uint8_t numb)
{
    reset_all_number();
    switch (numb)
    {
        case 0:
            NUMBER_0 (ON);
            break;

        case 1:
            NUMBER_1 (ON);
            break;

        case 2:
            NUMBER_2 (ON);
            break;
      
        case 3:
            NUMBER_3 (ON);
            break;

        case 4:
            NUMBER_4 (ON);
            break;
      
        case 5:
            NUMBER_5 (ON);
            break;
      
        default:
            break;     
    }
}

//---------------------------------------------------------------------------//
static void reset_all_digit (void)
{
    digit_activating (DIGIT_CODE_SIZE-1);
}

//---------------------------------------------------------------------------//
static void digit_activating (uint8_t digit)
{
    uint8_t num = 0;
    
    if ((digit >= 0 ) && (digit <= 10))
    {
        num = digit_code[digit];
        for (uint8_t count = 0; count < segment_numb; count++)
        {

            if ((num&0x01) == ON) {
                LL_GPIO_SetOutputPin (LEDpins[count].PORTx, LEDpins[count].PORT_Pin);
            }
            else {
                LL_GPIO_ResetOutputPin (LEDpins[count].PORTx, LEDpins[count].PORT_Pin);
            }
            num = num >> 1;
        }
    }
}

//---------------------------------------------------------------------------//
void LED_data_output (char * time, uint8_t LEDnumb)
{
    uint8_t digital;
    for (uint8_t j = 0; j < LEDnumb; j++)
    {
        select_number (j);
        if (*(time+j) >= 0x30)
        {  digital = (*(time+j) - 0x30); }
        else
        {  digital = 0;  }
        digit_activating (digital);
        delay_LED_digital (j);
        reset_all_digit ();
        //delay_us(500);
    }
}