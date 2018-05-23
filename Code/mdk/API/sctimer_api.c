/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "lpc54608_api.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_sctimer.h"

#include "pin_mux.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define CTIMER CTIMER1                 /* Timer 1 */
#define CTIMER_MAT_OUT kCTIMER_Match_1 /* Match output 1 */
#define BUS_CLK_FREQ CLOCK_GetFreq(kCLOCK_BusClk)
uint32_t event;

/*******************************************************************************
 * Code
 ******************************************************************************/
 void sctimer_pwm_init(void)
 {
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t sctimerClock;
	 	sctimerClock = BUS_CLK_FREQ;
	 float discycle = 99;
	  /* Print a note to terminal */
//    PRINTF("\r\nSCTimer example to output 2 center-aligned PWM signals\r\n");
//    PRINTF("\r\nYou will see a change in LED brightness if an LED is connected to the SCTimer output pins");
//    PRINTF("\r\nIf no LED is connected to the pin, then probe the signal using an oscilloscope");
    SCTIMER_GetDefaultConfig(&sctimerInfo);
    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

	  
	 
	  /* Configure first PWM with frequency 24kHZ from output 4 */
    pwmParam.output = kSCTIMER_Out_0;
    pwmParam.level = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = discycle;
    if (SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_CenterAlignedPwm, 1000U, sctimerClock, &event) == kStatus_Fail)
    {  
      
    }
    /* Start the timer */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_L); 

 }
 
 void enableCPPWM(float cycle)
 {
	 /* Disable interrupt to retain current dutycycle for a few seconds */
		SCTIMER_DisableInterrupts(SCT0, (1 << event));
	  /* Update PWM duty cycle */
    SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_4, cycle, event);
	  /* Enable interrupt flag to update PWM dutycycle */
    SCTIMER_EnableInterrupts(SCT0, (1 << event));

 }

void disableCPPWM(void)
{
		float discycle = 99;
	 /* Disable interrupt to retain current dutycycle for a few seconds */
		SCTIMER_DisableInterrupts(SCT0, (1 << event));
	  /* Update PWM duty cycle */
    SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_4, discycle, event);
	  /* Enable interrupt flag to update PWM dutycycle */
    SCTIMER_EnableInterrupts(SCT0, (1 << event));

}
