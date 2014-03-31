#import <Foundation/Foundation.h>
#import "protocols.h"
#import "RT.h"

@interface ArrayChunk : NSObject <IChunk, IIndexed, ICounted>
@property (nonatomic, readonly) id __strong *array;
@property (nonatomic, readonly) int off;
@property (nonatomic, readonly) int end;
- (instancetype)initWithArray:(id __strong *)array;
- (instancetype)initWithArray:(id __strong *)array andOff:(int)off;
- (instancetype)initWithArray:(id __strong *)array andOff:(int)off
                       andEnd:(int)end;
@end

@implementation ArrayChunk
- (instancetype)initWithArray:(id __strong *)array {
  self = [super init];
  if (self)
    _array = array;
  return self;
}
- (instancetype)initWithArray:(id __strong *)array andOff:(int)off {
  self = [self initWithArray:array];
  if (self)
    _off = off;
  return self;
}
- (instancetype)initWithArray:(id __strong *)array andOff:(int)off
                       andEnd:(int)end {
  self = [self initWithArray:array andOff:off];
  if (self)
    _end = end;
  return self;
}
- (int)_count {
  return (_end - _off);
}
- (id)nth:(int)n {
  return _array[_off + n];
}
- (id)nth:(int)n default:(id)notFound {
  if (n >= 0 && n < (_end - _off))
    return [self nth:n];
  return notFound;
}
- (id<IChunk>)dropFirst {
  if (_off == _end)
    @throw @"dropFirst of empty chunk";
  return [[ArrayChunk alloc] initWithArray:_array
                                    andOff:(_off + 1 )
                                    andEnd: _end];
}
- (id)reduce:(id<IFn>) f start:(id)start {
  id ret = [f invoke:start with:_array[_off]];
  if (isReduced(ret))
    return ret;
  for (int x = _off + 1; x < _end; x++) {
    ret = [f invoke:ret with:_array[x]];
    if (isReduced(ret))
      return ret;
  }
  return ret;
}
@end
