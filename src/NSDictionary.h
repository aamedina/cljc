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
                                         IAssociative, IMap>
+ (instancetype)EMPTY;
+ (instancetype)create:(NSArray *)arr;
@end

@implementation NSDictionary (ACollection)
+ (instancetype)EMPTY {
  return [NSDictionary dictionary];
}
+ (instancetype)create:(NSArray *)arr {
  if (([arr count] % 2) != 0)
    @throw @"Must initialize a map with an even number of elements";
  NSMutableDictionary *dict = [NSMutableDictionary dictionary];
  for(int i=0; i<arr.count; i+=2)
    [dict setObject:[arr objectAtIndex:i+1] forKey:[arr objectAtIndex:i]];
  return [NSDictionary dictionaryWithDictionary:dict];
}
- (NSString *)description {
  if ([self count] == 0)
    return @"{}";
  NSMutableArray *arr = [NSMutableArray array];
  for (id key in self) {
    id val = [self lookup:key];
    [arr addObject:key];
    [arr addObject:val];   
  }
  return [NSString stringWithFormat:@"{%@}",
                   [arr componentsJoinedByString:@" "]];
}
- (id)conj:(id<IMapEntry>)mapEntry {
  return [self assoc:[mapEntry key] withVal:[mapEntry val]];
}
- (id)first {
  return [[self seq] first];
}
- (id)rest {
  return [[self seq] rest];
}
- (id)cons:(id)obj {
  return [[self seq] cons:obj];
}
- (id)seq {
  if ([self count] == 0)
    return [NSNull null];
  else {
    NSMutableArray *arr = [NSMutableArray array];
    for (id key in self) {
      id val = [self objectForKey:key];
      [arr addObject:[[MapEntry alloc] initWithKey:key andVal:val]];
    }
    return arr;
  }  
}
- (id)lookup:(id)key {
  return [self lookup:key default:[NSNull null]];
}
- (id)lookup:(id)key default:(id)notFound {
  id val = [self objectForKey:key];
  if (val)
    return val;
  else
    return notFound;
}
- (NSNumber *)containsKey:(id)key {
  if ([self objectForKey:key])
    return @(YES);
  else
    return @(NO);
}
- (id)assoc:(id<NSCopying>)key withVal:(id)val {
  NSMutableDictionary *dict =
      [NSMutableDictionary dictionaryWithDictionary:self];
  [dict setObject:val forKey:key];
  return dict;
}
- (id)dissoc:(id)key {
  NSMutableDictionary *dict =
      [NSMutableDictionary dictionaryWithDictionary:self];
  [dict removeObjectForKey:key];
  return dict;
}
@end
