#include "dotmatrix.h"
#include "button.h"

extern SPI_HandleTypeDef hspi2;
void dotmatrix_main_test();
int dotmatrix_main(void);
int dotmatrix_main_func(void);
void arrow_display_up(void);
int arrow_state = 0;

uint8_t allon[] = {         // allon 문자 정의
   0b11111111,
   0b11111111,
   0b11111111,
   0b11111111,
   0b11111111,
   0b11111111,
   0b11111111,
   0b11111111
};


uint8_t smile[] = {         // 스마일 문자 정의
   0b00111100,
   0b01000010,
   0b10010101,
   0b10100001,
   0b10100001,
   0b10010101,
   0b01000010,
   0b00111100 };

uint8_t hart[] = {      // hart
   0b00000000,    // hart
   0b01100110,
   0b11111111,
   0b11111111,
   0b11111111,
   0b01111110,
   0b00111100,
   0b00011000
};

uint8_t one[] =
{0b00011000,
0b00111000,
0b00011000,
0b00011000,
0b00011000,
0b00011000,
0b01111110,
0b01111110};

uint8_t my_name[] =
{
      0b10010101,
      0B10010101,
      0B10010101,
      0B11110111,
      0B10010101,
      0B11110101,
      0B00000101,
      0B00000101
};

uint8_t col[4]={0,0,0,0};

void dotmatrix_main_test()
{
//  dotmatrix_main();

  while (1)
  {
        for (int i=0; i < 8; i++)
        {
         col[0] = ~(1 << i);  // 00000001  --> 11111110
         col[1] = my_name[i];
         HAL_SPI_Transmit(&hspi2, col, 2, 1);
         GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
         GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
         HAL_Delay(1);
        }
  }
}

uint8_t number_data[20][10] =
{
   {
      0b01110000,   //0
      0b10001000,
      0b10011000,
      0b10101000,
      0b11001000,
      0b10001000,
      0b01110000,
       0b00000000
   },
   {
      0b01000000,   //1
      0b11000000,
      0b01000000,
      0b01000000,
      0b01000000,
      0b01000000,
      0b11100000,
       6   // 점 0b00000110
   },
   {
      0b01110000,   //2
      0b10001000,
      0b00001000,
      0b00010000,
      0b00100000,
      0b01000000,
      0b11111000,
       6
   },
   {
      0b11111000,   //3
       0b00010000,
      0b00100000,
      0b00010000,
      0b00001000,
      0b10001000,
      0b01110000,
       6
   },
   {
      0b00010000,   //4
      0b00110000,
      0b01010000,
      0b10010000,
      0b11111000,
      0b00010000,
      0b00010000,
       6
   },
   {
      0b11111000,   //5
      0b10000000,
      0b11110000,
      0b00001000,
      0b00001000,
      0b10001000,
      0b01110000,
       6
   },
   {
      0b00110000,   //6
      0b01000000,
      0b10000000,
      0b11110000,
      0b10001000,
      0b10001000,
      0b01110000,
       6
   },
   {
      0b11111000,   //7
      0b10001000,
      0b00001000,
      0b00010000,
      0b00100000,
      0b00100000,
      0b00100000,
       6
   },
   {
      0b01110000,   //8
      0b10001000,
      0b10001000,
      0b01110000,
      0b10001000,
      0b10001000,
      0b01110000,
       6
   },
   {
      0b01110000,   //9
      0b10001000,
      0b10001000,
      0b01111000,
      0b00001000,
      0b00010000,
      0b01100000,
       6
   },
   {
      0b00000000,    // hart
      0b01100110,
      0b11111111,
      0b11111111,
      0b11111111,
      0b01111110,
      0b00111100,
      0b00011000
   }
};

uint8_t arrow_up[] =
{
      0B00011000,
      0B00111100,
      0B01111110,
      0B11111111,
      0B00011000,
      0B00011000,
      0B00011000,
      0B00000000
};

uint8_t arrow_down[] =
{
      0B00000000,
      0B00011000,
      0B00011000,
      0B00011000,
      0B11111111,
      0B01111110,
      0B00111100,
      0B00011000
};
unsigned char display_data[8];  // 최종 8x8 출력할 데이터
unsigned char display_data_up[8];  // 최종 8x8 출력할 데이터
unsigned char display_data_down[8];  // 최종 8x8 출력할 데이터
unsigned char scroll_buffer[50][8] = {0};  // 스코롤할 데이타가 들어있는 버퍼
int number_of_character = 11;  // 출력할 문자 갯수

// 초기화 작업
// 1. display_data에 number_data[0]에 있는 내용 복사
// 2. number_data를 scroll_buffer에 복사
// 3. dotmatrix의 led를 off
void init_dotmatrix(void)
{
   for (int i=0; i < 8; i++)
   {
      display_data[i] = number_data[i];
   }
   for (int i=1; i < number_of_character+1; i++)
   {
      for (int j=0; j < 8; j++) // scroll_buffer[0] = blank
      {
         scroll_buffer[i][j] = number_data[i-1][j];
      }
   }
}

// scroll 문자 출력 프로그램
int dotmatrix_main(void)
{
   static int count=0;  // 컬럼 count
   static int index=0;  // scroll_buffer의 2차원 index값
   static uint32_t past_time=0;  // 이전 tick값 저장

   init_dotmatrix();

   while(1)
   {
      uint32_t now = HAL_GetTick();  // 1ms
      // 1.처음시작시 past_time=0; now: 500 --> past_time=500
      if (now - past_time >= 500) // 500ms scroll
      {
         past_time = now;
         for (int i=0; i < 8; i++)
         {

            display_data[i] = (scroll_buffer[index][i] >> count) |
                  (scroll_buffer[index+1][i] << 8 - count);
         }
         if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
         {
            count =0;
            index++;  // 다음 scroll_buffer로 이동
            if (index == number_of_character+1) index=0;
            // 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
         }
      }
      for (int i=0; i < 8; i++)
      {
         // 공통 양극 방식
         // column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
         col[0] = ~(1 << i);  // 00000001  --> 11111110
         col[1] = display_data[i];
         HAL_SPI_Transmit(&hspi2, col, 2, 1);
         GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
         GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
         HAL_Delay(1);
      }
   }
   return 0;
}

int dotmatrix_main_func(void)
{
   static int count=0;  // 컬럼 count
   static int index=0;  // scroll_buffer의 2차원 index값
   static uint32_t past_time=0;  // 이전 tick값 저장

   uint32_t now = HAL_GetTick();  // 1ms


      if (now - past_time >= 500) // 500ms scroll
      {
         past_time = now;
         for (int i=0; i < 8; i++)
         {

            display_data[i] = (scroll_buffer[index][i] >> count) |
                  (scroll_buffer[index+1][i] << 8 - count);
         }
         if (++count == 8) // 8칼람을 다 처리 했으면 다음 scroll_buffer로 이동
         {
            count =0;
            index++;  // 다음 scroll_buffer로 이동
            if (index == number_of_character+1) index=0;
            // 11개의 문자를 다 처리 했으면 0번 scroll_buffer를 처리 하기위해 이동
         }
      }
      for (int i=0; i < 8; i++)
      {
         // 공통 양극 방식
         // column에는 0을 ROW에는 1을 출력해야 해당 LED가 on된다.
         col[0] = ~(1 << i);  // 00000001  --> 11111110
         col[1] = display_data[i];
         HAL_SPI_Transmit(&hspi2, col, 2, 1);
         GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
         GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
         HAL_Delay(1);
      }

}
void init_arrow_up(void)
{
   for (int i=0; i < 8; i++)
   {
      display_data_up[i] = arrow_up[i];
   }
   for (int i=0; i < 8; i++)
   {
      scroll_buffer[0][i] = arrow_up[i]; // scroll_buffer에 arrow_up 데이터를 저장
   }
}
void init_arrow_down(void)
{
   for (int i=0; i < 8; i++)
   {
      display_data_down[i] = arrow_down[i];
   }
   for (int i=7; i > 0; i--)
   {
      scroll_buffer[0][i] = arrow_down[i]; // scroll_buffer에 arrow_up 데이터를 저장
   }
}

void arrow_display_up(void)
{
   static uint32_t past_time=0;  // 이전 tick값 저장
   uint32_t now = HAL_GetTick();  // 현재 시간

   // 500ms마다 화살표 위로 시프트
   if (now - past_time >= 500)
   {
      past_time = now;

      // display_data를 위로 한 칸 시프트
      uint8_t temp = display_data_up[0]; // 첫 번째 줄 저장
      for (int i = 0; i < 7; i++) // 위로 한 칸씩 밀기
      {
         display_data_up[i] = display_data_up[i+1];
      }
      display_data_up[7] = temp; // 첫 번째 줄을 마지막 줄로 이동
   }

   // display_data 출력
   for (int i = 0; i < 8; i++)
   {
      col[0] = ~(1 << i);  // 00000001  --> 11111110
      col[1] = display_data_up[i];
      HAL_SPI_Transmit(&hspi2, col, 2, 1);
      GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
      GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
      HAL_Delay(1);
   }
}
void arrow_display_down(void)
{
   static uint32_t past_time=0;  // 이전 tick값 저장
   uint32_t now = HAL_GetTick();  // 현재 시간

   // 500ms마다 화살표 위로 시프트
   if (now - past_time >= 500)
   {
      past_time = now;

      // display_data를 위로 한 칸 시프트
      uint8_t temp = display_data_down[7]; // 첫 번째 줄 저장
      for (int i = 7; i > 0; i--) // 위로 한 칸씩 밀기
      {
         display_data_down[i] = display_data_down[i-1];
      }
      display_data_down[0] = temp; // 첫 번째 줄을 마지막 줄로 이동
   }

   // display_data 출력
   for (int i = 0; i < 8; i++)
   {
      col[0] = ~(1 << i);  // 00000001  --> 11111110
      col[1] = display_data_down[i];
      HAL_SPI_Transmit(&hspi2, col, 2, 1);
      GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
      GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
      HAL_Delay(1);
   }
}

void arrow_display(void)
{
   if(get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS)
      {
         arrow_state = !arrow_state;
      }
   if (arrow_state == 0)
   {
      arrow_display_up();
   }
   else
   {
      arrow_display_down();
   }
}
