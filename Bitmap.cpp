#include "Bitmap.h"

#include <iostream>
#include <cstring>

typedef unsigned DWORD; // was unsigned long
typedef void * HANDLE;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef int LONG; // was long, which means sizeof(2*int) on my computer

#define BI_RGB 0

Bitmap::Bitmap(int w, int h) {
    width = w;
    height = h;
    data = new unsigned char [width*height*3];
}

void Bitmap::add_pix(int i, int j, bool color, bool edge_path, bool optimized) {
    size_t index = (j * width + i) * 3;

    if (edge_path) {
        if (optimized) {
            data[index] = (unsigned char) (0);
            data[index + 1] = (unsigned char) (255);
            data[index + 2] = (unsigned char) (0);
            return;
        }
        data[index] = (unsigned char) (0);
        data[index + 1] = (unsigned char) (0);
        data[index + 2] = (unsigned char) (255);
    } else {
        for (size_t k = 0; k < 3; k++) {
            data[index + k] = (unsigned char) ((color) ? 0 : 255);
        }
    }
}

#pragma pack(push,1)
typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1; //
    WORD  bfReserved2; //
    DWORD bfOffBits;
} BITMAPFILEHEADER;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;//
    LONG  biXPelsPerMeter; //
    LONG  biYPelsPerMeter; //
    DWORD biClrUsed;
    DWORD biClrImportant; //
} BITMAPINFOHEADER;
#pragma pack(pop)

void Bitmap::out_bmp(const char * fname) {

    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    BYTE Palette [1024];

    memset(Palette, 0, 1024);
    memset(&bfh, 0, sizeof(bfh));
    memset(&bih, 0, sizeof(bih));

    bfh.bfType = 0x4D42;
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;
    bfh.bfSize = bfh.bfOffBits + sizeof(WORD) * width * height +
                 height * ((sizeof(WORD) * width) % 4);

    bih.biSize = sizeof(bih);
    bih.biBitCount = 24;
    bih.biClrUsed = 32768;
    bih.biCompression = BI_RGB;
    bih.biHeight = height;
    bih.biWidth = width;
    bih.biPlanes = 1;

    FILE * f2 = fopen(fname, "w+b");
    fwrite(&bfh, sizeof(bfh), 1, f2);
    fwrite(&bih, sizeof(bih), 1, f2);
    fwrite(&Palette, sizeof(Palette), 1, f2);
    for (size_t i = 0; i < height * width * 3; i++) {
        fwrite(&data[i], sizeof(char), 1, f2);
    }

}

