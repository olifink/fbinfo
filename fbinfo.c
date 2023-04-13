#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define fbdev "/dev/fb0"
int main(int argc, char const *argv[])
{
    printf("Trying framebuffer\n");

    int fbfd = open(fbdev, O_RDWR);
    if (fbfd >= 0)
    {
        printf("Success opening framebuffer\n");

        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;

        ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo);
        ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo);

        int fb_width = vinfo.xres;
        int fb_height = vinfo.yres;
        int fb_bpp = vinfo.bits_per_pixel;
        int fb_bytes = fb_bpp / 8;
        int stride = finfo.line_length;

        printf("vscreeninfo:\n\
fb_width:  %d\n\
fb_height: %d\n\
fb_bpp:    %d\n\
fb_bytes:  %d\n\
stride:    %d\n\
line_len:  %d\n",
               fb_width, fb_height, fb_bpp, fb_bytes, stride, fb_width * fb_bytes);

        // memory map frame buffer file
        int fb_data_size = fb_width * fb_height * fb_bytes;

        char *fbdata = mmap(0, fb_data_size,
                            PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, (off_t)0);

        // access frame buffer
        // memset(fbdata, 0, fb_data_size);

        int x = 10;
        int y = 10;
        int offset = (y * stride) + x * 4;
        int r = 0xff;
        int g = 0x00;
        int b = 0xff;
        fbdata[offset + 0] = b;
        fbdata[offset + 1] = g;
        fbdata[offset + 2] = r;
        fbdata[offset + 3] = 0; // May not be neeeded
        // cleanup
        munmap(fbdata, fb_data_size);
        close(fbfd);
    }
    else
    {
        printf("open failed with %d", fbfd);
    }

    return 0;
}
