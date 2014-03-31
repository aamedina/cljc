#import <Foundation/Foundation.h>
#import "protocols.h"
#import "Var.h"


void throwArityException (int actual, NSString *name) {
  NSString *msg = @"Wrong number of args (%d) passed to: %@";
  @throw [NSString stringWithFormat:msg, actual, name];
}
  
Var *var (NSString *ns, NSString *name) {
  return nil;
}

BOOL isReduced (id r) {
  return [r isKindOfClass:Reduced.class];
}

id (^createFunction())() {
  return ^id () { return nil; };
}
