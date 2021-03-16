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
#include <iostream>
#include <sstream>

class Val;
class Expr {
public:
    //Return true if two expressions are the same, and false otherwise
    virtual bool equals(Expr *e) = 0;
    
    //Return an int for the value of an expression
    virtual Val* interp() = 0;
    
    //Return a new expression with the new replaced variable when the original expression contains
    //a variable matching the string of subst method
    virtual Expr* subst(std::string s, Expr *e) = 0;
    
    //Helper function returns a regular string
    std::string to_str();
    //Print method to print the regular expression
    virtual void print(std::ostream& out) = 0;
    
    //Helper function returns a pretty string
    std::string to_pretty_str();
    //Declare all possible group to print
    typedef enum {
        print_group_none,
        print_group_add,
        print_group_add_or_mult,
    } print_mode_t;
    //Pretty print helper funcion
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) = 0;
    //Print method to print the pretty expression
    void pretty_print(std::ostream& out);
};

class NumExpr : public Expr {
public:
    int numExpr;
    NumExpr(int numExpr);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class VarExpr : public Expr {
public:
    std::string var;
    VarExpr(std::string var);
    virtual bool equals(Expr *other);
    std::string getStr();
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    AddExpr(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    MultExpr(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class LetExpr : public Expr {
public:
    std::string var;
    Expr *rhs;
    Expr *body;
    LetExpr(std::string var, Expr *rhs, Expr *body);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class BoolExpr : public Expr {
public:
    bool boolExpr;
    BoolExpr(bool boolExpr);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class EqExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    EqExpr(Expr *lhs, Expr *rhs);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class IfExpr : public Expr {
public:
    Expr *test_part;
    Expr *then_part;
    Expr *else_part;
    IfExpr(Expr *test_part, Expr *then_part, Expr *else_part);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class FunExpr : public Expr {
public:
    std::string formal_arg;
    Expr *body;
    FunExpr(std::string formal_arg, Expr *body);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class CallExpr : public Expr {
public:
    Expr *to_be_called;
    Expr *actual_arg;
    CallExpr(Expr *to_be_called, Expr *actual_arg);
    virtual bool equals(Expr *other);
    virtual Val* interp();
    virtual Expr* subst(std::string s, Expr *other);
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};
#endif /* expr_h */
