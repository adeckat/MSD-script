//
//  val.hpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#ifndef val_h
#define val_h

#include "pointer.h"
#include "expr.h"
#include "env.h"
#include "cont.h"
#include "step.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

class Expr;
class Env;
class Cont;

CLASS(Val) {
public:
    //Return true if two expressions are the same, and false otherwise
    virtual bool equals(PTR(Val) e) = 0;
    //Helper function to add and Mult
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    virtual PTR(Val) mult_by(PTR(Val) other_val) = 0;
    virtual std::string to_string() = 0;
    virtual bool is_true() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) = 0;
};

class NumVal : public Val {
public:
    int numVal;
    NumVal(int numVal);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual bool is_true();
    virtual PTR(Val) call(PTR(Val) actual_arg);
    virtual std::string to_string();
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

class BoolVal : public Val {
public:
    bool boolVal;
    BoolVal(bool boolVal);
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual bool is_true();
    virtual PTR(Val) call(PTR(Val) actual_arg);
    virtual std::string to_string();
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env); 
    virtual bool equals(PTR(Val) other);
    virtual PTR(Val) add_to(PTR(Val) other_val);
    virtual PTR(Val) mult_by(PTR(Val) other_val);
    virtual bool is_true();
    virtual PTR(Val) call(PTR(Val) actual_arg);
    virtual std::string to_string();
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

#endif /* val_hpp */
