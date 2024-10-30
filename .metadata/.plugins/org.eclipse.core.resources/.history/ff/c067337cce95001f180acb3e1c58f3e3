#include "main.h"
#include "button.h"
#include "extern.h"

#include <stdio.h>
#include <string.h>

void flash_main(void);
void flash_set_time(void);
void alarm_clock(void);
void flash_export_time_date(void);
void flash_import_time_date(void);
void set_pc_time_date(char *time);

/**************************************************
     Flash module organization(STM32F411)
                               512KBytes

Name        Block base address              size
====      ==================   =======
Sector 0    0x8000000-0x8003FFF           16K bytes
Sector 1    0x8004000-0x8007FFF           16K bytes
Sector 2    0x8008000-0x800BFFF           16K bytes
Sector 3    0x800C000-0x800FFFF           16K bytes
Sector 4    0x8010000-0x801FFFF           64K bytes
Sector 5    0x8020000-0x803FFFF          128K bytes
Sector 6    0x8040000-0x805FFFF          128K bytes
Sector 7    0x8060000-0x807FFFF          128K bytes  <----- We are using sector 7.

 ******************************************************/

// 0x8060000-0x807FFFF 의 빈공간에 사용자 데이터를 flash programming
// 하도록 설정 한다.
#define ADDR_FLASH_SECTOR7      ((uint32_t) 0x8060000)
#define FLASH_USER_START_ADDR   ((uint32_t) 0x8060000)
#define USER_DATA_ADDRESS        0x8060000
#define FLASH_USER_END_ADDR     ((uint32_t) 0x807FFFF)
#define FLASH_INIT_STATUS       0xFFFFFFFF        // flash 초기 상태
#define FLASH_NOT_INIT_STATUS   0xAAAAAAAA        // flash 초기 상태가 아니다
#define DATA_32                 ((uint32_t) 0x00000001)

extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
extern volatile int TIM10_DHT11_counter;
extern volatile int TIM10_1ms_counter;
extern void rrr(void);
extern unsigned char bcd2dec(unsigned char byte);
extern void ds1302_init_time_date(void);
extern RTC_TimeTypeDef sTime; // Time Information
extern RTC_DateTypeDef sDate; // Date Information

int hours = 0;
int minutes = 0;
int seconds = 0;
int years = 0;
int months = 0;
int dates = 0;

HAL_StatusTypeDef flash_write(uint32_t *data32, int size);
HAL_StatusTypeDef flash_read(uint32_t *data32, int size);
HAL_StatusTypeDef flash_erase();

uint32_t flash_read_value=0;

typedef struct student
{
	uint32_t magic;  // check for factory reset state or non-reset state  // 4byte -> 1 word
	int num;        // hakbun // 4byte -> 1 word
	char name[20];  // name // 20byte -> 5 word
	double grade;   // hakjum // 8byte -> 2 word
} t_student;


t_student student; // typedef type
t_set_time set_time;
t_ds1302 ds_time; // memory access

void flash_main(void)
{

	t_student *read_student;

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS; //  data read from 0x8060000

	if (flash_read_value == FLASH_INIT_STATUS)  // 초기에 아무런 데이터도 존재 하지 않을 경우
	{
		flash_erase();
		printf("Flash Memory Empty!\n");
		student.magic=0x55555555;  // --> 0101010101 remained
		student.num=1101815;
		strncpy((char *)&student.name,"Hong_Gil_Dong", strlen("Hong_Gil_Dong"));  // insult "Hong_Gil_Dong" // char type address
		student.grade=4.0;
		printf("w grade: %lf\n", student.grade);
		flash_write((uint32_t *) &student, sizeof(t_student));
	}
	else   // 1번 이상 flash memory에 데이터를 write 한 경우 // when not reset state.
	{
		flash_read((uint32_t *) &student, sizeof(t_student));

		printf("magic: %08x\n", student.magic);
		printf("num: %08x\n", 	student.num);
		printf("name: %s\n", student.name);
		printf("r grade: %lf\n", student.grade);
	}
}

void set_pc_time_date(char *time)
{
	char pc_hour[2];
	char pc_minute[2];
	char pc_second[2];
	char pc_date[2];
	char pc_month[2];
	char pc_year[2];

	strncpy(pc_year, time, 2);
	strncpy(pc_month, time+2, 2);
	strncpy(pc_date, time+4, 2);
	strncpy(pc_hour, time+6, 2);
	strncpy(pc_minute, time+8, 2);
	strncpy(pc_second, time+10, 2);

	hours = atoi(pc_hour);
	minutes = atoi(pc_minute);
	seconds = atoi(pc_second);
	years = atoi(pc_year);
	months = atoi(pc_month);
	dates = atoi(pc_date);

	flash_erase();

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS; //  data read from 0x8060000
	set_time.magic = 0x55555555;  // --> 0101010101 remained (user define)
	set_time.Hours = hours;
	set_time.Minutes = minutes;
	set_time.Seconds = seconds;
	set_time.Year = years;
	set_time.Month = months;
	set_time.Date = dates;
	flash_write((uint32_t *) &set_time, sizeof(set_time));

	ds_time.hour = hours;
	ds_time.minutes = minutes;
	ds_time.seconds = seconds;
	ds_time.year = years;
	ds_time.month = months;
	ds_time.date = dates;

	ds1302_init_time_date();

}

void alarm_clock(void)
{
	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS; //  data read from 0x8060000

	static int buzzer_trigger = 0;
#if 1
	static RTC_TimeTypeDef oldTime = {0};

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	if (oldTime.Seconds != sTime.Seconds)	// updated time information release. (one release per second)
	{
		printf("%2d:%2d:%2d\n", bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));
		oldTime.Seconds = sTime.Seconds; // update Second
	}

	if (bcd2dec(sTime.Hours) == hours && bcd2dec(sTime.Minutes) == minutes && bcd2dec(sTime.Seconds)  == seconds)
	{
		TIM10_DHT11_counter = 0;
		buzzer_trigger = 1;
	}

	if (buzzer_trigger)
	{
		if (TIM10_DHT11_counter >= 30000 || get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
		{
			TIM10_DHT11_counter = 0;
			buzzer_trigger = 0;
			flash_erase();
		}
		rrr();
	}
#else
	static int current_hh = 14;
	static int current_mm = 16;
	static int current_ss = 50;

	if (TIM10_1ms_counter >= 1000)
	{
		TIM10_1ms_counter = 0;
		current_ss++;
		if (current_ss == 60)
		{
			current_mm++;
			current_ss = 0;
			if (current_mm == 60)
			{
				current_hh++;
				current_mm = 0;
			}
		}
		printf("%2d : %2d : %2d\n", current_hh, current_mm, current_ss);
	}

	if (current_hh == hours && current_mm == minutes && current_ss == seconds)
	{
		TIM10_DHT11_counter = 0;
		buzzer_trigger = 1;
	}

	if (buzzer_trigger)
	{
		if (TIM10_DHT11_counter >= 30000 || get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
		{
			TIM10_DHT11_counter = 0;
			buzzer_trigger = 0;
			hours = 0;
			minutes = 0;
			seconds = 0;
			flash_erase();
		}
		rrr();
	}
#endif
}

void set_alarm_time(char *time)
{
	char hour[2];
	char minute[2];
	char second[2];

	strncpy(hour, time, 2);
	strncpy(minute, time+2, 2);
	strncpy(second, time+4, 2);

	hours = atoi(hour);
	minutes = atoi(minute);
	seconds = atoi(second);

	flash_erase();


}

void flash_set_time(void)
{

	t_set_time *read_set_time;

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS; //  data read from 0x8060000

	if (flash_read_value == FLASH_INIT_STATUS)  // 초기에 아무런 데이터도 존재 하지 않을 경우
	{
		flash_erase();
		printf("Flash Memory Empty!\n");
		set_time.magic = 0x55555555;  // --> 0101010101 remained (user define)
		set_time.Hours = hours;
		set_time.Minutes = minutes;
		set_time.Seconds = seconds;
		flash_write((uint32_t *) &set_time, sizeof(set_time));
	}
	else   // 1번 이상 flash memory에 데이터를 write 한 경우 // when not reset state.
	{
		flash_read((uint32_t *) &set_time, sizeof(set_time));

		//printf("magic: %08x\n", set_time.magic);
		printf("Hours: %02d\n", set_time.Hours);
		printf("Minutes: %02d\n", set_time.Minutes);
		printf("Seconds: %02d\n", set_time.Seconds);
	}
}

void flash_export_time_date(void)
{
	hours = ds_time.hour;
	minutes = ds_time.minutes;
	seconds = ds_time.seconds;
	years = ds_time.year;
	months = ds_time.month;
	dates = ds_time.date;

	t_set_time *read_set_time;

	flash_read_value = *(__IO uint32_t *) USER_DATA_ADDRESS; //  data read from 0x8060000
	set_time.magic = 0x55555555;  // --> 0101010101 remained (user define)
	set_time.Hours = hours;
	set_time.Minutes = minutes;
	set_time.Seconds = seconds;
	set_time.Year = years;
	set_time.Month = months;
	set_time.Date = dates;
	flash_write((uint32_t *) &set_time, sizeof(set_time));
	//HAL_Delay(10);
	//flash_read((uint32_t *) &set_time, sizeof(set_time));

	//printf("magic: %08x\n", set_time.magic);
	//printf("Hours: %02d\n", set_time.Hours);
	//printf("Minutes: %02d\n", set_time.Minutes);
	printf("*****BACKUP ****** DATE : %4d - %2d - %2d   TIME : %2d : %2d : %2d\n"
			, set_time.Year + 2000, set_time.Month, set_time.Date, set_time.Hours, set_time.Minutes, set_time.Seconds);

}

void flash_import_time_date(void)
{
	flash_read((uint32_t *) &set_time, sizeof(set_time));

	hours = set_time.Hours;
	minutes = set_time.Minutes;
	seconds = set_time.Seconds;
	years = set_time.Year;
	months = set_time.Month;
	dates = set_time.Date;

	ds_time.hour = hours;
	ds_time.minutes = minutes;
	ds_time.seconds = seconds;
	ds_time.year = years;
	ds_time.month = months;
	ds_time.date = dates;

	ds1302_init_time_date();
}


HAL_StatusTypeDef flash_write(uint32_t *data32, int size)
{
	uint32_t *mem32 = data32;

	/* Unlock to control */
	HAL_FLASH_Unlock(); // first, Unlook

	uint32_t Address = FLASH_USER_START_ADDR;

	//printf("t_student size: %d\n", size);

	/* Writing data to flash memory */
	for (int i=0; i < size; )
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, *mem32) == HAL_OK)
		{
			//printf("mem32: %0x\n", *mem32);
			mem32++; // After value store, go to next address
			Address = Address + 4; // unsigned int -> +4
			i += 4;
		}
		else
		{
			uint32_t errorcode = HAL_FLASH_GetError();
			return HAL_ERROR;
		}
	}
	/* Lock flash control register */
	HAL_FLASH_Lock();

	return HAL_OK;
}

HAL_StatusTypeDef flash_read(uint32_t *addr32, int size)
{
	uint32_t *data32 = addr32;
	uint32_t address = FLASH_USER_START_ADDR;
	uint32_t end_address = FLASH_USER_START_ADDR + size;

	while(address < end_address)
	{
		*data32 = *(uint32_t*) address;
		data32++;
		address = address + 4;
	}

	return HAL_OK;

}


HAL_StatusTypeDef flash_erase()
{
	uint32_t SectorError = 0;

	/* Unlock to control */
	HAL_FLASH_Unlock();

	/* Calculate sector index */
	uint32_t UserSector = 7;     // sector 번호
	uint32_t NbOfSectors = 1;    // sector 수

	/* Erase sectors */
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
	EraseInitStruct.Sector = UserSector;
	EraseInitStruct.NbSectors = NbOfSectors;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{
		uint32_t errorcode = HAL_FLASH_GetError();
		return HAL_ERROR;
	}

	/* Lock flash control register */
	HAL_FLASH_Lock();

	return HAL_OK;
}
