#include "Image.h"

int main(int argc, char** argv)
{
    Image test("test.png");
    
    // Image gray_avg = test;
    // gray_avg.grayscale_avg();
    // gray_avg.write("gray_avg.png");

    // Image gray_lum = test;
    // gray_lum.grayscale_lum();
    // gray_lum.write("gray_lum.png");

    // test.colorMask(0,0,1);
    // test.write("blue.png");

    // Image test1("test1.png");
    // Image test2("test2.png");

    // Image diff = test1;
    // diff.diffmap(test2);
    // diff.write("diff.png");

    Image test1("test1.png");
    Image test2("test2.png");

    test2.data[100] += 1;
    test2.data[101] += 1;
    test2.data[102] += 1;
    test2.data[200] += 1;
    test2.data[201] += 1;
    test2.data[202] += 1;
    test.write("other.png");

    Image diff = test1;
    diff.diffmap_scale(test2);
    diff.write("diff_scale.png");

    return 0;
}