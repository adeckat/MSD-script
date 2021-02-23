//
//  test_generator.hpp
//  test_msdscript
//
//  Created by Nga Huynh on 2/22/21.
//

#ifndef test_generator_hpp
#define test_generator_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

std::string random_string();
std::string random_expr_string();
std::string random_var_string();
std::string random_let_string();
std::string random_char();
std::string random_space();
std::string random_sign();
std::string random_parentheses(std::string in);
#endif /* test_generator_hpp */
