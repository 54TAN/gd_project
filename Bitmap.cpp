#include "Bitmap.h"

//#include <windows.h>
#include <iostream>
#include <cstring>

Bitmap::Bitmap(int w, int h)
{
    width = w;
    height = h;
    data = new unsigned char [width*height*3];
    /*for (size_t i = 0; i < width*height*3; i++) {
        data[i] = (char)255;
    }*/
}

void Bitmap::add_pix(int i, int j, bool color, bool edge_path)
{
    size_t index = (j*width + i)*3;
    if (edge_path) {
        data[index] = (unsigned char)(0);
        data[index + 1] = (unsigned char)(0);
        data[index + 2] = (unsigned char)(255);
    } else {
        for (size_t k = 0; k < 3; k++) {
            data[index + k] = (unsigned char)((color) ? 0 : 255);
        }
    }
}
/*

void Bitmap::out_bmp(const char * fname)
{
    HANDLE hFile;
    DWORD RW;

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

    hFile = CreateFile (fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

    WriteFile (hFile, &bfh, sizeof(bfh), &RW, NULL);
    WriteFile (hFile, &bih, sizeof(bih), &RW, NULL);
    WriteFile (hFile, Palette, 1024, &RW, NULL);

    for (size_t i = 0; i < height * width * 3; i++) {
        WriteFile (hFile, &data[i], sizeof(char), &RW, NULL);
    }
    CloseHandle(hFile);
}
*/
