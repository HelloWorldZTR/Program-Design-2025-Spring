#include <windows.h>

#ifndef BITMAP_H
#define BITMAP_H

class BitMap
{
private:
    char *filename;
    tagBITMAPFILEHEADER bfh; // Bitmap file header
    /*
    typedef struct tagBITMAPFILEHEADER {
        WORD bfType;
        DWORD bfSize;
        WORD bfReserved1;
        WORD bfReserved2;
        DWORD bfOffBits;
    } BITMAPFILEHEADER,*LPBITMAPFILEHEADER,*PBITMAPFILEHEADER;
    */
    tagBITMAPINFOHEADER bih; // Bitmap info header
    /*
    typedef struct tagBITMAPINFOHEADER {
        DWORD biSize;
        LONG biWidth;
        LONG biHeight;
        WORD biPlanes;
        WORD biBitCount;
        DWORD biCompression;
        DWORD biSizeImage;
        LONG biXPelsPerMeter;
        LONG biYPelsPerMeter;
        DWORD biClrUsed;
        DWORD biClrImportant;
    } BITMAPINFOHEADER,*LPBITMAPINFOHEADER,*PBITMAPINFOHEADER;
     */
    BYTE *pBits;
    size_t totalBytes; // Dont know why the file header dont contain this field
public:
    BitMap();
    BitMap(const BitMap& bitmap);
    BitMap(const char *filename); 
    ~BitMap();
    void readFromFile(const char *filename);
    void writeToFile(const char *filename);
    void rotate90();
};
#endif // BITMAP_H