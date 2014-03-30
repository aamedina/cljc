#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSArray (ACollection) <ICollection, ISeqable, ISeq, IMapEntry,
                                    ICounted, Object>
@end

@implementation NSArray (ACollection)
- (NSString *)toString {
  if ([self count] == 0)
    return @"[]";
  if ([self count] == 1)
    return [NSString stringWithFormat:@"[%@]", [[self _first] toString]];
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
- (int)_count {
  return [self count];
}
- (id)_conj:(id)val {
  return [self arrayByAddingObject:val];
}
- (id)_first {
  return [self firstObject];
}
- (id)_rest {
  return [self subarrayWithRange:NSMakeRange(1, [self count])];
}
- (id)_cons:(id)obj {
  NSMutableArray *arr = [NSMutableArray arrayWithArray:self];
  [arr insertObject:obj atIndex:0];
  return [NSArray arrayWithArray:arr];
}
- (id)_seq {
  if ([self count] == 0)
    return [NSNull null];
  else
    return self;
}
- (id)_key {
  if ([self count] == 2) {
    return [self objectAtIndex:0];
  }
  @throw @"Map entry cannot contain more than two objects";
}
- (id)_val {
  if ([self count] == 2) {
    return [self objectAtIndex:1];
  }
  @throw @"Map entry cannot contain more than two objects";
}
- (id)_nth:(int)n {
  return [self _nth:n default:[NSNull null]];
}
- (id)_nth:(int)n default:(id)not_found {
  return [self objectAtIndex:n];
}
@end
