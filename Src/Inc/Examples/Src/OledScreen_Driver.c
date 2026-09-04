/*
 * OledScreen_Driver.c
 *
 *  Created on: 30 Ağu 2026
 *      Author: Mansu
 */
#include "OledScreen_Driver.h"
#include "oled_fonts.h"

void Oled_Init(oled_driver_t *hi2c , reMap_mode mode , Scan_Direction scan_direction)
{
		mux_setting(hi2c);

	    set_display_offset(hi2c, 0);

	    set_display_start_line(hi2c, 0);

	    oled_set_reMap(hi2c, mode);

	    set_COM_Output(hi2c, scan_direction);

	    set_COM_pin_config(hi2c);

	    set_contrast(hi2c, 100);

	    entire_display(hi2c);

	    set_normal_display(hi2c);

	    oled_oscilator_setttings(hi2c);

	    enable_charge_pump(hi2c);

	    oled_set_memory_addressing_mode(hi2c);

	    oled_WakeUp(hi2c);


}

void oled_send_command(oled_driver_t *hi2c,
                       const uint8_t *command_data_pointer,
                       uint16_t length)
{
    uint8_t data[length + 1];

    data[0] = oled_command_control_byte;

    for(uint16_t i = 0; i < length; i++)
    {
        data[i + 1] = command_data_pointer[i];
    }

    HAL_I2C_Master_Transmit(hi2c->hi2c,
                            oled_i2c_address,
                            data,
                            sizeof(data),
                            100);
}

void oled_send_data(oled_driver_t *hi2c,
                    const uint8_t *data_pointer,
                    uint16_t length)
{
    uint8_t data[length + 1];

    data[0] = Control_data_control_byte;

    for(uint16_t i = 0; i < length; i++)
    {
        data[i + 1] = data_pointer[i];
    }

    HAL_I2C_Master_Transmit(hi2c->hi2c,
                            oled_i2c_address,
                            data,
                            sizeof(data),
                            100);
}

void oled_WakeUp(oled_driver_t *hi2c)
{
	uint8_t data[1] = {Oled_WakeUp};

	oled_send_command(hi2c, data, sizeof(data));
}



void oled_oscilator_setttings(oled_driver_t *hi2c)
{
	uint8_t data[2] = {oled_oscilator , OLED_OSCILLATOR_DEFAULT};

	oled_send_command(hi2c, data, sizeof(data));

}

void mux_setting(oled_driver_t *hi2c)
{
	uint8_t data[2] = {Set_Multiplex_Ratio , OLED_MUX_RATIO_64};

	oled_send_command(hi2c, data, sizeof(data));
}

void set_display_offset(oled_driver_t *hi2c , uint8_t offset)
{
	if(offset > 63)
	{
		offset = 63;
	}

		uint8_t data[2] = {oled_set_display , offset};

		oled_send_command(hi2c, data, sizeof(data));
}

void set_display_start_line(oled_driver_t *hi2c , uint8_t line)
{
	if(line > 63)
		line = 63;



	uint8_t data[1] = {line | line_start_address};

	oled_send_command(hi2c, data, sizeof(data));

}

void oled_set_reMap(oled_driver_t *hi2c , reMap_mode mode)
{
	uint8_t data[1] = {mode};

	oled_send_command(hi2c, data, sizeof(data));
}

void set_COM_Output(oled_driver_t *hi2c , Scan_Direction scan_direction)
{
	uint8_t data[1] = {scan_direction};

	oled_send_command(hi2c, data, sizeof(data));
}

void set_COM_pin_config(oled_driver_t *hi2c)
{
    uint8_t data[2] =
    {
        Set_COM_Pins_Hardware_Configuration,
        0x12
    }; /// 0x02 yerine 0x12 denicez hangisi bilmiyoruz

    oled_send_command(hi2c, data, sizeof(data));
}

void set_contrast(oled_driver_t *hi2c, uint8_t contrast)
{
	uint8_t data[2] = {set_contrast_control , contrast};

	oled_send_command(hi2c, data, sizeof(data));
}

void entire_display(oled_driver_t *hi2c)
{
	uint8_t data[1] = {entire_display_ram_info};

	oled_send_command(hi2c, data, sizeof(data));
}

void set_normal_display(oled_driver_t *hi2c)
{
	uint8_t data[1] = {normal_display};

	oled_send_command(hi2c, data, sizeof(data));

}

void enable_charge_pump(oled_driver_t *hi2c)
{
	uint8_t data[2] = {Charge_pump_address , Charge_Pump_Setting};

	oled_send_command(hi2c,data, sizeof(data));

}

void oled_set_memory_addressing_mode(oled_driver_t *oled)
{
    uint8_t data[2] =
    {
        SET_MEMORY_ADDRESSING_MODE,
        HORIZONTAL_ADDRESSING_MODE
    };

    oled_send_command(oled, data, sizeof(data));
}

void oled_set_column_address(oled_driver_t *oled,
                             uint8_t start_column,
                             uint8_t end_column)
{
    if(start_column > 127)
        start_column = 127;

    if(end_column > 127)
        end_column = 127;

    uint8_t data[3] =
    {
        SET_COLUMN_ADDRESS,
        start_column,
        end_column
    };

    oled_send_command(oled, data, sizeof(data));
}

void oled_set_page_address(oled_driver_t *oled,
                           uint8_t start_page,
                           uint8_t end_page)
{
    if(start_page > 7)
        start_page = 7;

    if(end_page > 7)
        end_page = 7;

    uint8_t data[3] =
    {
        SET_PAGE_ADDRESS,
        start_page,
        end_page
    };

    oled_send_command(oled, data, sizeof(data));
}

void oled_clear(oled_driver_t *oled)
{

    uint8_t clear_data[128] = {0};

    oled_set_column_address(oled, 0, 127);
    oled_set_page_address(oled, 0, 7);

    for(uint8_t i = 0; i < 8; i++)
    {
        oled_send_data(oled, clear_data, sizeof(clear_data));
    }

    oled_set_column_address(oled, 0, 127);
    oled_set_page_address(oled, 0, 7);
}


void oled_write_char(oled_driver_t *oled, char ch)
{
    uint8_t char_width;

    if(ch == '\n')
    {
        oled->cursor_page++;
        oled->cursor_column = 0;

        if(oled->cursor_page > 7)
            oled->cursor_page = 0;

        oled_set_cursor(oled,
                        oled->cursor_column,
                        oled->cursor_page);

        return;
    }

    if(ch == ' ')
        char_width = 4;
    else
        char_width = 6;

    if((oled->cursor_column + char_width) > 128)
    {
        oled->cursor_page++;
        oled->cursor_column = 0;

        if(oled->cursor_page > 7)
        {
            oled->cursor_page = 0;
        }

        oled_set_cursor(oled,
                        oled->cursor_column,
                        oled->cursor_page);
    }

    if(ch == ' ')
    {
        uint8_t word_space[4] = {0x00, 0x00, 0x00, 0x00};

        oled_send_data(oled, word_space, sizeof(word_space));

        oled->cursor_column += 4;

        return;
    }

    uint8_t space[1] = {0x00};

    if(ch >= 'A' && ch <= 'Z')
    {
        uint8_t index = ch - 'A';

        oled_send_data(oled, font5x7_AZ[index], 5);
        oled_send_data(oled, space, sizeof(space));

        oled->cursor_column += 6;

        return;
    }

    if(ch >= 'a' && ch <= 'z')
    {
        uint8_t index = ch - 'a';

        oled_send_data(oled, font5x7_az[index], 5);
        oled_send_data(oled, space, sizeof(space));

        oled->cursor_column += 6;

        return;
    }

    if(ch == '.' || ch == ',' || ch == ':' || ch == '-' || ch == '/')
    {
        oled_write_symbol(oled, ch);
        return;
    }

    if(ch >= '0' && ch <= '9')
    {
        uint8_t index = ch - '0';

        oled_send_data(oled, font5x7_09_numbers[index], 5);
        oled_send_data(oled, space, sizeof(space));

        oled->cursor_column += 6;

        return;
    }

}

void oled_printf(oled_driver_t *oled , const char *str)
{

	while(*str)
	{

		oled_write_char(oled, *str);

		str++;
	}


}

void oled_write_symbol(oled_driver_t *oled, char ch)
{
    uint8_t index;
    uint8_t space = 0x00;

    switch(ch)
    {
        case '.':
            index = 0;
            break;

        case ',':
            index = 1;
            break;

        case ':':
            index = 2;
            break;

        case '-':
            index = 3;
            break;

        case '/':
            index = 4;
            break;

        default:
            return;
    }

    oled_send_data(oled, font5x7_symbols[index], 5);
    oled_send_data(oled, &space, 1);

    oled->cursor_column += 6;
}

void oled_set_cursor(oled_driver_t *oled, uint8_t start_column, uint8_t start_page)
{


	if(start_page > 7)
		start_page = 7;

	if(start_column > 127)
		start_column = 127;

	oled->cursor_column = start_column;
	oled->cursor_page = start_page;

	oled_set_page_address(oled, start_page, 7);
	oled_set_column_address(oled, start_column, 127);

}


