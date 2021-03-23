//
//  val.hpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#ifndef val_hpp
#define val_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include "pointer.h"

class Expr;

CLASS(Val) {
public:
    //Return true if two expressions are the same, and false otherwise
    virtual bool equals(PTR(Val) e) = 0;
    //Helper function to add and Mult
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_by(PTR(Val) other_val) = 0;
    virtual PTR(Expr) to_expr() = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
};

class NumVal : public Val {
public:
    int numVal;
    NumVal(int numVal);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual PTR(Expr) to_expr();
    virtual bool is_true();
    virtual PTR(Val) call(PTR(Val) actual_arg);
};

class BoolVal : public Val {
public:
    bool boolVal;
    BoolVal(bool boolVal);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual PTR(Expr) to_expr();
    virtual bool is_true();
    virtual PTR(Val) call(PTR(Val) actual_arg);
};

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    FunVal(std::string formal_arg, PTR(Expr) body);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual PTR(Expr) to_expr();
    virtual bool is_true();
    virtual PTR(Val) call(PTR(Val) actual_arg);
};

#endif /* val_hpp */
