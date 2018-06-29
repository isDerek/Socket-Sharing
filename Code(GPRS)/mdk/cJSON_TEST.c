#include "cJSON.h"
#include "stdio.h"
#include "stddef.h"
#include "string.h"
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include "opt.h"



#include "ping.h"
#include "api.h"
#include "board.h"
#include "pin_mux.h"
#include <stdbool.h>
void cJSON11(void *arg)
{
while(1)
{
		char *s = "{\"list\":{\"name\":\"xiao hong\",\"age\":10},\"other\":{\"name\":\"hua hua\"}}";
  cJSON *root;
	root = cJSON_Parse(s);
  if(!root) {
      printf("get root faild !\n");
     
  }
  
  cJSON *js_list = cJSON_GetObjectItem(root, "list");
  if(!js_list) {
     printf("no list!\n");
    
 }
 printf("list type is %d\n",js_list->type);
 
 cJSON *name = cJSON_GetObjectItem(js_list, "name");
 if(!name) {
     printf("No name !\n");
     
 }
 printf("name type is %d\n",name->type);
 printf("name is %s\n",name->valuestring);
 
 cJSON *age = cJSON_GetObjectItem(js_list, "age");
 if(!age) {
     printf("no age!\n");
    
 }
 printf("age type is %d\n", age->type);
 printf("age is %d\n",age->valueint);
 
 cJSON *js_other = cJSON_GetObjectItem(root, "other");
 if(!js_other) {
     printf("no list!\n");
     
 }
 printf("list type is %d\n",js_other->type);
 
 cJSON *js_name = cJSON_GetObjectItem(js_other, "name");
 if(!js_name) {
     printf("No name !\n");
     
 }
 printf("name type is %d\n",js_name->type);
 printf("name is %s\n",js_name->valuestring);
 
 if(root)
     cJSON_Delete(root);
    
 }
}
int main()
{
	 CLOCK_EnableClock(kCLOCK_InputMux);
    
    /* attach 12 MHz clock to FLEXCOMM0 (debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    BOARD_InitPins();
    BOARD_BootClockFROHF48M();
    BOARD_InitDebugConsole();
   if(sys_thread_new("cJSON", cJSON11, NULL, 1024, 5) == NULL)
        LWIP_ASSERT("cJSON(): Task creation failed.", 0);	
    vTaskStartScheduler();

}
