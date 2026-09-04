#ifndef INC_OLED_FRAMEBUFFER_H_
#define INC_OLED_FRAMEBUFFER_H_

#include "main.h"
#include "OledScreen_Driver.h"

#define OLED_SCREEN_WIDTH            128
#define OLED_SCREEN_HEIGHT           64
#define OLED_128X64_SCREEN_BYTES     1024

typedef struct
{
    uint8_t buffer[OLED_128X64_SCREEN_BYTES];

    uint8_t width;
    uint8_t height;

    uint8_t cursor_x;
    uint8_t cursor_y;

} oled_framebuffer_t;

/* Init / Clear */
void oled_framebuffer_init(oled_framebuffer_t *framebuffer);

void oled_framebuffer_clear(oled_framebuffer_t *framebuffer);


/* Pixel */
void oled_framebuffer_set_pixel(oled_framebuffer_t *framebuffer,
                                uint8_t x,
                                uint8_t y);

void oled_framebuffer_clear_pixel(oled_framebuffer_t *framebuffer,
                                  uint8_t x,
                                  uint8_t y);

void oled_framebuffer_write_pixel(oled_framebuffer_t *framebuffer,
                                  int16_t x,
                                  int16_t y,
                                  uint8_t state);


/* Lines */
void oled_framebuffer_draw_hline(oled_framebuffer_t *framebuffer,
                                 uint8_t x,
                                 uint8_t y,
                                 uint8_t length);

void oled_framebuffer_draw_vline(oled_framebuffer_t *framebuffer,
                                 uint8_t x,
                                 uint8_t y,
                                 uint8_t length);

void oled_framebuffer_draw_line(oled_framebuffer_t *framebuffer,
                                int16_t x0,
                                int16_t y0,
                                int16_t x1,
                                int16_t y1);


/* Rectangle */
void oled_framebuffer_draw_rectangle(oled_framebuffer_t *framebuffer,
                                     uint8_t x,
                                     uint8_t y,
                                     uint8_t width,
                                     uint8_t height);

void oled_framebuffer_fill_rectangle(oled_framebuffer_t *framebuffer,
                                     uint8_t x,
                                     uint8_t y,
                                     uint8_t width,
                                     uint8_t height);


/* Circle */
void oled_framebuffer_draw_circle(oled_framebuffer_t *framebuffer,
                                  int16_t x0,
                                  int16_t y0,
                                  int16_t radius);

void oled_framebuffer_fill_circle(oled_framebuffer_t *framebuffer,
                                  int16_t x0,
                                  int16_t y0,
                                  int16_t radius);


/* OLED Update */
void oled_framebuffer_update(oled_framebuffer_t *framebuffer,
                             oled_driver_t *oled);

void oled_framebuffer_set_cursor(oled_framebuffer_t *framebuffer,
                                 uint8_t x,
                                 uint8_t y);

void oled_framebuffer_write_char(oled_framebuffer_t *framebuffer,
                                 char ch);

void oled_framebuffer_printf(oled_framebuffer_t *framebuffer,
                             const char *str);


#endif /* INC_OLED_FRAMEBUFFER_H_ */
