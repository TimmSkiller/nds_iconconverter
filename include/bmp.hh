#ifndef bmp_h
#define bmp_h

#include <stdint.h>
#include <stdio.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

enum Compression
{
	BI_RGB = 0,
	I_RLE8 = 1,
	BI_RLE4 = 2,
	BI_BITFIELDS = 3,
	BI_JPEG = 4,
	BI_PNG = 5,
	BI_ALPHABITFIELDS = 6,
	BI_CMYK = 11,
	BI_CMYKRLE8 = 12,
	BI_CMYKRLE4 = 13,
};

#define bmp_data_size(b) (bmp_data_size_(b.height, b.width, b.bitsPerPixel))
#define bmp_data_size_(w, h, bpp) (((w) * (h) * ((bpp))) / 16)

template <size_t w, size_t h, size_t bpp>
struct __attribute__((packed)) BMP
{

	// Header
	char magic_header[2] = {'B', 'M'};	// BM, BA, CI, CP, IC or PT
	u32 fsize = sizeof(BMP<w, h, bpp>); // File size
	u8 reserved[4];						// Unused
	u32 offset = 0x7A;					// Offset to data

	// DIB (v4)
	u32 size_dib = 0x6C;						// Size of the DIB header
	u32 width = w;								// Don't touch, done by template
	u32 height = h;								// Don't touch, done by template
	u16 planeCount = 1;							// Total planes used
	u16 bitsPerPixel = bpp;						// Don't touch, done by template
	u32 compression = BI_BITFIELDS;				// Use `enum Compression`
	u32 bitmapSize = bmp_data_size_(w, h, bpp); // Don't touch, done by template
	u32 printRes_1 = 0;
	u32 printRes_2 = 0;
	u32 colorsInPalette = 0; // Colors in pallette num
	u32 importantColors = 0x00000000;
	u32 redChannelMask = 0x001F;
	u32 greenChannelMask = 0x03E0;
	u32 blueChannelMask = 0x7C00;
	u32 alphaChannelMask = 0x00000000;
	char magic_dib[4] = {'W', 'i', 'n', ' '}; // "Win "

	u8 unused_1[0x24];
	u8 unused_2[4];
	u8 unused_3[4];
	u8 unused_4[4];

	// Pixel data
	u16 data[bmp_data_size_(h, w, bpp)];
};

template <size_t w, size_t h, size_t bpp>
void save_to_file(BMP<w, h, bpp> bmp, const char *filename)
{
	FILE *fp = fopen(filename, "w");

	fwrite(&bmp, sizeof(BMP<w, h, bpp>), 1, fp);

	fclose(fp);
}

#endif