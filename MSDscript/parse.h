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
Expr *parse_expr(std::istream &in);
Expr *parse_comparg(std::istream &in);
Expr *parse_addend(std::istream &in);
Expr *parse_multicand(std::istream &in);
Expr *parse_num(std::istream &in);
Expr *parse_var(std::istream &in);
Expr *parse_let(std::istream &in);
Expr *parse_true(std::istream &in);
Expr *parse_false(std::istream &in);
Expr *parse_if(std::istream &in);
Expr *parse_eq(std::istream &in);
std::string parse_keyword(std::istream &in);
static void consume(std::istream &in, int expect); 
static void skip_whitespace(std::istream &in);

#endif /* parse_hpp */
