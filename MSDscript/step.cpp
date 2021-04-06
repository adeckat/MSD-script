//
//  step.cpp
//  MSDScript
//
//  Created by Nga Huynh on 4/4/21.
//

#include "step.h"
#include "cont.h"
#include "pointer.h"

Step::mode_t Step::mode;
PTR(Expr) Step::expr; /* only for Step::interp_mode */
PTR(Env) Step::env;
PTR(Val) Step::val;
PTR(Cont) Step::cont;

PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    Step::mode = Step::interp_mode;
    Step::expr = e;
    Step::env = Env::empty;
    Step::val = nullptr;
    Step::cont = Cont::done;
    
    while (1) {
        if (Step::mode == Step::interp_mode) {
            Step::expr->step_interp();
        }
        else {
            if (Step::cont == Cont::done) {
                return Step::val;
            }
            else {
                Step::cont->step_continue();
            }
        }
    }
}
