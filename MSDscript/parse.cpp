//
//  parse.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/14/21.
//

#include "parse.h"
#include <cassert>
 
Expr *parse_str(std::string s){
    std::istringstream str(s);
    return parse(str);
}

Expr *parse(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse(in);
        return new Add(e, rhs);
    } else
        return e;
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
    return new Num(n);
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
    return new Variable(str);
}

Expr *parse_let(std::istream &in) {
    in.get();
    Expr *rhs, *body;
    std::string kw, var;
    kw = parse_keyword(in);
    if (kw == "let") {
        skip_whitespace(in);
        Expr *e = parse_var(in);
        Variable *other_e = dynamic_cast<Variable*>(e);
        var = other_e->getStr();
        skip_whitespace(in);
        if (in.peek() == '=') {
            consume(in, '=');
            rhs = parse(in);
            if (in.peek() == '_') {
                consume(in, '_');
                kw = parse_keyword(in);
                if (kw == "in") {
                    body = parse(in);
                } else {
                    throw std::runtime_error("E1 invalid input");
                }
            } else {
                throw std::runtime_error("E2 invalid input");
            }
        } else {
            throw std::runtime_error("E3 invalid input");
        }
        return new _let(var, rhs, body);
    }
    else {
        throw std::runtime_error("E4 invalid input");
    }
}

std::string parse_keyword(std::istream &in) {
    int c = in.peek();
    if (c == 'l' || c == 'i') {
        Expr *kw = parse_var(in);
        Variable *other_e = dynamic_cast<Variable*>(kw);
        std::string var = other_e->getStr();
        if (var == "let" || var == "in") {
            return var;
        }
        else {
            throw std::runtime_error("Invalid keyword");
        }
    }
    else {
        throw std::runtime_error("Invalid keyword");
    }
}

Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse(in);
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
        return parse_let(in);
    }
    else {
        consume(in, c);
        throw std::runtime_error("Invalid input");
    }
}

Expr *parse_addend(std::istream &in) {
    Expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else
        return e;
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
