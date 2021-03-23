//
//  expr_tests.cpp
//  MSDscript
//
//  Created by Nga Huynh on 1/30/21.
//

#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include "pointer.h"

TEST_CASE("equals") {
    PTR(NumExpr) val1 = NEW(NumExpr)(17);
    PTR(NumExpr) val2 = NEW(NumExpr)(24);
    PTR(NumExpr) val3 = NEW(NumExpr)(17);
    PTR(NumExpr) val4 = NEW(NumExpr)(24);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");
    
    CHECK(val1->equals(NEW(NumExpr)(17)) == true);
    CHECK(val3->equals(NEW(NumExpr)(24)) == false);
    
    CHECK(var1->equals(NEW(VarExpr)("x")) == true);
    CHECK(var1->equals(NEW(VarExpr)("X")) == false);
    CHECK(var1->equals(var2) == false);

    CHECK(val1->equals(var1) == false);
    CHECK(var2->equals(val2) == false);
    
    CHECK((NEW(AddExpr)(val1, val2))->equals(NEW(AddExpr)(val3, val4)) == true);
    CHECK((NEW(AddExpr)(val1, val2))->equals(NEW(AddExpr)(val2, val1)) == false);
    CHECK((NEW(AddExpr)(val3, val4))->equals(NEW(AddExpr)(val4, val3)) == false);
    
    CHECK((NEW(MultExpr)(val1, val2))->equals(NEW(MultExpr)(val3, val4)) == true);
    CHECK((NEW(MultExpr)(val1, val2))->equals(NEW(MultExpr)(val2, val1)) == false);
    CHECK((NEW(MultExpr)(val3, val4))->equals(NEW(MultExpr)(val4, val3)) == false);
    
    CHECK((NEW(AddExpr)(val1, val2))->equals(NEW(MultExpr)(val1, val2)) == false);
    CHECK((NEW(MultExpr)(val1, val2))->equals(NEW(AddExpr)(val3, val4)) == false);
    
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->equals(NEW(MultExpr)(var1, val1)) == false);
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->equals(NEW(LetExpr)("x", NEW(NumExpr)(17), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(24)))) == true);
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->equals(NEW(LetExpr)("y", NEW(NumExpr)(17), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(24)))) == false);
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->equals(NEW(LetExpr)("x", NEW(NumExpr)(24), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(24)))) == false);
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->equals(NEW(LetExpr)("x", NEW(NumExpr)(17), NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(24)))) == false);
    
    CHECK(var1->getStr() == "x");
    CHECK(var2->getStr() == "y");
    
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)) == true);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(false)) == false);
    CHECK((NEW(BoolExpr)(false))->equals(NEW(BoolExpr)(false)) == true);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(NumExpr)(24)) == false);
    CHECK((NEW(BoolExpr)(false))->equals(NEW(VarExpr)("x")) == false);
    CHECK((NEW(BoolExpr)(true))->equals(NEW(AddExpr)(val1, val2)) == false);
    CHECK((NEW(BoolExpr)(false))->equals(NEW(MultExpr)(val3, val4)) == false);
 
    CHECK((NEW(EqExpr)(val1, val2))->equals(NEW(EqExpr)(NEW(NumExpr)(17), NEW(NumExpr)(24))) == true);
    CHECK((NEW(EqExpr)(var1, var2))->equals(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y")))== true);
    CHECK((NEW(EqExpr)(val1, var1))->equals(NEW(EqExpr)(NEW(NumExpr)(17), NEW(VarExpr)("x"))) == true);
    CHECK((NEW(EqExpr)(var1, val1))->equals(NEW(EqExpr)(NEW(NumExpr)(17), NEW(VarExpr)("x"))) == false);
    CHECK((NEW(EqExpr)(var1, val1))->equals(NEW(AddExpr)(var1, val1)) == false);
    
    CHECK((NEW(IfExpr)(var1, val1, val2))->equals(NEW(IfExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17), NEW(NumExpr)(24))) == true);
    CHECK((NEW(IfExpr)(NEW(EqExpr)(var1, val1), NEW(NumExpr)(1), NEW(NumExpr)(2)))->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17)), NEW(NumExpr)(1), NEW(NumExpr)(2))) == true);
    CHECK((NEW(IfExpr)(NEW(EqExpr)(val1, var1), NEW(NumExpr)(1), NEW(NumExpr)(2)))->equals(NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17)), NEW(NumExpr)(1), NEW(NumExpr)(2))) == false);
    CHECK((NEW(IfExpr)(var1, val1, val2))->equals(NEW(LetExpr)("x", NEW(NumExpr)(17), NEW(NumExpr)(24))) == false);
    
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)))->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17)))) == true);
    CHECK((NEW(FunExpr)("y", NEW(MultExpr)(var2, val2)))->equals(NEW(MultExpr)(var2, NEW(AddExpr)(var2, val2))) == false);
    
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), val2))->equals(NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))), NEW(NumExpr)(24))) == true);
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), val2))->equals(NEW(MultExpr)(var2, NEW(AddExpr)(var2, val2))) == false);
    
    CHECK((NEW(NumVal)(13))->equals(NULL) == false);
    CHECK((NEW(NumVal)(17))->equals(NEW(NumVal)(17)) == true);
    
    CHECK((NEW(BoolVal)(true))->equals(NULL) == false);
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(true)) == true);
    CHECK((NEW(BoolVal)(false))->equals(NEW(BoolVal)(false)) == true);
    CHECK((NEW(BoolVal)(true))->equals(NEW(BoolVal)(false)) == false);
    
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(var1, val1)))->equals(NULL) == false);
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(var1, val1)))->equals(NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17)))) == true);
}

TEST_CASE("interp") {
    PTR(NumExpr) val1 = NEW(NumExpr)(17);
    PTR(NumExpr) val2 = NEW(NumExpr)(24);
    PTR(NumExpr) val3 = NEW(NumExpr)(17);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");
    
    CHECK(val1->interp()->equals(NEW(NumVal)(17)));
    CHECK(val2->interp()->equals(NEW(NumVal)(24)));
    
    CHECK_THROWS_WITH(var1->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH(var2->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((NEW(AddExpr)(val1, val2))->interp()->equals(NEW(NumVal)(41)));
    CHECK((NEW(AddExpr)(val1, val3))->interp()->equals(NEW(NumVal)(34)));
    CHECK_THROWS_WITH((NEW(AddExpr)(val1, var1))->interp(),
                      "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH((NEW(AddExpr)(var2, val2))->interp(),
                      "Variable(s) exist(s) in this expression");
    CHECK((NEW(AddExpr)(NEW(NumExpr)(1), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(3)), NEW(NumExpr)(1), NEW(NumExpr)(0))))
          ->interp()->equals(NEW(NumVal)(2)));
    
    CHECK((NEW(MultExpr)(val1, val2))->interp()->equals(NEW(NumVal)(408)));
    CHECK((NEW(MultExpr)(val1, val3))->interp()->equals(NEW(NumVal)(289)));
    CHECK_THROWS_WITH((NEW(MultExpr)(val1, var1))->interp(),
                      "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH((NEW(MultExpr)(var2, val2))->interp(),
                      "Variable(s) exist(s) in this expression");
    
    CHECK((NEW(LetExpr)("x", val1, val2))->interp()->equals(NEW(NumVal)(24)));
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->interp()->equals(NEW(NumVal)(41)));
    CHECK((NEW(LetExpr)("x", val1, NEW(LetExpr)("y", val2, NEW(AddExpr)(var1, val1))))->interp()->equals(NEW(NumVal)(34)));
    CHECK((NEW(LetExpr)("y", NEW(AddExpr)(val1, val2), NEW(AddExpr)(var2, NEW(NumExpr)(1))))->interp()->equals(NEW(NumVal)(42)));
    CHECK((NEW(LetExpr)("x", val1, NEW(LetExpr)("x", NEW(AddExpr)(var1, val2), NEW(AddExpr)(var1, NEW(NumExpr)(2)))))->interp()->equals(NEW(NumVal)(43)));
    CHECK((NEW(LetExpr)("x", val1, NEW(LetExpr)("x", val2, NEW(AddExpr)(var1, NEW(NumExpr)(2)))))
          ->interp()->equals(NEW(NumVal)(26)));
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(17), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(24), NEW(AddExpr)(var2, NEW(NumExpr)(2))), var1)))->interp()->equals(NEW(NumVal)(43)));
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(IfExpr)(NEW(EqExpr)(var1, NEW(NumExpr)(3)), NEW(NumExpr)(1), NEW(NumExpr)(0))))->interp()->equals(NEW(NumVal)(1)));
    CHECK((NEW(LetExpr)("x",NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(3)),NEW(MultExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x"))))->interp()->equals(NEW(NumVal)(25)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))->interp()->equals(NEW(NumVal)(6)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(NumExpr)(7)), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))->interp()->equals(NEW(NumVal)(7)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(BoolExpr)(true)), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))->interp()->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(BoolExpr)(true))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))->interp(), "Add of non-number error");
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(BoolExpr)(true))), NEW(AddExpr)(NEW(NumExpr)(5), NEW(NumExpr)(1))))->interp()->equals(NEW(NumVal)(6)));
    CHECK_THROWS_WITH((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(NumExpr)(7)), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(AddExpr)(NEW(NumExpr)(5), NEW(BoolExpr)(true)))))->interp(), "Add of non-number error");
    CHECK_THROWS_WITH((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(AddExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))->interp(), "Add of non-number error");
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(IfExpr)(NEW(BoolExpr)(false), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(6)))))
          ->interp()->equals(NEW(NumVal)(7)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(AddExpr)(var2, NEW(NumExpr)(2))), NEW(IfExpr)(NEW(BoolExpr)(true), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5))))))
          ->interp()->equals(NEW(NumVal)(6)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(AddExpr)(var2, NEW(NumExpr)(2))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5))))))
          ->interp()->equals(NEW(NumVal)(8)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(CallExpr)(NEW(VarExpr)("f"), NEW(AddExpr)(var2, NEW(NumExpr)(2)))), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5)))))
          ->interp()->equals(NEW(NumVal)(8)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("x", NEW(AddExpr)(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(2)), var1)), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5)))))
          ->interp()->equals(NEW(NumVal)(8)));
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(VarExpr)("f")))
          ->interp()->equals(NEW(FunVal)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1)))));
    
    CHECK((NEW(AddExpr)(NEW(MultExpr)(val1, NEW(LetExpr)("x", NEW(NumExpr)(1), var1)), val2))->interp()->equals(NEW(NumVal)(41)));
    CHECK_THROWS_WITH((NEW(LetExpr)("x", NEW(AddExpr)(var1, val1), NEW(AddExpr)(var1, val2)))->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((NEW(BoolExpr)(true))->interp()->equals(NEW(BoolVal)(true)) == true);
    CHECK((NEW(BoolExpr)(false))->interp()->equals(NEW(BoolVal)(true)) == false);
    
    CHECK((NEW(EqExpr)(val1, val2))->interp()->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(val1, NEW(NumExpr)(17)))->interp()->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(AddExpr)(val1, val2), NEW(NumExpr)(41)))->interp()->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(AddExpr)(val1, val2), NEW(MultExpr)(val1, val2)))->interp()->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2))))
          ->interp()->equals(NEW(BoolVal)(true)));
    CHECK_THROWS_WITH((NEW(EqExpr)(var1, NEW(VarExpr)("x")))->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp()->equals(NEW(NumVal)(1)));
    CHECK((NEW(IfExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp()->equals(NEW(NumVal)(2)));
    CHECK((NEW(IfExpr)(NEW(EqExpr)(val1, NEW(NumExpr)(17)), NEW(AddExpr)(val2, NEW(NumExpr)(24)), NEW(NumExpr)(5)))
          ->interp()->equals(NEW(NumVal)(48)));
    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(AddExpr)(val1, val2), NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp(), "Not a boolean value error");
    CHECK_THROWS_WITH((NEW(IfExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), NEW(NumExpr)(1), NEW(NumExpr)(2)))->interp(), "Not a boolean value error");
    
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)))->interp()->equals((NEW(FunVal)("x", NEW(AddExpr)(var1, val1)))));
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), val2))->interp()->equals(NEW(NumVal)(41)));
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(NumExpr)(5)))->interp()->equals(NEW(NumVal)(6)));
}

TEST_CASE("subst") {
    PTR(NumExpr) val1 = NEW(NumExpr)(17);
    PTR(NumExpr) val2 = NEW(NumExpr)(24);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");
    PTR(VarExpr) var3 = NEW(VarExpr)("X");
    
    CHECK(val1->subst("x", NEW(NumExpr)(24))->equals(val1));
    CHECK(val2->subst("y", NEW(NumExpr)(17))->equals(val2));
    
    CHECK(var1->subst("x", NEW(VarExpr)("y"))->equals(var2));
    CHECK(var2->subst("x", NEW(VarExpr)("X"))->equals(var2));
    CHECK(var1->subst("x", NEW(NumExpr)(17))->equals(val1));
    CHECK(var2->subst("y", NEW(AddExpr)(NEW(NumExpr)(17), NEW(NumExpr)(24)))->equals(NEW(AddExpr)(val1, val2)));
    
    CHECK((NEW(AddExpr)(val1, val2))->subst("x", NEW(VarExpr)("x"))->equals(NEW(AddExpr)(val1, val2)));
    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), val1))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), val1)));
    CHECK((NEW(AddExpr)(val2, NEW(VarExpr)("x")))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(AddExpr)(val2, NEW(VarExpr)("y"))));
    CHECK((NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("x")))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))));
    CHECK((NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y")))->subst("Y", NEW(VarExpr)("X"))
          ->equals(NEW(AddExpr)(NEW(VarExpr)("y"), NEW(VarExpr)("y"))));
    CHECK((NEW(AddExpr)(var1, (NEW(AddExpr)(val2, var2))))->subst("x", NEW(VarExpr)("X"))
          ->equals(NEW(AddExpr)(NEW(VarExpr)("X"), NEW(AddExpr)(val2, var2))));
    CHECK((NEW(AddExpr)(var1, (NEW(AddExpr)(val2, var1))))->subst("x", NEW(VarExpr)("X"))
          ->equals(NEW(AddExpr)(NEW(VarExpr)("X"), NEW(AddExpr)(val2, NEW(VarExpr)("X")))));
    
    CHECK((NEW(MultExpr)(val1, val2))->subst("x", NEW(VarExpr)("x"))->equals(NEW(MultExpr)(val1, val2)));
    CHECK((NEW(MultExpr)(var1, val1))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(MultExpr)(var2, val1)));
    CHECK((NEW(MultExpr)(val2, var1))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(MultExpr)(val2, var2)));
    CHECK((NEW(MultExpr)(var1, var1))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(MultExpr)(var2, var2)));
    CHECK((NEW(MultExpr)(var2, var2))->subst("Y", NEW(VarExpr)("X"))
          ->equals(NEW(MultExpr)(var2, var2)));
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val2, var2), var1))->subst("x", NEW(VarExpr)("X"))
          ->equals(NEW(MultExpr)(NEW(MultExpr)(val2, var2), var3)));
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val2, var1), var1))->subst("x", NEW(VarExpr)("X"))
          ->equals(NEW(MultExpr)(NEW(MultExpr)(val2, var3), var3)));
    
    CHECK((NEW(LetExpr)("x", val1, var1))->subst("x", NEW(VarExpr)("y"))
          ->equals(NEW(LetExpr)("x", val1, var1)));
    CHECK((NEW(LetExpr)("x", val1, var1))->subst("y", NEW(VarExpr)("X"))
          ->equals(NEW(LetExpr)("x", val1, var1)));
    CHECK((NEW(LetExpr)("x", NEW(AddExpr)(var1, val1), NEW(AddExpr)(var1, val2)))->subst("x", NEW(VarExpr)("y"))->equals(NEW(LetExpr)("x", NEW(AddExpr)(var2, val1), NEW(AddExpr)(var1, val2))));
    CHECK((NEW(LetExpr)("y", NEW(VarExpr)("x"), NEW(VarExpr)("x")))->subst("x", NEW(NumExpr)(3))->equals(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(NumExpr)(3))));
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))))
        ->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))
        ->equals(NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
    CHECK((NEW(LetExpr)("x", NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))))
          ->subst("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)))
          ->equals(NEW(LetExpr)("x", NEW(AddExpr)(NEW(VarExpr)("y"), NEW(NumExpr)(3)), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
    CHECK((NEW(LetExpr)("x", NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2))))
          ->subst("y", NEW(NumExpr)(8))
          ->equals(NEW(LetExpr)("x", NEW(NumExpr)(8), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(2)))));
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(8), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(2), NEW(VarExpr)("y")))))->subst("y", NEW(NumExpr)(9))
        ->equals(NEW(LetExpr)("x", NEW(NumExpr)(8), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(9))))));
    CHECK((NEW(LetExpr)("x", NEW(VarExpr)("y"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"))))->subst("y", NEW(NumExpr)(8))
        ->equals(NEW(LetExpr)("x", NEW(NumExpr)(8), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(8)))));
    CHECK((NEW(LetExpr)("z", NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(32))))->subst("z", NEW(NumExpr)(0))
        ->equals(NEW(LetExpr)("z", NEW(VarExpr)("x"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(32)))));
    CHECK((NEW(LetExpr)("z", NEW(VarExpr)("z"), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(32))))->subst("z", NEW(NumExpr)(0))
        ->equals(NEW(LetExpr)("z", NEW(NumExpr)(0), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(32)))));
    CHECK((NEW(LetExpr)("z", NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(32))))->subst("z", NEW(NumExpr)(0))
        ->equals(NEW(LetExpr)("z", NEW(AddExpr)(NEW(NumExpr)(0), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(32)))));
//    CHECK((NEW(LetExpr)("factrl", NEW(FunExpr)("factrl", NEW(FunExpr)("x", NEW(IfExpr)(NEW(EqExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)), NEW(NumExpr)(1), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))))), NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(NumExpr)(10)))))->interp()->equals(NEW(NumVal)(3628800)));
//    _let factrl = _fun (factrl)
//                    _fun (x)
//                      _if x == 1
//                      _then 1
//                      _else x * factrl(factrl)(x + -1)
//    _in  factrl(factrl)(10)
    
    CHECK((NEW(BoolExpr)(true))->subst("x", NEW(VarExpr)("y"))->equals(NEW(BoolExpr)(true)));
    CHECK((NEW(BoolExpr)(false))->subst("y", NEW(NumExpr)(24))->equals(NEW(BoolExpr)(false)));
    
    CHECK((NEW(EqExpr)(val1, val2))->subst("x", NEW(NumExpr)(13))->equals(NEW(EqExpr)(NEW(NumExpr)(17), NEW(NumExpr)(24))));
    CHECK((NEW(EqExpr)(var1, val2))->subst("x", NEW(NumExpr)(13))->equals(NEW(EqExpr)(NEW(NumExpr)(13), NEW(NumExpr)(24))));
    CHECK((NEW(EqExpr)(NEW(AddExpr)(var1, NEW(NumExpr)(2)), NEW(MultExpr)(NEW(NumExpr)(2), var1)))->subst("x", NEW(NumExpr)(2))
          ->equals(NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)))));
    
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)))->subst("x", NEW(NumExpr)(24))->equals((NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)))));
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(MultExpr)(var2, val1))))->subst("y", val2)->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(NumExpr)(24), NEW(NumExpr)(17))))));
    
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), val2))->subst("x", NEW(NumExpr)(1))->equals((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))), NEW(NumExpr)(24)))));
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(MultExpr)(var2, val1))), NEW(AddExpr)(var2, val2)))->subst("y", NEW(NumExpr)(17))->equals((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(MultExpr)(NEW(NumExpr)(17), NEW(NumExpr)(17)))), NEW(AddExpr)(NEW(NumExpr)(17), NEW(NumExpr)(24))))));
}

TEST_CASE("add_to") {
    CHECK_THROWS_WITH((NEW(AddExpr)(NEW(NumExpr)(17),NEW(NumExpr)(24)))->interp()->add_to(NULL), "Add of non-number error");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->add_to(NEW(NumVal)(17)), "Add of non-number error");
    CHECK_THROWS_WITH((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))))->add_to(NEW(NumVal)(17)), "Add of non-number error");
}

TEST_CASE("mult_by") {
    CHECK_THROWS_WITH( (NEW(AddExpr)(NEW(NumExpr)(24),NEW(NumExpr)(17)))->interp()->mult_by(NULL), "Mult of non-number error");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->mult_by(NEW(NumVal)(17)), "Mult of non-number error");
    CHECK_THROWS_WITH((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))))->mult_by(NEW(NumVal)(17)), "Mult of non-number error");
}

TEST_CASE("other test cases for Val class") {
    CHECK((NEW(BoolVal)(true))->to_expr()->equals(NEW(BoolExpr)(true)));
    CHECK((NEW(BoolVal)(false))->to_expr()->equals(NEW(BoolExpr)(false)));
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))))->to_expr()->equals(NEW(FunExpr)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17)))));
    CHECK_THROWS_WITH((NEW(NumVal)(17))->call(NEW(NumVal)(24)), "Not a function to be called error");
    CHECK_THROWS_WITH((NEW(BoolVal)(true))->call(NEW(NumVal)(24)), "Not a function to be called error");
    CHECK((NEW(FunVal)("x", NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))))->call(NEW(NumVal)(24))->equals(NEW(NumVal)(41)));
    CHECK((NEW(FunVal)("x", NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(17))))->call(NEW(NumVal)(24))->equals(NEW(NumVal)(408)));
}

TEST_CASE("print") {
    PTR(NumExpr) val1 = NEW(NumExpr)(17);
    PTR(NumExpr) val2 = NEW(NumExpr)(24);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");
     
    std::string toString;
    
    //Num tests
    toString = "17";
    CHECK(val1->to_str() == toString);
    
    toString = "24";
    CHECK(val2->to_str() == toString);
    
    //Variable tests
    toString = "x";
    CHECK(var1->to_str() == toString);
    
    toString = "y";
    CHECK(var2->to_str() == toString);
    
    //Add tests
    toString = "(17+24)";
    CHECK((NEW(AddExpr)(val1, val2))->to_str() == toString);
    
    toString = "(x+y)";
    CHECK((NEW(AddExpr)(var1, var2))->to_str() == toString);
 
    toString = "(17+x)";
    CHECK((NEW(AddExpr)(val1, var1))->to_str() == toString);
    
    toString = "((17+x)+24)";
    CHECK((NEW(AddExpr)(NEW(AddExpr)(val1, var1), val2))->to_str() == toString);
    
    toString = "((17*x)+24)";
    CHECK((NEW(AddExpr)(NEW(MultExpr)(val1, var1), val2))->to_str() == toString);
 
    toString = "(x+(y+24))";
    CHECK((NEW(AddExpr)(var1, NEW(AddExpr)(var2, val2)))->to_str() == toString);
 
    toString = "(x+(y*24))";
    CHECK((NEW(AddExpr)(var1, NEW(MultExpr)(var2, val2)))->to_str() == toString);
 
    toString = "((17+x)+(24+y))";
    CHECK((NEW(AddExpr)(NEW(AddExpr)(val1, var1), NEW(AddExpr)(val2, var2)))->to_str() == toString);

    toString = "((17*x)+(24*y))";
    CHECK((NEW(AddExpr)(NEW(MultExpr)(val1, var1), NEW(MultExpr)(val2, var2)))->to_str() == toString);
    
    //Mult tests
    toString = "(17*24)";
    CHECK((NEW(MultExpr)(val1, val2))->to_str() == toString);
    
    toString = "(x*y)";
    CHECK((NEW(MultExpr)(var1, var2))->to_str() == toString);
 
    toString = "(24*y)";
    CHECK((NEW(MultExpr)(val2, var2))->to_str() == toString);
    
    toString = "((17*x)*24)";
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val1, var1), val2))->to_str() == toString);
    
    toString = "((17+x)*24)";
    CHECK((NEW(MultExpr)(NEW(AddExpr)(val1, var1), val2))->to_str() == toString);
 
    toString = "(x*(y*24))";
    CHECK((NEW(MultExpr)(var1, NEW(MultExpr)(var2, val2)))->to_str() == toString);
    
    toString = "(x*(y+24))";
    CHECK((NEW(MultExpr)(var1, NEW(AddExpr)(var2, val2)))->to_str() == toString);
 
    toString = "((17+x)*(24+y))";
    CHECK((NEW(MultExpr)(NEW(AddExpr)(val1, var1), NEW(AddExpr)(val2, var2)))->to_str() == toString);
    
    toString = "((17*x)*(24*y))";
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val1, var1), NEW(MultExpr)(val2, var2)))->to_str() == toString);
    
    //_let tests
    toString = "(_let x=17 _in (x+24))";
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->to_str() == toString);
    
    toString = "(_let x=5 _in ((_let y=3 _in (y+2))+x))";
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)((NEW(LetExpr)("y", NEW(NumExpr) (3), NEW(AddExpr)(var2, NEW(NumExpr)(2)))), var1)))->to_str() == toString);
    
    toString = "_true";
    CHECK((NEW(BoolExpr)(true))->to_str() == toString);
    
    toString = "_false";
    CHECK((NEW(BoolExpr)(false))->to_str() == toString);
    
    toString = "(x==17)";
    CHECK((NEW(EqExpr)(var1, val1))->to_str() == toString);
    
    toString = "((1+2)==3)";
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_str() == toString);
    
    toString = "((_let x=17 _in x)==24)";
    CHECK((NEW(EqExpr)(NEW(LetExpr)("x", val1, var1), val2))->to_str() == toString);
    
    toString = "(_if (x==17) _then 1 _else 0)";
    CHECK((NEW(IfExpr)(NEW(EqExpr)(var1, val1), NEW(NumExpr)(1), NEW(NumExpr)(0)))->to_str() == toString);
    
    toString = "(_fun (x) (x+17))";
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)))->to_str() == toString);
    
    toString = "(_fun (x) (x+17))(24)";
    CHECK((NEW(CallExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), val2))->to_str() == toString);
}

TEST_CASE("pretty_print") {
    PTR(NumExpr) val1 = NEW(NumExpr)(17);
    PTR(NumExpr) val2 = NEW(NumExpr)(24);
    PTR(VarExpr) var1 = NEW(VarExpr)("x");
    PTR(VarExpr) var2 = NEW(VarExpr)("y");

    std::string toPrettyStr;

    //Num tests
    toPrettyStr = "17";
    CHECK(val1->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "24";
    CHECK(val2->to_pretty_str() == toPrettyStr);
    
    //Variable tests
    toPrettyStr = "x";
    CHECK(var1->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "y";
    CHECK(var2->to_pretty_str() == toPrettyStr);

    //Add tests
    toPrettyStr = "17 + 24";
    CHECK((NEW(AddExpr)(val1, val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x + y";
    CHECK((NEW(AddExpr)(var1, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "24 + y";
    CHECK((NEW(AddExpr)(val2, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) + 24";
    CHECK((NEW(AddExpr)(NEW(AddExpr)(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x + y + 24";
    CHECK((NEW(AddExpr)(var1, NEW(AddExpr)(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24";
    CHECK((NEW(AddExpr)(NEW(MultExpr)(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x + y * 24";
    CHECK((NEW(AddExpr)(var1, NEW(MultExpr)(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) + 24 + y";
    CHECK((NEW(AddExpr)(NEW(AddExpr)(val1, var1), NEW(AddExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24 * y";
    CHECK((NEW(AddExpr)(NEW(MultExpr)(val1, var1), NEW(MultExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) + 24 * y";
    CHECK((NEW(AddExpr)(NEW(AddExpr)(val1, var1), NEW(MultExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24 + y";
    CHECK((NEW(AddExpr)(NEW(MultExpr)(val1, var1), NEW(AddExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    //Mult tests
    toPrettyStr = "17 * 24";
    CHECK((NEW(MultExpr)(val1, val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x * y";
    CHECK((NEW(MultExpr)(var1, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "24 * y";
    CHECK((NEW(MultExpr)(val2, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) * 24";
    CHECK((NEW(MultExpr)(NEW(AddExpr)(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x * (y + 24)";
    CHECK((NEW(MultExpr)(var1, NEW(AddExpr)(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 * x) * 24";
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x * y * 24";
    CHECK((NEW(MultExpr)(var1, NEW(MultExpr)(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) * (24 + y)";
    CHECK((NEW(MultExpr)(NEW(AddExpr)(val1, var1), NEW(AddExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 * x) * 24 * y";
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val1, var1), NEW(MultExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) * 24 * y";
    CHECK((NEW(MultExpr)(NEW(AddExpr)(val1, var1), NEW(MultExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 * x) * (24 + y)";
    CHECK((NEW(MultExpr)(NEW(MultExpr)(val1, var1), NEW(AddExpr)(val2, var2)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 * 1 * 7 * _let x = 3\n            _in  _let x = 5\n                 _in  x + _let x = 5\n                          _in  x * 1";
    CHECK((NEW(MultExpr)(NEW(NumExpr)(3), NEW(MultExpr)(NEW(NumExpr)(1), NEW(MultExpr)(NEW(NumExpr)(7), NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))->to_pretty_str() == toPrettyStr);
    
    //_let tests
    toPrettyStr = "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x";
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)(var2, NEW(NumExpr)(2))), var1)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let x = 1\n_in  _let x = 2\n     _in  _let x = 3\n          _in  x + 4";
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(1), NEW(LetExpr)("x", NEW(NumExpr)(2), NEW(LetExpr)("x", NEW(NumExpr)(3), NEW(AddExpr)(var1, NEW(NumExpr)(4))))))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let x = 17\n_in  x + 24";
    CHECK((NEW(LetExpr)("x", val1, NEW(AddExpr)(var1, val2)))->to_pretty_str()== toPrettyStr);

    toPrettyStr = "5 * (_let x = 5\n     _in  x) + 1";
    CHECK((NEW(AddExpr)(NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x"))), NEW(NumExpr)(1)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "5 * _let x = 5\n    _in  x + 1";
    CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(var1, NEW(NumExpr)(1)))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 + 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1";
    CHECK((NEW(AddExpr)(NEW(NumExpr)(3),(NEW(MultExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 + 7 + _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1";
    CHECK((NEW(AddExpr)(NEW(NumExpr)(3),(NEW(AddExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_true";
    CHECK((NEW(BoolExpr)(true))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_false";
    CHECK((NEW(BoolExpr)(false))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x == 17";
    CHECK((NEW(EqExpr)(var1, val1))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(1 == 2) == 3";
    CHECK((NEW(EqExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(1 + 2) == 3";
    CHECK((NEW(EqExpr)(NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(NumExpr)(3)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 == 1 + 2";
    CHECK((NEW(EqExpr)(NEW(NumExpr)(3), NEW(AddExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "4 == 2 * 2";
    CHECK((NEW(EqExpr)(NEW(NumExpr)(4), NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "2 * 2 == 4";
    CHECK((NEW(EqExpr)(NEW(MultExpr)(NEW(NumExpr)(2), NEW(NumExpr)(2)), NEW(NumExpr)(4)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_let x = 17\n _in  x) == 24";
    CHECK((NEW(EqExpr)(NEW(LetExpr)("x", val1, var1), val2))->to_pretty_str() == toPrettyStr);

    //_if tests
    toPrettyStr = "_if x == 17\n_then 1\n_else 0";
    CHECK((NEW(IfExpr)(NEW(EqExpr)(var1, val1), NEW(NumExpr)(1), NEW(NumExpr)(0)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_if x == 17\n _then 1\n _else 0) == 17";
    CHECK((NEW(EqExpr)(NEW(IfExpr)(NEW(EqExpr)(var1, val1), NEW(NumExpr)(1), NEW(NumExpr)(0)), val1))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let same = 1 == 2\n_in  _if 1 == 2\n     _then _false + 5\n     _else 88";
    CHECK((NEW(LetExpr)("same", NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)), NEW(NumExpr)(88))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_let x = 17\n _in  x) * 24";
    CHECK((NEW(MultExpr)(NEW(LetExpr)("x", val1, var1), val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let x = 1 == 2\n_in  _let same = x\n     _in  _if 3 == 4\n          _then _false\n          _else _true";
    CHECK((NEW(LetExpr)("x", NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(LetExpr)("same", NEW(VarExpr)("x"), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4)), NEW(BoolExpr)(false), NEW(BoolExpr)(true)))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_let x = 1 == 2\n _in  _let same = x\n      _in  _if 3 == 4\n           _then _false\n           _else _true) + 17";
    CHECK((NEW(AddExpr)(NEW(LetExpr)("x", NEW(EqExpr)(NEW(NumExpr)(1), NEW(NumExpr)(2)), NEW(LetExpr)("same", NEW(VarExpr)("x"), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(3), NEW(NumExpr)(4)), NEW(BoolExpr)(false), NEW(BoolExpr)(true)))), val1))->to_pretty_str() == toPrettyStr);
    
    //_fun and call tests
    toPrettyStr = "_fun (x)\n  x + 1";
    CHECK((NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_fun (x)\n  _true";
    CHECK((NEW(FunExpr)("x", NEW(BoolExpr)(true)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_fun (x)\n  x + _true";
    CHECK((NEW(FunExpr)("x",NEW(AddExpr)(var1, NEW(BoolExpr)(true))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_fun (x)\n   x + 17) * 24";
    CHECK((NEW(MultExpr)(NEW(FunExpr)("x", NEW(AddExpr)(var1, val1)), val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  f(5)";
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _if _false\n     _then f(5)\n     _else f(6)";
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(IfExpr)(NEW(BoolExpr)(false), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(6)))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                y + 2\n     _in  _if _true\n          _then f(5)\n          _else g(5)";
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(AddExpr)(var2, NEW(NumExpr)(2))), NEW(IfExpr)(NEW(BoolExpr)(true), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(5)), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5))))))
          ->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                y + 2\n     _in  f(g(5))";
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(AddExpr)(var2, NEW(NumExpr)(2))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5))))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                f(y + 2)\n     _in  g(5)";
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("y", NEW(CallExpr)(NEW(VarExpr)("f"), NEW(AddExpr)(var2, NEW(NumExpr)(2)))), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5)))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (x)\n                f(2) + x\n     _in  g(5)";
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(AddExpr)(var1, NEW(NumExpr)(1))), NEW(LetExpr)("g", NEW(FunExpr)("x", NEW(AddExpr)(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(2)), var1)), NEW(CallExpr)(NEW(VarExpr)("g"), NEW(NumExpr)(5)))))->to_pretty_str() == toPrettyStr);
    
}

TEST_CASE("parse") {
    //interp tests
    CHECK(parse_str("17")->interp()->equals(NEW(NumVal)(17)));
    CHECK_THROWS_WITH(parse_str("x")->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH(parse_str("17 + x")->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH(parse_str("24 * y")->interp(), "Variable(s) exist(s) in this expression");
    CHECK(parse_str("17 +     -24")->interp()->equals(NEW(NumVal)(-7)));
    CHECK(parse_str("24   +   17")->interp()->equals(NEW(NumVal)(41)));
    CHECK(parse_str("   17 *  24")->interp()->equals(NEW(NumVal)(408)));
    CHECK(parse_str("  -17 *  24")->interp()->equals(NEW(NumVal)(-408)));
    CHECK(parse_str("17 + 17 * 24")->interp()->equals(NEW(NumVal)(425)));
    CHECK(parse_str("17 * 17 + 24")->interp()->equals(NEW(NumVal)(313)));
    CHECK(parse_str("(17 + 17) * (24 + 24)")->interp()->equals(NEW(NumVal)(1632)));
    CHECK(parse_str("17 * 17 + 24 * 24")->interp()->equals(NEW(NumVal)(865)));
    CHECK(parse_str("_let x = 17 _in x + 24")->interp()->equals(NEW(NumVal)(41)));
    CHECK_THROWS_WITH(parse_str("(17 + 17) * (24 + 24")->interp(),"Missing close parenthesis");
    CHECK_THROWS_WITH(parse_str("x = 17")->interp(), "Missing the second equal sign");
    CHECK_THROWS_WITH(parse_str("_nope x = 17 _in x + 24")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_lem x = 17 _in x + 24")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_let x = 17 _int x + 24")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_let x = 17 in x + 24")->interp(), "Consume mismatch");
    CHECK_THROWS_WITH(parse_str("_in x = 17 _let x + 24")->interp(), "E1 invalid input");
    CHECK_THROWS_WITH(parse_str("_let x = 17 _let x + 24")->interp(), "E4 invalid input");
    CHECK_THROWS_WITH(parse_str("_let x + 17 _in x + 24")->interp(), "Consume mismatch");
    CHECK_THROWS_WITH(parse_str("*let x + 17 _in x + 24")->interp(), "E2 invalid input");
    CHECK_THROWS_WITH(parse_str("-x")->interp(), "Invalid input");
    
    CHECK(parse_str("_if 17 == 17 _then 1 _else 0")->interp()->equals(NEW(NumVal)(1)));
    CHECK_THROWS_WITH(parse_str("_in x == 17 _then 1 _else 0")->interp(), "E1 invalid input");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _than 1 _else 0")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _then 1 _els 0")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_if x == 17 *then 1 _else 0")->interp(), "Consume mismatch");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _then 1 -else 0")->interp(), "Consume mismatch");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _else 1 _else 0")->interp(), "E6 invalid input");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _then 1 _then 0")->interp(), "E7 invalid input");
    
    CHECK(parse_str("_if _true _then 1 _else 0")->interp()->equals(NEW(NumVal)(1)));
    CHECK(parse_str("_if _false _then 1 _else 0")->interp()->equals(NEW(NumVal)(0)));
    CHECK_THROWS_WITH(parse_str("_if _tru _then 1 _else 0")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_if _fals _then 1 _else 0")->interp(),"Invalid keyword");

    CHECK(parse_str("_let f = (_fun (x) (x+1)) _in (f)(5)")->interp()->equals(NEW(NumVal)(6)));
    
//    CHECK_THROWS_WITH(parse_str("_let factrl = (_fun (factrl) (_fun (x) _if x == 1 _then 1 _else (x * factrl(factrl)(x + -1)))) _in  factrl(factrl)(10)")->interp(), "Invalid keyword");
    
    //print tests
    std::string toString;
    
    toString = "((17+x)+24)";
    CHECK(parse_str("(17+x)+24")->to_str() == toString);
    
    toString = "((17*x)+24)";
    CHECK(parse_str("17*x+24")->to_str() == toString);
    
    toString = "(x+(y+24))";
    CHECK(parse_str("x+y+24")->to_str() == toString);
 
    toString = "(x+(y*24))";
    CHECK(parse_str("x+y*24")->to_str() == toString);
 
    toString = "((17+x)+(24+y))";
    CHECK(parse_str("(17+x)+24+y")->to_str() == toString);

    toString = "((17*x)+(24*y))";
    CHECK(parse_str("17*x+24*y")->to_str() == toString);
    
    toString = "(24*y)";
    CHECK(parse_str("24*y")->to_str() == toString);
    
    toString = "((17*x)*24)";
    CHECK(parse_str("(17*x)*24)")->to_str() == toString);
    
    toString = "((17+x)*24)";
    CHECK(parse_str("(17+x)*24")->to_str() == toString);
 
    toString = "(x*(y*24))";
    CHECK(parse_str("x*y*24")->to_str() == toString);
    
    toString = "(x*(y+24))";
    CHECK(parse_str("x*(y+24)")->to_str() == toString);
 
    toString = "((17+x)*(24+y))";
    CHECK(parse_str("(17+x)*(24+y)")->to_str() == toString);
    
    toString = "((17*x)*(24*y))";
    CHECK(parse_str("(17*x)*24*y")->to_str() == toString);
    
    toString = "(_let x=17 _in (x+24))";
    CHECK(parse_str("_let x   = 17 _in   x+   24")->to_str() == toString);
    
    toString = "(_let x=5 _in ((_let y=3 _in (y+2))+x))";
    CHECK(parse_str("_let x = 5 _in (_let y=3 _in y+2) + x")->to_str() == toString);
    
    toString = "(5*(-2*(_let v=7 _in (8+(j+(3+(12+-24)))))))";
    CHECK(parse_str("5*-2* _let v=7 _in (8+(j+(3+(12 + -24))))")->to_str() == toString);
    
    toString = "(1851846184*(-463971904*(_let v=244238942 _in (854292678+(j+(1420341118+(1731904646+-184682208)))))))";
    CHECK(parse_str("1851846184*-463971904*_let v=244238942 _in 854292678+j+1420341118+(1731904646+-184682208)")->to_str() == toString);
    
    toString = ("_false");
    CHECK(parse_str("(_false)")->to_str() == toString);
    
    toString = "(x==17)";
    CHECK(parse_str("x  ==    17")->to_str() == toString);
    
    toString = "((1+2)==3)";
    CHECK(parse_str("  1 +     2 ==   3")->to_str() == toString);
    
    toString = "((_let x=17 _in x)==24)";
    CHECK(parse_str("(_let x = 17 _in x) == 24")->to_str() == toString);
    
    toString = "(_if (x==17) _then 1 _else 0)";
    CHECK(parse_str("_if     x    ==17 _then   1 _else 0")->to_str() == toString);
   
    toString = "(_fun (x) (x+17))";
    CHECK(parse_str("_fun    (  x  )   x    + 17")->to_str() == toString);
    
    toString = "((_fun (x) (x+17))*24)";
    CHECK(parse_str("((_fun (x)   x + 17) * 24)")->to_str() == toString);
    
    toString = "(_let f=(_fun (x) (x+1)) _in f(5))";
    CHECK(parse_str("_let f    = _fun (x) (x + 1) _in  f(5)")->to_str() == toString);
   
    toString = "(_let f=(_fun (x) (x+1)) _in (_if _false _then f(5) _else f(6)))";
    CHECK(parse_str("_let f = _fun (x)\n           x + 1\n_in  _if _false\n     _then f(5)\n     _else f(6)")->to_str() == toString);

    toString = "(_let f=(_fun (x) (x+1)) _in (_let g=(_fun (y) (y+2)) _in (_if _true _then f(5) _else g(5))))";
    CHECK(parse_str("_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                y + 2\n     _in  _if _true\n          _then f(5)\n          _else g(5)")->to_str() == toString);

    toString = "(_let f=(_fun (x) (x+1)) _in (_let g=(_fun (y) (y+2)) _in f(g(5))))";
    CHECK(parse_str("_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                y + 2\n     _in  f(g(5))")->to_str() == toString);
    
    //pretty_print tests
    std::string toPrettyStr;
    toPrettyStr = "(17 + x) + 24";
    CHECK(parse_str("(17+x)+24")->to_pretty_str() == toPrettyStr);
     
    toPrettyStr = "17 * x + 24";
    CHECK(parse_str("17*x+24")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x + y + 24";
    CHECK(parse_str("x+y+24")->to_pretty_str() == toPrettyStr);
 
    toPrettyStr = "x + y * 24";
    CHECK(parse_str("x+y*24")->to_pretty_str() == toPrettyStr);
 
    toPrettyStr = "(17 + x) + 24 + y";
    CHECK(parse_str("(17+x)+(24+y)")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24 * y";
    CHECK(parse_str("(17*x)+(24*y)")->to_pretty_str() == toPrettyStr);
        
    toPrettyStr = "(17 * x) * 24";
    CHECK(parse_str("(17*x)*24)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(17 + x) * 24";
    CHECK(parse_str("(17+x)*24")->to_pretty_str() == toPrettyStr);
 
    toPrettyStr = "x * y * 24";
    CHECK(parse_str("x*(y*24)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x * (y + 24)";
    CHECK(parse_str("x*(y+24)")->to_pretty_str() == toPrettyStr);
 
    toPrettyStr = "(17 + x) * (24 + y)";
    CHECK(parse_str("(17+x)*(24+y)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(17 * x) * 24 * y";
    CHECK(parse_str("(17*x)*(24*y)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x";
    CHECK(parse_str("_let x=5 _in (_let y=3 _in y+2) +x")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let x = 1\n_in  _let x = 2\n     _in  _let x = 3\n          _in  x + 4";
    CHECK(parse_str("_let x=1 _in _let x=2 _in _let x=3 _in x+4")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "5 * (_let x = 5\n     _in  x) + 1";
    CHECK(parse_str("(5 * _let x=5 _in x) + 1 ")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "5 * _let x = 5\n    _in  x + 1";
    CHECK(parse_str("5 * _let x=5 _in x+1")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "3 * 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1";
    CHECK(parse_str("3*7 * (_let x=3 _in (_let x=5 _in x+(_let x=5 _in x*1)))")
        ->to_pretty_str() == toPrettyStr);

    toPrettyStr = "3 + 7 + _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1";
    CHECK(parse_str("3 + 7 + (_let x=3 _in (_let x=5 _in x+(_let x=5 _in x*1)))")
        ->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "1851846184 * -463971904 * _let v = 244238942\n                          _in  854292678 + j + 1420341118 + 1731904646 + -184682208";
    CHECK(parse_str("1851846184*-463971904*_let v= 244238942 _in 854292678+j+1420341118+(1731904646+-184682208)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = ("_true");
    CHECK(parse_str("(_true)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x == 17";
    CHECK(parse_str("((x)==17)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(1 + 2) == 3";
    CHECK(parse_str("(1 +     2==3)")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(_let x = 17\n _in  x) == 24";
    CHECK(parse_str("(_let x = 17 _in x) == 24")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_if x == 17\n_then 1\n_else 0";
    CHECK(parse_str("(_if    (x    ==17) _then   (1) _else (0))")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let same = 1 == 2\n_in  _if 1 == 2\n     _then _false + 5\n     _else 88";
    CHECK(parse_str("(_let same = (1==2)   _in _if (1==2) _then (_false + 5) _else (88))")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_fun (x)\n  x + 17";
    CHECK(parse_str("(_fun (x) (x+17))")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_fun (x)\n   x + 17) * 24";
    CHECK(parse_str("((_fun (x) (x+17))*24)")->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  f(5)";
    CHECK(parse_str("(_let f=(_fun (x) (x+1)) _in f(5))")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _if _false\n     _then f(5)\n     _else f(6)";
    CHECK(parse_str("(_let f=(_fun (x) (x+1)) _in (_if _false _then f(5) _else f(6)))")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                y + 2\n     _in  _if _true\n          _then f(5)\n          _else g(5)";
    CHECK(parse_str("(_let f=(_fun (x) (x+1)) _in (_let g=(_fun (y) (y+2)) _in (_if _true _then f(5) _else g(5))))")->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let f = _fun (x)\n           x + 1\n_in  _let g = _fun (y)\n                y + 2\n     _in  f(g(5))";
    CHECK(parse_str("(_let f=(_fun (x) (x+1)) _in (_let g=(_fun (y) (y+2)) _in f(g(5))))")->to_pretty_str() == toPrettyStr);
    
//    toPrettyStr = "";
//    CHECK(parse_str("12(  13)(      _fun     (t   )     QM == 17   )(    24)")->to_pretty_str() == toPrettyStr);
    
//    toPrettyStr = "905162409(_fun (i)\n            mwFWMVKE)(_let f = -1873773093\n                      _in  71330000)";
//    CHECK(parse_str("905162409((_fun (i) mwFWMVKE))((_let f=-1873773093 _in 71330000))")->to_pretty_str() == toPrettyStr);
    
    //341188032(  48137588)(      _fun     (t   )     QM == 860682844   )(     2029259463)
    //905162409((_fun (i) mwFWMVKE))((_let f=-1873773093 _in 71330000))
//    CHECK(parse_str("_if     1311433591   _then  _if     X  _then       F  _else        -1625335828   *   _false _else    (_if     _true   ==    32309537_then _if     L  _then     _let   c =  1362618119 _in    (-977663374)   *   eQHMjGAEBk _else       -7896479  == (_if     -1725426414   _then     G   *         174797107    _else     _true)     _else    _true    +    -1128812070)   ==     MABowbMNz")->to_pretty_str() == " ");
}
