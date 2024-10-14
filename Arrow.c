#include "main.h"
#include "button.h"

extern SPI_HandleTypeDef hspi2;

void arrow_up_display(void);
void arrow_down_display(void);
void arrrow_display(void);
void init_arrow_up(void);
void init_arrow_down(void);

int arrow_state=1;

extern int get_button(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin, int button_num);

uint8_t arrow_col[4]={0,0,0,0};

uint8_t arrow_up[]={
      0b00011000,
      0b00111100,
      0b01111110,
      0b11111111,
      0b00111100,
      0b00111100,
      0b00111100,
      0b00000000
};


uint8_t arrow_down[]={
      0b00000000,
      0b00111100,
      0b00111100,
      0b00111100,
      0b11111111,
      0b01111110,
      0b00111100,
      0b00011000
};

unsigned char display_up_data[8];
unsigned char display_down_data[8];
unsigned char scroll_buff_up[50][8] = {0};
unsigned char scroll_buff_down[50][8] = {0};

void init_arrow_up(void)
{
   for (int i=0; i < 8; i++)
   {
      display_up_data[i] = arrow_up[i];
   }
   for (int i=0; i < 8; i++)
   {
      scroll_buff_up[0][i] = arrow_up[i]; // scroll_buffer에 arrow_up 데이터를 저장
   }
}

void init_arrow_down(void)
{
   for (int i=0; i < 8; i++)
   {
      display_down_data[i] = arrow_down[i];
   }
   for (int i=7; i > 0; i--)
   {
      scroll_buff_down[0][i] = arrow_down[i]; // scroll_buffer에 arrow_up 데이터를 저장
   }
}


void arrow_display(void){

   if(get_button(GPIOC, GPIO_PIN_0, BUTTON0) == BUTTON_PRESS){
      arrow_state = !arrow_state;
   }

   if(arrow_state){
      arrow_up_display();
   }
   else{
      arrow_down_display();
   }
}

void arrow_up_display(void){

   static uint32_t past_time=0;  // 이전 tick값 저장
      uint32_t now = HAL_GetTick();  // 현재 시간

      // 500ms마다 화살표 위로 시프트
      if (now - past_time >= 500)
      {
         past_time = now;

         // display_data를 위로 한 칸 시프트
         uint8_t up_buffer = display_up_data[0]; // 첫 번째 줄 저장
         for (int i = 0; i < 7; i++) // 위로 한 칸씩 밀기
         {
            display_up_data[i] = display_up_data[i+1];
         }
         display_up_data[7] = up_buffer; // 첫 번째 줄을 마지막 줄로 이동
      }

      // display_data 출력
      for (int i = 0; i < 8; i++)
      {
        arrow_col[0] = ~(1 << i);  // 00000001  --> 11111110
        arrow_col[1] = display_up_data[i];
         HAL_SPI_Transmit(&hspi2, arrow_col, 2, 1);
         GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
         GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
         HAL_Delay(1);
      }

}

void arrow_down_display(void){

   static uint32_t past_time=0;  // 이전 tick값 저장
      uint32_t now = HAL_GetTick();  // 현재 시간

      // 500ms마다 화살표 위로 시프트
      if (now - past_time >= 500)
      {
         past_time = now;

         // display_data를 위로 한 칸 시프트
         uint8_t down_buffer = display_down_data[7]; // 첫 번째 줄 저장
         for (int i = 7; i > 0; i--) // 위로 한 칸씩 밀기
         {
            display_down_data[i] = display_down_data[i-1];
         }
         display_down_data[0] = down_buffer; // 첫 번째 줄을 마지막 줄로 이동
      }

      // display_data 출력
      for (int i = 0; i < 8; i++)
      {
         arrow_col[0] = ~(1 << i);  // 00000001  --> 11111110
         arrow_col[1] = display_down_data[i];
         HAL_SPI_Transmit(&hspi2, arrow_col, 2, 1);
         GPIOB->ODR &= ~GPIO_PIN_15;   // latch핀을 pull-down
         GPIOB->ODR |= GPIO_PIN_15;   // latch핀을 pull-up
         HAL_Delay(1);
      }

}
