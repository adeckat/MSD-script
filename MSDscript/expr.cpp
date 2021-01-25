//
//  expr.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#include "catch.h"
#include "expr.h"

Num::Num(int val) {
    this->val = val;
}
bool Num::equals(Expr *other) {
    Num *other_num = dynamic_cast<Num*>(other);
    if (other_num == NULL) {
        return false;
    }
    else {
        return (this->val == other_num->val);
    }
}

Variables::Variables (std::string var) {
    this->var = var;
}
bool Variables::equals(Expr *other) {
    Variables *other_var = dynamic_cast<Variables*>(other);
    if (other_var == NULL) {
        return false;
    }
    else {
        return (this->var == other_var->var);
    }
}

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Add::equals(Expr *other) {
    Add *other_add = dynamic_cast<Add*>(other);
    if (other_add == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
    }
}

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Mult::equals(Expr *other) {
    Mult *other_mult = dynamic_cast<Mult*>(other);
    if (other_mult == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
    }
}

TEST_CASE("equals") {
    CHECK((new Num(17))->equals(new Num(17)) == true );
    CHECK((new Num(17))->equals(new Num(24)) == false);
    
    CHECK((new Variables("x"))->equals(new Variables("x")) == true);
    CHECK((new Variables("x"))->equals(new Variables("X")) == false);
    CHECK((new Variables("x"))->equals(new Variables("y")) == false);
    
    CHECK((new Num(17))->equals(new Variables("x")) == false);
    
    Num* val1 = new Num(17);
    Num* val2 = new Num(24);
    Num* val3 = new Num(17);
    Num* val4 = new Num(24);
    
    CHECK((new Add(val1, val2))->equals(new Add(val3, val4)) == true);
    CHECK((new Add(val1, val2))->equals(new Add(val2, val1)) == false);
    CHECK((new Add(val3, val4))->equals(new Add(val4, val3)) == false);
    
    CHECK((new Mult(val1, val2))->equals(new Mult(val3, val4)) == true);
    CHECK((new Mult(val1, val2))->equals(new Mult(val2, val1)) == false);
    CHECK((new Mult(val3, val4))->equals(new Mult(val4, val3)) == false);
    
    CHECK((new Add(val1, val2))->equals(new Mult(val1, val2)) == false);
    CHECK((new Add(val1, val2))->equals(new Mult(val3, val4)) == false);
}
