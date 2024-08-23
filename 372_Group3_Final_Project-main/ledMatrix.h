#ifndef LEDMATRIX_H
#define LEDMATRIX_H

void SPI_MASTER_Init();
void write_execute(unsigned char CMD, unsigned char data);
void initSPI();
void displayStopSign();
void turnOffDisplay();


#endif
