#ifndef GD_PROJECT_BITMAP_H
#define GD_PROJECT_BITMAP_H


class Bitmap {
public:
    unsigned char * data;
    int width;
    int height;
    
    Bitmap() = default;
    Bitmap(int, int);
    void add_pix(int, int, bool, bool, bool optimized = 0);
    void out_bmp(const char * );
};


#endif //GD_PROJECT_BITMAP_H
