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


//BoolVal and its implement
BoolVal::BoolVal(bool boolVal) {
    this->boolVal = boolVal;
}
bool BoolVal::equals(Val *v) {
    BoolVal *other_bool = dynamic_cast<BoolVal*>(v);
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
Expr* BoolVal::to_expr() {
    return new BoolExpr(this->boolVal);
}
bool BoolVal::is_true() {
    return this->boolVal;
}


