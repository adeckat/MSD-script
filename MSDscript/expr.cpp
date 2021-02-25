//
//  expr.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#include "expr.h"
#include "val.h"
#include <stdexcept>
#include <stdio.h>

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
    pretty_print_at(print_group_none, out, 0, 0);
}

//Num and its implementations
NumExpr::NumExpr(int numExpr) {
    this->numExpr = numExpr;
}
bool NumExpr::equals(Expr *other) {
    NumExpr *other_num = dynamic_cast<NumExpr*>(other);
    if (other_num == NULL) {
        return false;
    }
    else {
        return (this->numExpr == other_num->numExpr);
    }
}
Val *NumExpr::interp() {
    return new NumVal(this->numExpr);
}
bool NumExpr::has_variable() {
    return false;
}
Expr *NumExpr::subst(std::string s, Expr *other) {
    return this;
}
void NumExpr::print(std::ostream& out) {
    out << this->numExpr;
}
void NumExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    mode = print_group_none;
    inside = 0;
    out << this->numExpr;
}

//Variable and its implementation
VarExpr::VarExpr (std::string var) {
    this->var = var; 
}
bool VarExpr::equals(Expr *other) {
    VarExpr *other_var = dynamic_cast<VarExpr*>(other);
    if (other_var == NULL) {
        return false;
    }
    else {
        return (this->var == other_var->var);
    }
}
std::string VarExpr::getStr() {
    return this->var;
}
Val *VarExpr::interp() {
    throw std::runtime_error("Variable(s) exist(s) in this expression");
}
bool VarExpr::has_variable() {
    return true;
}
Expr *VarExpr::subst(std::string s, Expr *other) {
    if (this->var == s) {
        return other;
    }
    else {
        return this;
    }
}
void VarExpr::print(std::ostream& out) {
    out << this->var;
}
void VarExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    mode = print_group_none;
    inside = 0;
    out << this->var;
}

//Add and its implementation
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool AddExpr::equals(Expr *other) {
    AddExpr *other_add = dynamic_cast<AddExpr*>(other);
    if (other_add == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
    }
}
Val *AddExpr::interp() {
    return (this->lhs->interp()->add_to(this->rhs->interp()));
}
bool AddExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}
Expr *AddExpr::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new AddExpr(other_lhs, other_rhs);
}
void AddExpr::print(std::ostream& out) {
    out << "("; 
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}
void AddExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    inside += 2;
    if (mode >= print_group_add) {
        out << "(";
    }
    this->lhs->pretty_print_at(print_group_add, out, indentation, inside);
    out << " + ";
    this->rhs->pretty_print_at(print_group_none, out, indentation, 0);
    if (mode >= print_group_add) {
        out << ")";
    }
}

//Mult and its implementation
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool MultExpr::equals(Expr *other) {
    MultExpr *other_mult = dynamic_cast<MultExpr*>(other);
    if (other_mult == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
    }
}
Val * MultExpr::interp() {
    return (this->lhs->interp()->mult_by(this->rhs->interp()));
}
bool MultExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}
Expr *MultExpr::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new MultExpr(other_lhs, other_rhs);
}
void MultExpr::print(std::ostream& out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}
void MultExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    inside += 1;
    if (mode >= print_group_add_or_mult) {
        out << "(";
    }
    this->lhs->pretty_print_at(print_group_add_or_mult, out, indentation, inside);
    out << " * ";
    this->rhs->pretty_print_at(print_group_add, out, indentation, inside-1);
    if (mode >= print_group_add_or_mult) {
        out << ")";
    }
}

//_let and its implementation
LetExpr::LetExpr(std::string var, Expr *rhs, Expr *body) {
    this->var = var;
    this->rhs = rhs;
    this->body = body;
}
bool LetExpr::equals(Expr *other) {
    LetExpr *other_let = dynamic_cast<LetExpr*>(other);
    if (other_let == NULL) {
        return false;
    }
    else {
        return (this->var == other_let->var) && (this->rhs->equals(other_let->rhs)) && (this->body->equals(other_let->body));
    }
}
Val *LetExpr::interp() {
    Val* new_rhs = this->rhs->interp();
    return this->body->subst(var, new_rhs->to_expr())->interp();
}
bool LetExpr::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}
Expr *LetExpr::subst(std::string s, Expr *other) {
    if (this->var == s) {
        return new LetExpr(s, this->rhs->subst(s, other), this->body);
    }
    return new LetExpr(this->var, this->rhs->subst(s, other), this->body->subst(s, other));
} 
void LetExpr::print(std::ostream& out) {
    out << "(_let " << this->var << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";
}
void LetExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    if (mode > print_group_none && inside > 1) {
        out << "(";
    }
    int pos1 = (int) out.tellp();
    out << "_let " << this->var << " = ";
    this->rhs->pretty_print(out);
    out << "\n";
    int pos2 = (int) out.tellp();
    out << std::string(pos1 - indentation, ' ') << "_in  ";
    this->body->pretty_print_at(print_group_none, out, pos2, inside);
    if (mode > print_group_none && inside > 1) {
        out << ")";
    }
}
