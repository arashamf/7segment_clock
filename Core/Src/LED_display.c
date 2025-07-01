//Includes ------------------------------------------------------------------//
#include "LED_display.h"
#include "gpio.h"

//---------------------------------------------------------------------------//
void reset_number (uint8_t number)
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
void reset_all_number (void)
{
    NUMBER_0 (OFF);
    NUMBER_1 (OFF);
    NUMBER_2 (OFF);
    NUMBER_3 (OFF);
    NUMBER_4 (OFF);
    NUMBER_5 (OFF);
}

//---------------------------------------------------------------------------//
void select_number (uint8_t numb)
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
void reset_all_digit (void)
{
    Segment_A(OFF);
    Segment_B(OFF);
    Segment_C(OFF);
    Segment_D(OFF);
    Segment_E(OFF);
    Segment_F(OFF);
    Segment_G(OFF);
    Segment_DP(OFF);
}

//---------------------------------------------------------------------------//
void digit_0 (void)
{
  Segment_A(ON);
  Segment_B(ON);
  Segment_C(ON);
  Segment_D(ON);
  Segment_E(ON);
  Segment_F(ON);
}

//---------------------------------------------------------------------------//
void digit_1 (void)
{
  Segment_B(ON);
  Segment_C(ON);
}

//---------------------------------------------------------------------------//
void digit_2 (void)
{
  Segment_A(ON);
  Segment_B(ON);
  Segment_G(ON);
  Segment_E(ON);
  Segment_D(ON);
}

//---------------------------------------------------------------------------//
void digit_3 (void)
{
  Segment_A(ON);
  Segment_B(ON);
  Segment_G(ON);
  Segment_C(ON);
  Segment_D(ON);
}

//---------------------------------------------------------------------------//
void digit_4 (void)
{
  Segment_B(ON);
  Segment_C(ON);
  Segment_F(ON);
  Segment_G(ON);
}

//---------------------------------------------------------------------------//
void digit_5 (void)
{
  Segment_A(ON);
  Segment_F(ON);
  Segment_G(ON);
  Segment_C(ON);
  Segment_D(ON);
}

//---------------------------------------------------------------------------//
void digit_6(void)
{
  Segment_A(ON);
  Segment_C(ON);
  Segment_D(ON);
  Segment_E(ON);
  Segment_F(ON);
  Segment_G(ON);
}

//---------------------------------------------------------------------------//
void digit_7 (void)
{
  Segment_A(ON);
  Segment_B(ON);
  Segment_C(ON);
}

//---------------------------------------------------------------------------//
void digit_8 (void)
{
  Segment_A(ON);
  Segment_B(ON);
  Segment_C(ON);
  Segment_D(ON);
  Segment_E(ON);
  Segment_F(ON);
  Segment_G(ON);
}

//---------------------------------------------------------------------------//
void digit_9 (void)
{
  Segment_A(ON);
  Segment_B(ON);
  Segment_C(ON);
  Segment_D(ON);
  Segment_F(ON);
  Segment_G(ON);
}

//---------------------------------------------------------------------------//
void select_digit (uint8_t digit)
{
    reset_all_digit();
    switch (digit)
    {
        case 0:
            digit_0 ();
            break;

        case 1:
            digit_1 ();
            break;

        case 2:
            digit_2 ();
            break;
      
        case 3:
            digit_3 ();
            break;  

        case 4:
            digit_4 ();
            break;
      
        case 5:
            digit_5 ();
            break; 

        case 6:
            digit_6 ();
            break;

        case 7:
            digit_7 ();
            break;
      
        case 8:
            digit_8 ();
            break;  

        case 9:
            digit_9 ();
            break;
      
        default:
            break;     
    }
}

//---------------------------------------------------------------------------//
