#include "iEE_I2Clcd.h"
uint8_t pcf8574_addr = PCF8574_ADDR;
uint8_t iEE_I2Clcd::i2c_pcf8574_write( uint8_t data ) {
  uint8_t result;
  result = i2c.StartTransmission( (pcf8574_addr << 1) | TW_WRITE );
  if ( result ) {
    i2c.EndTransmission();
    return 1;
  } 
  i2c.write(data);
  i2c.EndTransmission();
  return 0; // ok
}

void iEE_I2Clcd::i2c_write4bits( uint8_t data ) {
  data = data | _backlight;
  i2c_pcf8574_write( data | BIT_CS );
  _delay_us(200);
  i2c_pcf8574_write( data );
  _delay_us(200);
}

void iEE_I2Clcd::begin(uint8_t addr ) {
  i2c.begin();
  pcf8574_addr = addr;
  // Reset the LCD module
  i2c_write4bits( 0x03 << 4 );
  _delay_ms(5);
  i2c_write4bits( 0x03 << 4 );
  _delay_ms(150);
  i2c_write4bits( 0x03 << 4 );
  i2c_write4bits( 0x02 << 4 );
  // Set LCD to 4-bit mode
  writeCommand(LCD_FUNCTION_SET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
  _delay_ms(5);
  _displaycontrol = LCD_DISP_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
  // Display and cursor settings
  // writeCommand(LCD_DISP_CTRL| LCD_DISP_ON); // before
  writeCommand(LCD_DISP_CTRL| _displaycontrol);
  _delay_ms(5);
  _displaymode = LCD_ENTRY_LEFT | LCD_ENTRYSHIFTDECREMENT;
  // writeCommand(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT); // before
  writeCommand(LCD_ENTRY_MODE_SET | _displaymode);
  _delay_ms(5);
  // clear display and return home
  writeCommand(LCD_CLEAR_DISP);
  _delay_ms(5);
  writeCommand(LCD_RETURN_HOME);
  _delay_ms(5);
}

void iEE_I2Clcd::writeCommand(uint8_t value) {
  uint8_t hi_nibble = value & 0xf0;   
  uint8_t lo_nibble = (value << 4) & 0xf0;
  i2c_write4bits( hi_nibble );
  i2c_write4bits( lo_nibble );
}

void iEE_I2Clcd::write(uint8_t value) {
  uint8_t hi_nibble = value & 0xf0;   
  uint8_t lo_nibble = (value << 4) & 0xf0;
  i2c_write4bits( hi_nibble | BIT_RS );
  i2c_write4bits( lo_nibble | BIT_RS );
}

void iEE_I2Clcd::print(const char *str) {
  uint8_t cnt=0;
  for (int i=0; str[i] != 0 && cnt++ < 20; i++) {
    write(str[i]);
  }
}

void iEE_I2Clcd::setCursor(uint8_t x, uint8_t y) {
  uint8_t addr;
  addr = ((y == 0) ? 0x00 : 0x40) + x;
  writeCommand(LCD_SET_DDRAM_ADDR | addr);
}

void iEE_I2Clcd::clear(){
  writeCommand(LCD_CLEAR_DISP);
  _delay_ms(5);
}

void iEE_I2Clcd::home(){
  writeCommand(LCD_RETURN_HOME);
  _delay_ms(5);
}

void iEE_I2Clcd::noDisplay(){
  _displaycontrol &= ~LCD_DISP_ON;
  writeCommand(LCD_DISP_CTRL | _displaycontrol);
}
void iEE_I2Clcd::display(){
  _displaycontrol |= LCD_DISP_ON;
  writeCommand(LCD_DISP_CTRL | _displaycontrol);
}

void iEE_I2Clcd::noCursor() {
  _displaycontrol &= ~LCD_CURSOR_ON;
	writeCommand(LCD_DISP_CTRL | _displaycontrol);
}

void iEE_I2Clcd::cursor() {
  _displaycontrol |= LCD_CURSOR_ON;
	writeCommand(LCD_DISP_CTRL | _displaycontrol);
}

void iEE_I2Clcd::noBlink() {
  _displaycontrol &= ~LCD_BLINK_ON;
	writeCommand(LCD_DISP_CTRL | _displaycontrol);
}

void iEE_I2Clcd::blink() {
  _displaycontrol |= LCD_BLINK_ON;
	writeCommand(LCD_DISP_CTRL | _displaycontrol);
}

void iEE_I2Clcd::scrollDisplayLeft() {
	writeCommand(LCD_CURSOR_SHIFT | LCD_DISP_MOVE | LCD_MOVE_LEFT);
}

void iEE_I2Clcd::scrollDisplayRight() {
	writeCommand(LCD_CURSOR_SHIFT | LCD_DISP_MOVE | LCD_MOVE_RIGHT);
}

void iEE_I2Clcd::leftToRight() {
  _displaymode |= LCD_ENTRY_LEFT;
	writeCommand(LCD_ENTRY_MODE_SET | _displaymode);
}

void iEE_I2Clcd::rightToleft() {
  _displaymode |= LCD_ENTRY_RIGHT;
	writeCommand(LCD_ENTRY_MODE_SET | _displaymode);
}

void iEE_I2Clcd::autoscroll() {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
	writeCommand(LCD_ENTRY_MODE_SET | _displaymode);
}

void iEE_I2Clcd::noAutoscroll() {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	writeCommand(LCD_ENTRY_MODE_SET | _displaymode);
}

void iEE_I2Clcd::noBlacklight(){
  _backlight = LCD_NOBACKLIGHT;
}
void iEE_I2Clcd::blacklight(){
  _backlight = LCD_BACKLIGHT;
}

void iEE_I2Clcd::createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	writeCommand(LCD_SET_CGRAM_ADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
	}
}