#include "oled_framebuffer.h"
#include "oled_fonts.h"

void oled_framebuffer_init(oled_framebuffer_t *framebuffer)
{
    framebuffer->width = OLED_SCREEN_WIDTH;
    framebuffer->height = OLED_SCREEN_HEIGHT;

    framebuffer->cursor_x = 0;
    framebuffer->cursor_y = 0;

    oled_framebuffer_clear(framebuffer);
}


void oled_framebuffer_clear(oled_framebuffer_t *framebuffer)
{
    for(uint16_t i = 0; i < OLED_128X64_SCREEN_BYTES; i++)
    {
        framebuffer->buffer[i] = 0x00;
    }
}


void oled_framebuffer_set_pixel(oled_framebuffer_t *framebuffer,
                                uint8_t x,
                                uint8_t y)
{
    if(x >= framebuffer->width ||
       y >= framebuffer->height)
    {
        return;
    }

    uint8_t page = y / 8;

    uint16_t index =
            (page * framebuffer->width) + x;

    uint8_t bit = y % 8;

    framebuffer->buffer[index] |= (1U << bit);
}


void oled_framebuffer_clear_pixel(oled_framebuffer_t *framebuffer,
                                  uint8_t x,
                                  uint8_t y)
{
    if(x >= framebuffer->width ||
       y >= framebuffer->height)
    {
        return;
    }

    uint8_t page = y / 8;

    uint16_t index =
            (page * framebuffer->width) + x;

    uint8_t bit = y % 8;

    framebuffer->buffer[index] &= ~(1U << bit);
}


void oled_framebuffer_write_pixel(oled_framebuffer_t *framebuffer,
                                  int16_t x,
                                  int16_t y,
                                  uint8_t state)
{
    if(x < 0 || y < 0)
    {
        return;
    }

    if(x >= framebuffer->width ||
       y >= framebuffer->height)
    {
        return;
    }

    uint8_t page = y / 8;

    uint16_t index =
            (page * framebuffer->width) + x;

    uint8_t bit = y % 8;

    if(state)
    {
        framebuffer->buffer[index] |= (1U << bit);
    }
    else
    {
        framebuffer->buffer[index] &= ~(1U << bit);
    }
}


void oled_framebuffer_draw_hline(oled_framebuffer_t *framebuffer,
                                 uint8_t x,
                                 uint8_t y,
                                 uint8_t length)
{
    for(uint16_t i = 0; i < length; i++)
    {
        if((x + i) >= framebuffer->width)
        {
            break;
        }

        oled_framebuffer_write_pixel(framebuffer,
                                     x + i,
                                     y,
                                     1);
    }
}


void oled_framebuffer_draw_vline(oled_framebuffer_t *framebuffer,
                                 uint8_t x,
                                 uint8_t y,
                                 uint8_t length)
{
    for(uint16_t i = 0; i < length; i++)
    {
        if((y + i) >= framebuffer->height)
        {
            break;
        }

        oled_framebuffer_write_pixel(framebuffer,
                                     x,
                                     y + i,
                                     1);
    }
}


void oled_framebuffer_draw_line(oled_framebuffer_t *framebuffer,
                                int16_t x0,
                                int16_t y0,
                                int16_t x1,
                                int16_t y1)
{
    int16_t dx =
            (x1 > x0) ? (x1 - x0) : (x0 - x1);

    int16_t sx =
            (x0 < x1) ? 1 : -1;

    int16_t dy =
            (y1 > y0) ? (y0 - y1) : (y1 - y0);

    int16_t sy =
            (y0 < y1) ? 1 : -1;

    int16_t error = dx + dy;

    while(1)
    {
        oled_framebuffer_write_pixel(framebuffer,
                                     x0,
                                     y0,
                                     1);

        if(x0 == x1 && y0 == y1)
        {
            break;
        }

        int16_t error2 = 2 * error;

        if(error2 >= dy)
        {
            error += dy;
            x0 += sx;
        }

        if(error2 <= dx)
        {
            error += dx;
            y0 += sy;
        }
    }
}


void oled_framebuffer_draw_rectangle(oled_framebuffer_t *framebuffer,
                                     uint8_t x,
                                     uint8_t y,
                                     uint8_t width,
                                     uint8_t height)
{
    if(width == 0 || height == 0)
    {
        return;
    }

    oled_framebuffer_draw_hline(framebuffer,
                                x,
                                y,
                                width);

    oled_framebuffer_draw_hline(framebuffer,
                                x,
                                y + height - 1,
                                width);

    oled_framebuffer_draw_vline(framebuffer,
                                x,
                                y,
                                height);

    oled_framebuffer_draw_vline(framebuffer,
                                x + width - 1,
                                y,
                                height);
}


void oled_framebuffer_fill_rectangle(oled_framebuffer_t *framebuffer,
                                     uint8_t x,
                                     uint8_t y,
                                     uint8_t width,
                                     uint8_t height)
{
    if(width == 0 || height == 0)
    {
        return;
    }

    for(uint16_t row = 0; row < height; row++)
    {
        if((y + row) >= framebuffer->height)
        {
            break;
        }

        oled_framebuffer_draw_hline(framebuffer,
                                    x,
                                    y + row,
                                    width);
    }
}


void oled_framebuffer_draw_circle(oled_framebuffer_t *framebuffer,
                                  int16_t x0,
                                  int16_t y0,
                                  int16_t radius)
{
    if(radius <= 0)
    {
        return;
    }

    int16_t x = radius;
    int16_t y = 0;
    int16_t error = 1 - radius;

    while(x >= y)
    {
        oled_framebuffer_write_pixel(framebuffer,
                                     x0 + x,
                                     y0 + y,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 + y,
                                     y0 + x,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 - y,
                                     y0 + x,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 - x,
                                     y0 + y,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 - x,
                                     y0 - y,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 - y,
                                     y0 - x,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 + y,
                                     y0 - x,
                                     1);

        oled_framebuffer_write_pixel(framebuffer,
                                     x0 + x,
                                     y0 - y,
                                     1);

        y++;

        if(error < 0)
        {
            error += (2 * y) + 1;
        }
        else
        {
            x--;

            error +=
                    (2 * (y - x)) + 1;
        }
    }
}


void oled_framebuffer_fill_circle(oled_framebuffer_t *framebuffer,
                                  int16_t x0,
                                  int16_t y0,
                                  int16_t radius)
{
    if(radius <= 0)
    {
        return;
    }

    for(int16_t y = -radius; y <= radius; y++)
    {
        for(int16_t x = -radius; x <= radius; x++)
        {
            if((x * x) + (y * y) <= (radius * radius))
            {
                oled_framebuffer_write_pixel(framebuffer,
                                             x0 + x,
                                             y0 + y,
                                             1);
            }
        }
    }
}


void oled_framebuffer_update(oled_framebuffer_t *framebuffer,
                             oled_driver_t *oled)
{
    oled_set_column_address(oled, 0, 127);

    oled_set_page_address(oled, 0, 7);

    for(uint8_t page = 0; page < 8; page++)
    {
        oled_send_data(oled,
                       &framebuffer->buffer[page * OLED_SCREEN_WIDTH],
                       OLED_SCREEN_WIDTH);
    }
}



void oled_framebuffer_set_cursor(oled_framebuffer_t *framebuffer,
                                 uint8_t x,
                                 uint8_t y)
{
    if(x >= framebuffer->width)
        x = framebuffer->width - 1;

    if(y >= framebuffer->height)
        y = framebuffer->height - 1;

    framebuffer->cursor_x = x;
    framebuffer->cursor_y = y;
}

void oled_framebuffer_write_char(oled_framebuffer_t *framebuffer,
                                 char ch)
{
    const uint8_t *character = NULL;

    uint8_t char_width = 6;

    if(ch == '\n')
    {
        framebuffer->cursor_x = 0;
        framebuffer->cursor_y += 8;

        if(framebuffer->cursor_y >= framebuffer->height)
            framebuffer->cursor_y = 0;

        return;
    }

    if(ch == ' ')
    {
        char_width = 4;

        if((framebuffer->cursor_x + char_width) > framebuffer->width)
        {
            framebuffer->cursor_x = 0;
            framebuffer->cursor_y += 8;

            if(framebuffer->cursor_y >= framebuffer->height)
                framebuffer->cursor_y = 0;
        }

        framebuffer->cursor_x += char_width;

        return;
    }

    if(ch >= 'A' && ch <= 'Z')
    {
        character = font5x7_AZ[ch - 'A'];
    }
    else if(ch >= 'a' && ch <= 'z')
    {
        character = font5x7_az[ch - 'a'];
    }
    else if(ch >= '0' && ch <= '9')
    {
        character = font5x7_09_numbers[ch - '0'];
    }
    else
    {
        switch(ch)
        {
            case '.':
                character = font5x7_symbols[0];
                break;

            case ',':
                character = font5x7_symbols[1];
                break;

            case ':':
                character = font5x7_symbols[2];
                break;

            case '-':
                character = font5x7_symbols[3];
                break;

            case '/':
                character = font5x7_symbols[4];
                break;

            default:
                return;
        }
    }

    if((framebuffer->cursor_x + char_width) > framebuffer->width)
    {
        framebuffer->cursor_x = 0;
        framebuffer->cursor_y += 8;

        if(framebuffer->cursor_y >= framebuffer->height)
            framebuffer->cursor_y = 0;
    }

    for(uint8_t column = 0; column < 5; column++)
    {
        uint8_t column_data = character[column];

        for(uint8_t bit = 0; bit < 7; bit++)
        {
            if(column_data & (1U << bit))
            {
                oled_framebuffer_write_pixel(framebuffer,
                                             framebuffer->cursor_x + column,
                                             framebuffer->cursor_y + bit,
                                             1);
            }
        }
    }

    framebuffer->cursor_x += 6;
}

void oled_framebuffer_printf(oled_framebuffer_t *framebuffer,
                             const char *str)
{
    while(*str)
    {
        oled_framebuffer_write_char(framebuffer, *str);
        str++;
    }
}

