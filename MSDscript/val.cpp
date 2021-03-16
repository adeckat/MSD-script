//
//  val.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#include "val.h"
#include "expr.h"

//NumVal and its implement
NumVal::NumVal(int num) {
    this->numVal = num;
}
bool NumVal::equals(Val *other) {
    NumVal *other_num = dynamic_cast<NumVal*>(other);
    if (other_num == NULL) {
        return false;
    }
    else {
        return (this->numVal == other_num->numVal);
    }
}
Val *NumVal::add_to(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) {
        throw std::runtime_error("Add of non-number error");
    }
    return new NumVal(this->numVal + other_num->numVal);
}
Val *NumVal::mult_by(Val *other_val) {
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) {
        throw std::runtime_error("Mult of non-number error");
    }
    return new NumVal(this->numVal * other_num->numVal);
}
Expr *NumVal::to_expr() {
    return new NumExpr(this->numVal);
}
bool NumVal::is_true() {
    throw std::runtime_error("Not a boolean value error");
}
Val *NumVal::call(Val *actual_arg) {
    throw std::runtime_error("Not a function to be called error");
}

//BoolVal and its implement
BoolVal::BoolVal(bool boolVal) {
    this->boolVal = boolVal;
}
bool BoolVal::equals(Val *other_val) {
    BoolVal *other_bool = dynamic_cast<BoolVal*>(other_val);
    if (other_bool == NULL) {
        return false;
    }
    else {
        return (this->boolVal == other_bool->boolVal);
    }
}
Val *BoolVal::add_to(Val *other_val) {
    throw std::runtime_error("Add of non-number error");
}
Val *BoolVal::mult_by(Val *other_val) {
    throw std::runtime_error("Mult of non-number error");
}
Expr *BoolVal::to_expr() {
    return new BoolExpr(this->boolVal);
}
bool BoolVal::is_true() {
    return this->boolVal;
}
Val *BoolVal::call(Val *actual_arg) {
    throw std::runtime_error("Not a function to be called error");
}

//FunVal and its implement
FunVal::FunVal(std::string formal_arg, Expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}
bool FunVal::equals(Val *other_val) {
    FunVal *other_fun = dynamic_cast<FunVal*>(other_val);
    if (other_fun == NULL) {
        return false;
    }
    else {
        return (this->formal_arg == other_fun->formal_arg) && (this->body->equals(other_fun->body));
    }
}
Val *FunVal::add_to(Val *other_val) {
    throw std::runtime_error("Add of non-number error");
}
Val *FunVal::mult_by(Val *other_val) {
    throw std::runtime_error("Mult of non-number error");
}
Expr *FunVal::to_expr() {
    return new FunExpr(this->formal_arg, this->body);
}
bool FunVal::is_true() {
    throw std::runtime_error("Not a boolean value error");
}
Val *FunVal::call(Val *actual_arg) {
    return (this->body->subst(formal_arg, actual_arg->to_expr()))->interp();
}
