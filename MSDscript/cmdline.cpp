//
//  cmdline.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/19/21.
//

#include "cmdline.h"
void use_arguments(int argc, char * arg[]) {
    if (argc == 1) {
        return;
    }
    else if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (arg[i] == (std::string)"--help") {
                std::cout<<"Only 'help' and 'test' arguments are allowed to use\n";
                exit(0);
            }
            if (arg[i] == (std::string)"--test") {
                if (i == 1) {
                    std::cout<<"Test passed\n";
                }
                else {
                    for (int j = 1; j < i; j++) {
                        if (arg[j] == (std::string)"--test") {
                            std::cerr<<"Error: Test argument has been seen already\n";
                            exit(1);
                        }
                        else {
                            std::cout<<"Test passed\n";
                        }
                    }
                }
            }
            else {
                std::cerr<<"Error: The argument '" << arg[i] << "' is not allowed\n";
                exit(1);
            }
        }
    }
}
