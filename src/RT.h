#import <Foundation/Foundation.h>
#import "protocols.h"
#import "Var.h"


static void throwArityException (int actual, NSString *name) {
  NSString *msg = @"Wrong number of args (%d) passed to: %@";
  @throw [NSString stringWithFormat:msg, actual, name];
}
  
static Var *var (NSString *ns, NSString *name) {
  return nil;
}

static BOOL isReduced (id r) {
  return [r isKindOfClass:Reduced.class];
}

static id (^createFunction())() {
  return ^id () { return nil; };
}
