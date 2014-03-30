#import <Foundation/Foundation.h>
#import "protocols.h"

@interface MapEntry : NSArray <IMapEntry>
@property (nonatomic, readonly) id _key;
@property (nonatomic, readonly) id _val;
- (instancetype)initWithKey:(id)key andVal:(id)val;
@end

@implementation MapEntry
- (instancetype)initWithKey:(id)key andVal:(id)val {
  self = [super init];
  if (self) {
    __key = key;
    __val = val;
  }
  return self;
}
- (NSUInteger)count { return 2; }
- (id)key { return __key; }
- (id)val { return __val; }
@end

@interface NSDictionary (ACollection) <ICollection, ISeqable, ISeq, ILookup,
                                         IAssociative, IMap, ICounted, Object>
@end

@implementation NSDictionary (ACollection)
- (NSString *)toString {
  if ([self count] == 0)
    return @"{}";
  else if ([self count] == 1)
    return [NSString stringWithFormat:@"{%@ %@}",
                     [[self _first] _key],
                     [[self _first] _val]];
  NSMutableArray *arr = [NSMutableArray array];
  for (id key in self) {
    id val = [self _lookup:key];
    if ([key respondsToSelector:@selector(toString)])
      [arr addObject:[key toString]];
    else
      [arr addObject:key];
    if ([val respondsToSelector:@selector(toString)])
      [arr addObject:[val toString]];
    else
      [arr addObject:val];   
  }
  return [NSString stringWithFormat:@"{%@}",
                   [arr componentsJoinedByString:@" "]];
}
- (int)_count {
  return [self count];
}
- (id)_conj:(id<IMapEntry>)map_entry {
  return [self _assoc:[map_entry _key] withVal:[map_entry _val]];
}
- (id)_first {
  return [[self _seq] _first];
}
- (id)_rest {
  return [[self _seq] _rest];
}
- (id)_cons:(id)obj {
  return [[self _seq] _cons:obj];
}
- (id)_seq {
  if ([self count] == 0)
    return [NSNull null];
  else {
    NSMutableArray *arr = [NSMutableArray array];
    for(id key in self) {
      id val = [self objectForKey:key];
      [arr addObject:[[MapEntry alloc] initWithKey:key andVal:val]];
    }
    return arr;
  }  
}
- (id)_lookup:(id)key {
  return [self _lookup:key default:[NSNull null]];
}
- (id)_lookup:(id)key default:(id)not_found {
  id val = [self objectForKey:key];
  if (val)
    return val;
  else
    return not_found;
}
- (NSNumber *)_contains_key:(id)key {
  if ([self objectForKey:key])
    return @(YES);
  else
    return @(NO);
}
- (id)_assoc:(id<NSCopying>)key withVal:(id)val {
  NSMutableDictionary *dict =
      [NSMutableDictionary dictionaryWithDictionary:self];
  [dict setObject:val forKey:key];
  return dict;
}
- (id)_dissoc:(id)key {
  NSMutableDictionary *dict =
      [NSMutableDictionary dictionaryWithDictionary:self];
  [dict removeObjectForKey:key];
  return dict;
}
@end
