#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSSet (ACollection) <ICollection, ICounted, ISeqable, ILookup, ISet,
                                  ISeq, Object>
+ (instancetype)EMPTY;
+ (instancetype)create:(NSArray *)arr;
@end

@implementation NSSet (ACollection)
+ (instancetype)EMPTY {
  return [NSSet set];
}
+ (instancetype)create:(NSArray *)arr {
  return [NSSet setWithArray:arr];
}
- (NSString *)toString {
  if ([self count] == 0)
    return @"#{}";
  NSMutableArray *arr = [NSMutableArray array];
  for (id obj in [self _seq]) {
    if ([obj respondsToSelector:@selector(toString)])
      [arr addObject:[obj toString]];
    else
      [arr addObject:obj];    
  }
  return [NSString stringWithFormat:@"#{%@}",
                   [arr componentsJoinedByString:@" "]];
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
- (int)_count {
  return [self count];
}
- (id)_conj:(id)val {
  return [self setByAddingObject:val];
}
- (id)_seq {
  if ([self count] == 0)
    return [NSNull null];
  else
    return [self allObjects];
}
- (id)_lookup:(id)key {
  return [self _lookup:key default:[NSNull null]];
}
- (id)_lookup:(id)key default:(id)not_found {
  id val = [self member:key];
  if (val)
    return val;
  else
    return not_found;
}
- (id)_disjoin:(id)val {
  NSMutableSet *xs = [NSMutableSet setWithSet:self];
  [xs removeObject:val];
  return [NSSet setWithSet:xs];
}
@end

