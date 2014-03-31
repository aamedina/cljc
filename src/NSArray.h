#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSArray (ACollection) <ICollection, ISeqable, ISeq, IMapEntry,
                                    Object>
@end

@implementation NSArray (ACollection)
- (NSString *)toString {
  if ([self count] == 0)
    return @"[]";
  NSMutableArray *objects = [NSMutableArray array];
  for (id obj in self) {
    if ([obj respondsToSelector:@selector(toString)])      
      [objects addObject:[obj toString]];
    else
      [objects addObject:obj];
  }
  return [NSString stringWithFormat:@"[%@]",
                   [objects componentsJoinedByString:@" "]];
}
- (id)conj:(id)val {
  return [self arrayByAddingObject:val];
}
- (id)first {
  return [self firstObject];
}
- (id)rest {
  return [self subarrayWithRange:NSMakeRange(1, [self count])];
}
- (id)cons:(id)obj {
  NSMutableArray *arr = [NSMutableArray arrayWithArray:self];
  [arr insertObject:obj atIndex:0];
  return [NSArray arrayWithArray:arr];
}
- (id)seq {
  if ([self count] == 0)
    return [NSNull null];
  else
    return self;
}
- (id)key {
  if ([self count] == 2) {
    return [self objectAtIndex:0];
  }
  @throw @"Map entry cannot contain more than two objects";
}
- (id)val {
  if ([self count] == 2) {
    return [self objectAtIndex:1];
  }
  @throw @"Map entry cannot contain more than two objects";
}
- (id)nth:(int)n {
  return [self nth:n default:[NSNull null]];
}
- (id)nth:(int)n default:(id)notFound {
  id obj = [self objectAtIndex:n];
  if (obj)
    return obj;
  else
    return notFound;
}
@end
