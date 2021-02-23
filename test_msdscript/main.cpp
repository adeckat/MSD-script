//
//  main.cpp
//  test_msdscript
//
//  Created by Nga Huynh on 2/20/21.
//

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "cmdline.h"

int main(int argc, char * argv[]) {
    try {
        use_arguments(argc, argv);
        return 0;
    } catch (std::runtime_error exn) {
        std::cerr << exn.what() << "\n";
        return 1;
    }
}
