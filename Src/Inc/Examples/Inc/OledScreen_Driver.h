/*
 * OledScreen_Driver.h
 *
 *  Created on: 30 Ağu 2026
 *      Author: Mansu
 */
#include "main.h"
#ifndef INC_OLEDSCREEN_DRIVER_H_
#define INC_OLEDSCREEN_DRIVER_H_

#define oled_i2c_address			(0x3C<<1)
#define oled_command_control_byte 		0x00
#define Control_data_control_byte		0x40
#define Oled_WakeUp					0xAF
#define Oled_Sleep					0xAE
#define oled_oscilator				0xD5
#define OLED_OSCILLATOR_DEFAULT 	0x80
#define Set_Pre_charge_Period		0xD9
#define Set_Multiplex_Ratio			0xA8
#define OLED_MUX_RATIO_64	0x3F
#define oled_set_display		0xD3
#define line_start_address		0x40
#define Set_COM_Pins_Hardware_Configuration 0xDA
#define set_contrast_control			0x81
#define entire_display_ram_info		0xA4
#define normal_display				0xA6
#define Charge_pump_address			0x8D
#define Charge_Pump_Setting			0x14

#define SET_MEMORY_ADDRESSING_MODE   0x20
#define SET_COLUMN_ADDRESS           0x21
#define SET_PAGE_ADDRESS             0x22

#define HORIZONTAL_ADDRESSING_MODE   0x00

typedef struct
{
	I2C_HandleTypeDef *hi2c;

	uint8_t cursor_column;
	uint8_t cursor_page;

}oled_driver_t;

typedef enum
{
	reset_mode = 0xA0,
	normal_mode = 0xA1

}reMap_mode;


typedef enum
{
    SCAN_NORMAL  = 0xC0,
    SCAN_REVERSE = 0xC8

} Scan_Direction;

void Oled_Init(oled_driver_t *hi2c , reMap_mode mode ,Scan_Direction scan_direction);

void oled_WakeUp(oled_driver_t *hi2c);

void oled_send_command(oled_driver_t *hi2c,
                       const uint8_t *command_data_pointer,
                       uint16_t length);

void oled_send_data(oled_driver_t *hi2c,
                    const uint8_t *data_pointer,
                    uint16_t length);

void oled_oscilator_setttings(oled_driver_t *hi2c);

void mux_setting(oled_driver_t *hi2c);

void set_display_offset(oled_driver_t *hi2c , uint8_t offset);

void set_display_start_line(oled_driver_t *hi2c , uint8_t line);

void set_COM_Output(oled_driver_t *hi2c , Scan_Direction scan_direction);

void set_COM_pin_config(oled_driver_t *hi2c);

void set_contrast(oled_driver_t *hi2c, uint8_t contrast);

void oled_set_reMap(oled_driver_t *hi2c, reMap_mode mode);

void entire_display(oled_driver_t *hi2c);

void set_normal_display(oled_driver_t *hi2c);

void enable_charge_pump(oled_driver_t *hi2c);

void oled_set_memory_addressing_mode(oled_driver_t *oled);

void oled_set_column_address(oled_driver_t *oled,
                             uint8_t start_column,
                             uint8_t end_column);

void oled_set_page_address(oled_driver_t *oled,
                           uint8_t start_page,
                           uint8_t end_page);

void oled_clear(oled_driver_t *oled);

void oled_write_char(oled_driver_t *oled, char ch);

void oled_printf(oled_driver_t *oled , const char *str);

void oled_set_cursor(oled_driver_t *oled, uint8_t collum, uint8_t page);

void oled_write_symbol(oled_driver_t *oled, char ch);
#endif /* INC_OLEDSCREEN_DRIVER_H_ */
