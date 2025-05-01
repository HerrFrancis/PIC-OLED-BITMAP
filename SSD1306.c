/******************************************************************************
 SSD1306 OLED driver for CCS PIC C compiler (SSD1306.c)                       
                                                                              
 The driver is for I2C mode only.                                             
                                                                              
 http://simple-circuit.com/                                                   
                                                                              
 This is a optimized and enhanced version of the original driver              
 which was downloaded from:
 https://simple-circuit.com/pic16f877a-ssd1306-oled-display/
 Modified in 2025 by Herr Technik (Youtube: Herr Technik)
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


uint8_t _i2caddr, _vccstate, x_pos = 1, y_pos = 1;

//--------------------------------------------------------------------------//

int1 wrap = TRUE;

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


void ssd1306_command(uint8_t c) {
    uint8_t control = 0x00;   // Co = 0, D/C = 0
    I2C_Start(SSD1306_STREAM);
    I2C_Write(SSD1306_STREAM, _i2caddr);
    I2C_Write(SSD1306_STREAM, control);
    I2C_Write(SSD1306_STREAM, c);
    I2C_Stop(SSD1306_STREAM);
}

void SSD1306_Init(uint8_t vccstate = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = SSD1306_I2C_ADDRESS) {
  _vccstate = vccstate;
  _i2caddr  = i2caddr;
  #ifdef SSD1306_RST
    output_low(SSD1306_RST);
    output_drive(SSD1306_RST);
    delay_ms(10);
    output_high(SSD1306_RST);
  #endif
  // Init sequence
  ssd1306_command(SSD1306_DISPLAYOFF);                    // 0xAE
  ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  ssd1306_command(0x80);                                  // the suggested ratio 0x80

  ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
  ssd1306_command(SSD1306_LCDHEIGHT - 1);

  ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  ssd1306_command(0x0);                                   // no offset
  ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x10); }
  else
    { ssd1306_command(0x14); }
  ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
  ssd1306_command(0x00);                                  // 0x0 act like ks0108
  ssd1306_command(SSD1306_SEGREMAP | 0x1);
  ssd1306_command(SSD1306_COMSCANDEC);

 #if defined SSD1306_128_32
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x02);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  ssd1306_command(0x8F);

#elif defined SSD1306_128_64
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x12);
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x9F); }
  else
    { ssd1306_command(0xCF); }

#elif defined SSD1306_96_16
  ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  ssd1306_command(0x2);   //ada x12
  ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x10); }
  else
    { ssd1306_command(0xAF); }

#endif

  ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
  if (vccstate == SSD1306_EXTERNALVCC)
    { ssd1306_command(0x22); }
  else
    { ssd1306_command(0xF1); }
  ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  ssd1306_command(0x40);
  ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6

  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

  ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
}

void SSD1306_StartScrollRight(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_RIGHT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);  // start page
  ssd1306_command(0X00);
  ssd1306_command(stop);   // end page
  ssd1306_command(0X00);
  ssd1306_command(0XFF);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StartScrollLeft(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_LEFT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X00);
  ssd1306_command(0XFF);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StartScrollDiagRight(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  ssd1306_command(0X00);
  ssd1306_command(SSD1306_LCDHEIGHT);
  ssd1306_command(SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X01);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StartScrollDiagLeft(uint8_t start, uint8_t stop) {
  ssd1306_command(SSD1306_SET_VERTICAL_SCROLL_AREA);
  ssd1306_command(0X00);
  ssd1306_command(SSD1306_LCDHEIGHT);
  ssd1306_command(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
  ssd1306_command(0X00);
  ssd1306_command(start);
  ssd1306_command(0X00);
  ssd1306_command(stop);
  ssd1306_command(0X01);
  ssd1306_command(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_StopScroll(void) {
  ssd1306_command(SSD1306_DEACTIVATE_SCROLL);
}

void SSD1306_Dim(int1 dim) {
  uint8_t contrast;
  if (dim)
    contrast = 0; // Dimmed display
  else {
    if (_vccstate == SSD1306_EXTERNALVCC)
      contrast = 0x9F;
    else
      contrast = 0xCF;
  }
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  ssd1306_command(SSD1306_SETCONTRAST);
  ssd1306_command(contrast);
}

void SSD1306_SetTextWrap(int1 w) {
  wrap = w;
}

void SSD1306_InvertDisplay(int1 i) {
  if (i)
    ssd1306_command(SSD1306_INVERTDISPLAY_);
  else
    ssd1306_command(SSD1306_NORMALDISPLAY);
}

void SSD1306_GotoXY(uint8_t x, uint8_t y) {
  if((x > 21) || y > 8)
    return;
  x_pos = x;
  y_pos = y;
}

void SSD1306_PutC(uint8_t c) {
  uint8_t font_c;
  if((c < ' ') || (c > '~'))
    c = '?';
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(6 * (x_pos - 1));
  ssd1306_command(6 * (x_pos - 1) + 4); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(y_pos - 1); // Page start address (0 = reset)
  ssd1306_command(y_pos - 1); // Page end address
  
  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);
  
  for(uint8_t i = 0; i < 5; i++ ) {
    if(c < 'S')
      font_c = font[(c - 32) * 5 + i];
    else
      font_c = font2[(c - 'S') * 5 + i];
  
    I2C_Write(SSD1306_STREAM, font_c);
  }
  I2C_Stop(SSD1306_STREAM);

  x_pos = x_pos % 21 + 1;
  if (wrap && (x_pos == 1))
    y_pos = y_pos % 8 + 1;

}

void SSD1306_PutCustomC(char *c) {
  uint8_t line;
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(6 * (x_pos - 1));
  ssd1306_command(6 * (x_pos - 1) + 4); // Column end address (127 = reset)

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(y_pos - 1); // Page start address (0 = reset)
  ssd1306_command(y_pos - 1); // Page end address
  
  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);
  
  for(uint8_t i = 0; i < 5; i++ ) {
    line = c[i];
    I2C_Write(SSD1306_STREAM, line);
  }
  I2C_Stop(SSD1306_STREAM);

  x_pos = x_pos % 21 + 1;
  if (wrap && (x_pos == 1))
    y_pos = y_pos % 8 + 1;

}

void SSD1306_ClearDisplay(uint8_t content = 0x00) {
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);    // Column start address
  #if defined SSD1306_128_64 || defined SSD1306_128_32
  ssd1306_command(127);  // Column end address
  #else
    ssd1306_command(95); // Column end address
  #endif

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0);   // Page start address (0 = reset)
  #if defined SSD1306_128_64
  ssd1306_command(7);   // Page end address
  #elif defined SSD1306_128_32
  ssd1306_command(3);   // Page end address
  #elif defined SSD1306_96_16
  ssd1306_command(1);   // Page end address
  #endif

  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);

  for(uint16_t i = 0; i < SSD1306_TOTALPAGES; i++ )
    I2C_Write(SSD1306_STREAM, content);

  I2C_Stop(SSD1306_STREAM);
}


void SSD1306_Bitmap(int16 bitmap) // const uint8_t *bitmap
{
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(0);    // Column start address
  #if defined SSD1306_128_64 || defined SSD1306_128_32
  ssd1306_command(127);  // Column end address
  #else
    ssd1306_command(95); // Column end address
  #endif

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(0);   // Page start address (0 = reset)
  #if defined SSD1306_128_64
  ssd1306_command(7);   // Page end address
  #elif defined SSD1306_128_32
  ssd1306_command(3);   // Page end address
  #elif defined SSD1306_96_16
  ssd1306_command(1);   // Page end address
  #endif

  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);

  for(uint16_t i = 0; i < SSD1306_TOTALPAGES; i++)
  {
    uint8_t temp;
    read_program_memory(bitmap + i, &temp, 1); // read from ROM to RAM 
    I2C_Write(SSD1306_STREAM, temp);
  }
 
  I2C_Stop(SSD1306_STREAM);
}

void SSD1306_TinyBitmap(int16 bitmap, uint8_t x, uint8_t y, uint16_t width, uint16_t height) 
{
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
  
  ssd1306_command(SSD1306_COLUMNADDR);
  ssd1306_command(x);    // Column start address, minimum 0
  ssd1306_command(x + width - 1); // Column end address, maximum 127 or 95 only for the 96x16 screen

  ssd1306_command(SSD1306_PAGEADDR);
  ssd1306_command(y);   // Page start address, minimum 0
  ssd1306_command(y + height - 1);   // Page end address, maximum 7 (for 128x64 display), 3 (for 128x32 display) and 1 for (96x16) display

  I2C_Start(SSD1306_STREAM);
  I2C_Write(SSD1306_STREAM, _i2caddr);
  I2C_Write(SSD1306_STREAM, 0x40);
  uint16_t size = width * height;
  for(uint16_t i = 0; i < size; i++)
  {
    uint8_t temp;
    read_program_memory(bitmap + i, &temp, 1); // read from ROM to RAM
    I2C_Write(SSD1306_STREAM, temp);
  }
  I2C_Stop(SSD1306_STREAM);
}

