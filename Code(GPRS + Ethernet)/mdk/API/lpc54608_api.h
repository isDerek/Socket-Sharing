#include "LPC54608.h"
/************************************/
/*usart api added by derek 2017.10.25*/
/************************************/
void usart_writebyByte(USART_Type *base);
void usart_readbyByte(USART_Type *base);
int usart_readable(USART_Type *base);
void usart2_readbyByte(USART_Type *base);
/************************************/
/*gpio api added by derek 2017.10.25*/
/************************************/
int LED_W_ON(void);
int	LED_W_OFF(void);
int LED_G_ON(void);
int LED_G_OFF(void);
int LED_B_ON(void);
int LED_B_OFF(void);
int LED_Y_ON(void);
int LED_Y_OFF(void);
int PIO1_30_ON(void);
int PIO1_30_OFF(void);
int SW_ON(void);
int SW_OFF(void);
int PIO0_21_ON(void);
int PIO0_21_OFF(void);
int PIO0_28_ON(void);
int PIO0_28_OFF(void);
int RELAY0_ON(void);
int RELAY0_OFF(void);
int PIO1_22_ON(void);
int PIO1_22_OFF(void);
int PIO0_25_ON(void);
int PIO0_25_OFF(void);
int PIO1_0_ON(void);
int PIO1_0_OFF(void);
int RS485_TX_ON(void);
int RS485_TX_OFF(void);
int RS485_RX_ON(void);
int RS485_RX_OFF(void);
int PWM_ON(void);
int PWM_OFF(void);
int CP_ON(void);
int CP_OFF(void);



/************************************/
/*adc api added by derek 2017.10.25*/
/************************************/
void ADC_Configuration(void);
void ADC_ClockPower_Configuration(void);
float adc_read_result(void);

/************************************/
/*sctimer api added by derek 2017.10.30*/
/************************************/
void sctimer_pwm_init(void);
void enableCPPWM(float cycle);
void disableCPPWM(void);

/************************************/
/*eeprom api added by derek 2017.10.31*/
/************************************/
void write_EEPROM(EEPROM_Type *base, uint32_t pageNum, uint32_t *data);
void read_EEPROM(uint32_t base,uint32_t pageNum,uint8_t size,char *data);
void readFromEEPROM(uint8_t addr,char* data,uint8_t size);
uint8_t writeToEEPROM(uint8_t addr,char* data,uint8_t size);
void eeprom_init(void);


/************************************/
/*flash api added by derek 2017.12.08*/
/************************************/

/** Erase a flash sector
 *
 * The size erased depends on the used device
 *
 * @param address address in the sector which needs to be erased
 * @param return Success if no errors were encountered, otherwise one of the error states
 */
void erase_sector(int address);
void erase_page(int startaddress,int endaddress);
/** Program flash
 *
 * Before programming the used area needs to be erased. The erase state is checked
 * before programming, and will return an error if not erased.
 *
 * @param address starting address where the data needs to be programmed (must be longword alligned: two LSBs must be zero)
 * @param data pointer to array with the data to program
 * @param length number of bytes to program (must be a multiple of 4. must be a multiple of 8 when K64F)
 * @param return Success if no errors were encountered, otherwise one of the error states
 */
int program_flash(int address,char *data,unsigned int length);


/************************************/
/*i2c api added by derek 2017.10.30*/
/************************************/
void i2c_config(void);
int	i2c_write(uint16_t addr);
int i2c_read(uint16_t addr);

/************************************/
/*gprs api added by derek 2018.07.06*/
/************************************/
void PWROpenToggle(void);
void PWRCloseToggle(void);
void gprsReset(void);
