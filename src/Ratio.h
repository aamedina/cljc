#import <Foundation/Foundation.h>
#import <llvm/ADT/APInt.h>

using namespace llvm;

@interface Ratio : NSObject
@property (nonatomic, readonly) APInt numerator;
@property (nonatomic, readonly) APInt denominator;
- (instancetype)initWithNumerator:(APInt)numerator
                   andDenominator:(APInt)denominator;
@end

static const APInt ZERO(64, 0, true), ONE(64, 1, true), TEN(64, 10, true);

@implementation Ratio
- (NSString *)description {
  return [NSString stringWithFormat:@"%@/%@",
                   @(_numerator.toString(10, true).c_str()),
                   @(_denominator.toString(10, true).c_str())];
}
- (instancetype)initWithNumerator:(APInt)numerator
                   andDenominator:(APInt)denominator {
  self = [super init];
  if (self) {
    _numerator = numerator;
    _denominator = denominator;
  }
  return self;
}
@end

