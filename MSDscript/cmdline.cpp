//
//  cmdline.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/19/21.
//
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "cmdline.h"
#include "parse.h"

void use_arguments(int argc, char * arg[]) {
    if (argc == 1) {
        return;
    }
    else if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (arg[i] == (std::string)"--help") {
                std::cout << "These arguments are allowed to use: --help, --test, --interp, --print, and --pretty_print\n";
                exit(0);
            }
            if (arg[i] == (std::string)"--test") {
                if (i == 1) {
                    int result = Catch::Session().run();
                    if (result == 0) {
                        exit(0);
                    }
                    else {
                        exit(1);
                    }
                }
                else {
                    std::cerr << "Error: Test argument has been seen already\n";
                    exit(1);
                }
            }
            if (arg[i] == (std::string)"--interp") {
                Expr* n = parse(std::cin);
                std::cout << n->interp() << " ";
                std::cout << "\n";
                exit(0);
            }
            if (arg[i] == (std::string)"--print") {
                Expr* n = parse(std::cin);
                n->print(std::cout);
                std::cout << "\n";
                exit(0);
            }
            if (arg[i] == (std::string)"--pretty_print") {
                Expr* n = parse(std::cin);
                std::cout << n->to_pretty_str();
                std::cout << "\n";
                exit(0);
            }
            else {
                std::cerr<<"Error: The argument '" << arg[i] << "' is not allowed\n";
                exit(1);
            }
        }
    }
}
