//
//  expr.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#include "expr.h"
#include <stdexcept>

std::string Expr::to_str() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

std::string Expr::to_pretty_str() {
    std::stringstream out("");
    this->pretty_print(out);
    return out.str();
}

void Expr::pretty_print(std::ostream& out) {
    pretty_print_at(print_group_none, out);
}

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
    return this;
}
void Num::print(std::ostream& out) {
    out << this->val;
}
void Num::pretty_print_at(print_mode_t mode, std::ostream& out) {
    mode = print_group_none;
    out << this->val;
}

//Variable and its implementation
Variable::Variable (std::string var) {
    this->var = var; 
}
bool Variable::equals(Expr *other) {
    Variable *other_var = dynamic_cast<Variable*>(other);
    if (other_var == NULL) {
        return false;
    }
    else {
        return (this->var == other_var->var);
    }
}
int Variable::interp() {
    throw std::runtime_error("Variable(s) exist(s) in this expression");
}
bool Variable::has_variable() {
    return true;
}
Expr *Variable::subst(std::string s, Expr *other) {
    if (this->var == s) {
        return other;
    }
    else {
        return this;
    }
}
void Variable::print(std::ostream& out) {
    out << this->var;
}
void Variable::pretty_print_at(print_mode_t mode, std::ostream& out) {
    mode = print_group_none;
    out << this->var;
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
    return (this->lhs->interp() + this->rhs->interp());
}
bool Add::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}
Expr *Add::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new Add(other_lhs, other_rhs);
}
void Add::print(std::ostream& out) {
    out << "("; 
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}
void Add::pretty_print_at(print_mode_t mode, std::ostream& out) {
    if (mode >= print_group_add) {
        out << "(";
    }
    this->lhs->pretty_print_at(print_group_add, out);
    out << " + ";
    this->rhs->pretty_print_at(print_group_none, out);
    if (mode >= print_group_add) {
        out << ")";
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
    return (this->lhs->interp() * this->rhs->interp());
}
bool Mult::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}
Expr *Mult::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new Mult(other_lhs, other_rhs);
}
void Mult::print(std::ostream& out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}
void Mult::pretty_print_at(print_mode_t mode, std::ostream& out) {
    if (mode >= print_group_add_or_mult) {
        out << "(";
    }
    this->lhs->pretty_print_at(print_group_add_or_mult, out);
    out << " * ";
    this->rhs->pretty_print_at(print_group_add, out);
    if (mode >= print_group_add_or_mult) {
        out << ")";
    }
}
