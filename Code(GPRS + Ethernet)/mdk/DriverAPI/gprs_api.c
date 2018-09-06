#include<lpc54608_api.h>
#include<fsl_gpio.h>
#include<UserConfig.h>
#define I2C1_CLK_PORT 0
#define I2C1_CLK_PIN 14

void PWROpenToggle(void){
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
	while(gprsPWRTimer!=3){
	}
	gprsOpenFlag = false;
	gprsPWRTimer = 0;
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
}

void PWRCloseToggle(void){
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
	gprsCloseFlag = true;
	while(gprsPWRTimer!=3){
	}
	gprsCloseFlag = false;
	gprsPWRTimer = 0;
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
}
void gprsReset(void){
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
	gprsCloseFlag = true;
	while(gprsPWRTimer!=3){
	}
	gprsCloseFlag = false;
	gprsPWRTimer = 0;
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
	gprsOpenFlag = true;
	while(gprsPWRTimer!=1){
	}
	gprsOpenFlag = false;
	gprsPWRTimer = 0;
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);
	gprsOpenFlag = true;
	while(gprsPWRTimer!=3){
	}		
	gprsOpenFlag = false;
	gprsPWRTimer = 0;
	GPIO_TogglePinsOutput(GPIO,I2C1_CLK_PORT,1<<I2C1_CLK_PIN);		
}
