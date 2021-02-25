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
#include "val.h"


void use_arguments(int argc, char * argv[]) {
    if (argc == 1) {
        return;
    }
    else if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            if (argv[i] == (std::string)"--help") {
                std::cout << "These arguments are allowed to use:\n";
                std::cout << "\t--help: list of available argument\n";
                std::cout << "\t--test: run tests\n";
                std::cout << "\t--interp: run interp function to profuce a result of the standard input expression\n";
                std::cout << "\t--print: print the standard input expression in a regular way\n";
                std::cout << "\t--pretty-print: print the standard input expression in a pretty way\n";
                exit(0);
            }
            if (argv[i] == (std::string)"--test") {
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
            if (argv[i] == (std::string)"--interp") {
                Expr* n = parse(std::cin);
                std::cout << n->interp()->to_expr()->to_str();
                std::cout << "\n";
                exit(0);
            }
            if (argv[i] == (std::string)"--print") {
                Expr* n = parse(std::cin);
                n->print(std::cout);
                std::cout << "\n";
                exit(0);
            }
            if (argv[i] == (std::string)"--pretty-print") {
                Expr* n = parse(std::cin);
                std::cout << n->to_pretty_str();
                std::cout << "\n";
                exit(0);
            }
            else {
                std::cerr<<"Error: The argument '" << argv[i] << "' is not allowed\n";
                exit(1);
            }
        }
    }
}
