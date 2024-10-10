#include "main.h" // HAL GPIO ...
#include <string.h> // strcry strncmp ...
#include <stdlib.h> // atoi itoa ...

#include "extern.h"
#include "button.h""

void get_rtc_date_time(void);
void set_rtc(char *date_time);
void stopwatch(void);
void stop_mode(void);
void run_mode(void);
void clock_mode(void);
void set_sec(void);
void set_min(void);

unsigned char dec2bcd(unsigned char byte);
unsigned char bcd2dec(unsigned char byte);

extern void led_all_off(void);

extern volatile int SW_counter;

RTC_TimeTypeDef sTime = {0}; // Time Information
RTC_DateTypeDef sDate = {0}; // Date Information

int clock_sw_index = 0;
int sw_run_stop = 0;
int clock_run = 0;
int min = 0, sec = 0, ssec = 0;

void (*lets_sw[])() =
{
	stop_mode,
	run_mode
};

void (*lets_clock[])() =
{
	clock_mode,
	set_sec,
	set_min
};

// uint8_t Year
// ex) Data format stored in 2024 Year : 0010 0100 -> 2 4
unsigned char bcd2dec(unsigned char byte)
{
	unsigned char high, low;

	low = byte & 0x0f; // store low value
	high = (byte >> 4) * 10; // store high value

	return (high + low); // return unsigned type (high + low)
}

// convert dec to bcd
// ex) 24 (00011000) -> 0010 0100
unsigned char dec2bcd(unsigned char byte)
{
	unsigned char high, low;

	high = (byte / 10) << 4;
	low = byte % 10;

	return (high + low); // return unsigned type (high + low)
}

// From STM32 RTC, Let's write getting Date, Time information function.
void get_rtc_date_time(void)
{

	char lcd_buff[40];

	sprintf(lcd_buff, "%2d-%2d %2d:%2d:%2d   ", bcd2dec(sDate.Month), bcd2dec(sDate.Date),
					bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));
	move_cursor(0, 0);
	lcd_string(lcd_buff);
	(*lets_clock[clock_run])();
}

void clock_mode(void)
{
	static RTC_TimeTypeDef oldTime = {0};
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS && clock_sw_index == 0)
	{
		led_all_off();
		clock_run = 1;
	}
	if (oldTime.Seconds != sTime.Seconds)	// updated time information release. (one release per second)
	{
		oldTime.Seconds = sTime.Seconds; // update Second
	}
	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		led_all_off();
		clock_sw_index = 1;
	}

}

void set_sec(void)
{
	if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
	{
		sTime.Seconds = dec2bcd((bcd2dec(sTime.Seconds) + 1) % 60);
	}
	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		clock_run = 2;
	}
	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		led_all_off();
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
		clock_run = 0;
	}
}

void set_min(void)
{
	if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
	{
		if ((sTime.Minutes & 0x0F) < 9) {  // Units place 0-9 increment
			sTime.Minutes += 1;
		} else {  // If units place is 9
			sTime.Minutes &= 0xF0;  // Reset units place to 0
			if ((sTime.Minutes >> 4) < 5) {  // Tens place 0-5 increment
				sTime.Minutes += 0x10;
			} else {  // Reset to 0 if seconds exceed 59
				sTime.Minutes = 0;
			}
		}
	}
	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		clock_run = 1;
	}
	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		led_all_off();
		HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
		HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
		clock_run = 0;
	}
}


void stopwatch(void)
{
	if (clock_sw_index)
	{
		(*lets_sw[sw_run_stop])();
		if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
		{
			sw_run_stop = 1;
		}
	}
	else
	{
		move_cursor(1, 0);
		lcd_string("            CLK ");
	}
}


void stop_mode(void)
{
	char lcd_buff[40];
	sprintf(lcd_buff, "%2d:%2d:%2d    SW  ", min, sec, ssec);
	move_cursor(1, 0);
	lcd_string(lcd_buff);
	if (get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
	{
		led_all_off();
		clock_sw_index = 0;
	}
	if (get_button(GPIOC, GPIO_PIN_2, BUTTON2) == BUTTON_PRESS)
	{
		min = 0; sec = 0; ssec = 0;
	}

}

void run_mode(void)
{
	char lcd_buff[40];
	sprintf(lcd_buff, "%2d:%2d:%2d    SW  ", min, sec, ssec);
	move_cursor(1, 0);
	lcd_string(lcd_buff);
	if (SW_counter >= 1000)
	{
		SW_counter = 0;
		sec++;
		sec %= 60;
	}
	ssec = SW_counter / 16;
	ssec %= 60;

	if (get_button(GPIOC, GPIO_PIN_1, BUTTON1) == BUTTON_PRESS)
	{
		sw_run_stop = 0;
	}
}

// setrtc241008154500
// setrtcYYMMDDhhmmss
// date_time get 241008154500's address
#if 1
void set_rtc(char *date_time)
{
	char YY[4], MM[4], DD[4], h[4], m[4], s[4];

	strncpy(YY, date_time, 2);
	strncpy(MM, date_time+2, 2);
	strncpy(DD, date_time+4, 2);
	strncpy(h, date_time+6, 2);
	strncpy(m, date_time+8, 2);
	strncpy(s, date_time+10 , 2);

	// ascii -> int -> bcd -> rtc
	sDate.Year = dec2bcd(atoi(YY));
	sDate.Month = dec2bcd(atoi(MM));
	sDate.Date = dec2bcd(atoi(DD));

	sTime.Hours = dec2bcd(atoi(h));
	sTime.Minutes = dec2bcd(atoi(m));
	sTime.Seconds = dec2bcd(atoi(s));

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);


}
#else
void set_rtc(char *date_time)
{
	char date_infor[13];
	int YY, MM, DD, h, m, s;
	strncpy(date_infor, date_time, 12);
	date_infor[12] = '\0';
	sscanf(date_infor, "%d d% %d %d %d %d", YY, MM, DD, h, m, s);

	sDate.Year = dec2bcd(YY);
	sDate.Month = dec2bcd(MM);
	sDate.Date = dec2bcd(DD);

	sTime.Hours = dec2bcd(h);
	sTime.Minutes = dec2bcd(m);
	sTime.Seconds = dec2bcd(s);

	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD);
}
#endif
