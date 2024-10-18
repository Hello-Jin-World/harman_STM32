#include "main.h" // HAL GPIO ...
#include <string.h> // strcry strncmp ...
#include <stdlib.h> // atoi itoa ...

#include "extern.h"

void get_rtc_date_time(void);
void set_rtc(char *date_time);
unsigned char dec2bcd(unsigned char byte);
unsigned char bcd2dec(unsigned char byte);

void get_rtc_date(void);
void get_rtc_time(void);

RTC_TimeTypeDef sTime = {0}; // Time Information
RTC_DateTypeDef sDate = {0}; // Date Information
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
	static RTC_TimeTypeDef oldTime = {0};
	char lcd_buff[40];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	if (oldTime.Seconds != sTime.Seconds)	// updated time information release. (one release per second)
	{
		if (pdatetime) // equal -> if (pdatetime >= 1)
		{
			// Format -> YYYY-MM-DD HH:mm:ss
			printf("%4d-%2d-%2d %2d:%2d:%2d\n", bcd2dec(sDate.Year) + 2000, bcd2dec(sDate.Month), bcd2dec(sDate.Date)
					, bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));

#if 1 // first line : date, second line : time
			sprintf(lcd_buff, "%2d-%2d-%2d", bcd2dec(sDate.Year), bcd2dec(sDate.Month), bcd2dec(sDate.Date));
			move_cursor(0, 0);
			lcd_string(lcd_buff);
			sprintf(lcd_buff, "%2d:%2d:%2d", bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));
			move_cursor(1, 0);
			lcd_string(lcd_buff);
#else
			sprintf(lcd_buff, "%2d-%2d-%2d%2d:%2d:%2d\n", bcd2dec(sDate.Year), bcd2dec(sDate.Month), bcd2dec(sDate.Date)
					, bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));

			move_cursor(0, 0);
			lcd_string(lcd_buff);
#endif
		}

		oldTime.Seconds = sTime.Seconds; // update Second
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

// From STM32 RTC, Let's write getting Date, Time information function.
void get_rtc_date(void)
{
	static RTC_TimeTypeDef oldTime = {0};
	char lcd_buff[40];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	if (oldTime.Seconds != sTime.Seconds)	// updated time information release. (one release per second)
	{
		// Format -> YYYY-MM-DD HH:mm:ss
		printf("%4d-%2d-%2d %2d:%2d:%2d\n", bcd2dec(sDate.Year) + 2000, bcd2dec(sDate.Month), bcd2dec(sDate.Date)
				, bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));

		sprintf(lcd_buff, "%2d-%2d-%2d", bcd2dec(sDate.Year), bcd2dec(sDate.Month), bcd2dec(sDate.Date));
		move_cursor(0, 0);
		lcd_string(lcd_buff);

		oldTime.Seconds = sTime.Seconds; // update Second
	}
}

// From STM32 RTC, Let's write getting Date, Time information function.
void get_rtc_time(void)
{
	static RTC_TimeTypeDef oldTime = {0};
	char lcd_buff[40];

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

	if (oldTime.Seconds != sTime.Seconds)	// updated time information release. (one release per second)
	{
		// Format -> YYYY-MM-DD HH:mm:ss
		printf("%4d-%2d-%2d %2d:%2d:%2d\n", bcd2dec(sDate.Year) + 2000, bcd2dec(sDate.Month), bcd2dec(sDate.Date)
				, bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));


		sprintf(lcd_buff, "%2d:%2d:%2d", bcd2dec(sTime.Hours), bcd2dec(sTime.Minutes), bcd2dec(sTime.Seconds));
		move_cursor(1, 0);
		lcd_string(lcd_buff);

		oldTime.Seconds = sTime.Seconds; // update Second
	}
}
