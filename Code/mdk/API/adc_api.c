#include "lpc54608_api.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_adc.h"
#include "fsl_power.h"

#define DEMO_ADC_BASE ADC0
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER 0U  // added by derek 2017.10.20
#define DEMO_ADC_IRQ_ID ADC0_SEQA_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC ADC0_SEQA_IRQHandler
volatile bool gAdcConvSeqAIntFlag;
static adc_result_info_t gAdcResultInfoStruct;
adc_result_info_t *volatile gAdcResultInfoPtr = &gAdcResultInfoStruct;
	/* Enable the power and clock for ADC. */
	void ADC_ClockPower_Configuration(void)
{
    /* SYSCON power. */
    POWER_DisablePD(kPDRUNCFG_PD_VDDA);    /* Power on VDDA. */
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);    /* Power on the ADC converter. */
    POWER_DisablePD(kPDRUNCFG_PD_VD2_ANA); /* Power on the analog power supply. */
    POWER_DisablePD(kPDRUNCFG_PD_VREFP);   /* Power on the reference voltage source. */
    POWER_DisablePD(kPDRUNCFG_PD_TS);      /* Power on the temperature sensor. */




}
/*
 * Configure the ADC as normal converter in polling mode.
 */
void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;

    /* Configure the converter. */
    adcConfigStruct.clockMode = kADC_ClockSynchronousMode; /* Using sync clock source. */
    adcConfigStruct.clockDividerNumber = 1;                /* The divider for sync clock is 2. */
    adcConfigStruct.resolution = kADC_Resolution12bit;
    adcConfigStruct.enableBypassCalibration = false;
    adcConfigStruct.sampleTimeNumber = 0U;
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

    /* Use the sensor input to channel 0. */
    ADC_EnableTemperatureSensor(DEMO_ADC_BASE, true);

    /* Enable channel 0's conversion in Sequence A. */
    adcConvSeqConfigStruct.channelMask = (1U << 0); /* Includes channel 4. */   /*added by derek 2017.10.19*/
    adcConvSeqConfigStruct.triggerMask = 0U;
    adcConvSeqConfigStruct.triggerPolarity = kADC_TriggerPolarityNegativeEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(DEMO_ADC_BASE, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(DEMO_ADC_BASE, true); /* Enable the conversion sequence A. */
}
/*
 * ISR for ADC conversion sequence A done.
 */
void DEMO_ADC_IRQ_HANDLER_FUNC(void)
{
    if (kADC_ConvSeqAInterruptFlag == (kADC_ConvSeqAInterruptFlag & ADC_GetStatusFlags(DEMO_ADC_BASE)))
    {
        ADC_GetChannelConversionResult(DEMO_ADC_BASE, DEMO_ADC_SAMPLE_CHANNEL_NUMBER, gAdcResultInfoPtr);
        ADC_ClearStatusFlags(DEMO_ADC_BASE, kADC_ConvSeqAInterruptFlag);
        gAdcConvSeqAIntFlag = true;
    }
}
float adc_read_result(void)
{
float result;
//	      GETCHAR();// occupyed by usart init
        gAdcConvSeqAIntFlag = false;
        ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);
	        while (!gAdcConvSeqAIntFlag)
        {
        }
				result = gAdcResultInfoStruct.result;
//					PRINTF("gAdcResultInfoStruct.result        = %d\r\n", gAdcResultInfoStruct.result);
//					PRINTF("gAdcResultInfoStruct.channelNumber = %d\r\n", gAdcResultInfoStruct.channelNumber);
//					PRINTF("gAdcResultInfoStruct.overrunFlag   = %d\r\n", gAdcResultInfoStruct.overrunFlag ? 1U : 0U);
//					PRINTF("\r\n");
				
				return result/4096;

		
				
}
