#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "matrix.h"
#include "image.h"
#include "test.h"
#include "args.h"

void feature_normalize2(image im)
{
    int i;
    float min = im.data[0];
    float max = im.data[0];
    for(i = 0; i < im.w*im.h*im.c; ++i){
        if(im.data[i] > max) max = im.data[i];
        if(im.data[i] < min) min = im.data[i];
    }
    for(i = 0; i < im.w*im.h*im.c; ++i){
        im.data[i] = (im.data[i] - min)/(max-min);
    }
}

int tests_total = 0;
int tests_fail = 0;

int within_eps(float a, float b){
    return a-EPS<b && b<a+EPS;
}

int same_image(image a, image b){
    int i;
    if(a.w != b.w || a.h != b.h || a.c != b.c) {
        printf("Expected %d x %d x %d image, got %d x %d x %d\n", b.w, b.h, b.c, a.w, a.h, a.c);
        return 0;
    }
    for(i = 0; i < a.w*a.h*a.c; ++i){
        if(!within_eps(a.data[i], b.data[i])) 
        {
            printf("The value should be %f, but it is %f! \n", b.data[i], a.data[i]);
            return 0;
        }
    }
    return 1;
}

void test_get_pixel(){
    image im = load_image("data/dots.png");
    // Test within image
    TEST(within_eps(0, get_pixel(im, 0,0,0)));
    TEST(within_eps(1, get_pixel(im, 1,0,1)));
    TEST(within_eps(0, get_pixel(im, 2,0,1)));

    // Test padding
    TEST(within_eps(1, get_pixel(im, 0,3,1)));
    TEST(within_eps(1, get_pixel(im, 7,8,0)));
    TEST(within_eps(0, get_pixel(im, 7,8,1)));
    TEST(within_eps(1, get_pixel(im, 7,8,2)));
    free_image(im);
}

void test_set_pixel(){
    image gt = load_image("data/dots.png");
    image d = make_image(4,2,3);
    set_pixel(d, 0,0,0,0); set_pixel(d, 0,0,1,0); set_pixel(d, 0,0,2,0); 
    set_pixel(d, 1,0,0,1); set_pixel(d, 1,0,1,1); set_pixel(d, 1,0,2,1); 
    set_pixel(d, 2,0,0,1); set_pixel(d, 2,0,1,0); set_pixel(d, 2,0,2,0); 
    set_pixel(d, 3,0,0,1); set_pixel(d, 3,0,1,1); set_pixel(d, 3,0,2,0); 

    set_pixel(d, 0,1,0,0); set_pixel(d, 0,1,1,1); set_pixel(d, 0,1,2,0); 
    set_pixel(d, 1,1,0,0); set_pixel(d, 1,1,1,1); set_pixel(d, 1,1,2,1); 
    set_pixel(d, 2,1,0,0); set_pixel(d, 2,1,1,0); set_pixel(d, 2,1,2,1); 
    set_pixel(d, 3,1,0,1); set_pixel(d, 3,1,1,0); set_pixel(d, 3,1,2,1); 

    // Test images are same
    TEST(same_image(d, gt));
    free_image(gt);
    free_image(d);
}

void test_grayscale()
{
    image im = load_image("data/colorbar.png");
    image gray = rgb_to_grayscale(im);
    image gt = load_image("figs/gray.png");
    TEST(same_image(gray, gt));
    free_image(im);
    free_image(gray);
    free_image(gt);
}

void test_copy()
{
    image gt = load_image("data/dog.jpg");
    image c = copy_image(gt);
    TEST(same_image(c, gt));
    free_image(gt);
    free_image(c);
}

void test_shift()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    shift_image(c, 1, .1);
    TEST(within_eps(c.data[0], im.data[0]));
    TEST(within_eps(c.data[im.w*im.h + 13], im.data[im.w*im.h+13] + .1));
    TEST(within_eps(c.data[2*im.w*im.h + 72], im.data[2*im.w*im.h+72]));
    TEST(within_eps(c.data[im.w*im.h + 47], im.data[im.w*im.h+47] + .1));
    free_image(im);
    free_image(c);
}

void test_rgb_to_hsv()
{
    image im = load_image("data/dog.jpg");
    rgb_to_hsv(im);
    image hsv = load_image("figs/dog.hsv.png");
    TEST(same_image(im, hsv));
    free_image(im);
    free_image(hsv);
}

void test_hsv_to_rgb()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    rgb_to_hsv(im);
    hsv_to_rgb(im);
    TEST(same_image(im, c));
    free_image(im);
    free_image(c);
}

void test_nn_resize()
{
    image im = load_image("data/dogsmall.jpg");
    image resized = nn_resize(im, im.w*4, im.h*4);
    image gt = load_image("figs/dog4x-nn-for-test.png");
    TEST(same_image(resized, gt));
    free_image(im);
    free_image(resized);
    free_image(gt);

    image im2 = load_image("data/dog.jpg");
    image resized2 = nn_resize(im2, 713, 467);
    image gt2 = load_image("figs/dog-resize-nn.png");
    TEST(same_image(resized2, gt2));
    free_image(im2);
    free_image(resized2);
    free_image(gt2);
}

void test_bl_resize()
{
    image im = load_image("data/dogsmall.jpg");
    image resized = bilinear_resize(im, im.w*4, im.h*4);
    image gt = load_image("figs/dog4x-bl.png");
    TEST(same_image(resized, gt));
    free_image(im);
    free_image(resized);
    free_image(gt);

    image im2 = load_image("data/dog.jpg");
    image resized2 = bilinear_resize(im2, 713, 467);
    image gt2 = load_image("figs/dog-resize-bil.png");
    TEST(same_image(resized2, gt2));
    free_image(im2);
    free_image(resized2);
    free_image(gt2);
}

void test_multiple_resize()
{
    image im = load_image("data/dog.jpg");
    int i;
    for (i = 0; i < 10; i++){
        image im1 = bilinear_resize(im, im.w*4, im.h*4);
        image im2 = bilinear_resize(im1, im1.w/4, im1.h/4);
        free_image(im);
        free_image(im1);
        im = im2;
    }
    image gt = load_image("figs/dog-multipleresize.png");
    TEST(same_image(im, gt));
    free_image(im);
    free_image(gt);
}


void test_highpass_filter(){
    image im = load_image("data/dog.jpg");
    image f = make_highpass_filter();
    image blur = convolve_image(im, f, 0);
    clamp_image(blur);

    
    image gt = load_image("figs/dog-highpass.png");
    TEST(same_image(blur, gt));
    free_image(im);
    free_image(f);
    free_image(blur);
    free_image(gt);
}

void test_emboss_filter(){
    image im = load_image("data/dog.jpg");
    image f = make_emboss_filter();
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    
    image gt = load_image("figs/dog-emboss.png");
    TEST(same_image(blur, gt));
    free_image(im);
    free_image(f);
    free_image(blur);
    free_image(gt);
}

void test_sharpen_filter(){
    image im = load_image("data/dog.jpg");
    image f = make_sharpen_filter();
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);


    image gt = load_image("figs/dog-sharpen.png");
    TEST(same_image(blur, gt));
    free_image(im);
    free_image(f);
    free_image(blur);
    free_image(gt);
}

void test_convolution(){
    image im = load_image("data/dog.jpg");
    image f = make_box_filter(7);
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    image gt = load_image("figs/dog-box7.png");
    TEST(same_image(blur, gt));
    free_image(im);
    free_image(f);
    free_image(blur);
    free_image(gt);
}

void test_gaussian_filter(){
    image f = make_gaussian_filter(7);
    int i;

    for(i = 0; i < f.w * f.h * f.c; i++){
        f.data[i] *= 100;
    }

    image gt = load_image("figs/gaussian_filter_7.png");
    TEST(same_image(f, gt));    
    free_image(f);
    free_image(gt);
}

void test_gaussian_blur(){
    image im = load_image("data/dog.jpg");
    image f = make_gaussian_filter(2);
    image blur = convolve_image(im, f, 1);
    clamp_image(blur);

    image gt = load_image("figs/dog-gauss2.png");
    TEST(same_image(blur, gt));    
    free_image(im);
    free_image(f);
    free_image(blur);
    free_image(gt);
}

void test_hybrid_image(){
    image man = load_image("data/melisa.png");
    image woman = load_image("data/aria.png");
    image f = make_gaussian_filter(2);
    image lfreq_man = convolve_image(man, f, 1);
    image lfreq_w = convolve_image(woman, f, 1);
    image hfreq_w = sub_image(woman , lfreq_w);
    image reconstruct = add_image(lfreq_man , hfreq_w);
    image gt = load_image("figs/hybrid.png");
    clamp_image(reconstruct);
    TEST(same_image(reconstruct, gt));
    free_image(man);
    free_image(woman);
    free_image(f);
    free_image(lfreq_man);
    free_image(lfreq_w);
    free_image(hfreq_w);
    free_image(reconstruct);
    free_image(gt);
}

void test_frequency_image(){
    image im = load_image("data/dog.jpg");
    image f = make_gaussian_filter(2);
    image lfreq = convolve_image(im, f, 1);
    image hfreq = sub_image(im, lfreq);
    image reconstruct = add_image(lfreq , hfreq);

    image low_freq = load_image("figs/low-frequency.png");
    image high_freq = load_image("figs/high-frequency-clamp.png");

    clamp_image(lfreq);
    clamp_image(hfreq);
    TEST(same_image(lfreq, low_freq));
    TEST(same_image(hfreq, high_freq));
    TEST(same_image(reconstruct, im));
    free_image(im);
    free_image(f);
    free_image(lfreq);
    free_image(hfreq);
    free_image(reconstruct);
    free_image(low_freq);
    free_image(high_freq);
}

void test_sobel(){
    image im = load_image("data/dog.jpg");
    image *res = sobel_image(im);
    image mag = res[0];
    image theta = res[1];
    feature_normalize2(mag);
    feature_normalize2(theta);

    image gt_mag = load_image("figs/magnitude.png");
    image gt_theta = load_image("figs/theta.png");
    TEST(gt_mag.w == mag.w && gt_theta.w == theta.w);
    TEST(gt_mag.h == mag.h && gt_theta.h == theta.h);
    TEST(gt_mag.c == mag.c && gt_theta.c == theta.c);
    if( gt_mag.w != mag.w || gt_theta.w != theta.w || 
        gt_mag.h != mag.h || gt_theta.h != theta.h || 
        gt_mag.c != mag.c || gt_theta.c != theta.c ) return;
    int i;
    for(i = 0; i < gt_mag.w*gt_mag.h; ++i){
        if(within_eps(gt_mag.data[i], 0)){
            gt_theta.data[i] = 0;
            theta.data[i] = 0;
        }
        if(within_eps(gt_theta.data[i], 0) || within_eps(gt_theta.data[i], 1)){
            gt_theta.data[i] = 0;
            theta.data[i] = 0;
        }
    }

    TEST(same_image(mag, gt_mag));
    TEST(same_image(theta, gt_theta));
    free_image(im);
    free_image(mag);
    free_image(theta);
    free_image(gt_mag);
    free_image(gt_theta);
    free(res);
}

void test_structure()
{
    image im = load_image("data/dogbw.png");
    image s = structure_matrix(im, 2);
    feature_normalize2(s);
    image gt = load_image("figs/structure.png");
    TEST(same_image(s, gt));
    free_image(im);
    free_image(s);
    free_image(gt);
}

void test_cornerness()
{
    image im = load_image("data/dogbw.png");
    image s = structure_matrix(im, 2);
    image c = cornerness_response(s);
    feature_normalize2(c);
    image gt = load_image("figs/response.png");
    TEST(same_image(c, gt));
    free_image(im);
    free_image(s);
    free_image(c);
    free_image(gt);
}

void run_tests()
{
    //test_matrix();
    test_get_pixel();
    test_set_pixel();
    test_copy();
    test_shift();
    test_grayscale();
    test_rgb_to_hsv();
    test_hsv_to_rgb();
    test_nn_resize();
    test_bl_resize();
    test_multiple_resize();
    test_gaussian_filter();
    test_sharpen_filter();
    test_emboss_filter();
    test_highpass_filter();
    test_convolution();
    test_gaussian_blur();
    test_hybrid_image();
    test_frequency_image();
    test_sobel();
    test_structure();
    test_cornerness();
    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}

