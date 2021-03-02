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

Expr *parse_multicand(std::istream &in) {
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
        c = in.peek();
        if (c == 'l') {
            return parse_let(in);
        }
        else if (c == 't') {
            return parse_true(in);
        }
        else if (c =='f') {
            return parse_false(in);
        }
        else if (c =='i') {
            return parse_if(in);
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

Expr *parse_let(std::istream &in) {
    Expr *rhs, *body;
    std::string kw, var;
    kw = parse_let_keyword(in);
    if (kw == "let") {
        skip_whitespace(in);
        Expr *e = parse_var(in);
        VarExpr *other_e = dynamic_cast<VarExpr*>(e);
        var = other_e->getStr();
        skip_whitespace(in);
        if (in.peek() == '=') {
            consume(in, '=');
            skip_whitespace(in);
            rhs = parse_expr(in);
            skip_whitespace(in);
            if (in.peek() == '_') {
                consume(in, '_');
                kw = parse_let_keyword(in);
                if (kw == "in") {
                    skip_whitespace(in);
                    body = parse_expr(in);
                } else {
                    throw std::runtime_error("E3 invalid input");
                }
            } else {
                throw std::runtime_error("E4 invalid input");
            }
        } else {
            throw std::runtime_error("E5 invalid input");
        }
        return new LetExpr(var, rhs, body);
    }
    else {
        throw std::runtime_error("E6 invalid input");
    }
}

Expr *parse_if(std::istream &in) {
    Expr *test_part, *then_part, *else_part;
    std::string kw;
    kw = parse_if_keyword(in);
    if (kw == "if") {
        skip_whitespace(in);
        test_part = parse_expr(in);
        skip_whitespace(in);
        if (in.peek() == '_') {
            consume(in, '_');
            kw = parse_if_keyword(in);
            if (kw == "then") {
                skip_whitespace(in);
                then_part = parse_expr(in);
                skip_whitespace(in);
                if (in.peek() == '_') {
                    consume(in, '_');
                    kw = parse_if_keyword(in);
                    if (kw == "else") {
                        skip_whitespace(in);
                        else_part = parse_expr(in);
                    } else {
                        throw std::runtime_error("E7 invalid input");
                    }
                } else {
                    throw std::runtime_error("E8 invalid input");
                }
            } else {
                throw std::runtime_error("E9 invalid input");
            }
        }
        else {
            throw std::runtime_error("E10 invalid input");
        }
        return new IfExpr(test_part, then_part, else_part);
    }
    else {
        throw std::runtime_error("E11 invalid input");
    }
}

Expr *parse_true(std::istream &in) {
    std::string kw = parse_bool_keyword(in);
    if (kw == "true") {
        return new BoolExpr(true);
    } else {
        throw std::runtime_error("E12 invalid input");
    }
}

Expr *parse_false(std::istream &in) {
    std::string kw = parse_bool_keyword(in);
    if (kw == "false") {
        return new BoolExpr(false);
    } else {
        throw std::runtime_error("E13 invalid input");
    }
}

std::string parse_let_keyword(std::istream &in) {
    Expr *kw = parse_var(in);
    VarExpr *other_e = dynamic_cast<VarExpr*>(kw);
    std::string var = other_e->getStr();
    if (var == "let" || var == "in") {
        return var;
    }
    else {
        throw std::runtime_error("E1 invalid keyword");
    }
}

std::string parse_bool_keyword(std::istream &in) {
    Expr *kw = parse_var(in);
    VarExpr *other_e = dynamic_cast<VarExpr*>(kw);
    std::string var = other_e->getStr();
    if (var == "true" || var == "false") {
        return var;
    }
    else {
        throw std::runtime_error("E2 invalid keyword");
    }
}

std::string parse_if_keyword(std::istream &in) {
    Expr *kw = parse_var(in);
    VarExpr *other_e = dynamic_cast<VarExpr*>(kw);
    std::string var = other_e->getStr();
    if (var == "if" || var == "then" || var == "else") {
        return var;
    }
    else {
        throw std::runtime_error("E3 invalid keyword");
    }
}

static void consume(std::istream &in, int expect) {
    int c = in.get();
    assert(c == expect);
}

static void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}
