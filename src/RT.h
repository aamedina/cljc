#import <Foundation/Foundation.h>
#import "protocols.h"
#import "Var.h"

namespace RT {
  void throwArityException (int actual, NSString *name) {
    NSString *msg = @"Wrong number of args (%d) passed to: %@";
    @throw [NSString stringWithFormat:msg, actual, name];
  }
  
  Var *var (NSString *ns, NSString *name) {
    return nil;
  }
}

id (^createFunction())() {
  return ^id () { return nil; };
}
