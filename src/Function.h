#import <Foundation/Foundation.h>
#import "protocols.h"

@interface Function : NSObject <IFn>
@property (copy) id(^fn)(id arg, ...);
+ (instancetype)withFn:(id(^)(id arg, ...));
- (instancetype)initWithFn:(id(^)(id arg, ...));
@end

@interface Function : NSObject <IFn>
@property (copy) id(^fn)(id arg, ...);
+ (instancetype)withFn:(id(^)(id arg, ...))fn {
  return [[Function alloc] initWithFn:fn];
}
- (instancetype)initWithFn:(id(^)(id arg, ...))fn {
  self = [super init];
  if (self)
    self.fn = fn;
  return self;
}
@end
