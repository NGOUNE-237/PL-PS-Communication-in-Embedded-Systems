/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xgpio.h"
#include "xparameters.h"
#include "xil_printf.h"


int main()
{
    init_platform();


    XGpio input, output;
    unsigned int a,b, lower32, upper32; // input to NOT gate
    u64 y; // output
    XGpio_Initialize(&output, XPAR_AXI_GPIO_0_DEVICE_ID); //Initialize the input xgpio and ensure it matches the gpio_0 in the ip
    XGpio_Initialize(&input, XPAR_AXI_GPIO_1_DEVICE_ID);
    //u32 a=3,b=4;


    //Input is 1, output is 0
    //configure channel 1 of the input gpio instance as an input pin
    XGpio_SetDataDirection(&input, 1, 1);
    XGpio_SetDataDirection(&input, 2, 1);


    //configure channel 1 of the output gpio instance as an output pin
    XGpio_SetDataDirection(&output, 1, 0);
    XGpio_SetDataDirection(&output, 2, 0);

    while(1)
    {

        xil_printf("Enter first 32-bit unsigned integer: \n");
         scanf("%u", &a);

         xil_printf("Enter second 32-bit unsigned integer: \n");
         scanf("%u", &b);

         xil_printf("You entered: %u and %u\n", a, b);


	XGpio_DiscreteWrite(&output, 1, a);
	XGpio_DiscreteWrite(&output, 2, b);

	//for (volatile int i = 0; i < 1000000; i++); // crude delay loop


	lower32 = XGpio_DiscreteRead(&input, 1);
    upper32 = XGpio_DiscreteRead(&input, 2);


    y = ((u64)upper32 << 32) | lower32;

    xil_printf("The output of %u and %u is %llu\n", a, b, y);
    xil_printf("Lower is %u and Upper is %u\n", lower32, upper32);

    xil_printf("In Hex Upper32: 0x%08X, Lower32: 0x%08X\n", upper32, lower32);

    xil_printf("Combined 64-bit value in hex: 0x%08X%08X\n",
               (u32)(y >> 32), (u32)(y & 0xFFFFFFFF));


    }






    cleanup_platform();
    return 0;
}
