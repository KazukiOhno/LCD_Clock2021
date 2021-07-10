/**
  CCP5 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    ccp5.h

  @Summary
    This is the generated header file for the CCP5 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for CCP5.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F26K22
        Driver Version    :  2.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef CCP5_H
#define CCP5_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/** 
  Section: Data type definitions
*/
        
/**
 @Summary
   Defines the values to convert from 16bit to two 8 bit and vice versa

 @Description
   This routine used to get two 8 bit values from 16bit also
   two 8 bit value are combine to get 16bit.

 Remarks:
   None
 */

typedef union CCPR5Reg_tag
{
   struct
   {
      uint8_t ccpr5l;
      uint8_t ccpr5h;
   };
   struct
   {
      uint16_t ccpr5_16Bit;
   };
} CCP5_PERIOD_REG_T ;

/**
  Section: COMPARE Module APIs
*/

/**
  @Summary
    Initializes the CCP5

  @Description
    This routine initializes the CCP5_Initialize.
    This routine must be called before any other CCP5 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    uint16_t compare;

    CCP5_Initialize();
    CCP5_SetCompareCount(compare);
    </code>
 */
void CCP5_Initialize(void);

/**
  @Summary
    Loads 16-bit compare value.

  @Description
    This routine loads the 16 bit compare value.

  @Preconditions
    CCP5_Initialize() function should have been
    called before calling this function.

  @Param
    compareCount: 16-bit unsigned value

  @Returns
    None

  @Example
    <code>
    uint16_t compare;

    CCP5_Initialize();
    CCP5_SetCompareCount(compare);
    </code>
*/
void CCP5_SetCompareCount(uint16_t compareCount);

/**
  @Summary
    Determines the completion of the data compare.

  @Description
    This routine is used to determine if data comparison is completed.
    When data comparison is complete routine returns 1. It returns 0 otherwise.

  @Preconditions
    CCP5_Initialize()function should have been called before calling this function.

  @Returns
   true - Indicates compare is complete
   false - Indicates compare is not complete

  @Param
    None

  @Example
    <code>
    uint16_t capture;

    CCP5_Initialize();
    CCP5_SetCompareCount(compare);
    while(!CCP5_IsCompareComplete());
    </code>
*/
bool CCP5_IsCompareComplete(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  //CCP5_H
/**
 End of File
*/
