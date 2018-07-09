#include<lpc54608_api.h>
#include<fsl_gpio.h>
#include<UserConfig.h>
#define LED_W_PORT 0
#define LED_W_PIN  2
#define LED_B_PORT 0
#define LED_B_PIN  29
#define LED_G_PORT 1
#define LED_G_PIN  29
#define LED_Y_PORT 0
#define LED_Y_PIN  22
#define PIO1_30_PORT 1
#define PIO1_30_PIN 30
#define SW_PORT 0
#define SW_PIN 3
//#define RST_PORT  
//#define RST_PIN
#define PIO0_21_PORT 0
#define PIO0_21_PIN 21


#define PIO0_28_PORT 0
#define PIO0_28_PIN 28
#define RELAY_PORT 0
#define RELAY_PIN 24
#define PIO1_22_PORT 1
#define PIO1_22_PIN 22
#define PIO0_25_PORT 0
#define PIO0_25_PIN 25
#define PIO1_0_PORT 1
#define PIO1_0_PIN 0   

#define RS485_TX_PORT 0
#define RS485_TX_PIN 9
#define RS485_RX_PORT 0
#define RS485_RX_PIN 8
#define CP_PORT 0
#define CP_PIN 10
#define PWM_PORT 0
#define PWM_PIN 17
#define I2C1_CLK_PORT 0
#define I2C1_CLK_PIN 14
int LED_W_ON(void)
{
	GPIO_WritePinOutput(GPIO, LED_W_PORT, LED_W_PIN, 1);
	respond = 1;
	return 1;
}

int LED_G_ON(void)
{
	GPIO_WritePinOutput(GPIO, LED_G_PORT, LED_G_PIN, 1);
	respond = 1;
	return 1;
}

int LED_B_ON(void)
{
	GPIO_WritePinOutput(GPIO, LED_B_PORT, LED_B_PIN, 1);
	respond = 1;
	return 1;
}

int LED_Y_ON(void)
{
	GPIO_WritePinOutput(GPIO, LED_Y_PORT, LED_Y_PIN, 1);
	respond = 1;
	return 1;
}

int	LED_W_OFF(void)
{
	GPIO_WritePinOutput(GPIO, LED_W_PORT, LED_W_PIN, 0);
	respond = 1;
	return 0;
}

int LED_B_OFF(void)
{
	GPIO_WritePinOutput(GPIO, LED_B_PORT, LED_B_PIN, 0);
	respond = 1;
	return 0;
}

int LED_G_OFF(void)
{
	GPIO_WritePinOutput(GPIO, LED_G_PORT, LED_G_PIN, 0);
	respond = 1;
	return 0;
}

int LED_Y_OFF(void)
{
	GPIO_WritePinOutput(GPIO, LED_Y_PORT, LED_Y_PIN, 0);
	respond = 1;
	return 0;
}
int PIO1_30_ON(void)
{
	GPIO_WritePinOutput(GPIO,PIO1_30_PORT, PIO1_30_PIN, 1);
	respond = 1;
	return 1;
}
int PIO1_30_OFF(void)
{
	GPIO_WritePinOutput(GPIO, PIO1_30_PORT, PIO1_30_PIN, 0);
	respond = 1;
	return 0;
}
int SW_ON(void)
{
	GPIO_WritePinOutput(GPIO,SW_PORT, SW_PIN, 1);
	respond = 1;
	return 1;
}
int SW_OFF(void)
{
	GPIO_WritePinOutput(GPIO,SW_PORT, SW_PIN, 0);
	respond = 1;
	return 0;
}
int PIO0_21_ON(void)
{
	GPIO_WritePinOutput(GPIO,PIO0_21_PORT, PIO0_21_PIN, 1);
	respond = 1;
	return 1;
}
int PIO0_21_OFF(void)
{
	GPIO_WritePinOutput(GPIO,PIO0_21_PORT, PIO0_21_PIN, 0);
	respond = 1;
	return 0;
}
int PIO0_28_ON(void)
{
	GPIO_WritePinOutput(GPIO,PIO0_28_PORT, PIO0_28_PIN, 1);
	respond = 1;
	return 1;
}
int PIO0_28_OFF(void)
{
	GPIO_WritePinOutput(GPIO,PIO0_28_PORT, PIO0_28_PIN, 0);
	respond = 1;
	return 0;
}
int RELAY0_ON(void)
{
	GPIO_WritePinOutput(GPIO,RELAY_PORT, RELAY_PIN, 1);
	respond = 1;
	return 1;
}
int RELAY0_OFF(void)
{
	GPIO_WritePinOutput(GPIO,RELAY_PORT, RELAY_PIN, 0);
	respond = 1;
	return 0;
}
int PIO1_22_ON(void)
{
	GPIO_WritePinOutput(GPIO,PIO1_22_PORT, PIO1_22_PIN, 1);
	respond = 1;
	return 1;
}
int PIO1_22_OFF(void)
{
	GPIO_WritePinOutput(GPIO,PIO1_22_PORT, PIO1_22_PIN, 0);
	respond = 1;
	return 0;
}
int PIO0_25_ON(void)
{
	GPIO_WritePinOutput(GPIO,PIO0_25_PORT, PIO0_25_PIN, 1);
	respond = 1;
	return 1;
}
int PIO0_25_OFF(void)
{
	GPIO_WritePinOutput(GPIO,PIO0_25_PORT, PIO0_25_PIN, 0);
	respond = 1;
	return 0;
}
int PIO1_0_ON(void)
{
	GPIO_WritePinOutput(GPIO,PIO1_0_PORT, PIO1_0_PIN, 1);
	respond = 1;
	return 1;
}
int PIO1_0_OFF(void)
{
	GPIO_WritePinOutput(GPIO,PIO1_0_PORT, PIO1_0_PIN, 0);
	respond = 1;
	return 0;
}
int RS485_TX_ON(void)
{
	GPIO_WritePinOutput(GPIO,RS485_TX_PORT, RS485_TX_PIN, 1);
	respond = 1;
	return 1;
}
int RS485_TX_OFF(void)
{
	GPIO_WritePinOutput(GPIO,RS485_TX_PORT, RS485_TX_PIN, 0);
	respond = 1;
	return 0;
}
int RS485_RX_ON(void)
{
	GPIO_WritePinOutput(GPIO,RS485_RX_PORT, RS485_RX_PIN, 1);
	respond = 1;
	return 1;
}
int RS485_RX_OFF(void)
{
	GPIO_WritePinOutput(GPIO,RS485_RX_PORT, RS485_RX_PIN, 0);
	respond = 1;
	return 0;
}
int PWM_ON(void)
{
	GPIO_WritePinOutput(GPIO,PWM_PORT, PWM_PIN, 1);
	respond = 1;
	return 1;
}
int PWM_OFF(void)
{
	GPIO_WritePinOutput(GPIO,PWM_PORT, PWM_PIN, 0);
	respond = 1;
	return 0;
}
int CP_ON(void)
{
	GPIO_WritePinOutput(GPIO,CP_PORT, CP_PIN, 1);
	respond = 1;
	return 1;
}
int CP_OFF(void)
{
	GPIO_WritePinOutput(GPIO,CP_PORT, CP_PIN, 0);
	respond = 1;
	return 0;
}
