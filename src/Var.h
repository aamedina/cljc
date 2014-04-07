#import <Foundation/Foundation.h>
#import <pthread.h>
#import <llvm/Support/Threading.h>
#import <llvm/Support/ThreadLocal.h>
#import <llvm/Config/config.h>
#import <llvm/Support/Atomic.h>
#import <llvm/Support/Mutex.h>
#import "protocols.h"

@interface TBox : NSObject
@property (nonatomic) volatile id val;
@property (nonatomic, readonly) pthread_t thread;
- (instancetype)initWithThread:(pthread_t)thread andVal:(id)val;
@end

@implementation TBox
- (instancetype)initWithThread:(pthread_t)thread andVal:(id)val {
  self = [super init];
  if (self) {
    _thread = thread;
    _val = val;
  }
  return self;
}
@end

@class Var;

@interface Unbound : NSObject <IFn>
@property (nonatomic, readonly) Var *v;
- (instancetype)initWithVar:(Var *)v;
@end

@implementation Unbound
- (instancetype)initWithVar:(Var *)v {
  self = [super init];
  if (self) {
    _v = v;
  }
  return self;
}
@end

@class Frame;

@interface Frame : NSObject
@end

@implementation Frame
@end

@interface Var : NSObject
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
- (NSString *)description {
  return [NSString stringWithFormat:@"#'%@/%@", _ns, _name];
}
@end
