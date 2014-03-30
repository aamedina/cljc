#import <Foundation/Foundation.h>
#import "protocols.h"

@interface Cons : NSObject <ICollection, ISeqable, ISeq, ICounted, Object,
                              INext, IEquiv, NSFastEnumeration>
@property (nonatomic, readonly) id first;
@property (nonatomic, readonly) id<ISeq> rest;
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
    ret = [ret _cons:obj];
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
      last_obj = [lst _first];
      lst = [lst _rest];
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
    _first = first;
  return self;
}
- (instancetype)initWithFirst:(id)first andRest:(id<ISeq>)rest {
  self = [self initWithFirst:first];
  if (self)
    _rest = rest;
  return self;
}
- (NSString *)toString {
  if (![self _first])
    return @"()";
  else if ([self _first] && ![self _rest])
    return [NSString stringWithFormat:@"(%@)", [self _first]];
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
  if ([self _first]) {
    return 1 + [[self _rest] _count];
  }
  return 0;
}
- (id)_conj:(id)val {
  return [self _cons:val];
}
- (id)_first {
  return _first;
}
- (id)_rest {
  return _rest;
}
- (id)_next {
  return _rest;
}
- (BOOLEAN)_equiv:(id)other {
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
- (id)_cons:(id)obj {
  return [[Cons alloc] initWithFirst:obj andRest:self];
}
- (id)_seq {
  if (_first)
    return self;
  else
    return [NSNull null];
}
@end
