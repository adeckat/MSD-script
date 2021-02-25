//
//  val.hpp
//  MSDScript
//
//  Created by Nga Huynh on 2/24/21.
//

#ifndef val_hpp
#define val_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

class Expr;
class Val {
public:
    //Return true if two expressions are the same, and false otherwise
    virtual bool equals(Val *e) = 0;
    //Helper function to add and Mult
    virtual Val* add_to(Val *other_val) = 0;
    virtual Val* mult_by(Val *other_val) = 0;
    virtual Expr* to_expr() = 0;
};

class NumVal : public Val {
public:
    int numVal;
    NumVal(int numVal);
    virtual bool equals(Val *other);
    virtual Val* add_to(Val *other_val);
    virtual Val* mult_by(Val *other_val);
    virtual Expr* to_expr();
};

#endif /* val_hpp */