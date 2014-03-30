#import <Foundation/Foundation.h>
#import <llvm/ADT/StringRef.h>
#import <llvm/ADT/Twine.h>
#import <llvm/ADT/APInt.h>
#import <llvm/ADT/APFloat.h>
#import "protocols.h"
#import "NSArray.h"
#import "Cons.h"
#import "NSDictionary.h"
#import "NSSet.h"
#import "NSString.h"
#import "Ratio.h"
#import "BigInt.h"
#import "Symbol.h"
#import "Keyword.h"

template <typename T>
static NSNumber *num (T x) {
  return @0;
}

static APInt divide (APInt x, APInt y) {
  return x / y;
}

template <typename T>
static APInt divide (T x, APInt y) {
  return x / y;
}

template <typename S>
static APInt divide (APInt x, S y) {
  return x / y;
}

template <typename T, typename S>
static T divide (T x, S y) {
  return x / y;
}

template <typename T, typename S>
static S divide (T x, S y) {
  return x / y;
}
