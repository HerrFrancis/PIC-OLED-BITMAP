# OLED Display driver using PIC C

This project is a C program developed with the PIC C Compiler to control an SSD1306 OLED display. It demonstrates low-level control of the display via I²C communication using a PIC microcontroller. The code is optimized for clarity and reliability, making it an ideal learning tool and a base for embedded GUI applications. What makes this project different from other drivers is that it relies on the Program Memory, instead of ROM memory, which makes it suitable for low-end PIC microcontrollers such as the PIC16F887 or PIC16F877A. This project also includes a useful Windows program that can convert monochromatic .bmp images into a text file that can be copy and pasted into PIC C compiler for quick implementation. Finally, the program adds "tiny bitmaps", which are small bitmaps that can be used for simple animations or graphical user interfaces. They can be placed on any part of the screen.
# IMPORTANT NOTE REGARDING THE .EXE FILE
The height of the image you convert must be a multiple of 8, the format must be 24 bit Bitmap (.bmp), otherwise the program will not work.

## 📹 Watch the Tutorial

Check out the detailed explanation and demonstration on my YouTube channel:

▶️ [Watch the video on YouTube](https://www.youtube.com/watch?v=NlyU326dKjg)  
📺 Channel: [Herr Technik](https://www.youtube.com/@HerrTechnikMKI)
