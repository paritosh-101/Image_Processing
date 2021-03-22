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

    // Image test1("test1.png");
    // Image test2("test2.png");

    // test2.data[100] += 1;
    // test2.data[101] += 1;
    // test2.data[102] += 1;
    // test2.data[200] += 1;
    // test2.data[201] += 1;
    // test2.data[202] += 1;
    // test.write("other.png");

    // Image diff = test1;
    // diff.diffmap_scale(test2);
    // diff.write("diff_scale.png");

    // double ker[] = { -2/9.0, -1/9.0, 0
    //                 -1/9.0, 1/9.0, 1/9.0,
    //                 0, 1/9.0, 2/9.0 }; //emboss

    double ker[] = { 1/16.0, 2/16.0, 1/16.0,
                    2/16.0, 4/16.0, 2/16.0,
                    1/16.0, 2/16.0, 1/16.0 }; //gaussian blur

    Image t0 = test;
    Image t1 = test;
    Image t2 = test;

    t0.std_convolve_clamp_to_0(0, 3, 3, ker, 1, 1);
    t0.std_convolve_clamp_to_0(1, 3, 3, ker, 1, 1);
    t0.std_convolve_clamp_to_0(2, 3, 3, ker, 1, 1);

    t0.write("blurred.png");
    
    t1.std_convolve_clamp_to_border(0, 3, 3, ker, 1, 1);
    t1.std_convolve_clamp_to_border(1, 3, 3, ker, 1, 1);
    t1.std_convolve_clamp_to_border(2, 3, 3, ker, 1, 1);

    t1.write("blurred1.png");

    t2.std_convolve_cyclic(0, 3, 3, ker, 1, 1);
    t2.std_convolve_cyclic(1, 3, 3, ker, 1, 1);
    t2.std_convolve_cyclic(2, 3, 3, ker, 1, 1);

    t2.write("blurred2.png");

    Image diff = t0;
    diff.diffmap_scale(t2);
    diff.write("diff_blurred.png");

    return 0;
}