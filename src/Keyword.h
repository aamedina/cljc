#import <Foundation/Foundation.h>
#import "Symbol.h"
#import "protocols.h"

@interface Keyword : NSObject <Object>
@property Symbol *ns;
@property Symbol *name;
@end

@implementation Keyword
- (instancetype)initWithName:(Symbol *)name {
  self = [super init];
  if (self)
    _name = name;
  return self;
}
- (instancetype)initWithNamespace:(Symbol *)ns andName:(Symbol *)name {
  self = [super init];
  if (self) {
    _ns = ns;
    _name = name;
  }
  return self;
}
- (NSString *)toString {
  if (_ns && _name)
    return [NSString stringWithFormat:@":%@/%@",
                     [_ns toString],
                     [_name toString]];
  return [NSString stringWithFormat:@":%@",
                   [_name toString]];
}
@end

