//
//  test.m
//  test
//
//  Created by Nga Huynh on 2/2/21.
//

#import <XCTest/XCTest.h>
#include "run.h"

@interface test : XCTestCase
@end

@implementation test
- (void)testAll {
    if (!run_tests())
        XCTFail(@"failed");
}
@end
