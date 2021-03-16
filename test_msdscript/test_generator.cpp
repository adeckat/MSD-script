//
//  test_generator.cpp
//  test_msdscript
//
//  Created by Nga Huynh on 2/22/21.
//

#include <time.h>
#include <stdlib.h>
#include "test_generator.h"

//Create random num, character, Add, and Mult by posibility
std::string random_expr_string() {
    int chance = rand() % 100;
    if (chance < 40) {
        return random_parentheses(random_space() + random_sign() + std::to_string(rand()));
    }
    else if (chance >= 40 && chance < 46) {
        return random_parentheses(random_space() + random_char());
    }
    else if (chance >= 46 && chance < 52) {
        return random_parentheses(random_space() + random_var_string());
    }
    else if (chance >= 52 && chance < 58){
        return random_parentheses(random_space() + random_expr_string()) + random_space() + " + " + random_space() + random_parentheses(random_expr_string());
    }
    else if (chance >= 58 && chance < 64) {
        return random_parentheses(random_space() + random_expr_string()) + random_space() + " * " + random_space() + random_parentheses(random_expr_string());
    }
    else if (chance >= 64 && chance < 70) {
        return random_parentheses(random_parentheses(random_expr_string()) + "(" + random_space() + random_expr_string() + random_space() + ")");
    }
    else if (chance >= 70 && chance < 76) {
        return random_parentheses(random_bool());
    }
    else if (chance >= 76 && chance < 82) {
        return random_parentheses(random_let_string());
    }
    else if (chance >= 82 && chance < 88) {
        return random_parentheses(random_eq_string());
    }
    else if (chance >= 88 && chance < 94) {
        return random_parentheses(random_if_string());
    }
    else {
        return random_parentheses(random_fun_string());
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
    std::string str = "_let " + random_space() + random_char() + random_space() + " = " + random_space() + random_expr_string() + random_space() + " _in " + random_space() + random_expr_string();
    return str;
}

//Create random equation
std::string random_eq_string() {
    std::string str = random_space() + random_expr_string() + random_space() + " == " + random_space() + random_expr_string();
    return str;
}

//Create random if
std::string random_if_string() {
    std::string str = "_if " + random_space() + random_expr_string() + random_space() + "_then " + random_space() + random_expr_string() + random_space() + " _else " + random_space() + random_expr_string();
    return str;
}

//Create random function
std::string random_fun_string() {
    std::string str = "_fun " + random_space() + "(" + random_space() + random_char() + random_space() + ")" + random_space() + random_expr_string();
    return str;
}

//Create random character
std::string random_char() {
    char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string character = "";
    character += letters[rand() % 52];
    return character;
}

//Create random boolean
std::string random_bool() {
    int boolean = rand() % 10;
    if (boolean < 5) {
        return "_true";
    }
    else {
        return "_false";
    }
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
