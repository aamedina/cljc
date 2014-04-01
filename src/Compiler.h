#import <Foundation/Foundation.h>
#import "Symbol.h"
#import "RT.h"

static const Symbol *DEF = [Symbol intern:@"def"];
static const Symbol *LOOP = [Symbol intern:@"loop*"];
static const Symbol *RECUR = [Symbol intern:@"recur"];
static const Symbol *IF = [Symbol intern:@"if"];
static const Symbol *LET = [Symbol intern:@"let*"];
static const Symbol *LETFN = [Symbol intern:@"letfn*"];
static const Symbol *DO = [Symbol intern:@"do"];
static const Symbol *FN = [Symbol intern:@"fn*"];
static const Symbol *FNONCE = [Symbol intern:@"fn*"];
