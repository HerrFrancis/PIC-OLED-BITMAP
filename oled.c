/******************************************************************************
 SSD1306 OLED driver for CCS PIC C compiler (SSD1306.c)                       
                                                                              
 The driver is for I2C mode only.                                             
                                                                              
 http://simple-circuit.com/                                                   
                                                                              
 This is an optimized and enhanced version of the original driver              
 which was downloaded from:
 https://simple-circuit.com/pic16f877a-ssd1306-oled-display/
 Modified in 2025 by Herr Francis (Youtube: Herr Technik)
                                  E-mail: contactotransistorizedmx@gmail.com
*******************************************************************************/


#include <stdint.h>

//------------------------------ Definitions ---------------------------------//

#define SSD1306_I2C_ADDRESS   0x7A

#if !defined SSD1306_128_32 && !defined SSD1306_96_16
#define SSD1306_128_64
#endif
#if defined SSD1306_128_32 && defined SSD1306_96_16
  #error "Only one SSD1306 display can be specified at once"
#endif

#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH            128
  #define SSD1306_LCDHEIGHT            64
  #define SSD1306_TOTALPAGES         1024
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH            128
  #define SSD1306_LCDHEIGHT            32
  #define SSD1306_TOTALPAGES          512
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH             96
  #define SSD1306_LCDHEIGHT            16
  #define SSD1306_TOTALPAGES          192
#endif

#define SSD1306_SETCONTRAST          0x81
#define SSD1306_DISPLAYALLON_RESUME  0xA4
#define SSD1306_DISPLAYALLON         0xA5
#define SSD1306_NORMALDISPLAY        0xA6
#define SSD1306_INVERTDISPLAY_       0xA7
#define SSD1306_DISPLAYOFF           0xAE
#define SSD1306_DISPLAYON            0xAF
#define SSD1306_SETDISPLAYOFFSET     0xD3
#define SSD1306_SETCOMPINS           0xDA
#define SSD1306_SETVCOMDETECT        0xDB
#define SSD1306_SETDISPLAYCLOCKDIV   0xD5
#define SSD1306_SETPRECHARGE         0xD9
#define SSD1306_SETMULTIPLEX         0xA8
#define SSD1306_SETLOWCOLUMN         0x00
#define SSD1306_SETHIGHCOLUMN        0x10
#define SSD1306_SETSTARTLINE         0x40
#define SSD1306_MEMORYMODE           0x20
#define SSD1306_COLUMNADDR           0x21
#define SSD1306_PAGEADDR             0x22
#define SSD1306_COMSCANINC           0xC0
#define SSD1306_COMSCANDEC           0xC8
#define SSD1306_SEGREMAP             0xA0
#define SSD1306_CHARGEPUMP           0x8D
#define SSD1306_EXTERNALVCC          0x01
#define SSD1306_SWITCHCAPVCC         0x02

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL                      0x2F
#define SSD1306_DEACTIVATE_SCROLL                    0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A


uint8_t _i2caddr, _vccstate, x_pos = 0, y_pos = 0;

//--------------------------------------------------------------------------//

uint8_t wrap = 1;

const char Font[] = {
0x00, 0x00, 0x00, 0x00, 0x00, // blank
0x00, 0x00, 0x5F, 0x00, 0x00, // !
0x00, 0x07, 0x00, 0x07, 0x00, // "
0x14, 0x7F, 0x14, 0x7F, 0x14, // #
0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
0x23, 0x13, 0x08, 0x64, 0x62, // %
0x36, 0x49, 0x56, 0x20, 0x50, // &
0x00, 0x08, 0x07, 0x03, 0x00, // '
0x00, 0x1C, 0x22, 0x41, 0x00, // (
0x00, 0x41, 0x22, 0x1C, 0x00, // )
0x2A, 0x1C, 0x7F, 0x1C, 0x2A, // *
0x08, 0x08, 0x3E, 0x08, 0x08, // +
0x00, 0x80, 0x70, 0x30, 0x00, // ,
0x08, 0x08, 0x08, 0x08, 0x08, // -
0x00, 0x00, 0x60, 0x60, 0x00, // .
0x20, 0x10, 0x08, 0x04, 0x02, // /
0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
0x00, 0x42, 0x7F, 0x40, 0x00, // 1
0x72, 0x49, 0x49, 0x49, 0x46, // 2
0x21, 0x41, 0x49, 0x4D, 0x33, // 3
0x18, 0x14, 0x12, 0x7F, 0x10, // 4
0x27, 0x45, 0x45, 0x45, 0x39, // 5
0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
0x41, 0x21, 0x11, 0x09, 0x07, // 7
0x36, 0x49, 0x49, 0x49, 0x36, // 8
0x46, 0x49, 0x49, 0x29, 0x1E, // 9
0x00, 0x00, 0x14, 0x00, 0x00, // :
0x00, 0x40, 0x34, 0x00, 0x00, // ;
0x00, 0x08, 0x14, 0x22, 0x41, // <
0x14, 0x14, 0x14, 0x14, 0x14, // =
0x00, 0x41, 0x22, 0x14, 0x08, // >
0x02, 0x01, 0x59, 0x09, 0x06, // ?
0x3E, 0x41, 0x5D, 0x59, 0x4E, // @
0x7C, 0x12, 0x11, 0x12, 0x7C, // A
0x7F, 0x49, 0x49, 0x49, 0x36, // B
0x3E, 0x41, 0x41, 0x41, 0x22, // C
0x7F, 0x41, 0x41, 0x41, 0x3E, // D
0x7F, 0x49, 0x49, 0x49, 0x41, // E
0x7F, 0x09, 0x09, 0x09, 0x01, // F
0x3E, 0x41, 0x41, 0x51, 0x73, // G
0x7F, 0x08, 0x08, 0x08, 0x7F, // H
0x00, 0x41, 0x7F, 0x41, 0x00, // I
0x20, 0x40, 0x41, 0x3F, 0x01, // J
0x7F, 0x08, 0x14, 0x22, 0x41, // K
0x7F, 0x40, 0x40, 0x40, 0x40, // L
0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
0x7F, 0x04, 0x08, 0x10, 0x7F, // N
0x3E, 0x41, 0x41, 0x41, 0x3E, // O
0x7F, 0x09, 0x09, 0x09, 0x06, // P
0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
0x7F, 0x09, 0x19, 0x29, 0x46  // R
};
const char Font2[] = {
0x26, 0x49, 0x49, 0x49, 0x32, // S
0x03, 0x01, 0x7F, 0x01, 0x03, // T
0x3F, 0x40, 0x40, 0x40, 0x3F, // U
0x1F, 0x20, 0x40, 0x20, 0x1F, // V
0x3F, 0x40, 0x38, 0x40, 0x3F, // W
0x63, 0x14, 0x08, 0x14, 0x63, // X
0x03, 0x04, 0x78, 0x04, 0x03, // Y
0x61, 0x59, 0x49, 0x4D, 0x43, // Z
0x00, 0x7F, 0x41, 0x41, 0x41, // [
0x02, 0x04, 0x08, 0x10, 0x20, // \
0x00, 0x41, 0x41, 0x41, 0x7F, // ]
0x04, 0x02, 0x01, 0x02, 0x04, // ^
0x40, 0x40, 0x40, 0x40, 0x40, // _
0x00, 0x03, 0x07, 0x08, 0x00, // `
0x20, 0x54, 0x54, 0x78, 0x40, // a
0x7F, 0x28, 0x44, 0x44, 0x38, // b
0x38, 0x44, 0x44, 0x44, 0x28, // c
0x38, 0x44, 0x44, 0x28, 0x7F, // d
0x38, 0x54, 0x54, 0x54, 0x18, // e
0x00, 0x08, 0x7E, 0x09, 0x02, // f
0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
0x7F, 0x08, 0x04, 0x04, 0x78, // h
0x00, 0x44, 0x7D, 0x40, 0x00, // i
0x20, 0x40, 0x40, 0x3D, 0x00, // j
0x7F, 0x10, 0x28, 0x44, 0x00, // k
0x00, 0x41, 0x7F, 0x40, 0x00, // l
0x7C, 0x04, 0x78, 0x04, 0x78, // m
0x7C, 0x08, 0x04, 0x04, 0x78, // n
0x38, 0x44, 0x44, 0x44, 0x38, // o
0xFC, 0x18, 0x24, 0x24, 0x18, // p
0x18, 0x24, 0x24, 0x18, 0xFC, // q
0x7C, 0x08, 0x04, 0x04, 0x08, // r
0x48, 0x54, 0x54, 0x54, 0x24, // s
0x04, 0x04, 0x3F, 0x44, 0x24, // t
0x3C, 0x40, 0x40, 0x20, 0x7C, // u
0x1C, 0x20, 0x40, 0x20, 0x1C, // v
0x3C, 0x40, 0x30, 0x40, 0x3C, // w
0x44, 0x28, 0x10, 0x28, 0x44, // x
0x4C, 0x90, 0x90, 0x90, 0x7C, // y
0x44, 0x64, 0x54, 0x4C, 0x44, // z
0x00, 0x08, 0x36, 0x41, 0x00, // {
0x00, 0x00, 0x77, 0x00, 0x00, // |
0x00, 0x41, 0x36, 0x08, 0x00, // }
0x02, 0x01, 0x02, 0x04, 0x02  // ~
};

// Function used to send commands to the Oled display as
// a single byte
void oledCommand(uint8_t c) 
{
    uint8_t control = 0x00;   // Co = 0, D/C = 0
    I2C_Start(SSD1306_STREAM);
    I2C_Write(SSD1306_STREAM, _i2caddr);
    I2C_Write(SSD1306_STREAM, control);
    I2C_Write(SSD1306_STREAM, c);
    I2C_Stop(SSD1306_STREAM);
}

// The user must call this function before calling any other functions
// because this function configures the display
void oledBegin(uint8_t vccstate = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = SSD1306_I2C_ADDRESS) 
{
  _vccstate = vccstate;
  _i2caddr  = i2caddr;
  #ifdef SSD1306_RST
    output_low(SSD1306_RST);
    output_drive(SSD1306_RST);
    delay_ms(10);
    output_high(SSD1306_RST);
  #endif
  // Init sequence
  oledCommand(SSD1306_DISPLAYOFF);                    // 0xAE
  oledCommand(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  oledCommand(0x80);                                  // the suggested ratio 0x80

  oledCommand(SSD1306_SETMULTIPLEX);                  // 0xA8
  oledCommand(SSD1306_LCDHEIGHT - 1);

  oledCommand(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  oledCommand(0x0);                                   // no offset
  oledCommand(SSD1306_SETSTARTLINE | 0x0);            // line #0
  oledCommand(SSD1306_CHARGEPUMP);                    // 0x8D
  if (vccstate == SSD1306_EXTERNALVCC)
    { oledCommand(0x10); }
  else
    { oledCommand(0x14); }
  oledCommand(SSD1306_MEMORYMODE);                    // 0x20
  oledCommand(0x00);                                  // 0x0 act like ks0108
  oledCommand(SSD1306_SEGREMAP | 0x1);
  oledCommand(SSD1306_COMSCANDEC);

 #if defined SSD1306_128_32
  oledCommand(SSD1306_SETCOMPINS);                    // 0xDA
  oledCommand(0x02);
  oledCommand(SSD1306_SETCONTRAST);                   // 0x81
  oledCommand(0x8F);

#elif defined SSD1306_128_64
  oledCommand(SSD1306_SETCOMPINS);                    // 0xDA
  oledCommand(0x12);
  oledCommand(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { oledCommand(0x9F); }
  else
    { oledCommand(0xCF); }

#elif defined SSD1306_96_16
  oledCommand(SSD1306_SETCOMPINS);                    // 0xDA
  oledCommand(0x2);   //ada x12
  oledCommand(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { oledCommand(0x10); }
  else
    { oledCommand(0xAF); }

#endif

  oledCommand(SSD1306_SETPRECHARGE);                  // 0xd9
  if (vccstate == SSD1306_EXTERNALVCC)
    { oledCommand(0x22); }
  else
    { oledCommand(0xF1); }
  oledCommand(SSD1306_SETVCOMDETECT);                 // 0xDB
  oledCommand(0x40);
  oledCommand(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  oledCommand(SSD1306_NORMALDISPLAY);                 // 0xA6

  oledCommand(SSD1306_DEACTIVATE_SCROLL);

  oledCommand(SSD1306_DISPLAYON);//--turn on oled panel
}

// The next scroll functions are just intended to create the scrolling effect
void oledScrollRight(uint8_t start, uint8_t stop) 
{
  oledCommand(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  oledCommand(0X00);
  oledCommand(start);  // start page
  oledCommand(0X00);
  oledCommand(stop);   // end page
  oledCommand(0X00);
  oledCommand(0XFF);
  oledCommand(SSD1306_ACTIVATE_SCROLL);
}

void oledScrollLeft(uint8_t start, uint8_t stop) 
{
  oledCommand(SSD1306_LEFT_HORIZONTAL_SCROLL);
  oledCommand(0X00);
  oledCommand(start);
  oledCommand(0X00);
  oledCommand(stop);
  oledCommand(0X00);
  oledCommand(0XFF);
  oledCommand(SSD1306_ACTIVATE_SCROLL);
}

void oledScrollDiagRight(uint8_t start, uint8_t stop) 
{
  oledCommand(SSD1306_SET_VERTICAL_SCROLL_AREA);
  oledCommand(0X00);
  oledCommand(SSD1306_LCDHEIGHT);
  oledCommand(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  oledCommand(0X00);
  oledCommand(start);
  oledCommand(0X00);
  oledCommand(stop);
  oledCommand(0X01);
  oledCommand(SSD1306_ACTIVATE_SCROLL);
}

void oledScrollDiagLeft(uint8_t start, uint8_t stop) 
{
  oledCommand(SSD1306_SET_VERTICAL_SCROLL_AREA);
  oledCommand(0X00);
  oledCommand(SSD1306_LCDHEIGHT);
  oledCommand(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  oledCommand(0X00);
  oledCommand(start);
  oledCommand(0X00);
  oledCommand(stop);
  oledCommand(0X01);
  oledCommand(SSD1306_ACTIVATE_SCROLL);
}

void oledStopScroll(void) 
{
  oledCommand(SSD1306_DEACTIVATE_SCROLL);
}

// This function will dim the display
void oledDim(int1 dim) 
{
  uint8_t contrast;
  if (dim)
    contrast = 0; // Dimmed display
  else 
  {
    if (_vccstate == SSD1306_EXTERNALVCC)
      contrast = 0x9F;
    else
      contrast = 0xCF;
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  oledCommand(SSD1306_SETCONTRAST);
  oledCommand(contrast);
}

// If text wrap is activated, the cursor will go to the next line automatically
// when it reaches the last column, if it's not activated, the text in that line will
// be overwritten from left to right
void oledSetTextWrap(uint8_t w) 
{
   if (w == 0)
   {
      wrap = 0;
   }
   else
   {
      wrap = 1;
   }
}

// It will invert the display, white pixels become black and black pixels become white
void oledInvertDisplay(int1 i) 
{
  if (i)
    oledCommand(SSD1306_INVERTDISPLAY_);
  else
    oledCommand(SSD1306_NORMALDISPLAY);
}

// This function has effect only on the oledPrint function. You can place the character in any column (x)
// but the lines (y) can only be selected in groups of 8 pixels in what we call rows.
void oledXY(uint8_t x, uint8_t y) 
{
   // sanity check to ensure that if the user tries to write beyond the valid boundaries, the
   // function will have no effect
   #if defined SSD1306_128_64 
   if((x > 123) || y > 7)
      return;
   #elif defined SSD1306_128_32
   if((x > 123) || y > 3)
      return;
   #elif defined SSD1306_96_16
   if((x > 91) || y > 1)
      return;
   #endif

  x_pos = x;
  y_pos = y;
}

// This is a average print function, you can call it with a whole string such as
// "hello world", not only a single character. This function will automatically handle 
// the cursor control for you.
void oledPrint(uint8_t c) 
{
   // ignore it, if x-y coordinates are out of valid boundaries
   #if defined SSD1306_128_64 
   if((x_pos > 123) || y_pos > 7)
      return;
   #elif defined SSD1306_128_32
   if((x_pos > 123) || y_pos > 3)
      return;
   #elif defined SSD1306_96_16
   if((x_pos > 91) || y_pos > 1)
      return;
   #endif
   
   // if it's not a printable character, ignore it.
   if((c < ' ') || (c > '~'))
      return;
    
  uint8_t line;

   // Select which columns and pages are going to be modified
   oledCommand(SSD1306_COLUMNADDR);
   oledCommand(x_pos);
   oledCommand(x_pos + 4); // Column end address (127 = reset)
   
   oledCommand(SSD1306_PAGEADDR);
   oledCommand(y_pos); // Page start address (0 = reset)
   oledCommand(y_pos); // Page end address
   
   I2C_Start(SSD1306_STREAM);
   I2C_Write(SSD1306_STREAM, _i2caddr);
   I2C_Write(SSD1306_STREAM, 0x40);
   
  // Each character consists of 5 lines, so you have to send each line as a byte
  for(uint8_t i = 0; i < 5; i++ ) 
  {
    if (c < 'S')
    {
      line = font[(c - 32) * 5 + i];
    }
    else
    {
      line = font2[(c - 'S') * 5 + i];
    }
    I2C_Write(SSD1306_STREAM, line);
  }
  
  I2C_Stop(SSD1306_STREAM);

   // This handles the process of calculating where to place the next character
   #if defined SSD1306_128_64
   x_pos += 6;
   if (x_pos > 123)
   {
      x_pos = 0;
      if (wrap)
      {
         y_pos = (y_pos + 1) % 8;
      }
   }

   #elif defined SSD1306_128_32
   x_pos += 6;
   if (x_pos > 123)
   {
      x_pos = 0;
      if (wrap)
      {
         y_pos = (y_pos + 1) % 4;
      }
   }
   #elif defined SSD1306_96_16
   x_pos += 6;
   if (x_pos > 91)
   {
      x_pos = 0;
      if (wrap)
      {
         y_pos = 1 - y_pos;
      }
   }
   #endif
}

// This is a optimized function that replaces the original functions SSD1306_ClearDisplay
// and SSD1306_FillScreen just by using a optional argument with a default value of 0x00 
// meaning black. But of course it can be any 8-bit value that you want.
void oledClear(uint8_t content = 0x00) 
{
  oledCommand(SSD1306_COLUMNADDR);
  oledCommand(0);    // Column start address
  #if defined SSD1306_128_64 || defined SSD1306_128_32
  oledCommand(127);  // Column end address
  #else
    oledCommand(95); // Column end address
  #endif

  oledCommand(SSD1306_PAGEADDR);
  oledCommand(0);   // Page start address (0 = reset)
  #if defined SSD1306_128_64
  oledCommand(7);   // Page end address
  #elif defined SSD1306_128_32
  oledCommand(3);   // Page end address
  #elif defined SSD1306_96_16
  oledCommand(1);   // Page end address
  #endif

  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);

  for(uint16_t i = 0; i < SSD1306_TOTALPAGES; i++ )
    I2C_Write(SSD1306_STREAM, content);

  I2C_Stop(SSD1306_STREAM);
}

// This function handles the process of printing a bitmap of any size in the form of an array stored in program
// memory, the first argument is the address to that array in the ROM (e.g.: &logo)
// the second and third arguments are just the width that are automatically generated by my windows program
// for this just keep in mind that the height of the bitmap will always be a multiple of 8.
// finally, the x and y coordinates will tell the function where to place the bitmap, just make sure that you 
// are within valid boundaries, because otherwise the function will have no effect
void oledBitmap(int16 bitmap, uint16_t width, uint16_t height, uint8_t x = 0, uint8_t y = 0)
{
   // verifies that the bitmap is within valid boundaries
  #if  defined SSD1306_128_64
  if (x > 127 || x + width - 1 > 127) 
  {
   return;
  }
  if (y > 7 || y + height - 1 > 7)
  {
   return;
  }
  #elif defined SSD1306_128_32
  if (x > 127 || x + width - 1 > 127)
  {
   return;
  }
  if (y > 3 || y + height - 1 > 3)
  {
   return;
  }
  #elif defined SSD1306_96_16 
  if (x > 95 || x + width - 1 > 95)
  {
   return;
  }
  if (y > 1 || y + height - 1 > 1)
  {
   return;
  }
  #endif
  
  // tells the driver where it is going to place the bitmap
  oledCommand(SSD1306_COLUMNADDR);
  oledCommand(x);    // Column start address, minimum 0
  oledCommand(x + width - 1); // Column end address, maximum 127 or 95 only for the 96x16 screen

  oledCommand(SSD1306_PAGEADDR);
  oledCommand(y);   // Page start address, minimum 0
  oledCommand(y + height - 1);   // Page end address, maximum 7 (for 128x64 display), 3 (for 128x32 display) and 1 for (96x16) display

  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);

   // calculates the size of the bitmap
  uint16_t size = width * height;
  // and sends the bitmap one byte at a time (8 pixels)
  for(uint16_t i = 0; i < size; i++)
  {
    uint8_t temp;
    read_program_memory(bitmap + i, &temp, 1); // read from ROM to RAM
    I2C_Write(SSD1306_STREAM, temp);
  }
  I2C_Stop(SSD1306_STREAM);
}
