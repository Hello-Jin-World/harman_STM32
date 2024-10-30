#include "main.h"

#define ADDR_SECONDS        0x80 // write
#define ADDR_MINUTES        0x82 // write // read = write + 1
#define ADDR_HOURS          0x84
#define ADDR_DATE           0x86
#define ADDR_MONTH          0x88
#define ADDR_DAYOFWEEK      0x8A
#define ADDR_YEAR           0x8C
#define ADDR_WRITEPROTECTED 0x8E

void ds1302_init_time_date(void);
void ds1302_write(uint8_t addr, uint8_t data);
void ds1302_set_time(void);
void ds1302_main(void);
void ds1302_DataLine_Output(void);
void ds1302_DataLine_Input(void);
void ds1302_clock(void);
void ds1302_init_gpio_low(void);
uint8_t ds1302_read(uint8_t addr);
void ds1302_tx(uint8_t tx);
void ds1302_rx(char *data8);
void ds1302_read_time(void);
void ds1302_read_date(void);
void ds1302_flash_clock(void);
