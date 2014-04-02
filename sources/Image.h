#ifndef CSE168_IMAGE_H_INCLUDED
#define CSE168_IMAGE_H_INCLUDED

#include "Vector3.h"

class Image
{
public:
    struct Pixel
    {
        unsigned char r, g, b;
        Pixel(unsigned char ir, unsigned char ig, unsigned char ib) {set(ir, ig, ib);}
        Pixel() : r(0), g(0), b(0) {}
        void set(unsigned char ir, unsigned char ig, unsigned char ib) {r = ir; g = ig; b = ib;}
    };

    Image();
    ~Image();

    void resize(int width, int height);
    void setPixel(int x, int y, const Vector3& p);
    void setPixel(int x, int y, const Pixel& p);

    void draw();
    void drawScanline(int y);
    void clear(const Vector3& c);
    void writePPM(char* pcFile); // write data to a ppm image file
    void writePPM(char *pcName, unsigned char *data, int width, int height);

    unsigned char* getCharPixels()  {return (unsigned char*)m_pixels;}
    int width() const               {return m_width;}
    int height() const              {return m_height;}

private:
    Pixel* m_pixels;
    int m_width;
    int m_height;
};

extern Image * g_image;

#endif // CSE168_IMAGE_H_INCLUDED
