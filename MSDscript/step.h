//
//  step.hpp
//  MSDScript
//
//  Created by Nga Huynh on 4/4/21.
//

#ifndef step_h
#define step_h

#include <stdio.h>

#include "pointer.h"

class Expr;
class Env;
class Val;
class Cont;

CLASS(Step) {
public:
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;
    
    static mode_t mode;     /* chooses mode */ 
    
    static PTR(Expr) expr;  /* for interp mode */
    static PTR(Env) env;    /* for interp mode */
    
    static PTR(Val) val;    /* for continue_mode */
    
    static PTR(Cont) cont; /* all modes */
    
    static PTR(Val) interp_by_steps(PTR(Expr) e);
};
#endif /* step_hpp */
