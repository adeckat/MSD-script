//
//  val.cpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#include "val.h"
#include "expr.h"

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


