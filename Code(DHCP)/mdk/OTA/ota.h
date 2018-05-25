#ifndef OTA_H
#define OTA_H

void OTAInit(void);
void updateCode(void);
int calculateBinSize(char* binStartAddress, int checkTotalSize);
#endif
