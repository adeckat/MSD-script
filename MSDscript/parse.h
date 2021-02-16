//
//  parse.hpp
//  MSDScript
//
//  Created by Nga Huynh on 2/14/21.
//

#ifndef parse_hpp
#define parse_hpp

#include "expr.h"
#include <stdio.h>

Expr *parse_str(std::string s);
Expr *parse(std::istream &in);
Expr *parse_num(std::istream &in);
Expr *parse_var(std::istream &in);
Expr *parse_let(std::istream &in);
std::string parse_keyword(std::istream &in);
Expr *parse_multicand(std::istream &in);
Expr *parse_addend(std::istream &in);
static void consume(std::istream &in, int expect); 
static void skip_whitespace(std::istream &in);

#endif /* parse_hpp */
