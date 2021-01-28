//
//  expr.h
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#ifndef expr_h
#define expr_h

#include <stdio.h>
#include <string>

class Expr {
public:
    //Return true if two expressions are the same, and false otherwise
    virtual bool equals(Expr *e) = 0;
    //Return an int for the value of an expression
    virtual int interp() = 0;
    //Return true if the expression is a variable or contains a variable, and false otherwise
    virtual bool has_variable() = 0;
    //Return a new expression with the new replaced variable when the original expression contains
    //a variable matching the string of subst method
    virtual Expr *subst(std::string s, Expr *e) = 0;
};

class Num : public Expr {
public:
    int val;
    Num(int val);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr *subst(std::string s, Expr *other);
};

class Variables : public Expr {
public:
    std::string var;
    Variables(std::string var);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr *subst(std::string s, Expr *other);
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Add(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr *subst(std::string s, Expr *other);
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    Mult(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual int interp();
    virtual bool has_variable();
    virtual Expr *subst(std::string s, Expr *other);
};

#endif /* expr_h */
