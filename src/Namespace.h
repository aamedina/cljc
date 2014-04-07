#import <Foundation/Foundation.h>
#import "protocols.h"
#import "Symbol.h"
#import "NSDictionary.h"
#import <tbb/tbb.h>
#import <tbb/atomic.h>
#import <tbb/concurrent_hash_map.h>

@class Namespace;

// static tbb::concurrent_hash_map<Symbol*, Namespace*> namespaces;

// static NSDictionary *DEFAULT_IMPORTS =
//     [NSDictionary create:@[
//         @1,@2,
//         @"hello", @"there"]];
// static NSArray *imports =
//     @[[Symbol intern:@"hello"], [Symbol intern:@"hello"]];
// static Symbol *sym = [Symbol intern:@"hello"];

// static NSDictionary *DEFAULT_IMPS = [NSDictionary create:imports];

static NSMutableDictionary *namespaces = [NSMutableDictionary dictionary];

@interface Namespace : NSObject
@property (nonatomic, readonly) Symbol *name;
@property (nonatomic, readonly) NSMutableDictionary *mappings;
@property (nonatomic, readonly) NSMutableDictionary *aliases;
+ (id)all;
- (instancetype)initWithName:(Symbol *)name;
@end

@implementation Namespace
- (NSString *)description {
  return [self.name description];
}
+ (id)all {
  return [namespaces allValues];
}
- (instancetype)initWithName:(Symbol *)name {
  self = [super init];
  if (self) {
    _name = name;
    // _mappings = DEFAULT_IMPORTS;
    _aliases = [NSMutableDictionary dictionary];
    
  }
  return self;
}
@end
