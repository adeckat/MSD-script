//
//  parse.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/14/21.
//

#include "parse.h"
#include <cassert>

//Input string to parse
PTR(Expr) parse_str(std::string s){
    std::istringstream str(s);
    return parse_expr(str);
}

//Parse an expr
PTR(Expr) parse_expr(std::istream &in) {
    PTR(Expr) e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        c = in.peek();
        if (c == '=') {
            consume(in, '=');
            PTR(Expr) rhs = parse_expr(in);
            return NEW(EqExpr)(e, rhs);
        }
        else {
            throw std::runtime_error("Missing the second equal sign");
        }
    } else
        return e;
}

//Parse a comparg
PTR(Expr) parse_comparg(std::istream &in) {
    PTR(Expr) e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    } else
        return e;
}

//Parse an addend
PTR(Expr) parse_addend(std::istream &in) {
    PTR(Expr) e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    } else
        return e;
}

//Parse a multicand
PTR(Expr) parse_multicand(std::istream &in) {
    PTR(Expr) e;
    e = parse_inner(in);
    skip_whitespace(in);
    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        e = NEW(CallExpr)(e, actual_arg);
    }
    return e;
}

//Parse an inner
PTR(Expr) parse_inner(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("Missing close parenthesis");
        return e;
    }
    else if (isalpha(c)){
        return parse_var(in);
    }
    else if (c == '_') {
        consume(in, '_');
        std::string kw = parse_keyword(in);
        if (kw == "let") {
            return parse_let(in);
        }
        else if (kw == "true") {
            return parse_true(in);
        }
        else if (kw == "false") {
            return parse_false(in);
        }
        else if (kw == "if") {
            return parse_if(in);
        }
        else if (kw == "fun") {
            return parse_fun(in);
        }
        else {
            throw std::runtime_error("E1 invalid input");
        }
    }
    else {
        consume(in, c);
        throw std::runtime_error("E2 invalid input");
    }
}

//Parse a num expr
PTR(Expr) parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
        }
        else if (c == '-' || isalpha(c)) {
            throw std::runtime_error("Invalid input");
        }
        else {
            break;
        }
    }
    if (negative) {
        n = -n;
    }
    return NEW(NumExpr)(n);
}

//Parse a var epxr
PTR(Expr) parse_var(std::istream &in) {
    int c;
    std::string str;
    while (1) {
        c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            str += c;
        }
        else {
            break;
        }
    }
    return NEW(VarExpr)(str);
}

//Parse a _let expr
PTR(Expr) parse_let(std::istream &in) {
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;
    std::string kw, var;
    skip_whitespace(in);
    lhs = parse_var(in);
    PTR(VarExpr) other_lhs = CAST(VarExpr)(lhs);
    var = other_lhs->getStr();
    skip_whitespace(in);
    consume(in, '=');
    rhs = parse_expr(in);
    skip_whitespace(in);
    consume(in, '_');
    kw = parse_keyword(in);
    if (kw != "in")
        throw std::runtime_error("E4 invalid input");
    body = parse_expr(in);
    return NEW(LetExpr)(var, rhs, body);
}

//Parse an _if expr
PTR(Expr) parse_if(std::istream &in) {
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    std::string kw;
    test_part = parse_expr(in);
    skip_whitespace(in);
    consume(in, '_');
    kw = parse_keyword(in);
    if (kw != "then")
        throw std::runtime_error("E6 invalid input");
    then_part = parse_expr(in);
    skip_whitespace(in);
    consume(in, '_');
    kw = parse_keyword(in);
    if (kw != "else")
        throw std::runtime_error("E7 invalid input");
    else_part = parse_expr(in);
    return NEW(IfExpr)(test_part, then_part, else_part);
}

//Parse a _fun expr
PTR(Expr) parse_fun(std::istream &in) {
    PTR(Expr) lhs;
    PTR(Expr) body;
    std::string var;
    skip_whitespace(in);
    consume(in, '(');
    skip_whitespace(in);
    lhs = parse_var(in);
    PTR(VarExpr) other_lhs = CAST(VarExpr)(lhs);
    var = other_lhs->getStr();
    skip_whitespace(in);
    consume(in, ')');
    body = parse_expr(in);
    return NEW(FunExpr)(var, body);
}

//Parse a _true epxr
PTR(Expr) parse_true(std::istream &in) {
    return NEW(BoolExpr)(true);
}

//Parse a _false expr
PTR(Expr) parse_false(std::istream &in) {
    return NEW(BoolExpr)(false);
}

//Parse keywords
std::string parse_keyword(std::istream &in) {
    PTR(Expr) kw = parse_var(in);
    PTR(VarExpr) other_e = CAST(VarExpr)(kw);
    std::string var = other_e->getStr();
    if (var == "let" || var == "in") {
        return var;
    }
    else if (var == "true" || var == "false") {
        return var;
    }
    else if (var == "if" || var == "then" || var == "else") {
        return var;
    }
    else if (var == "fun") {
        return var;
    }
    else {
        throw std::runtime_error("Invalid keyword");
    }
}

static void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error("Consume mismatch");
    }
}

static void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}
