/*
  iEE_I2Clcd.h - LCD I2C library for iEE-M328
  Modified 2024 by Jiranuwat.k (https://jiranuwats.gitbook.io/docs/)
  Ref. https://iot-kmutnb.github.io/blogs/arduino/avr_gcc_part-5
  This work is licensed under a Creative Commons Attribution-ShareAlike 4.0 International License.
*/
#ifndef IEE_I2C_LCD_H
#define IEE_I2C_LCD_H
#include <avr/io.h>
#include <util/delay.h>
#include "iEE_I2C.h"
  class iEE_I2Clcd 
    {
      private:
        uint8_t pcf8574_addr;
        #define PCF8574_ADDR        (0x27)
        #define PCF8574A_ADDR       (0x3F)

        // LCD commands
        #define LCD_CLEAR_DISP      (0x01)
        #define LCD_RETURN_HOME     (0x02)
        #define LCD_ENTRY_MODE_SET  (0x04)
        #define LCD_DISP_CTRL       (0x08)
        #define LCD_CURSOR_SHIFT    (0x10)
        #define LCD_FUNCTION_SET    (0x20)
        #define LCD_SET_CGRAM_ADDR  (0x40)
        #define LCD_SET_DDRAM_ADDR  (0x80)

        // Flags for display entry mode
        #define LCD_ENTRY_RIGHT     (0x00)
        #define LCD_ENTRY_LEFT      (0x02)
        #define LCD_ENTRYSHIFTINCREMENT 0x01
        #define LCD_ENTRYSHIFTDECREMENT 0x00
        // Flags for display on/off control
        #define LCD_DISP_ON         (0x04)
        #define LCD_CURSOR_ON       (0x02)
        #define LCD_BLINK_ON        (0x01)
        #define LCD_BLINK_OFF       (0x00)
        #define LCD_CURSOR_OFF      (0x00)
        // Flags for display/cursor shift
        #define LCD_DISP_MOVE       (0x08)
        #define LCD_CURSOR_MOVE     (0x00)
        #define LCD_MOVE_RIGHT      (0x04)
        #define LCD_MOVE_LEFT       (0x00)
        // Flags for function set
        #define LCD_4BITMODE       (0x00)
        #define LCD_2LINE          (0x08)
        #define LCD_5x8DOTS        (0x00)
        #define  BIT_RS  (0x01) 
        #define  BIT_RW  (0x02)
        #define  BIT_CS  (0x04)
        //#define  BIT_BL  (0x08)  // Backlight
        #define LCD_BACKLIGHT   0x08
        #define LCD_NOBACKLIGHT 0x00
        uint8_t _displaycontrol;
        uint8_t _displaymode;
        uint8_t _backlight = 0x08;
        uint8_t i2c_pcf8574_write(uint8_t data);
        void    i2c_write4bits(uint8_t data);
        void expanderWrite(uint8_t _data);
      public:
        void begin(uint8_t addr);
        void print(const char *);
        void setCursor(uint8_t, uint8_t);
        void writeCommand(uint8_t);
        void write(uint8_t);
        //  high level commands use
        void clear();
        void home();
        void noDisplay();
        void display();
        void noCursor();
        void cursor();
        void noBlink();
        void blink();
        void scrollDisplayLeft();
        void scrollDisplayRight();
        void leftToRight();
        void rightToleft();
        void autoscroll();
        void noAutoscroll();
        void noBlacklight();
        void blacklight();
        void createChar(uint8_t location, uint8_t charmap[]);
    };
#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_LCD)
extern iEE_I2Clcd lcd;
#endif
#endif