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
Val *MultExpr::interp() {
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
    if (mode > print_group_none && inside >= 1) {
        out << "(";
    }
    int pos1 = (int) out.tellp();
    out << "_let " << this->var << " = ";
    this->rhs->pretty_print(out);
    out << "\n";
    int pos2 = (int) out.tellp();
    out << std::string(pos1 - indentation, ' ') << "_in  ";
    this->body->pretty_print_at(print_group_none, out, pos2, inside);
    if (mode > print_group_none && inside >= 1) {
        out << ")";
    }
}

//Bool and its implementations
BoolExpr::BoolExpr(bool boolExpr) {
    this->boolExpr = boolExpr; 
}
bool BoolExpr::equals(Expr *e) {
    BoolExpr *other_bool = dynamic_cast<BoolExpr*>(e);
    if (other_bool == NULL) {
        return false;
    }
    else {
        return (this->boolExpr == other_bool->boolExpr);
    }

}
Val *BoolExpr::interp() {
    return new BoolVal(this->boolExpr);
}
bool BoolExpr::has_variable() {
    return false;
}
Expr *BoolExpr::subst(std::string var, Expr *e) {
    return new BoolExpr(this->boolExpr);

}
void BoolExpr::print(std::ostream& out) {
    if (this->boolExpr == true) {
        out << "_true";
    }
    else {
        out << "_false";
    }
}
void BoolExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    this->print(out);
}

//Equalation and its implementations
EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool EqExpr::equals(Expr *other) {
    EqExpr *other_eq = dynamic_cast<EqExpr*>(other);
    if (other_eq == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_eq->lhs) && this->rhs->equals(other_eq->rhs));
    }
}
Val *EqExpr::interp() {
    return new BoolVal((this->lhs->interp()->equals(this->rhs->interp())));
}
bool EqExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}
Expr *EqExpr::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new EqExpr(other_lhs, other_rhs);
}
void EqExpr::print(std::ostream& out) {
    out << "(";
    this->lhs->print(out);
    out << "==";
    this->rhs->print(out);
    out << ")";
}
void EqExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    inside += 2;
    if (mode >= print_group_add) {
        out << "(";
    }
    this->lhs->pretty_print_at(print_group_add, out, indentation, inside);
    out << " == ";
    this->rhs->pretty_print_at(print_group_none, out, indentation, 0);
    if (mode >= print_group_add) {
        out << ")";
    }
}

//If and its implementations
IfExpr::IfExpr(Expr *test_part, Expr *then_part, Expr *else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}
bool IfExpr::equals(Expr *e) {
    IfExpr *other_if = dynamic_cast<IfExpr*>(e);
    if (other_if == NULL) {
        return false;
    }
    else {
        return (this->test_part->equals(other_if->test_part)) &&
                (this->then_part->equals(other_if->then_part)) &&
                (this->else_part->equals(other_if->else_part));
    }
}
Val *IfExpr::interp() {
    if (this->test_part->interp()->is_true()) {
        return this->then_part->interp();
    }
    else {
        return this->else_part->interp();
    }
}
bool IfExpr::has_variable() {
    return (this->test_part->has_variable() || this->then_part->has_variable() ||
            this->else_part->has_variable());
}
Expr * IfExpr::subst(std::string var, Expr *e) {
    Expr *other_test = this->test_part->subst(var, e);
    Expr *other_then = this->then_part->subst(var, e);
    Expr *other_else = this->else_part->subst(var, e);
    return new IfExpr(other_test, other_then, other_else);
}
void IfExpr::print(std::ostream& out) {
    out << "(_if ";
    this->test_part->print(out);
    out << " _then ";
    this->then_part->print(out);
    out << " _else ";
    this->else_part->print(out);
    out << ")";
}
void IfExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    if (mode > print_group_none && inside >= 1) {
        out << "(";
    }
    int pos1 = (int) out.tellp();
    out << "_if ";
    this->test_part->pretty_print(out);
    out << "\n";
    int pos2 = (int) out.tellp();
    int n = pos1 - indentation;
    out << std::string(n, ' ') << "_then ";
    this->then_part->pretty_print_at(print_group_none, out, pos2, inside);
    out << "\n";
    out << std::string(n, ' ') << "_else ";
    this->else_part->pretty_print(out);
    if (mode > print_group_none && inside >= 1) {
        out << ")";
    }
}
