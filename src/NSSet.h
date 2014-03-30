#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSSet (ACollection) <ICollection, ICounted, ISeqable, ILookup, ISet,
                                  ISeq, Object>
@end

@implementation NSSet (ACollection)
- (NSString *)toString {
  if ([self count] == 0)
    return @"#{}";
  else if ([self count] == 1)
    return [NSString stringWithFormat:@"#{%@}", [[self _seq] _first]];
  else
    return [NSString stringWithFormat:@"#{%@}",
                     [[self _seq] componentsJoinedByString:@" "]];
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

