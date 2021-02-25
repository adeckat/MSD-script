//
//  test_generator.cpp
//  test_msdscript
//
//  Created by Nga Huynh on 2/22/21.
//

#include <time.h>
#include <stdlib.h>
#include "test_generator.h"

//Create random string to test
std::string random_string() {
    if ((rand() % 10) < 5) {
        return random_expr_string();
    }
    else if ((rand() % 10) >= 5 && (rand() % 10) < 7.5) {
        return random_var_string();
    }
    else {
        return random_let_string();
    }
}

//Create random num, character, Add, and Mult by posibility
std::string random_expr_string() {
    if ((rand() % 10) < 6) {
        return random_space() + random_sign() + std::to_string(rand());
    }
    else if ((rand() % 10) == 6) {
        return random_space() + random_char();
    }
    else if (((rand() % 10) >= 7) && ((rand() % 10) < 8.5)){
        return random_parentheses(random_expr_string()) + random_space() + " + " + random_space() + random_parentheses(random_expr_string());
    }
    else {
        return random_parentheses(random_expr_string()) + random_space() + " * " + random_space() + random_parentheses(random_expr_string());
    }
}

//Create random string
std::string random_var_string() {
    int length = rand() % 10 + 1;
    std::string str = "";
    for (int i = 0; i < length; i++) {
        str += random_char();
    }
    return str;
}

//Create random let
std::string random_let_string() {
    std::string str = "_let " + random_char() + " = " + random_expr_string() + " _in " + random_expr_string();
    return str;
}

//Create random character
std::string random_char() {
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string character = "";
    character += letters[rand() % 52];
    return character;
}

//Create random space
std::string random_space() {
    int space = rand() % 5;
    return std::string(space, ' ');
}

//Create random negative sign
std::string random_sign() {
    int sign = rand() % 10;
    if (sign < 3) {
        return "-";
    }
    else {
        return "";
    }
}

//Create random parentheses
std::string random_parentheses (std::string in) {
    int prt = rand() % 10;
    if (prt < 2) {
        return "(" + in + ")";
    }
    else {
        return in;
    }
}
