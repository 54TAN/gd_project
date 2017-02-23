#include "Bitmap.h"

typedef unsigned DWORD; // was unsigned long
typedef void * HANDLE;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef int LONG;

#define BI_RGB 0

#include <iostream>
#include <cstring>
#include <fstream>

Bitmap::Bitmap(int w, int h) {
    width = w;
    height = h;
    data = new unsigned char [width*height*3];
    /*for (size_t i = 0; i < width*height*3; i++) {
        data[i] = (char)255;
    }*/
}

void Bitmap::add_pix(int i, int j, bool color, bool edge_path) {
    size_t index = (j * width + i) * 3;
    if (edge_path) {
        data[index] = (unsigned char) (0);
        data[index + 1] = (unsigned char) (0);
        data[index + 2] = (unsigned char) (255);
    } else {
        for (size_t k = 0; k < 3; k++) {
            data[index + k] = (unsigned char) ((color) ? 0 : 255);
        }
    }
}

typedef struct tagBITMAPFILEHEADER {
    WORD  bfType;
    DWORD bfSize;
    WORD  bfReserved1; //
    WORD  bfReserved2; //
    DWORD bfOffBits;
} BITMAPFILEHEADER;

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

    //hFile = CreateFile (fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    std::ofstream file(fname);
    file << bfh.bfType << bfh.bfSize << bfh.bfReserved1 << bfh.bfReserved2 << bfh.bfOffBits;

    file << bih.biSize << bih.biWidth << bih.biHeight << bih.biPlanes << bih.biBitCount
         << bih.biCompression << bih.biSizeImage << bih.biXPelsPerMeter << bih.biYPelsPerMeter
         << bih.biClrUsed << bih.biClrImportant;
/*
    for (int i = 0; i < 1024; i++) {
        file << Palette[i];
    }


    //WriteFile (hFile, &bfh, sizeof(bfh), &RW, NULL);
    //WriteFile (hFile, &bih, sizeof(bih), &RW, NULL);
    //WriteFile (hFile, Palette, 1024, &RW, NULL);

    for (size_t i = 0; i < height * width * 3; i++) {
        //WriteFile (hFile, &data[i], sizeof(char), &RW, NULL);
        file << &data[i];
    }

*/
}

