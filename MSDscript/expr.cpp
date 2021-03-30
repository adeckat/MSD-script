//
//  expr.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/24/21.
//

#include "expr.h"
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
bool NumExpr::equals(PTR(Expr) other) {
    PTR(NumExpr) other_num = CAST(NumExpr)(other);
    if (other_num == NULL) {
        return false;
    }
    else {
        return (this->numExpr == other_num->numExpr);
    }
}
//Interp and return value of a Num expression
PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(this->numExpr);
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
bool VarExpr::equals(PTR(Expr) other) {
    PTR(VarExpr) other_var = CAST(VarExpr)(other);
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
PTR(Val) VarExpr::interp(PTR(Env) env) {
    return env->lookup(this->var);
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
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
//Compare an Add expression with another expression to see if they are equal
bool AddExpr::equals(PTR(Expr) other) {
    PTR(AddExpr) other_add = CAST(AddExpr)(other);
    if (other_add == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_add->lhs) && this->rhs->equals(other_add->rhs));
    }
}
//Interp and return value of an Add expression
PTR(Val) AddExpr::interp(PTR(Env) env) {
    return (this->lhs->interp(env)->add_to(this->rhs->interp(env)));
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
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
//Compare a Mult expression with another expression to see if they are equal
bool MultExpr::equals(PTR(Expr) other) {
    PTR(MultExpr) other_mult = CAST(MultExpr)(other);
    if (other_mult == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_mult->lhs) && this->rhs->equals(other_mult->rhs));
    }
}
//Interp and return value of a Mult expression
PTR(Val) MultExpr::interp(PTR(Env) env) {
    return (this->lhs->interp(env)->mult_by(this->rhs->interp(env)));
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
LetExpr::LetExpr(std::string var, PTR(Expr) rhs, PTR(Expr) body) {
    this->var = var;
    this->rhs = rhs;
    this->body = body;
}
//Compare a _let expression with another expression to see if they are equal
bool LetExpr::equals(PTR(Expr) other) {
    PTR(LetExpr) other_let = CAST(LetExpr)(other);
    if (other_let == NULL) {
        return false;
    }
    else {
        return (this->var == other_let->var) && (this->rhs->equals(other_let->rhs)) && (this->body->equals(other_let->body));
    }
}
//Interp and return value of a _let expression
PTR(Val) LetExpr::interp(PTR(Env) env) {
    PTR(Val) other_rhs = this->rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(var, other_rhs, env);
    return this->body->interp(new_env);
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
bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) other_bool = CAST(BoolExpr)(e);
    if (other_bool == NULL) {
        return false;
    }
    else {
        return (this->boolExpr == other_bool->boolExpr);
    }

}
//Interp and return value of a Bool expression
PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(this->boolExpr);
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
EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
//Compare an Eq expression with another expression to see if they are equal
bool EqExpr::equals(PTR(Expr) other) {
    PTR(EqExpr) other_eq = CAST(EqExpr)(other);
    if (other_eq == NULL) {
        return false;
    }
    else {
        return (this->lhs->equals(other_eq->lhs) && this->rhs->equals(other_eq->rhs));
    }
}
//Interp and return value of an Eq expression
PTR(Val) EqExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)((this->lhs->interp(env)->equals(this->rhs->interp(env))));
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
IfExpr::IfExpr(PTR(Expr) test_part, PTR(Expr) then_part, PTR(Expr) else_part) {
    this->test_part = test_part;
    this->then_part = then_part;
    this->else_part = else_part;
}
//Compare an _if expression with another expression to see if they are equal
bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) other_if = CAST(IfExpr)(e);
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
PTR(Val) IfExpr::interp(PTR(Env) env) {
    if (this->test_part->interp(env)->is_true()) {
        return this->then_part->interp(env);
    }
    else {
        return this->else_part->interp(env);
    }
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
//    inside += 1;
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
FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}
//Compare a _fun expression with another expression to see if they are equal
bool FunExpr::equals(PTR(Expr) other) {
    PTR(FunExpr) other_fun = CAST(FunExpr)(other);
    if (other_fun == NULL) {
        return false;
    }
    else {
        return (this->formal_arg == other_fun->formal_arg) && (this->body->equals(other_fun->body));
    }
}
//Interp and return value of a _fun expression
PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg, body, env);
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
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}
//Compare a Call expression with another expression to see if they are equal
bool CallExpr::equals (PTR(Expr) other) {
    PTR(CallExpr) other_call = CAST(CallExpr)(other);
    if (other_call == NULL) {
        return false;
    }
    else {
        return (this->to_be_called->equals(other_call->to_be_called)) && (this->actual_arg->equals(other_call->actual_arg));
    }
}
//Interp and return value of a Call expression
PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called->interp(env)->call(actual_arg->interp(env));
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
//    inside += 1;
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

