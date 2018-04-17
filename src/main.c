#include <math.h>
#include <string.h>
#include "image.h"
#include "test.h"
#include "args.h"

int main(int argc, char **argv)
{
    char *in = find_char_arg(argc, argv, "-i", "data/dog.jpg");
    char *out = find_char_arg(argc, argv, "-o", "out");
    //float scale = find_float_arg(argc, argv, "-s", 1);
    if(argc < 2){
        printf("usage: %s [test | grayscale]\n", argv[0]);  
    } else if (0 == strcmp(argv[1], "test")){
        run_tests();
    } else if (0 == strcmp(argv[1], "grayscale")){
        image im = load_image(in);
        image g = rgb_to_grayscale(im);
        save_image(g, out);
        free_image(im);
        free_image(g);
    }
    return 0;
}
