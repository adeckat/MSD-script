//
//  main.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/19/21.
//

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

