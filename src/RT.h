#import <Foundation/Foundation.h>
#import "protocols.h"
#import "Var.h"
#import "NSDictionary.h"
#import "Namespace.h"

static const int HASHTABLE_THRESHOLD = 16;

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

static id<IMap> map (id init, ...) {
  id obj;
  va_list arglist;
  NSMutableArray *arr = [NSMutableArray array];
  if (init) {
    [arr addObject: init];
    va_start(arglist, init);
    while ((obj = va_arg(arglist, id)))
      [arr addObject: obj];
    va_end(arglist);
  } else {
    return [NSDictionary dictionary];
  }
  if ([arr count] < HASHTABLE_THRESHOLD)
    return [NSDictionary create:arr];
  return [NSDictionary create:arr];
}
