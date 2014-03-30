#import <Foundation/Foundation.h>
#import "protocols.h"

@interface Symbol : NSObject <Object>
@property NSString *ns;
@property NSString *name;
@end

@implementation Symbol
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
  if (_ns && _name)
    return [NSString stringWithFormat:@"%@/%@", _ns, _name];
  return [NSString stringWithFormat:@"%@", _name];    
}
@end
