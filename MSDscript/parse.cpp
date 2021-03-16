//
//  parse.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/14/21.
//

#include "parse.h"
#include <cassert>

//Input string to parse
Expr *parse_str(std::string s){
    std::istringstream str(s);
    return parse_expr(str);
}

//Parse an expr
Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        c = in.peek();
        if (c == '=') {
            consume(in, '=');
            Expr *rhs = parse_expr(in);
            return new EqExpr(e, rhs);
        }
        else {
            throw std::runtime_error("Missing the second equal sign");
        }
    } else
        return e;
}

//Parse a comparg
Expr *parse_comparg(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    } else
        return e;
}

//Parse an addend
Expr *parse_addend(std::istream &in) {
    Expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new MultExpr(e, rhs);
    } else
        return e;
}

//Parse a multicand
Expr *parse_multicand(std::istream &in) {
    Expr *e;
    e = parse_inner(in);
    skip_whitespace(in);
    while (in.peek() == '(') {
        consume(in, '(');
        Expr *actual_arg = parse_expr(in);
        consume(in, ')');
        e = new CallExpr(e, actual_arg);
    }
    return e;
}

//Parse an inner
Expr *parse_inner(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
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
Expr *parse_num(std::istream &in) {
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
    return new NumExpr(n);
}

//Parse a var epxr
Expr *parse_var(std::istream &in) {
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
    return new VarExpr(str);
}

//Parse a _let expr
Expr *parse_let(std::istream &in) {
    Expr *lhs, *rhs, *body;
    std::string kw, var;
    skip_whitespace(in);
    lhs = parse_var(in);
    VarExpr *other_lhs = dynamic_cast<VarExpr*>(lhs);
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
    return new LetExpr(var, rhs, body); 
}

//Parse an _if expr
Expr *parse_if(std::istream &in) {
    Expr *test_part, *then_part, *else_part;
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
    return new IfExpr(test_part, then_part, else_part);
}

//Parse a _fun expr
Expr *parse_fun(std::istream &in) {
    Expr *lhs, *body;
    std::string var;
    skip_whitespace(in);
    consume(in, '(');
    skip_whitespace(in);
    lhs = parse_var(in);
    VarExpr *other_lhs = dynamic_cast<VarExpr*>(lhs);
    var = other_lhs->getStr();
    skip_whitespace(in);
    consume(in, ')');
    body = parse_expr(in);
    return new FunExpr(var, body);
}

//Parse a _true epxr
Expr *parse_true(std::istream &in) {
    return new BoolExpr(true);
}

//Parse a _false expr
Expr *parse_false(std::istream &in) {
    return new BoolExpr(false);
}

//Parse keywords
std::string parse_keyword(std::istream &in) {
    Expr *kw = parse_var(in);
    VarExpr *other_e = dynamic_cast<VarExpr*>(kw);
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
