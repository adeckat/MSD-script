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

TEST_CASE("equals") {
    NumExpr* val1 = new NumExpr(17);
    NumExpr* val2 = new NumExpr(24);
    NumExpr* val3 = new NumExpr(17);
    NumExpr* val4 = new NumExpr(24);
    VarExpr* var1 = new VarExpr("x");
    VarExpr* var2 = new VarExpr("y");
    
    CHECK(val1->equals(new NumExpr(17)) == true);
    CHECK(val3->equals(new NumExpr(24)) == false);
    
    CHECK(var1->equals(new VarExpr("x")) == true);
    CHECK(var1->equals(new VarExpr("X")) == false);
    CHECK(var1->equals(var2) == false);

    CHECK(val1->equals(var1) == false);
    CHECK(var2->equals(val2) == false);
    
    CHECK((new AddExpr(val1, val2))->equals(new AddExpr(val3, val4)) == true);
    CHECK((new AddExpr(val1, val2))->equals(new AddExpr(val2, val1)) == false);
    CHECK((new AddExpr(val3, val4))->equals(new AddExpr(val4, val3)) == false);
    
    CHECK((new MultExpr(val1, val2))->equals(new MultExpr(val3, val4)) == true);
    CHECK((new MultExpr(val1, val2))->equals(new MultExpr(val2, val1)) == false);
    CHECK((new MultExpr(val3, val4))->equals(new MultExpr(val4, val3)) == false);
    
    CHECK((new AddExpr(val1, val2))->equals(new MultExpr(val1, val2)) == false);
    CHECK((new MultExpr(val1, val2))->equals(new AddExpr(val3, val4)) == false);
    
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->equals(new MultExpr(var1, val1)) == false);
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->equals(new LetExpr("x", new NumExpr(17), new AddExpr(new VarExpr("x"), new NumExpr(24)))) == true);
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->equals(new LetExpr("y", new NumExpr(17), new AddExpr(new VarExpr("x"), new NumExpr(24)))) == false);
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->equals(new LetExpr("x", new NumExpr(24), new AddExpr(new VarExpr("x"), new NumExpr(24)))) == false);
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->equals(new LetExpr("x", new NumExpr(17), new AddExpr(new VarExpr("y"), new NumExpr(24)))) == false);
    
    CHECK(var1->getStr() == "x");
    CHECK(var2->getStr() == "y");
    
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)) == true);
    CHECK((new BoolExpr(true))->equals(new BoolExpr(false)) == false);
    CHECK((new BoolExpr(false))->equals(new BoolExpr(false)) == true);
    CHECK((new BoolExpr(true))->equals(new NumExpr(24)) == false);
    CHECK((new BoolExpr(false))->equals(new VarExpr("x")) == false);
    CHECK((new BoolExpr(true))->equals(new AddExpr(val1, val2)) == false);
    CHECK((new BoolExpr(false))->equals(new MultExpr(val3, val4)) == false);
 
    CHECK((new EqExpr(val1, val2))->equals(new EqExpr(new NumExpr(17), new NumExpr(24))) == true);
    CHECK((new EqExpr(var1, var2))->equals(new EqExpr(new VarExpr("x"), new VarExpr("y")))== true);
    CHECK((new EqExpr(val1, var1))->equals(new EqExpr(new NumExpr(17), new VarExpr("x"))) == true);
    CHECK((new EqExpr(var1, val1))->equals(new EqExpr(new NumExpr(17), new VarExpr("x"))) == false);
    CHECK((new EqExpr(var1, val1))->equals(new AddExpr(var1, val1)) == false);
    
    CHECK((new IfExpr(var1, val1, val2))->equals(new IfExpr(new VarExpr("x"), new NumExpr(17), new NumExpr(24))) == true);
    CHECK((new IfExpr(new EqExpr(var1, val1), new NumExpr(1), new NumExpr(2)))->equals(new IfExpr(new EqExpr(new VarExpr("x"), new NumExpr(17)), new NumExpr(1), new NumExpr(2))) == true);
    CHECK((new IfExpr(new EqExpr(val1, var1), new NumExpr(1), new NumExpr(2)))->equals(new IfExpr(new EqExpr(new VarExpr("x"), new NumExpr(17)), new NumExpr(1), new NumExpr(2))) == false);
    CHECK((new IfExpr(var1, val1, val2))->equals(new LetExpr("x", new NumExpr(17), new NumExpr(24))) == false);
    
    CHECK((new NumVal(13))->equals(NULL) == false);
    CHECK((new NumVal(17))->equals(new NumVal(17)) == true);
    
    CHECK((new BoolVal(true))->equals(NULL) == false);
    CHECK((new BoolVal(true))->equals(new BoolVal(true)) == true);
    CHECK((new BoolVal(false))->equals(new BoolVal(false)) == true);
    CHECK((new BoolVal(true))->equals(new BoolVal(false)) == false);
    CHECK((new BoolVal(true))->to_expr()->equals(new BoolExpr(true)));
    CHECK((new BoolVal(false))->to_expr()->equals(new BoolExpr(false)));
}

TEST_CASE("interp") {
    NumExpr* val1 = new NumExpr(17);
    NumExpr* val2 = new NumExpr(24);
    NumExpr* val3 = new NumExpr(17);
    VarExpr* var1 = new VarExpr("x");
    VarExpr* var2 = new VarExpr("y");
    
    CHECK(val1->interp()->equals(new NumVal(17)));
    CHECK(val2->interp()->equals(new NumVal(24)));
    
    CHECK_THROWS_WITH(var1->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH(var2->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((new AddExpr(val1, val2))->interp()->equals(new NumVal(41)));
    CHECK((new AddExpr(val1, val3))->interp()->equals(new NumVal(34)));
    CHECK_THROWS_WITH((new AddExpr(val1, var1))->interp(),
                      "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH((new AddExpr(var2, val2))->interp(),
                      "Variable(s) exist(s) in this expression");
    CHECK((new AddExpr(new NumExpr(1), new IfExpr(new EqExpr(new NumExpr(3), new NumExpr(3)), new NumExpr(1), new NumExpr(0))))
          ->interp()->equals(new NumVal(2)));
    
    CHECK((new MultExpr(val1, val2))->interp()->equals(new NumVal(408)));
    CHECK((new MultExpr(val1, val3))->interp()->equals(new NumVal(289)));
    CHECK_THROWS_WITH((new MultExpr(val1, var1))->interp(),
                      "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH((new MultExpr(var2, val2))->interp(),
                      "Variable(s) exist(s) in this expression");
    
    CHECK((new LetExpr("x", val1, val2))->interp()->equals(new NumVal(24)));
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->interp()->equals(new NumVal(41)));
    CHECK((new LetExpr("x", val1, new LetExpr("y", val2, new AddExpr(var1, val1))))->interp()->equals(new NumVal(34)));
    CHECK((new LetExpr("y", new AddExpr(val1, val2), new AddExpr(var2, new NumExpr(1))))->interp()->equals(new NumVal(42)));
    CHECK((new LetExpr("x", val1, new LetExpr("x", new AddExpr(var1, val2), new AddExpr(var1, new NumExpr(2)))))->interp()->equals(new NumVal(43)));
    CHECK((new LetExpr("x", val1, new LetExpr("x", val2, new AddExpr(var1, new NumExpr(2)))))
          ->interp()->equals(new NumVal(26)));
    CHECK((new LetExpr("x", new NumExpr(17), new AddExpr(new LetExpr("y", new NumExpr(24), new AddExpr(var2, new NumExpr(2))), var1)))->interp()->equals(new NumVal(43)));
    CHECK((new LetExpr("x", new NumExpr(3), new IfExpr(new EqExpr(var1, new NumExpr(3)), new NumExpr(1), new NumExpr(0))))
          ->interp()->equals(new NumVal(1)));
    CHECK((new LetExpr("x",new AddExpr(new NumExpr(2), new NumExpr(3)),new MultExpr(new VarExpr("x"), new VarExpr("x"))))
          ->interp()->equals(new NumVal(25))    );
   
    CHECK((new AddExpr(new MultExpr(val1, new LetExpr("x", new NumExpr(1), var1)), val2))->interp()->equals(new NumVal(41)));
    CHECK_THROWS_WITH((new LetExpr("x", new AddExpr(var1, val1), new AddExpr(var1, val2)))->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)) == true);
    CHECK((new BoolExpr(false))->interp()->equals(new BoolVal(true)) == false);
    
    CHECK((new EqExpr(val1, val2))->interp()->equals(new BoolVal(false)));
    CHECK((new EqExpr(val1, new NumExpr(17)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new AddExpr(val1, val2), new NumExpr(41)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new AddExpr(val1, val2), new MultExpr(val1, val2)))->interp()->equals(new BoolVal(false)));
    CHECK((new EqExpr(new AddExpr(new NumExpr(2), new NumExpr(2)), new MultExpr(new NumExpr(2), new NumExpr(2))))
          ->interp()->equals(new BoolVal(true)));
    CHECK_THROWS_WITH((new EqExpr(var1, new VarExpr("x")))->interp(), "Variable(s) exist(s) in this expression");
    
    CHECK((new IfExpr(new BoolExpr(true), new NumExpr(1), new NumExpr(2)))->interp()->equals(new NumVal(1)));
    CHECK((new IfExpr(new BoolExpr(false), new NumExpr(1), new NumExpr(2)))->interp()->equals(new NumVal(2)));
    CHECK((new IfExpr(new EqExpr(val1, new NumExpr(17)), new AddExpr(val2, new NumExpr(24)), new NumExpr(5)))
          ->interp()->equals(new NumVal(48)));
    CHECK_THROWS_WITH((new IfExpr(new AddExpr(val1, val2), new NumExpr(1), new NumExpr(2)))->interp(), "Not a boolean value error");
}

TEST_CASE("has_variables") {
    NumExpr* val1 = new NumExpr(17);
    NumExpr* val2 = new NumExpr(24);
    NumExpr* val3 = new NumExpr(17);
    NumExpr* val4 = new NumExpr(24);
    VarExpr* var1 = new VarExpr("x");
    VarExpr* var2 = new VarExpr("y");
    VarExpr* var3 = new VarExpr("X");
    
    CHECK(val3->has_variable() == false);
    CHECK(val4->has_variable() == false);
    
    CHECK(var1->has_variable() == true);
    CHECK(var3->has_variable() == true);
    
    CHECK((new AddExpr(var1, val1))->has_variable() == true);
    CHECK((new AddExpr(val2, var2))->has_variable() == true);
    CHECK((new AddExpr(var1, var2))->has_variable() == true);
    CHECK((new AddExpr(val1, val2))->has_variable() == false);
    CHECK((new AddExpr(var1, new AddExpr(val2, var2)))->has_variable() == true);
    CHECK((new AddExpr(val1, new AddExpr(val2, val3)))->has_variable() == false);
    
    
    CHECK((new MultExpr(var1, val1))->has_variable() == true);
    CHECK((new MultExpr(val2, var2))->has_variable() == true);
    CHECK((new MultExpr(var1, var2))->has_variable() == true);
    CHECK((new MultExpr(val1, val2))->has_variable() == false);
    CHECK((new MultExpr(new MultExpr(val2, var2), var1))->has_variable() == true);
    CHECK((new MultExpr(new MultExpr(val2, val3), val1))->has_variable() == false);
    
    CHECK((new LetExpr("x", new AddExpr(val1, val2), new AddExpr(var1, val3)))
          ->has_variable() == true);
    CHECK((new LetExpr("x", val1, new AddExpr(val2, val3)))->has_variable() == false);
    
    CHECK((new BoolExpr(true))->has_variable() == false);
    CHECK((new BoolExpr(false))->has_variable() == false);
    
    CHECK((new EqExpr(val1, val2))->has_variable() == false);
    CHECK((new EqExpr(var1, val1))->has_variable() == true);
    CHECK((new EqExpr(val2, var2))->has_variable() == true);
    
    CHECK((new IfExpr(new EqExpr(val1, new NumExpr(17)), new AddExpr(val2, new NumExpr(24)), new NumExpr(5)))->has_variable() == false);
    CHECK((new IfExpr(new EqExpr(val1, new NumExpr(17)), new VarExpr("x"), new VarExpr("y")))->has_variable() == true);
}

TEST_CASE("subst") {
    NumExpr* val1 = new NumExpr(17);
    NumExpr* val2 = new NumExpr(24);
    VarExpr* var1 = new VarExpr("x");
    VarExpr* var2 = new VarExpr("y");
    VarExpr* var3 = new VarExpr("X");
    
    CHECK(val1->subst("x", new NumExpr(24))->equals(val1));
    CHECK(val2->subst("y", new NumExpr(17))->equals(val2));
    
    CHECK(var1->subst("x", new VarExpr("y"))->equals(var2));
    CHECK(var2->subst("x", new VarExpr("X"))->equals(var2));
    CHECK(var1->subst("x", new NumExpr(17))->equals(val1));
    CHECK(var2->subst("y", new AddExpr(new NumExpr(17), new NumExpr(24)))->equals(new AddExpr(val1, val2)));
    
    CHECK((new AddExpr(val1, val2))->subst("x", new VarExpr("x"))->equals(new AddExpr(val1, val2)));
    CHECK((new AddExpr(new VarExpr("x"), val1))->subst("x", new VarExpr("y"))
          ->equals(new AddExpr(new VarExpr("y"), val1)));
    CHECK((new AddExpr(val2, new VarExpr("x")))->subst("x", new VarExpr("y"))
          ->equals(new AddExpr(val2, new VarExpr("y"))));
    CHECK((new AddExpr(new VarExpr("x"), new VarExpr("x")))->subst("x", new VarExpr("y"))
          ->equals(new AddExpr(new VarExpr("y"), new VarExpr("y"))));
    CHECK((new AddExpr(new VarExpr("y"), new VarExpr("y")))->subst("Y", new VarExpr("X"))
          ->equals(new AddExpr(new VarExpr("y"), new VarExpr("y"))));
    CHECK((new AddExpr(var1, (new AddExpr(val2, var2))))->subst("x", new VarExpr("X"))
          ->equals(new AddExpr(new VarExpr("X"), new AddExpr(val2, var2))));
    CHECK((new AddExpr(var1, (new AddExpr(val2, var1))))->subst("x", new VarExpr("X"))
          ->equals(new AddExpr(new VarExpr("X"), new AddExpr(val2, new VarExpr("X")))));
    
    CHECK((new MultExpr(val1, val2))->subst("x", new VarExpr("x"))->equals(new MultExpr(val1, val2)));
    CHECK((new MultExpr(var1, val1))->subst("x", new VarExpr("y"))
          ->equals(new MultExpr(var2, val1)));
    CHECK((new MultExpr(val2, var1))->subst("x", new VarExpr("y"))
          ->equals(new MultExpr(val2, var2)));
    CHECK((new MultExpr(var1, var1))->subst("x", new VarExpr("y"))
          ->equals(new MultExpr(var2, var2)));
    CHECK((new MultExpr(var2, var2))->subst("Y", new VarExpr("X"))
          ->equals(new MultExpr(var2, var2)));
    CHECK((new MultExpr(new MultExpr(val2, var2), var1))->subst("x", new VarExpr("X"))
          ->equals(new MultExpr(new MultExpr(val2, var2), var3)));
    CHECK((new MultExpr(new MultExpr(val2, var1), var1))->subst("x", new VarExpr("X"))
          ->equals(new MultExpr(new MultExpr(val2, var3), var3)));
    
    CHECK((new LetExpr("x", val1, var1))->subst("x", new VarExpr("y"))
          ->equals(new LetExpr("x", val1, var1)));
    CHECK((new LetExpr("x", val1, var1))->subst("y", new VarExpr("X"))
          ->equals(new LetExpr("x", val1, var1)));
    CHECK((new LetExpr("x", new AddExpr(var1, val1), new AddExpr(var1, val2)))->subst("x", new VarExpr("y"))->equals(new LetExpr("x", new AddExpr(var2, val1), new AddExpr(var1, val2))));
    CHECK((new LetExpr("y", new VarExpr("x"), new VarExpr("x")))->subst("x", new NumExpr(3))->equals(new LetExpr("y", new NumExpr(3), new NumExpr(3))));
    CHECK((new LetExpr("x", new NumExpr(1), new AddExpr(new VarExpr("x"), new NumExpr(2))))
        ->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
        ->equals(new LetExpr("x", new NumExpr(1), new AddExpr(new VarExpr("x"), new NumExpr(2)))));
    CHECK((new LetExpr("x", new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(2))))
          ->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
          ->equals(new LetExpr("x", new AddExpr(new VarExpr("y"), new NumExpr(3)), new AddExpr(new VarExpr("x"), new NumExpr(2)))));
    CHECK((new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new NumExpr(2))))
          ->subst("y", new NumExpr(8))
          ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new NumExpr(2)))));
    CHECK((new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new AddExpr(new NumExpr(2), new VarExpr("y")))))->subst("y", new NumExpr(9))
        ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new AddExpr(new NumExpr(2), new NumExpr(9))))));
    CHECK((new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new VarExpr("y"))))->subst("y", new NumExpr(8))
        ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new NumExpr(8)))));
    CHECK((new LetExpr("z", new VarExpr("x"), new AddExpr(new VarExpr("z"), new NumExpr(32))))->subst("z", new NumExpr(0))
        ->equals(new LetExpr("z", new VarExpr("x"), new AddExpr(new VarExpr("z"), new NumExpr(32)))));
    CHECK((new LetExpr("z", new VarExpr("z"), new AddExpr(new VarExpr("z"), new NumExpr(32))))->subst("z", new NumExpr(0))
        ->equals(new LetExpr("z", new NumExpr(0), new AddExpr(new VarExpr("z"), new NumExpr(32)))));
    CHECK((new LetExpr("z", new AddExpr(new VarExpr("z"), new NumExpr(2)), new AddExpr(new VarExpr("z"), new NumExpr(32))))->subst("z", new NumExpr(0))
        ->equals(new LetExpr("z", new AddExpr(new NumExpr(0), new NumExpr(2)), new AddExpr(new VarExpr("z"), new NumExpr(32)))));
    
    CHECK((new BoolExpr(true))->subst("x", new VarExpr("y"))->equals(new BoolExpr(true)));
    CHECK((new BoolExpr(false))->subst("y", new NumExpr(24))->equals(new BoolExpr(false)));
    
    CHECK((new EqExpr(val1, val2))->subst("x", new NumExpr(13))->equals(new EqExpr(new NumExpr(17), new NumExpr(24))));
    CHECK((new EqExpr(var1, val2))->subst("x", new NumExpr(13))->equals(new EqExpr(new NumExpr(13), new NumExpr(24))));
    CHECK((new EqExpr(new AddExpr(var1, new NumExpr(2)), new MultExpr(new NumExpr(2), var1)))->subst("x", new NumExpr(2))
          ->equals(new EqExpr(new AddExpr(new NumExpr(2), new NumExpr(2)), new MultExpr(new NumExpr(2), new NumExpr(2)))));
    
}

TEST_CASE("add_to") {
    CHECK_THROWS_WITH((new AddExpr(new NumExpr(17),new NumExpr(24)))->interp()->add_to(NULL), "Add of non-number error");
    CHECK_THROWS_WITH((new BoolVal(true))->add_to(new NumVal(17)), "Add of non-number error");
}

TEST_CASE("mult_by") {
    CHECK_THROWS_WITH( (new AddExpr(new NumExpr(24),new NumExpr(17)))->interp()->mult_by(NULL), "Mult of non-number error");
    CHECK_THROWS_WITH((new BoolVal(true))->mult_by(new NumVal(17)), "Mult of non-number error");
}

TEST_CASE("print") {
    NumExpr* val1 = new NumExpr(17);
    NumExpr* val2 = new NumExpr(24);
    VarExpr* var1 = new VarExpr("x");
    VarExpr* var2 = new VarExpr("y");
     
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
    CHECK((new AddExpr(val1, val2))->to_str() == toString);
    
    toString = "(x+y)";
    CHECK((new AddExpr(var1, var2))->to_str() == toString);
 
    toString = "(17+x)";
    CHECK((new AddExpr(val1, var1))->to_str() == toString);
    
    toString = "((17+x)+24)";
    CHECK((new AddExpr(new AddExpr(val1, var1), val2))->to_str() == toString);
    
    toString = "((17*x)+24)";
    CHECK((new AddExpr(new MultExpr(val1, var1), val2))->to_str() == toString);
 
    toString = "(x+(y+24))";
    CHECK((new AddExpr(var1, new AddExpr(var2, val2)))->to_str() == toString);
 
    toString = "(x+(y*24))";
    CHECK((new AddExpr(var1, new MultExpr(var2, val2)))->to_str() == toString);
 
    toString = "((17+x)+(24+y))";
    CHECK((new AddExpr(new AddExpr(val1, var1), new AddExpr(val2, var2)))->to_str() == toString);

    toString = "((17*x)+(24*y))";
    CHECK((new AddExpr(new MultExpr(val1, var1), new MultExpr(val2, var2)))->to_str() == toString);
    
    //Mult tests
    toString = "(17*24)";
    CHECK((new MultExpr(val1, val2))->to_str() == toString);
    
    toString = "(x*y)";
    CHECK((new MultExpr(var1, var2))->to_str() == toString);
 
    toString = "(24*y)";
    CHECK((new MultExpr(val2, var2))->to_str() == toString);
    
    toString = "((17*x)*24)";
    CHECK((new MultExpr(new MultExpr(val1, var1), val2))->to_str() == toString);
    
    toString = "((17+x)*24)";
    CHECK((new MultExpr(new AddExpr(val1, var1), val2))->to_str() == toString);
 
    toString = "(x*(y*24))";
    CHECK((new MultExpr(var1, new MultExpr(var2, val2)))->to_str() == toString);
    
    toString = "(x*(y+24))";
    CHECK((new MultExpr(var1, new AddExpr(var2, val2)))->to_str() == toString);
 
    toString = "((17+x)*(24+y))";
    CHECK((new MultExpr(new AddExpr(val1, var1), new AddExpr(val2, var2)))->to_str() == toString);
    
    toString = "((17*x)*(24*y))";
    CHECK((new MultExpr(new MultExpr(val1, var1), new MultExpr(val2, var2)))->to_str() == toString);
    
    //_let tests
    toString = "(_let x=17 _in (x+24))";
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->to_str() == toString);
    
    toString = "(_let x=5 _in ((_let y=3 _in (y+2))+x))";
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr((new LetExpr("y", new NumExpr (3), new AddExpr(var2, new NumExpr(2)))), var1)))->to_str() == toString);
    
    toString = "_true";
    CHECK((new BoolExpr(true))->to_str() == toString);
    
    toString = "_false";
    CHECK((new BoolExpr(false))->to_str() == toString);
    
    toString = "(x==17)";
    CHECK((new EqExpr(var1, val1))->to_str() == toString);
    
    toString = "((1+2)==3)";
    CHECK((new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_str() == toString);
    
    toString = "((_let x=17 _in x)==24)";
    CHECK((new EqExpr(new LetExpr("x", val1, var1), val2))->to_str() == toString);
    
    toString = "(_if (x==17) _then 1 _else 0)";
    CHECK((new IfExpr(new EqExpr(var1, val1), new NumExpr(1), new NumExpr(0)))->to_str() == toString);
}

TEST_CASE("pretty_print") {
    NumExpr* val1 = new NumExpr(17);
    NumExpr* val2 = new NumExpr(24);
    VarExpr* var1 = new VarExpr("x");
    VarExpr* var2 = new VarExpr("y");

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
    CHECK((new AddExpr(val1, val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x + y";
    CHECK((new AddExpr(var1, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "24 + y";
    CHECK((new AddExpr(val2, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) + 24";
    CHECK((new AddExpr(new AddExpr(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x + y + 24";
    CHECK((new AddExpr(var1, new AddExpr(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24";
    CHECK((new AddExpr(new MultExpr(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x + y * 24";
    CHECK((new AddExpr(var1, new MultExpr(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) + 24 + y";
    CHECK((new AddExpr(new AddExpr(val1, var1), new AddExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24 * y";
    CHECK((new AddExpr(new MultExpr(val1, var1), new MultExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) + 24 * y";
    CHECK((new AddExpr(new AddExpr(val1, var1), new MultExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "17 * x + 24 + y";
    CHECK((new AddExpr(new MultExpr(val1, var1), new AddExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    //Mult tests
    toPrettyStr = "17 * 24";
    CHECK((new MultExpr(val1, val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x * y";
    CHECK((new MultExpr(var1, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "24 * y";
    CHECK((new MultExpr(val2, var2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) * 24";
    CHECK((new MultExpr(new AddExpr(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x * (y + 24)";
    CHECK((new MultExpr(var1, new AddExpr(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 * x) * 24";
    CHECK((new MultExpr(new MultExpr(val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "x * y * 24";
    CHECK((new MultExpr(var1, new MultExpr(var2, val2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) * (24 + y)";
    CHECK((new MultExpr(new AddExpr(val1, var1), new AddExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 * x) * 24 * y";
    CHECK((new MultExpr(new MultExpr(val1, var1), new MultExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 + x) * 24 * y";
    CHECK((new MultExpr(new AddExpr(val1, var1), new MultExpr(val2, var2)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "(17 * x) * (24 + y)";
    CHECK((new MultExpr(new MultExpr(val1, var1), new AddExpr(val2, var2)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 * 1 * 7 * _let x = 3\n            _in  _let x = 5\n                 _in  x + _let x = 5\n                          _in  x * 1";
    CHECK((new MultExpr(new NumExpr(3), new MultExpr(new NumExpr(1), new MultExpr(new NumExpr(7), new LetExpr("x", new NumExpr(3), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))->to_pretty_str() == toPrettyStr); 
    
    //_let tests
    toPrettyStr = "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x";
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new LetExpr("y", new NumExpr(3), new AddExpr(var2, new NumExpr(2))), var1)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let x = 1\n_in  _let x = 2\n     _in  _let x = 3\n          _in  x + 4";
    CHECK((new LetExpr("x", new NumExpr(1), new LetExpr("x", new NumExpr(2), new LetExpr("x", new NumExpr(3), new AddExpr(var1, new NumExpr(4))))))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_let x = 17\n_in  x + 24";
    CHECK((new LetExpr("x", val1, new AddExpr(var1, val2)))->to_pretty_str()== toPrettyStr);

    toPrettyStr = "5 * (_let x = 5\n     _in  x) + 1";
    CHECK((new AddExpr(new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x"))), new NumExpr(1)))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "5 * _let x = 5\n    _in  x + 1";
    CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(var1, new NumExpr(1)))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 + 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1";
    CHECK((new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 + 7 + _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1";
    CHECK((new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_true";
    CHECK((new BoolExpr(true))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_false";
    CHECK((new BoolExpr(false))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "x == 17";
    CHECK((new EqExpr(var1, val1))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(1 == 2) == 3";
    CHECK((new EqExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(1 + 2) == 3";
    CHECK((new EqExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(3)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "3 == 1 + 2";
    CHECK((new EqExpr(new NumExpr(3), new AddExpr(new NumExpr(1), new NumExpr(2))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "4 == 2 * 2";
    CHECK((new EqExpr(new NumExpr(4), new MultExpr(new NumExpr(2), new NumExpr(2))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "2 * 2 == 4";
    CHECK((new EqExpr(new MultExpr(new NumExpr(2), new NumExpr(2)), new NumExpr(4)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_let x = 17\n _in  x) == 24";
    CHECK((new EqExpr(new LetExpr("x", val1, var1), val2))->to_pretty_str() == toPrettyStr);

    toPrettyStr = "_if x == 17\n_then 1\n_else 0";
    CHECK((new IfExpr(new EqExpr(var1, val1), new NumExpr(1), new NumExpr(0)))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_if x == 17\n _then 1\n _else 0) == 17";
    CHECK((new EqExpr(new IfExpr(new EqExpr(var1, val1), new NumExpr(1), new NumExpr(0)), val1))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let same = 1 == 2\n_in  _if 1 == 2\n     _then _false + 5\n     _else 88";
    CHECK((new LetExpr("same", new EqExpr(new NumExpr(1), new NumExpr(2)), new IfExpr(new EqExpr(new NumExpr(1), new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)), new NumExpr(88))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_let x = 17\n _in  x) * 24";
    CHECK((new MultExpr(new LetExpr("x", val1, var1), val2))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "_let x = 1 == 2\n_in  _let same = x\n     _in  _if 3 == 4\n          _then _false\n          _else _true";
    CHECK((new LetExpr("x", new EqExpr(new NumExpr(1), new NumExpr(2)), new LetExpr("same", new VarExpr("x"), new IfExpr(new EqExpr(new NumExpr(3), new NumExpr(4)), new BoolExpr(false), new BoolExpr(true)))))->to_pretty_str() == toPrettyStr);
    
    toPrettyStr = "(_let x = 1 == 2\n _in  _let same = x\n      _in  _if 3 == 4\n           _then _false\n           _else _true) + 17";
    CHECK((new AddExpr(new LetExpr("x", new EqExpr(new NumExpr(1), new NumExpr(2)), new LetExpr("same", new VarExpr("x"), new IfExpr(new EqExpr(new NumExpr(3), new NumExpr(4)), new BoolExpr(false), new BoolExpr(true)))), val1))->to_pretty_str() == toPrettyStr);
    
//    _if x
//    _then _true
//    _else _let Y = _let F = 17
//                   _in  S * 24 == 17
//          _in  _true
//    CHECK((new IfExpr(new VarExpr("x"), new BoolExpr(true), new LetExpr("Y", new LetExpr("F", new NumExpr(17), new EqExpr(new MultExpr(new VarExpr("S"), new NumExpr(24)), new NumExpr(17))), new BoolExpr(true))))->to_pretty_str() == "");
//
//    CHECK((new IfExpr(new VarExpr("x"), new IfExpr(new VarExpr("x"), new VarExpr("y"), new MultExpr(new NumExpr(17), new NumExpr(24))), new EqExpr(new IfExpr(new EqExpr(new BoolExpr(true), new NumExpr(17)), new IfExpr(new VarExpr("x"), new LetExpr("c", new NumExpr(17), new MultExpr(new NumExpr(17), new NumExpr(24))), new IfExpr(new NumExpr(17), new MultExpr(new VarExpr("G"), new NumExpr(24)), new BoolExpr(true))), new AddExpr(new BoolExpr(true), new NumExpr(24))), new VarExpr("y"))))->to_pretty_str() == "");
}

TEST_CASE("parse") {
    //interp tests
    CHECK(parse_str("17")->interp()->equals(new NumVal(17)));
    CHECK_THROWS_WITH(parse_str("x")->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH(parse_str("17 + x")->interp(), "Variable(s) exist(s) in this expression");
    CHECK_THROWS_WITH(parse_str("24 * y")->interp(), "Variable(s) exist(s) in this expression");
    CHECK(parse_str("17 +     -24")->interp()->equals(new NumVal(-7)));
    CHECK(parse_str("24   +   17")->interp()->equals(new NumVal(41)));
    CHECK(parse_str("   17 *  24")->interp()->equals(new NumVal(408)));
    CHECK(parse_str("  -17 *  24")->interp()->equals(new NumVal(-408)));
    CHECK(parse_str("17 + 17 * 24")->interp()->equals(new NumVal(425)));
    CHECK(parse_str("17 * 17 + 24")->interp()->equals(new NumVal(313)));
    CHECK(parse_str("(17 + 17) * (24 + 24)")->interp()->equals(new NumVal(1632)));
    CHECK(parse_str("17 * 17 + 24 * 24")->interp()->equals(new NumVal(865)));
    CHECK(parse_str("_let x = 17 _in x + 24")->interp()->equals(new NumVal(41)));
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
    
    CHECK(parse_str("_if 17 == 17 _then 1 _else 0")->interp()->equals(new NumVal(1)));
    CHECK_THROWS_WITH(parse_str("_in x == 17 _then 1 _else 0")->interp(), "E1 invalid input");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _than 1 _else 0")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _then 1 _els 0")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_if x == 17 *then 1 _else 0")->interp(), "Consume mismatch");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _then 1 -else 0")->interp(), "Consume mismatch");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _else 1 _else 0")->interp(), "E6 invalid input");
    CHECK_THROWS_WITH(parse_str("_if x == 17 _then 1 _then 0")->interp(), "E7 invalid input");
    
    CHECK(parse_str("_if _true _then 1 _else 0")->interp()->equals(new NumVal(1)));
    CHECK(parse_str("_if _false _then 1 _else 0")->interp()->equals(new NumVal(0)));
    CHECK_THROWS_WITH(parse_str("_if _tru _then 1 _else 0")->interp(), "Invalid keyword");
    CHECK_THROWS_WITH(parse_str("_if _fals _then 1 _else 0")->interp(),"Invalid keyword");
    
    
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
    
//    CHECK(parse_str("_if     1311433591   _then  _if     X  _then       F  _else        -1625335828   *   _false _else    (_if     _true   ==    32309537_then _if     L  _then     _let   c =  1362618119 _in    (-977663374)   *   eQHMjGAEBk _else       -7896479  == (_if     -1725426414   _then     G   *         174797107    _else     _true)     _else    _true    +    -1128812070)   ==     MABowbMNz")->to_pretty_str() == " ");
}
