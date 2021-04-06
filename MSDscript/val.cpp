//
//  val.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#include "val.h"
#include <stdexcept>
#include <stdio.h>

//NumVal and its implement
NumVal::NumVal(int num) {
    this->numVal = num;
}
bool NumVal::equals(PTR(Val) other) {
    PTR(NumVal) other_num = CAST(NumVal)(other);
    if (other_num == NULL) {
        return false;
    }
    else {
        return (this->numVal == other_num->numVal);
    }
}
PTR(Val) NumVal::add_to(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL) {
        throw std::runtime_error("Add of non-number error");
    }
    return NEW(NumVal)(this->numVal + other_num->numVal);
}
PTR(Val) NumVal::mult_by(PTR(Val) other_val) {
    PTR(NumVal) other_num = CAST(NumVal)(other_val);
    if (other_num == NULL) {
        throw std::runtime_error("Mult of non-number error");
    }
    return NEW(NumVal)(this->numVal * other_num->numVal);
}
bool NumVal::is_true() {
    throw std::runtime_error("Not a boolean value error");
}
PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("Not a function to be called error");
}
std::string NumVal::to_string() {
    return std::to_string(numVal);
}
void NumVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    throw std::runtime_error("NumVal call error");
}

//BoolVal and its implement
BoolVal::BoolVal(bool boolVal) {
    this->boolVal = boolVal;
}
bool BoolVal::equals(PTR(Val) other_val) {
    PTR(BoolVal) other_bool = CAST(BoolVal)(other_val);
    if (other_bool == NULL) {
        return false;
    }
    else {
        return (this->boolVal == other_bool->boolVal);
    }
}
PTR(Val) BoolVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("Add of non-number error");
}
PTR(Val) BoolVal::mult_by(PTR(Val) other_val) {
    throw std::runtime_error("Mult of non-number error");
}
bool BoolVal::is_true() {
    return this->boolVal;
}
PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("Not a function to be called error");
}
std::string BoolVal::to_string() {
    if (this->boolVal == true) {
        return "_true";
    }
    else {
        return "_false";
    }
}
void BoolVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    throw std::runtime_error("BoolVal call error");
}

//FunVal and its implement
FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}
bool FunVal::equals(PTR(Val) other_val) {
    PTR(FunVal) other_fun = CAST(FunVal)(other_val);
    if (other_fun == NULL) {
        return false;
    }
    else {
        return (this->formal_arg == other_fun->formal_arg) && (this->body->equals(other_fun->body)) && (this->env->equals(other_fun->env));  
    }
}
PTR(Val) FunVal::add_to(PTR(Val) other_val) {
    throw std::runtime_error("Add of non-number error");
}
PTR(Val) FunVal::mult_by(PTR(Val) other_val) {
    throw std::runtime_error("Mult of non-number error");
}
bool FunVal::is_true() {
    throw std::runtime_error("Not a boolean value error");
}
PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return this->body->interp(NEW(ExtendedEnv)(formal_arg, actual_arg, env)); 
}
std::string FunVal::to_string() {
    return "[function]";
}
void FunVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg_val, env);
    Step::cont = rest;
}
