bwjpeg-parallel

This branch contains modifications for the grayscale encoding. The inpu timage is a grayscale image with one luminence value per pixel.
It also contains code for sending data through a parallel port which is made up of 8 GPIO pins and 2 handshaking signals.

stm32f4-dcmi-jpeg

This is a Keil project which interfaces the Digital Camera Media Interface(DCMI) on the STM32F407Vg found on the STM32F4Discovery board, with a NTSC Decoder IC(TW9910) which converts analog video to digital video. This project also compresses the digital image obtained from the camera to the jpeg format. 

There are three branches in this repo. The links to each branch are given:
 
 1. [master](https://github.com/t27/stm32f4-dcmi-jpeg/) - This branch contains the encoding code for a color image.
 2. [bwjpeg](https://github.com/t27/stm32f4-dcmi-jpeg/tree/bwjpeg) - This branch contains the encoder and changes made for a black and white image
 3. [bwjpeg-parallel](https://github.com/t27/stm32f4-dcmi-jpeg/tree/bwjpeg-parallel) - This branch contains the encoder for black and white images but also includes changes for using 8 GPIO pins as a parallel port.

