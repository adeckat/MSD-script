//
//  val.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#include "val.h"
#include <stdexcept>
#include <stdio.h>

std::string Val::to_string() {
    std::stringstream out("");
    this->print(out); 
    return out.str();
}

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
void NumVal::print(std::ostream& out) {
    out << this->numVal;
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
void BoolVal::print(std::ostream& out) {
    if (this->boolVal == true) {
        out << "_true";
    }
    else {
        out << "_false";
    }
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
void FunVal::print(std::ostream& out) {
    out << "[function]";
}
