#import <Foundation/Foundation.h>
#import "protocols.h"
#import "Var.h"

namespace RT {
  Var *var (NSString *ns, NSString *name) {
    return nil;
  }
}

id (^createFunction())() {
  return ^id () { return nil; };
}
