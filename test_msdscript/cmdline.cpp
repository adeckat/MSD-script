//
//  cmdline.cpp
//  test_msdscript
//
//  Created by Nga Huynh on 2/23/21.
//

#include "cmdline.h"
#include "test_generator.h"
#include "exec.h"
void use_arguments(int argc, char * argv[]) {
    srand((unsigned int)clock());
   
    //Check if there is any path to program
    if (argc == 1) {
        std::cout << "Please insert path to program to test\n";
        return;
    }
    else if (argc == 2) {
        //TEST 1 program
        const char * const interp_argv[] = {argv[1], "--interp"};
        const char * const print_argv[] = {argv[1], "--print"};
        const char * const pretty_print_argv[] = {argv[1], "--pretty-print"};
        
        for (int i = 0; i < 100; i++) {
            std::string in = random_string();
            std::cout << "Trying " << in << "\n"; 
            
            ExecResult interp_result = exec_program(2, interp_argv, in);
            ExecResult print_result = exec_program(2, print_argv, in);  
            ExecResult pretty_print_result = exec_program(2, pretty_print_argv, in);

            ExecResult interp_again1_result = exec_program(2, interp_argv, print_result.out);
            ExecResult interp_again2_result = exec_program(2, interp_argv, pretty_print_result.out);
            
            if (print_result.exit_code == 1 || pretty_print_result.exit_code == 1)
                throw std::runtime_error("Non-zero exit 1");
            if (interp_result.exit_code == 0)
                std::cout << "\tinterp okay\n";
            if (interp_again1_result.out != interp_result.out || interp_again2_result.out != interp_result.out) //
                throw std::runtime_error("Different results");
        }
        exit(0);
    }
    else if (argc == 3) {
        //COMPARE 2 programs
        const char * const interp1_argv[] = {argv[1], "--interp"};
        const char * const print1_argv[] = {argv[1], "--print"};
        const char * const pretty_print1_argv[] = {argv[1], "--pretty-print"};
        
        const char * const interp2_argv[] = {argv[2], "--interp"};
        const char * const print2_argv[] = {argv[2], "--print"};
        const char * const pretty_print2_argv[] = {argv[2], "--pretty-print"};
        
        for (int i = 0; i < 100; i++) {
            std::string in = random_string();
            std::cout << "Trying " << in << "\n";
            
            ExecResult interp1_result = exec_program(2, interp1_argv, in);
            ExecResult print1_result = exec_program(2, print1_argv, in);
            ExecResult pretty_print1_result = exec_program(2, pretty_print1_argv, in);
            
            ExecResult interp2_result = exec_program(2, interp2_argv, in);
            ExecResult print2_result = exec_program(2, print2_argv, in);
            ExecResult pretty_print2_result = exec_program(2, pretty_print2_argv, in);

            if (print1_result.exit_code == 1 || print2_result.exit_code == 1 ||
                pretty_print1_result.exit_code == 1 || pretty_print2_result.exit_code == 1)
                throw std::runtime_error("Non-zero exit 4");
            if (interp1_result.exit_code == 0 && interp1_result.exit_code == 0)
                std::cout << "\tinterp okay\n";
//            std::cout << "Mine: " << interp1_result.out << std::endl;
//            std::cout << "Yours: " << interp2_result.out << std::endl;
            if (interp1_result.out != interp2_result.out)
                throw std::runtime_error("Different interp results");
//            std::cout << "Mine: " << print1_result.out << std::endl;
//            std::cout << "Yours: " <<  print2_result.out << std::endl;
            if (print1_result.out != print2_result.out)
                throw std::runtime_error("Different print results");
//            std::cout << "Mine: " << pretty_print1_result.out << std::endl;
//            std::cout << "Yours: " <<  pretty_print2_result.out << std::endl;
            if (pretty_print1_result.out != pretty_print2_result.out)
                throw std::runtime_error("Different pretty print results");
        }
        exit(0);
    }
    else {
        std::cerr << "Error: Only 2 programs allowed\n";
        exit(1);
    }
}
