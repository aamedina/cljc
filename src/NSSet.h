#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSSet (ACollection) <ICollection, ISeqable, ILookup, ISet, ISeq,
                                  Object>
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
  for (id obj in [self seq]) {
    if ([obj respondsToSelector:@selector(toString)])
      [arr addObject:[obj toString]];
    else
      [arr addObject:obj];    
  }
  return [NSString stringWithFormat:@"#{%@}",
                   [arr componentsJoinedByString:@" "]];
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
- (int)count {
  return [self count];
}
- (id)conj:(id)val {
  return [self setByAddingObject:val];
}
- (id)seq {
  if ([self count] == 0)
    return [NSNull null];
  else
    return [self allObjects];
}
- (id)lookup:(id)key {
  return [self lookup:key default:[NSNull null]];
}
- (id)lookup:(id)key default:(id)notFound {
  id val = [self member:key];
  if (val)
    return val;
  else
    return notFound;
}
- (id)disjoin:(id)val {
  NSMutableSet *xs = [NSMutableSet setWithSet:self];
  [xs removeObject:val];
  return [NSSet setWithSet:xs];
}
@end

