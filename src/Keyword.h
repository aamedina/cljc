#import <Foundation/Foundation.h>
#import "Symbol.h"
#import "protocols.h"
#import <tbb/concurrent_hash_map.h>
#import <city.h>

using namespace tbb;
using namespace std;

@class Keyword;

static concurrent_hash_map<Symbol*, Keyword*> table;

@interface Keyword : NSObject
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
- (NSString *)description {
  if (_ns && _name)
    return [NSString stringWithFormat:@":%@/%@", _ns, _name];
  return [NSString stringWithFormat:@":%@", _name];
}
@end
