//
//  expr.h
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#ifndef expr_h
#define expr_h

#include "pointer.h"
#include "val.h"
#include "env.h" 
#include "step.h"
#include "cont.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

class Val;
class Env;

CLASS(Expr) {
public:
    //Return true if two expressions are the same, and false otherwise
    virtual bool equals(PTR(Expr) e) = 0;
    
    //Return an int for the value of an expression
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    virtual void step_interp() = 0;
    
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
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class VarExpr : public Expr {
public:
    std::string var;
    VarExpr(std::string var);
    virtual bool equals(PTR(Expr) other);
    std::string getStr();
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class LetExpr : public Expr {
public:
    std::string var;
    PTR(Expr) rhs;
    PTR(Expr) body;
    LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class BoolExpr : public Expr {
public:
    bool boolExpr;
    BoolExpr(bool boolExpr);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class IfExpr : public Expr {
public:
    PTR(Expr) test_part;
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class FunExpr : public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;
    FunExpr(std::string formal_arg, PTR(Expr) body);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};

class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    virtual bool equals(PTR(Expr) other);
    virtual PTR(Val) interp(PTR(Env) env);
    virtual void step_interp();
    virtual void print(std::ostream& out);
    virtual void pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside);
};
#endif /* expr_h */
