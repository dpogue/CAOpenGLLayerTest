#include <stdio.h>
#include "PGLLayer.hpp"

int main() {
    Class cls = objc_lookUpClass("PGLLayer");
    if (cls != nil) {
        printf("Found the class\n");
    }

    PGL::Layer* layer = PGL::Layer::layer();
    if (layer != nullptr) {
        printf("Created a layer\n");
    }

    return 0;
}
