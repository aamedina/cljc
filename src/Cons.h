#import <Foundation/Foundation.h>
#import "protocols.h"

@class Cons;

@interface Cons : NSObject <ICollection, ISeqable, ISeq, ICounted, Object,
                              INext, IEquiv, NSFastEnumeration>
@property (nonatomic, readonly) id _first;
@property (nonatomic, readonly) Cons *_rest;
+ (instancetype)EMPTY;
+ (instancetype)create:(NSArray *)arr;
- (instancetype)initWithFirst:(id)first;
- (instancetype)initWithFirst:(id)first andRest:(id<ISeq>)rest;
@end

@implementation Cons
+ (instancetype)EMPTY {
  return [[Cons alloc] init];
}
+ (instancetype)create:(NSArray *)arr {
  Cons *ret = [Cons EMPTY];
  for(id obj in [arr reverseObjectEnumerator]) {
    ret = [ret cons:obj];
  }
  return ret;
}
- (NSUInteger)countByEnumeratingWithState:(NSFastEnumerationState *)state
                                  objects:(id __unsafe_unretained [])stackbuf
                                    count:(NSUInteger)stackbufLength {
  NSUInteger count = 0;
  unsigned long countOfItemsAlreadyEnumerated = state->state;
  Cons *lst = self;
  int cnt = [lst _count];
  if (countOfItemsAlreadyEnumerated < cnt) {
    state->itemsPtr = stackbuf;
    id last_obj;
    while ((countOfItemsAlreadyEnumerated < cnt) &&
           (count < stackbufLength)) {
      last_obj = [lst first];
      lst = [lst rest];
      stackbuf[count] = last_obj;
      countOfItemsAlreadyEnumerated++;
      count++;
    }
    state->mutationsPtr = &state->extra[0];
  } else {
    count = 0;
  }  
  state->state = countOfItemsAlreadyEnumerated;
  return count;
}
- (instancetype)initWithFirst:(id)first {
  self = [super init];
  if (self)
    __first = first;
  return self;
}
- (instancetype)initWithFirst:(id)first andRest:(id)rest {
  self = [self initWithFirst:first];
  if (self)
    __rest = rest;
  return self;
}
- (NSString *)toString {
  if (![self first])
    return @"()";
  else if ([self first] && ![self rest])
    return [NSString stringWithFormat:@"(%@)", [self first]];
  NSMutableArray *objects = [NSMutableArray array];
  for (id obj in self) {
    if ([obj respondsToSelector:@selector(toString)])      
      [objects addObject:[obj toString]];
    else
      [objects addObject:obj];
  }
  return [NSString stringWithFormat:@"(%@)",
                   [objects componentsJoinedByString:@" "]];
}
- (int)_count {
  if (self._first) {
    return 1 + [self._rest _count];
  }
  return 0;
}
- (id)conj:(id)val {
  return [self cons:val];
}
- (id)first {
  return __first;
}
- (id)rest {
  return __rest;
}
- (id)next {
  if ([__rest _count] > 0)
    return __rest;
  return NIL;
}
- (BOOLEAN)equiv:(id)other {
  if (other == self)
    return T;
  int i = 0;
  NSMutableArray *arr = [NSMutableArray array];
  for (id obj in self) {
    [arr addObject:obj];
  }
  for (id obj in other) {
    if (![obj isEqual:[arr objectAtIndex:i++]])
      return F;
  }
  return T;
}
- (id)cons:(id)obj {
  return [[Cons alloc] initWithFirst:obj andRest:self];
}
- (id)seq {
  if (__first)
    return self;
  else
    return [NSNull null];
}
@end
