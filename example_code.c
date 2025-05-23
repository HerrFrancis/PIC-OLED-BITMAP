// Boilerplate configuration //
#include <16F877A.h>
#fuses HS, NOWDT, NOPROTECT, BROWNOUT    
#use delay(clock = 20MHz)
#use I2C(MASTER, I2C1, FAST = 100000, stream = SSD1306_STREAM)

// Libraries //
   // Include SSD1306 OLED driver source code
   #define SSD1306_128_64 // You have to specify which model you are using
   #include <oled.c>    

const uint8_t bitmap[] = { // size 32 bytes, width 16, height 2.
0x00, 0xFE, 0x56, 0x56, 0x02, 0xFA, 0xF2, 0xEA, 0xEA, 0xF2, 0xFA, 0x02, 0x56, 0x56, 0xFE, 0x00,
0x00, 0x7F, 0x75, 0x75, 0x60, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x60, 0x75, 0x75, 0x7F, 0x00
};

const uint8_t ghost[] = { // size 32 bytes, width 16, height 2.
0xFF, 0x3F, 0x07, 0x03, 0x71, 0xF9, 0xC8, 0x40, 0x00, 0x71, 0xF9, 0xCB, 0x47, 0x3F, 0xFF, 0xFF,
0xFF, 0xC0, 0xE0, 0xF0, 0xE0, 0xC0, 0xF0, 0xF0, 0xC0, 0xC0, 0xE0, 0xF0, 0xE0, 0xC0, 0xFF, 0xFF
};

const uint8_t pacman[] = { // size 32 bytes, width 16, height 2.
0xFF, 0x3F, 0xCF, 0xF7, 0xFB, 0xFD, 0xFD, 0x7E, 0x66, 0x66, 0xBE, 0xBD, 0xBD, 0xDB, 0xD7, 0xEF,
0xFF, 0xFC, 0xF3, 0xEF, 0xDF, 0xBF, 0xBF, 0x7F, 0x7F, 0x7E, 0x7D, 0xBD, 0xBD, 0xDB, 0xEB, 0xF7
};

const uint8_t like[] = { // size 1024 bytes, width 128, height 8.
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x03, 0x01, 0xF1, 0xF9, 0xF9, 0xF1,
0xE3, 0x07, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF,
0x3F, 0x1F, 0x9F, 0xCF, 0xCF, 0xE7, 0xE7, 0xF3, 0xF1, 0xF8, 0xFC, 0xFF, 0xFF, 0xFF, 0xCF, 0x87,
0x81, 0x90, 0x9C, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x3F, 0x3F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0x30, 0x00, 0x00, 0x00,
0x00, 0xF0, 0xF0, 0xF0, 0x10, 0xC0, 0xF0, 0xF0, 0xF0, 0xC0, 0xE0, 0xF0, 0x30, 0x10, 0x00, 0xF0,
0xF0, 0xF0, 0xB0, 0xB0, 0xB0, 0xB0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0x00,
0x00, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x73, 0x73, 0x23, 0x00, 0x88, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
0x3F, 0x1F, 0x8F, 0x8F, 0xC7, 0xE3, 0x03, 0x03, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0F, 0x0F, 0x0F, 0x0C, 0x0C, 0x0C, 0x00,
0x0F, 0x0F, 0x0F, 0x01, 0x08, 0x0F, 0x0F, 0x07, 0x01, 0x03, 0x0F, 0x0F, 0x0E, 0x00, 0x0E, 0x0F,
0x0F, 0x0F, 0x0D, 0x0D, 0x0D, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x3F, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEF, 0xE6, 0xC6, 0x06, 0x00, 0xF0, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F, 0x1F, 0x8F, 0xC7, 0xE3, 0xF1, 0xF1, 0xF8, 0xFC,
0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF,
0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFE,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0x1C, 0x0C, 0x1C, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xE7, 0xC3, 0x81, 0x18, 0x3C, 0x7E, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xDF, 0x8F, 0x0F, 0x21, 0x60, 0x06, 0x81, 0x19, 0x0F, 0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xBF, 0xB1, 0x31, 0x21, 0x24, 0x05, 0x8D, 0x8D, 0xFF, 0xFF, 0x83, 0x01, 0x21, 0x7F, 0x3F, 0x9F,
0x81, 0xE1, 0xFF, 0x1F, 0x01, 0x01, 0x25, 0x25, 0x25, 0x01, 0x91, 0xFB, 0xBF, 0xB3, 0x31, 0x21,
0x65, 0x24, 0x0D, 0x8D, 0xDD, 0xFF, 0xC7, 0x83, 0x01, 0x39, 0x3C, 0x3C, 0x99, 0xD9, 0xFB, 0x3F,
0x03, 0x01, 0xE1, 0xED, 0x8D, 0x05, 0x21, 0xF1, 0xFF, 0x3F, 0x01, 0x01, 0xF9, 0xFF, 0x0F, 0x01,
0x01, 0x25, 0x25, 0x05, 0x01, 0x91, 0xFB, 0x3F, 0x03, 0x01, 0x21, 0x25, 0x25, 0x25, 0xFD, 0xFF,
0xFF, 0xFF, 0xFF, 0x7F, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF1, 0xE3, 0xC7, 0x8F, 0x1F, 0x3F, 0x7F, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F,
0x3F, 0x3F, 0x3F, 0x00, 0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8,
0xF1, 0xE3, 0xC7, 0xC7, 0xC0, 0xE0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
const uint8_t herr[] = { // size 1024 bytes, width 128, height 8.
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0xBE, 0xFE, 0xFF, 0xFF, 0xFF,
0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7C,
0xFC, 0xFC, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFE, 0xFE, 0x1C, 0x08, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80,
0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xE0, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F,
0x0F, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC,
0xFC, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x03, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC7, 0xEF, 0xFF, 0xFF, 0x7E, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC7, 0xFF, 0xFF, 0xFE, 0x3C, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC1, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0,
0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC1, 0xC1, 0x80,
0x80, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x01, 0x01, 0x01, 0x01, 0x7F, 0x7F, 0x7F,
0x7F, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x71, 0x71, 0x71, 0x71, 0x71, 0x70, 0x00,
0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x03, 0x03, 0x07, 0x1F, 0x7F, 0x7F, 0x78, 0x70, 0x40, 0x00,
0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x03, 0x03, 0x03, 0x0F, 0x3F, 0x7F, 0x7C, 0x78, 0x60, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x0F, 0x1F, 0xBF,
0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0x18, 0x08, 0xE8, 0xE8, 0xE8, 0xEC,
0xEC, 0xEE, 0xEE, 0xEF, 0xEF, 0xEF, 0xEF, 0x0F, 0x07, 0xE7, 0xEF, 0xEF, 0xEF, 0xEF, 0xE7, 0xE3,
0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0,
0xC0, 0x80, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0,
0xE0, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0,
0xE0, 0xE0, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0xE0,
0xE0, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xE0, 0xE0, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x07, 0x07, 0x0F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x78, 0x70, 0x70,
0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x03, 0x03,
0x03, 0x03, 0x02, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x70, 0x70, 0x70, 0x70, 0x70, 0xFF, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0x3F, 0xFE, 0xF8, 0xE0, 0xFF, 0xFF,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
0xFF, 0x7C, 0xFE, 0xFF, 0xFF, 0xC7, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1C, 0x1C, 0x1C,
0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, 0x0F,
0x0F, 0x07, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F,
0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x03, 0x07, 0x1F, 0x1F,
0x1F, 0x1F, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x1F, 0x1F,
0x1F, 0x00, 0x00, 0x03, 0x07, 0x1F, 0x1F, 0x1E, 0x18, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void main()
{
   // Initialize the SSD1306 OLED with an I2C addr = 0x7A (default address)
   oledBegin(SSD1306_SWITCHCAPVCC, 0x78); // SSD1306_I2C_ADDRESS
   
   // prints like and subscribe bitmap on the entire screen (that's why the x, y parameter aren't set to anything)
   oledBitmap(&like, 128, 8);
   delay_ms(3500);
   oledClear(); // Clear the screen
   oledXY(0, 0); // Change the cursor to first column, first row
   oledPrint("Thanks for"); // Prints the first part of the message...
   oledXY(0, 1);
   oledPrint("downloading"); // the second part...
   oledXY(0, 2);
   oledPrint("this driver :D"); // the last part.
   delay_ms(2000);
   oledXY(0,0);
   oledPrint("Lorem ipsum dolor sit amet, consectetur adipiscing elit.");
   oledPrint("Sed pharetra turpis mauris, sit amet rhoncus mauris bibendum eget.");
   oledPrint("In porta purus ex. Nam non vulputate lorem.");
   delay_ms(3000);
   oledBitmap(&herr, 128, 8); // Prints the bitmap
   delay_ms(3500);
   
   // Fills the screen with tiny IC bitmaps
   for (int x = 0; x < 128; x = x + 16) 
   {
      for (int y = 0; y < 8; y = y + 2)
      {
         oledBitmap(&bitmap, 16, 2, x, y); // You change the position via x and y, that is the top-left corner of the bitmap
         delay_ms(250);
      }
   }
   oledClear();
   oledBitmap(&bitmap, 16, 2); // Draws IC bitmap
   oledScrollRight(0, 7); // Everything from line 0 to 7 will scroll rightwards
   delay_ms(2500);
   oledScrollLeft(0, 7); // now leftwards
   delay_ms(2500);
   oledStopScroll(); // and then stops.
   
   // Pacman
   oledClear(0xFF); // Clear the entire screen with white background
   oledScrollRight(0, 7); // Scroll rightwards
   oledClear(0xFF); // Clear again
   delay_ms(100);
   oledBitmap(&pacman, 16, 2, 0, 1); // Print Pacman bitmap at firt column, second row
   delay_ms(1500);
   oledBitmap(&ghost, 16, 2, 0, 1); // print a Ghost at the same location...
   delay_ms(1500);
   oledBitmap(&ghost, 16, 2, 0, 1); // Again
   delay_ms(1500);
   oledBitmap(&ghost, 16, 2, 0, 1); // And again.
}
