//
//  env.cpp
//  MSDScript
//
//  Created by Nga Huynh on 3/29/21.
//

#include "env.h"
#include <stdexcept>
#include <stdio.h>

PTR(Env) Env::empty = NEW(EmptyEnv)();

EmptyEnv::EmptyEnv() {}
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("Free variable: " + find_name);
}
bool EmptyEnv::equals(PTR(Env) other) {
    PTR(EmptyEnv) other_env = CAST(EmptyEnv)(other);
    if (other_env == empty) {
        return true;
    }
    else {
        return false;
    }
}

ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}
PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == this->name) {
        return this->val;
    }
    else {
        return rest->lookup(find_name);
    }
}
bool ExtendedEnv::equals(PTR(Env) other) {
    PTR(ExtendedEnv) other_env = CAST(ExtendedEnv)(other);
    if (other_env == NULL) {
        return false;
    }
    else {
        return ((this->name == other_env->name) && this->val->equals(other_env->val) && this->rest->equals(other_env->rest));
    }
}
