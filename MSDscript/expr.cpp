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

//Helper function to print
std::string Expr::to_str() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}

//Helper funtion to print pretty
std::string Expr::to_pretty_str() {
    std::stringstream out(""); 
    this->pretty_print(out);
    return out.str();
}

//Print pretty
void Expr::pretty_print(std::ostream& out) {
    pretty_print_at(print_group_none, out, 0, 0);
}

/* Num and its implementations */
//Num expression constructor
NumExpr::NumExpr(int numExpr) {
    this->numExpr = numExpr;
}
//Compare a Num expression with another expression to see if they are equal
bool NumExpr::equals(Expr *other) {
    NumExpr *other_num = dynamic_cast<NumExpr*>(other);
    if (other_num == NULL) {
        return false;
    }
    else {
        return (this->numExpr == other_num->numExpr);
    }
}
//Interp and return value of a Num expression
Val *NumExpr::interp() {
    return new NumVal(this->numExpr);
}
//Return this because a Num expression does not contain (a) variable(s) to be subst
Expr *NumExpr::subst(std::string s, Expr *other) {
    return this;
}
//Print out a Num expression
void NumExpr::print(std::ostream& out) {
    out << this->numExpr;
}
//Helper function to print out a Num expression in a pretty way
void NumExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    mode = print_group_none;
    inside = 0;
    out << this->numExpr;
}

/* Variable and its implementation */
//Var expression constructor
VarExpr::VarExpr (std::string var) {
    this->var = var; 
}
//Compare a Var expression with another expression to see if they are equal
bool VarExpr::equals(Expr *other) {
    VarExpr *other_var = dynamic_cast<VarExpr*>(other);
    if (other_var == NULL) {
        return false;
    }
    else {
        return (this->var == other_var->var);
    }
}
//Get the string from a Var expression
std::string VarExpr::getStr() {
    return this->var;
}
//Interp a Var expression and throw error because variable could not be interp
Val *VarExpr::interp() {
    throw std::runtime_error("Variable(s) exist(s) in this expression");
}
//Check if a Var expression contains a specific variable then substitute with a given expression
Expr *VarExpr::subst(std::string s, Expr *other) {
    if (this->var == s) {
        return other;
    }
    else {
        return this;
    }
}
//Print out a Var expression
void VarExpr::print(std::ostream& out) {
    out << this->var;
}
//Helper function to print out a Var expression in a pretty way
void VarExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    mode = print_group_none;
    inside = 0;
    out << this->var;
}

/* Add and its implementation */
//Add expression constructor
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
//Compare an Add expression with another expression to see if they are equal
bool AddExpr::equals(Expr *other) {
    AddExpr *other_add = dynamic_cast<AddExpr*>(other);
    if (other_add == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
    }
}
//Interp and return value of an Add expression
Val *AddExpr::interp() {
    return (this->lhs->interp()->add_to(this->rhs->interp()));
}
//Check if either lhs or rhs of an Add expression contains a specific variable then substitute with a given expression
Expr *AddExpr::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new AddExpr(other_lhs, other_rhs);
}
//Print out an Add expression
void AddExpr::print(std::ostream& out) {
    out << "("; 
    this->lhs->print(out);
    out << "+";
    this->rhs->print(out);
    out << ")";
}
//Helper function to print out an Add expression in a pretty way
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

/* Mult and its implementation */
//Mult expression constructor
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
//Compare a Mult expression with another expression to see if they are equal
bool MultExpr::equals(Expr *other) {
    MultExpr *other_mult = dynamic_cast<MultExpr*>(other);
    if (other_mult == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
    }
}
//Interp and return value of a Mult expression
Val *MultExpr::interp() {
    return (this->lhs->interp()->mult_by(this->rhs->interp()));
}
//Check if either lhs or rhs of a Mult expression contains a specific variable then substitute with a given expression
Expr *MultExpr::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new MultExpr(other_lhs, other_rhs);
}
//Print out a Mult expression
void MultExpr::print(std::ostream& out) {
    out << "(";
    this->lhs->print(out);
    out << "*";
    this->rhs->print(out);
    out << ")";
}
//Helper function to print out a Mult expression in a pretty way
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

/* _let and its implementation */
//_let expression constructor
LetExpr::LetExpr(std::string var, Expr *rhs, Expr *body) {
    this->var = var;
    this->rhs = rhs;
    this->body = body;
}
//Compare a _let expression with another expression to see if they are equal
bool LetExpr::equals(Expr *other) {
    LetExpr *other_let = dynamic_cast<LetExpr*>(other);
    if (other_let == NULL) {
        return false;
    }
    else {
        return (this->var == other_let->var) && (this->rhs->equals(other_let->rhs)) && (this->body->equals(other_let->body));
    }
}
//Interp and return value of a _let expression
Val *LetExpr::interp() {
    Val* new_rhs = this->rhs->interp();
    return this->body->subst(var, new_rhs->to_expr())->interp();
}
//Check if a _let expression contains a specific variable then substitute rhs and body with a given expression
Expr *LetExpr::subst(std::string s, Expr *other) {
    if (this->var == s) {
        return new LetExpr(s, this->rhs->subst(s, other), this->body);
    }
    return new LetExpr(this->var, this->rhs->subst(s, other), this->body->subst(s, other));
}
//Print out a _let expression
void LetExpr::print(std::ostream& out) {
    out << "(_let " << this->var << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";
}
//Helper function to print out a _let expression in a pretty way
void LetExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    if (mode > print_group_none && inside >= 1) {
        out << "(";
    }
    int pos1 = (int) out.tellp();
    out << "_let " << this->var << " = ";
    this->rhs->pretty_print_at(print_group_none, out, indentation, inside);
    out << "\n";
    int pos2 = (int) out.tellp();
    int n = pos1 - indentation;
    out << std::string(n, ' ') << "_in  ";
    this->body->pretty_print_at(print_group_none, out, pos2, inside);
    if (mode > print_group_none && inside >= 1) {
        out << ")";
    }
}

/* Bool and its implementations */
//Bool expression constructor
BoolExpr::BoolExpr(bool boolExpr) {
    this->boolExpr = boolExpr; 
}
//Compare a Bool expression with another expression to see if they are equal
bool BoolExpr::equals(Expr *e) {
    BoolExpr *other_bool = dynamic_cast<BoolExpr*>(e);
    if (other_bool == NULL) {
        return false;
    }
    else {
        return (this->boolExpr == other_bool->boolExpr);
    }

}
//Interp and return value of a Bool expression
Val *BoolExpr::interp() {
    return new BoolVal(this->boolExpr);
}
//Return this because a Bool expression does not contain (a) variable(s) to be subst
Expr *BoolExpr::subst(std::string var, Expr *e) {
    return new BoolExpr(this->boolExpr);

}
//Print out a Bool expression
void BoolExpr::print(std::ostream& out) {
    if (this->boolExpr == true) {
        out << "_true";
    }
    else {
        out << "_false";
    }
}
//Helper function to print out a Bool expression in a pretty way
void BoolExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    this->print(out);
}

/* Equalation and its implementations */
//Eq expression constructor
EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
//Compare an Eq expression with another expression to see if they are equal
bool EqExpr::equals(Expr *other) {
    EqExpr *other_eq = dynamic_cast<EqExpr*>(other);
    if (other_eq == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_eq->lhs) && this->rhs->equals(other_eq->rhs));
    }
}
//Interp and return value of an Eq expression
Val *EqExpr::interp() {
    return new BoolVal((this->lhs->interp()->equals(this->rhs->interp())));
}
//Check if either lhs or rhs of an Eq expression contains a specific variable then substitute with a given expression
Expr *EqExpr::subst(std::string s, Expr *other) {
    Expr *other_lhs = this->lhs->subst(s, other);
    Expr *other_rhs = this->rhs->subst(s, other);
    return new EqExpr(other_lhs, other_rhs);
}
//Print out an Eq expression
void EqExpr::print(std::ostream& out) {
    out << "(";
    this->lhs->print(out);
    out << "==";
    this->rhs->print(out);
    out << ")";
}
//Helper function to print out an Eq expression in a pretty way
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

/* _if and its implementations */
//_if expression constructor
IfExpr::IfExpr(Expr *test_part, Expr *then_part, Expr *else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}
//Compare an _if expression with another expression to see if they are equal
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
//Interp and return value of an _if expression
Val *IfExpr::interp() {
    if (this->test_part->interp()->is_true()) {
        return this->then_part->interp();
    }
    else {
        return this->else_part->interp();
    }
}
//Check if an _if expression contains a specific variable then substitute with a given expression
Expr * IfExpr::subst(std::string var, Expr *e) {
    Expr *other_test = this->test_part->subst(var, e);
    Expr *other_then = this->then_part->subst(var, e);
    Expr *other_else = this->else_part->subst(var, e);
    return new IfExpr(other_test, other_then, other_else);
}
//Print out an _if expression
void IfExpr::print(std::ostream& out) {
    out << "(_if ";
    this->test_part->print(out);
    out << " _then ";
    this->then_part->print(out);
    out << " _else ";
    this->else_part->print(out);
    out << ")";
}
//Helper function to print out n _if expression in a pretty way
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
    pos2 = (int) out.tellp();
    out << std::string(n, ' ') << "_else ";
    this->else_part->pretty_print_at(print_group_none, out, pos2, inside);
    if (mode > print_group_none && inside >= 1) {
        out << ")";
    }
}

/* _fun and its implementation */
//_fun expression constructor
FunExpr::FunExpr(std::string formal_arg, Expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}
//Compare a _fun expression with another expression to see if they are equal
bool FunExpr::equals(Expr *other) {
    FunExpr *other_fun = dynamic_cast<FunExpr*>(other);
    if (other_fun == NULL) {
        return false;
    }
    else {
        return (this->formal_arg == other_fun->formal_arg) && (this->body->equals(other_fun->body));
    }
}
//Interp and return value of a _fun expression
Val *FunExpr::interp() {
    return new FunVal(formal_arg, body);
}
//Check if a _fun expression contains a specific variable then substitute rhs and body with a given expression
Expr *FunExpr::subst(std::string s, Expr *other) {
    if (this->formal_arg == s) {
        return new FunExpr(this->formal_arg, this->body);
    }
    Expr* other_body = this->body->subst(s, other);
    return new FunExpr(this->formal_arg, other_body);
}
//Print out a _fun expression
void FunExpr::print(std::ostream& out) {
    out << "(_fun (" << this->formal_arg << ") ";
    this->body->print(out);
    out << ")";
}
//Helper function to print out a _fun expression in a pretty way
void FunExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    if (mode > print_group_none && inside >= 1) {
        out << "(";
    }
    int pos1 = (int) out.tellp();
    out << "_fun (" << this->formal_arg << ")";
    out << "\n";
    int pos2 = (int) out.tellp();
    int n = pos1 - indentation;
    out << std::string(n, ' ') << "  ";
    this->body->pretty_print_at(print_group_none, out, pos2, inside);
    if (mode > print_group_none && inside >= 1) {
        out << ")";
    }
}

/* Call and its implementation */
//Call expression constructor
CallExpr::CallExpr(Expr *to_be_called, Expr *actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}
//Compare a Call expression with another expression to see if they are equal
bool CallExpr::equals (Expr *other) {
    CallExpr *other_call = dynamic_cast<CallExpr*>(other);
    if (other_call == NULL) {
        return  false;
    }
    else {
        return (this->to_be_called->equals(other_call->to_be_called)) && (this->actual_arg->equals(other_call->actual_arg));
    }
}
//Interp and return value of a Call expression
Val *CallExpr::interp() {
    return to_be_called->interp()->call(actual_arg->interp());
}
//Check if either lhs or rhs of a Call expression contains a specific variable then substitute with a given expression
Expr *CallExpr::subst(std::string s, Expr *other) {
    Expr *other_to_be_called = this->to_be_called->subst(s, other);
    Expr *other_actual_arg = this->actual_arg->subst(s, other);
    return new CallExpr(other_to_be_called, other_actual_arg);
}
//Print out a Call expression
void CallExpr::print(std::ostream& out) {
    this->to_be_called->print(out);
    out << "(";
    this->actual_arg->print(out);
    out << ")";
}
//Helper function to print out a Call expression in a pretty way
void CallExpr::pretty_print_at(print_mode_t mode, std::ostream& out, int indentation, int inside) {
    inside += 1;
//    if (mode >= print_group_add_or_mult) {
//        out << "(";
//    }
    this->to_be_called->pretty_print_at(print_group_add_or_mult, out, indentation, inside);
    out << "(";
    this->actual_arg->pretty_print_at(print_group_none, out, indentation, inside);
    out << ")";
//    if (mode >= print_group_add_or_mult) {
//        out << ")";
//    }
}
