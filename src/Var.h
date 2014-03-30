#import <Foundation/Foundation.h>
#import "protocols.h"

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
  return [NSString stringWithFormat:@"%@/%@", _ns, _name];
}
@end
