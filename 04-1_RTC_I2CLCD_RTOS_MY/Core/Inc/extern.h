extern void i2c_lcd_main(void);
extern void i2c_lcd_init(void);
extern void lcd_data(uint8_t data);
extern void lcd_string(uint8_t *str);
extern void move_cursor(uint8_t row, uint8_t column);

extern RTC_HandleTypeDef hrtc;
extern uint8_t pdatetime; // default -> print datetime;

extern int get_button(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, int button_num);
