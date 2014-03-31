#import <Foundation/Foundation.h>
#import "protocols.h"
#import "RT.h"

@interface ArraySeq : NSObject <ISeq, IIndexed, ICounted, IReduce>
@property (nonatomic, readonly) id __strong *array;
@property (nonatomic, readonly) int i;
+ (instancetype)create;
+ (instancetype)create:(id __strong *)array, ...;
- (instancetype)initWithArray:(id __strong *)array andI:(int)i;
@end

@implementation ArraySeq
+ (instancetype)create {
  return nil;
}
+ (instancetype)create:(id __strong *)array, ... {
  if (!array)
    return nil;
  return [[ArraySeq alloc] initWithArray:array andI:0];
}
- (instancetype)initWithArray:(id __strong *)array andI:(int)i {
  self = [super init];
  if (self) {
    _array = array;
    _i = i;
  }
  return self;
}
- (int)_count {
  return _i;
}
- (id)nth:(int)n {
  return nil;
}
- (id)nth:(int)n default:(id)notFound {
  return nil;
}
- (id)first {
  return nil;
}
- (id)rest {
  return nil;
}
- (id)cons:(id)obj {
  return nil;
}
- (id)reduce:(id<IFn>)f {
  return nil;
}
- (id)reduce:(id<IFn>)f withInit:(id)start {
  return nil;
}
@end

static id createFromObject (int array[]) {
  return nil;
}

static id createFromObject (float array[]) {
  return nil;
}

static id createFromObject (double array[]) {
  return nil;
}

static id createFromObject (long array[]) {
  return nil;
}

static id createFromObject (char array[]) {
  return nil;
}

static id createFromObject (short array[]) {
  return nil;
}
