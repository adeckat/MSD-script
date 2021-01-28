//
//  expr.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#include "catch.h"
#include "expr.h"
#include <stdexcept>

//Num and its implementations
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
int Num::interp() {
    return this->val;
}
bool Num::has_variable() {
    return false;
}
Expr *Num::subst(std::string s, Expr *other) {
    return new Num(this->val);
}

//Variable and its implementation
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
int Variables::interp() {
    throw std::runtime_error("This is a variable");
}
bool Variables::has_variable() {
    return true;
}
Expr *Variables::subst(std::string s, Expr *other) {
    if (this->var == s) {
        return other;
    }
    else {
        return new Variables(this->var);
    }
}

//Add and its implementation
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
int Add::interp() {
    if (this->lhs->has_variable() || this->rhs->has_variable()) {
        throw std::runtime_error("Variable(s) exist(s) in this expression");
    }
    else {
        int sum;
        sum = this->lhs->interp() + this->rhs->interp();
        return sum;
    }
}
bool Add::has_variable() {
    if (this->lhs->has_variable() || this->rhs->has_variable()) {
        return true;
    }
    else {
        return false;
    }
}
Expr *Add::subst(std::string s, Expr *other) {
    if (!(this->lhs->has_variable()) && !(this->rhs->has_variable())) {
        return new Add(this->lhs, this->rhs);
    }
    else {
        if ((this->lhs->subst(s, other)->equals(other)) && this->rhs->subst(s, other)->equals(other)) {
            return new Add(other, other);
        }
        else if (this->lhs->subst(s, other)->equals(other)) {
            return new Add(other, this->rhs);
        }
        else if (this->rhs->subst(s, other)->equals(other)) {
            return new Add(this->lhs, other);
        }
        else {
            return new Add(this->lhs, this->rhs);
        }
    }
}

//Mult and its implementation
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
int Mult::interp() {
    if (this->lhs->has_variable() || this->rhs->has_variable()) {
        throw std::runtime_error("Variable(s) exist(s) in this expression");
    }
    else {
        int prod;
        prod = this->lhs->interp() * this->rhs->interp();
        return prod;
    }
}
bool Mult::has_variable() {
    if (this->lhs->has_variable() || this->rhs->has_variable()) {
        return true;
    }
    else {
        return false;
    }
}
Expr *Mult::subst(std::string s, Expr *other) {
    if (!(this->lhs->has_variable()) && !(this->rhs->has_variable())) {
        return new Mult(this->lhs, this->rhs);
    }
    else {
        if ((this->lhs->subst(s, other)->equals(other)) && this->rhs->subst(s, other)->equals(other)) {
            return new Mult(other, other);
        }
        else if (this->lhs->subst(s, other)->equals(other)) {
            return new Mult(other, this->rhs);
        }
        else if (this->rhs->subst(s, other)->equals(other)) {
            return new Mult(this->lhs, other);
        }
        else {
            return new Mult(this->lhs, this->rhs);
        }
    }
}

TEST_CASE("equals") {
    Num* val1 = new Num(17);
    Num* val2 = new Num(24);
    Num* val3 = new Num(17);
    Num* val4 = new Num(24);
    Variables* var1 = new Variables("x");
    Variables* var2 = new Variables("y");
    
    CHECK(val1->equals(new Num(17)) == true);
    CHECK(val3->equals(new Num(24)) == false);
    
    CHECK(var1->equals(new Variables("x")) == true);
    CHECK(var1->equals(new Variables("X")) == false);
    CHECK(var1->equals(var2) == false);

    CHECK(val1->equals(var1) == false);
    
    CHECK((new Add(val1, val2))->equals(new Add(val3, val4)) == true);
    CHECK((new Add(val1, val2))->equals(new Add(val2, val1)) == false);
    CHECK((new Add(val3, val4))->equals(new Add(val4, val3)) == false);
    
    CHECK((new Mult(val1, val2))->equals(new Mult(val3, val4)) == true);
    CHECK((new Mult(val1, val2))->equals(new Mult(val2, val1)) == false);
    CHECK((new Mult(val3, val4))->equals(new Mult(val4, val3)) == false);
    
    CHECK((new Add(val1, val2))->equals(new Mult(val1, val2)) == false);
    CHECK((new Add(val1, val2))->equals(new Mult(val3, val4)) == false);
}

TEST_CASE("interp") {
    Num* val1 = new Num(17);
    Num* val2 = new Num(24);
    Num* val3 = new Num(17);
    Variables* var1 = new Variables("x");
    Variables* var2 = new Variables("y");
    
    CHECK(val1->interp() == 17);
    CHECK(val2->interp() == 24);
    
    CHECK_THROWS_WITH(var1->interp(), "This is a variable");
    CHECK_THROWS_WITH(var2->interp(), "This is a variable");
    
    CHECK((new Add(val1, val2))->interp() == 41);
    CHECK((new Add(val1, val3))->interp() == 34);
    CHECK_THROWS_WITH((new Add(val1, var1))->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH((new Add(var2, val2))->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((new Mult(val1, val2))->interp() == 408);
    CHECK((new Mult(val1, val3))->interp() == 289);
    CHECK_THROWS_WITH((new Mult(val1, var1))->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH((new Mult(var2, val2))->interp(), "Variable(s) exist(s) in this expression");
}

TEST_CASE("has_variables") {
    Num* val1 = new Num(17);
    Num* val2 = new Num(24);
    Num* val3 = new Num(17);
    Num* val4 = new Num(24);
    Variables* var1 = new Variables("x");
    Variables* var2 = new Variables("y");
    Variables* var3 = new Variables("X");
    
    CHECK(val3->has_variable() == false);
    CHECK(val4->has_variable() == false);
    
    CHECK(var1->has_variable() == true);
    CHECK(var3->has_variable() == true);
    
    CHECK((new Add(var1, val1))->has_variable() == true);
    CHECK((new Add(val2, var2))->has_variable() == true);
    CHECK((new Add(var1, var2))->has_variable() == true);
    CHECK((new Add(val1, val2))->has_variable() == false);
    
    CHECK((new Mult(var1, val1))->has_variable() == true);
    CHECK((new Mult(val2, var2))->has_variable() == true);
    CHECK((new Mult(var1, var2))->has_variable() == true);
    CHECK((new Mult(val1, val2))->has_variable() == false);
}

TEST_CASE("subst") {
    Num* val1 = new Num(17);
    Num* val2 = new Num(24);
    Variables* var1 = new Variables("x");
    Variables* var2 = new Variables("y");
    
    CHECK(val1->subst("x", new Num(24))->equals(val1));
    CHECK(val2->subst("y", new Num(17))->equals(val2));
    
    CHECK(var1->subst("x", new Variables("y"))->equals(var2));
    CHECK(var2->subst("x", new Variables("X"))->equals(var2));
    CHECK(var1->subst("x", new Num(17))->equals(val1));
    CHECK(var2->subst("y", new Add(new Num(17), new Num(24)))->equals(new Add(val1, val2)));
    
    CHECK((new Add(val1, val2))->subst("x", new Variables("x"))->equals(new Add(val1, val2)));
    CHECK((new Add(new Variables("x"), val1))->subst("x", new Variables("y"))
          ->equals(new Add(new Variables("y"), val1)));
    CHECK((new Add(val2, new Variables("x")))->subst("x", new Variables("y"))
          ->equals(new Add(val2, new Variables("y"))));
    CHECK((new Add(new Variables("x"), new Variables("x")))->subst("x", new Variables("y"))
          ->equals(new Add(new Variables("y"), new Variables("y"))));
    CHECK((new Add(new Variables("y"), new Variables("y")))->subst("Y", new Variables("X"))
          ->equals(new Add(new Variables("y"), new Variables("y"))));
    
    CHECK((new Mult(val1, val2))->subst("x", new Variables("x"))->equals(new Mult(val1, val2)));
    CHECK((new Mult(var1, val1))->subst("x", new Variables("y"))
          ->equals(new Mult(var2, val1)));
    CHECK((new Mult(val2, var1))->subst("x", new Variables("y"))
          ->equals(new Mult(val2, var2)));
    CHECK((new Mult(var1, var1))->subst("x", new Variables("y"))
          ->equals(new Mult(var2, var2)));
    CHECK((new Mult(var2, var2))->subst("Y", new Variables("X"))
          ->equals(new Mult(var2, var2)));
}

