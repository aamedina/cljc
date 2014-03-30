#import <Foundation/Foundation.h>
#import <pthread.h>
#import <llvm/Support/Threading.h>
#import <llvm/Support/ThreadLocal.h>
#import <llvm/Config/config.h>
#import <llvm/Support/Atomic.h>
#import <llvm/Support/Mutex.h>
#import "protocols.h"

@interface TBox : NSObject
@property volatile id val;
@property pthread_t thread;
@end

@interface Unbound <IFn>
@end

@interface Var : NSObject <Object>
@property NSString *ns;
@property NSString *name;
@end

@implementation Var
- (instancetype)initWithName:(NSString *)name {
  self = [super init];
  if (self)
    _name = name;
  return self;
}
- (instancetype)initWithNamespace:(NSString *)ns andName:(NSString *)name {
  self = [super init];
  if (self) {
    _ns = ns;
    _name = name;
  }
  return self;
}
- (NSString *)toString {
  return [NSString stringWithFormat:@"#'%@/%@", _ns, _name];
}
@end
