//
//  parse.hpp
//  MSDScript
//
//  Created by Nga Huynh on 2/14/21.
//

#ifndef parse_hpp
#define parse_hpp

#include "expr.h"
#include "pointer.h"
#include <stdio.h>

PTR(Expr) parse_str(std::string s);
PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_addend(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_true(std::istream &in);
PTR(Expr) parse_false(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_eq(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);
std::string parse_keyword(std::istream &in);
static void consume(std::istream &in, int expect); 
static void skip_whitespace(std::istream &in);

#endif /* parse_hpp */
