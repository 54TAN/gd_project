#ifndef GD_PROJECT_BITMAP_H
#define GD_PROJECT_BITMAP_H


struct Bitmap {
    unsigned char * data;
    int width;
    int height;
    
    Bitmap() = default;
    Bitmap(int w, int h);
    void add_pix(int i, int j, bool color, bool edge_path, bool optimized = 0);
    void out_bmp(const char * fname);
};


#endif //GD_PROJECT_BITMAP_H
