#ifndef __JPEG_H__
#define __JPEG_H__

#include "stdint.h"

#ifdef RELEASE
  #include "halconf.h"
#endif //RELEASE


#define ENABLE_WATERMARK (0)
#ifndef IMGWHL 
#define IMGWHL
#define IMG_WIDTH   (180)   //
#define IMG_HEIGHT  (120)   //
#define NUM_LINES   (IMG_HEIGHT/8)    // number of lines in image= hieght/8
#endif

//---------------- J P E G ---------------

// Application should provide this function for JPEG stream flushing
//extern void write_jpeg(uint8_t* buff, unsigned size);
extern uint32_t jpeg_addr_ptr(void);

typedef struct huffman_s
{
	const unsigned char  (*haclen)[12];
	const unsigned short (*hacbit)[12];
	const unsigned char  *hdclen;
	const unsigned short *hdcbit;
	const unsigned char  *qtable;
	short                dc;
}
huffman_t;

extern huffman_t huffman_ctx[3];

#define	HUFFMAN_CTX_Y	&huffman_ctx[0]
#define	HUFFMAN_CTX_Cb	&huffman_ctx[1]
#define	HUFFMAN_CTX_Cr	&huffman_ctx[2]

void huffman_start(short height, short width);
void huffman_resetdc(void);
void huffman_stop(void);
void huffman_encode(huffman_t *const ctx, const short data[64]);

#ifdef ENABLE_RGB
// color mapping
typedef unsigned char color;
typedef struct {
	color Red;
	color Green;
	color Blue;
} RGB;

// encode RGB 24 line [size: 15,360 bytes]
void encode_line_rgb24(uint8_t *    _line_buffer,
                       unsigned int _line_number);

// encode RGB 16 line [size: 10,240 bytes]
void encode_line_rgb16(uint8_t *    _line_buffer,
                       unsigned int _line_number);
#endif // ENABLE_RGB

// encode YUV line [size: 10,240 bytes]
void encode_line_yuv(uint8_t *    _line_buffer,
                     unsigned int _line_number);

// write re-start interval termination character
//  _rsi    :   3-bit restart interval character [0..7]
void write_RSI(unsigned int _rsi);

#if ENABLE_WATERMARK
// write VT watermark in Y8x8 lumiance array
void embed_vt_watermark(void);
#endif

#endif//__JPEG_H__
