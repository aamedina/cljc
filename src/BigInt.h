#import <Foundation/Foundation.h>
#import <llvm/ADT/APInt.h>
#import <llvm/ADT/APSInt.h>
#import <llvm/ADT/APFloat.h>
#import <llvm/ADT/StringExtras.h>
#import <llvm/ADT/StringRef.h>
#import <llvm/Support/MathExtras.h>
#import <llvm/ADT/SmallVector.h>

using namespace llvm;

@interface BigInt : NSObject <Object>
@property (nonatomic, readonly) int64_t lpart;
@property (nonatomic, readonly) APSInt bipart;
+ (BigInt *)ZERO;
+ (BigInt *)ONE;
+ (BigInt *)valueOf:(int64_t)val;
+ (BigInt *)fromAPInt:(APInt)val;
- (instancetype)initWithNumber:(int64_t)lpart;
- (instancetype)initWithNumber:(int64_t)lpart withBipart:(APInt)bipart;
- (unsigned)bitLength;
- (BigInt *)add:(BigInt *)y;
- (BigInt *)multiply:(BigInt *)y;
- (BigInt *)quotient:(BigInt *)y;
- (BigInt *)remainder:(BigInt *)y;
- (BOOL)lt:(BigInt *)y;
- (APInt)toAPInt;
@end

@implementation BigInt
+ (BigInt *)ZERO {
  return [[BigInt alloc] initWithNumber:0 withBipart:APInt()];
}
+ (BigInt *)ONE {
  return [[BigInt alloc] initWithNumber:1 withBipart:APInt()];
}
+ (BigInt *)valueOf:(int64_t)val {
  return [[BigInt alloc] initWithNumber:val];
}
+ (BigInt *)fromAPInt:(APInt)val {
  if (val.getBitWidth() < 64)
    return [[BigInt alloc] initWithNumber:val.getLimitedValue()];
  else
    return [[BigInt alloc] initWithNumber:0 withBipart:val];
}
- (instancetype)initWithNumber:(int64_t)lpart {
  self = [super init];
  if (self) {
    _lpart = lpart;
    _bipart = ZERO;
  }
  return self;
}
- (instancetype)initWithNumber:(int64_t)lpart withBipart:(APInt)bipart {
  self = [super init];
  if (self) {
    _lpart = lpart;
    _bipart = bipart;
  }
  return self;
}
- (unsigned)bitLength {
  return _bipart.getBitWidth();
}
- (BigInt *)add:(BigInt *)y {
  if ((_bipart == 0) && (y.bipart == 0)) {
    int64_t ret = _lpart + y.lpart;
    if ((ret ^ _lpart) >= 0 || (ret ^ y.lpart) >= 0)
      return [BigInt valueOf:ret];
  }
  return [BigInt fromAPInt:([self toAPInt] + [y toAPInt])];
}
- (BigInt *)multiply:(BigInt *)y {
  if ((_bipart == 0) && (y.bipart == 0)) {
    int64_t ret = _lpart * y.lpart;
    if (y.lpart == 0 ||
        ((ret / y.lpart == _lpart) &&
         (_lpart != std::numeric_limits<int64_t>::max())))
      return [BigInt valueOf:ret];
  }
  return [BigInt fromAPInt:([self toAPInt] * [y toAPInt])];
}
- (BigInt *)quotient:(BigInt *)y {
  if ((_bipart == 0) && (y.bipart == 0)) {
    return [BigInt valueOf:(_lpart / y.lpart)];
  }
  return [BigInt fromAPInt:[self toAPInt].sdiv([y toAPInt])];
}
- (BigInt *)remainder:(BigInt *)y {
  if ((_bipart == 0) && (y.bipart == 0)) {
    return [[BigInt alloc] initWithNumber:(_lpart % y.lpart)];
  }
  return [BigInt fromAPInt:[self toAPInt].srem([y toAPInt])];
}
- (BOOL)lt:(BigInt *)y {
  if ((_bipart == 0) && (y.bipart == 0)) {
    return _lpart < y.lpart;
  }
  return [self toAPInt].ult([y toAPInt]);
}
- (NSString *)toString {
  if (_bipart == 0)
    return [NSString stringWithFormat:@"%llu", _lpart];
  else
    return @(_bipart.toString(10, true).c_str());
}
- (APInt)toAPInt {
  if (_bipart == 0)
    return APInt(64, _lpart, true);
  else
    return _bipart;
}
- (APFloat)toAPFloat {
  if (_bipart == 0)
    return APFloat((double) _lpart);
  else
    return APFloat(_bipart.roundToDouble());
}
@end

// @interface BigFloat : NSObject <Object>
// @property (nonatomic, readonly) APFloat bignum;
// - (instancetype)initWithFloat:(float)f;
// - (instancetype)initWithDouble:(double)d;
// - (instancetype)initWithString:(NSString *)str;
// @end

// @implementation BigFloat
// - (instancetype)initWithFloat:(float)f {
//   self = [super init];
//   if (self) {
//     APFloat flt(f);
//     _bignum = flt;
//   }
//   return self;
// }
// - (instancetype)initWithDouble:(double)d {
//   self = [super init];
//   if (self) {
//     APFloat flt(d);
//     _bignum = flt;
//   }
//   return self;
// }
// - (instancetype)initWithString:(NSString *)str {
//   self = [super init];
//   if (self) {
//     StringRef n([str UTF8String]);
//     APFloat flt(APFloat::IEEEdouble, n);
//     _bignum = flt;
//   }
//   return self;
// }
// - (NSString *)toString {
//   SmallVectorImpl<char> str(1024);
//   _bignum.toString(&str, 0, 1024);
//   StringRef s(str);
//   return @(s.str().c_str());
// }
// @end
