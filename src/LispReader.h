#import <Foundation/Foundation.h>
#import <llvm/ADT/StringRef.h>
#import <llvm/ADT/Twine.h>
#import <llvm/ADT/APInt.h>
#import <llvm/ADT/APSInt.h>
#import <llvm/ADT/APFloat.h>
#import "protocols.h"
#import "RT.h"
#import "NSArray.h"
#import "Cons.h"
#import "NSDictionary.h"
#import "NSSet.h"
#import "NSString.h"
#import "Ratio.h"
#import "BigInt.h"
#import "Symbol.h"
#import "Keyword.h"
#import <iostream>
#import <string>

using namespace llvm;

static Symbol *QUOTE = [[Symbol alloc] initWithName:@"quote"];
static Symbol *VAR = [[Symbol alloc] initWithName:@"var"];
static Symbol *SYNTAX_QUOTE = [[Symbol alloc] initWithName:@"syntax-quote"];
static Symbol *UNQUOTE = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                   andName:@"unquote"];
static Symbol *UNQUOTE_SPLICING =
    [[Symbol alloc] initWithNamespace:@"clojure.core"
                              andName:@"unquote-splicing"];
static Symbol *CONCAT = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                  andName:@"concat"];
static Symbol *SEQ = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                               andName:@"seq"];
static Symbol *LIST = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                andName:@"list"];
static Symbol *APPLY = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                 andName:@"apply"];
static Symbol *HASHMAP = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                 andName:@"hash-map"];
static Symbol *HASHSET = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                 andName:@"hash-set"];
static Symbol *VECTOR = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                  andName:@"vector"];
static Symbol *WITH_META = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                     andName:@"with-meta"];
static Symbol *META = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                andName:@"meta"];
static Symbol *DEREF = [[Symbol alloc] initWithNamespace:@"clojure.core"
                                                 andName:@"deref"];
static Keyword *UNKNOWN =
    [[Keyword alloc] initWithName:[[Symbol alloc] initWithName:@"unknown"]];

@protocol Reader <NSObject>
- (unichar)read;
- (unichar)peek;
@end

@interface PushbackReader : NSObject <Reader>
@property NSMutableString *buf;
- (void)unread:(unichar)ch;
@end

@implementation PushbackReader
- (unichar)read {
  @synchronized(_buf) {
    if ([_buf length] > 0) {
      unichar ch = [_buf characterAtIndex:0];
      [_buf deleteCharactersInRange:NSMakeRange(0,1)];
      return ch;
    }
    return nil;
  }
}
- (unichar)peek {
  return [_buf characterAtIndex:0];
}
- (void)unread:(unichar)ch {
  @synchronized(_buf) {
    [_buf insertString:[NSString stringWithFormat:@"%C", ch] atIndex:0];
  }
}
- (instancetype)init {
  self = [super init];
  if (self) {
    _buf = [NSMutableString string];
  }
  return self;
}
- (instancetype)initWithString:(NSString*)str {
  self = [self init];
  if (self) {
    [_buf setString:str];
  }
  return self;
}
@end

static unichar macros[256] = {'"', ';', '\'', '@', '^', '`', '~', '(', ')',
                              '[', ']', '{', '}', '\\', '%', '#'};

static unichar dispatchMacros[256] = {'"', '\'', '^', '(', '{', '=', '!', '_'};

static BOOL isMacro (unichar ch) {
  return (ch < 256 && macros[ch]);
}

static BOOL isTerminatingMacro (unichar ch) {
  return (ch != '#' && ch != '\'' && ch != '%' && isMacro(ch));
}

static id matchSymbol (NSString *s) {
  BOOL isKeyword = NO;
  if ([s characterAtIndex:0] == ':') {
    isKeyword = YES;
    s = [s substringFromIndex:1];
  }
  NSArray *nsname = [s componentsSeparatedByString:@"/"];
  NSString *ns = nil;
  NSString *name = nil;
  if ([nsname count] == 1)
    name = [nsname objectAtIndex:0];
  else if ([nsname count] == 2) {
    ns = [nsname objectAtIndex:0];
    name = [nsname objectAtIndex:1];
  }
  if (isKeyword) {
    Symbol *nsSymbol = nil;
    Symbol *nameSymbol = nil;
    if (ns) {
      nsSymbol = [[Symbol alloc] initWithName: ns];
      nameSymbol = [[Symbol alloc] initWithName: name];
      return [[Keyword alloc] initWithNamespace: nsSymbol andName: nameSymbol];
    }
    nameSymbol = [[Symbol alloc] initWithName: name];
    return [[Keyword alloc] initWithName: nameSymbol];
  }
  if (ns)
    return [[Symbol alloc] initWithNamespace:ns andName:name];
  return [[Symbol alloc] initWithName:name];
}

static id interpretToken (NSString *s) {
  if ([s isEqualToString:@"nil"]) {
    return NIL;
  } else if ([s isEqualToString:@"true"]) {
    return T;
  } else if ([s isEqualToString:@"false"]) {
    return F;
  }
  id ret = nil;
  ret = matchSymbol(s);
  if (ret) return ret;
  @throw [NSString stringWithFormat:@"Invalid token: %@", s];
}

static NSString *readToken (PushbackReader *rdr, unichar ch) {
  NSMutableString *str = [NSMutableString string];
  [str appendFormat:@"%C", ch];
  for (;;) {
    ch = [rdr read];
    if (!ch) return str;
    if (isspace(ch) || ch == ',' || isTerminatingMacro(ch)) {
      [rdr unread:ch];      
      return str;
    }
    [str appendFormat:@"%C", ch];
  }
}

static NSArray *regexWithResults(NSString *haystack,
                                 NSString *pattern){
  NSArray *arr = [NSArray array];
  NSArray *arrTextCheckingResults;
  NSError *err = NULL;
  NSRegularExpression *regex =
      [NSRegularExpression
        regularExpressionWithPattern:pattern
                             options:NSRegularExpressionAnchorsMatchLines
                               error:&err];
  NSMutableArray *arrMutable = [NSMutableArray array]; 
  NSTextCheckingResult *match =
      [regex firstMatchInString:haystack
                        options:0
                          range:NSMakeRange(0, [haystack length])];
  for (int i = 1; i < match.numberOfRanges; i++) {
    if ([match rangeAtIndex:i].location < [haystack length]) {
      NSString *capture = [haystack substringWithRange:[match rangeAtIndex:i]];
      [arrMutable addObject:capture];
    } else {
      [arrMutable addObject:NIL];
    }
  }
  arr = arrMutable;
  return arr;
}

static id readString (const char *s);

static NSString *intPat =
    @"([-+]?)(?:(0)|([1-9][0-9]*)|0[xX]([0-9A-Fa-f]+)|0([0-7]+)|([1-9][0-9]?)[rR]([0-9A-Za-z]+)|0[0-9]+)(N)?";
static NSString *floatPat = @"([-+]?[0-9]+(\\.[0-9]*)?([eE][-+]?[0-9]+)?)(M)?";
static NSString *ratioPat = @"([-+]?[0-9]+)/([0-9]+)";

static id matchNumber (NSMutableString *str) {  
  NSArray *groups =
      regexWithResults([NSString stringWithString:str], ratioPat);
  if ([groups count] == 2) {
    StringRef numerator([groups[0] UTF8String]);
    if (numerator.front() == '+')
      numerator = numerator.drop_front();
    StringRef denominator([groups[1] UTF8String]);
    unsigned bitsNeeded = APInt::getBitsNeeded(numerator, 10) + 1;
    unsigned bitsNeededDenom = APInt::getBitsNeeded(denominator, 10) + 1;
    APInt uapnum(bitsNeeded, numerator, 10);
    APSInt apnum(uapnum);    
    APInt uapdenom(bitsNeededDenom, denominator, 10);
    APSInt apdenom(uapdenom);
    BigInt *bign = [BigInt fromAPInt:apnum];
    BigInt *bigd = [BigInt fromAPInt:apdenom];
    return [[Ratio alloc]initWithNumerator:apnum andDenominator:apdenom];      
  }
  groups = regexWithResults([NSString stringWithString:str], floatPat);
  if (groups[1] != NIL) {
    StringRef n([groups[0] UTF8String]);
    return @(APFloat(APFloat::IEEEdouble, n).convertToDouble());
  }
  groups = regexWithResults([NSString stringWithString:str], intPat);
  if ([groups count] == 8) {
    if (groups[1] != NIL) {
      if (groups[7] != NIL)
        return [BigInt ZERO];
      return @0;
    }
    bool negate = ([str characterAtIndex:0] == '-');
    NSString *n;
    int radix = 10;
    if (groups[2] != NIL) {
      n = groups[2];
      radix = 10;
    } else if (groups[3] != NIL) {
      n = groups[3];
      radix = 16;
    } else if (groups[4] != NIL) {
      n = groups[4];
      radix = 8;
    } else if (groups[6] != NIL) {
      n = groups[6];
      radix = stoi(std::string([groups[5] UTF8String]));
    }
    if (!n) return nil;
    StringRef num([n UTF8String]);
    unsigned bitsNeeded = APSInt::getBitsNeeded(num, radix) + 2;    
    APInt ubignum(bitsNeeded, num, radix);
    APSInt bignum(ubignum);
    if (negate) bignum = -bignum;
    if (groups[7] != NIL) return [BigInt fromAPInt:bignum];
    else if (bitsNeeded < 64) return @(bignum.getSExtValue());
    else return [BigInt fromAPInt:bignum];
  }
  return nil;
}

static id readNumber (PushbackReader *rdr, unichar init) {
  NSMutableString *sb = [NSMutableString string];
  [sb appendFormat:@"%C", init];
  for (;;) {
    unichar ch = [rdr read];
    if (!ch || isspace(ch) || ch == ',' || isMacro(ch)) {
      [rdr unread:ch];
      break;
    }
    [sb appendFormat:@"%C", ch];
  }
  id n = matchNumber(sb);
  if (!n) @throw [NSString stringWithFormat:@"Invalid number: %@", sb];
  return n;  
}

static id read (PushbackReader *rdr, BOOL eofIsError, id eofValue,
                BOOL isRecursive);

static NSMutableArray *readDelimitedList(unichar delim, PushbackReader *rdr,
                                         BOOL isRecursive);

static id readMacro (PushbackReader *rdr, unichar init) {
  unichar ch = init;
  switch (ch) {
    case ';':
      return NIL;
    case '"': {
      NSMutableString *str = [NSMutableString string];
      ch = [rdr read];
      while (ch != '"') {
        [str appendFormat:@"%C", ch];
        ch = [rdr read];
      }
      return str;
    }
    case '\\': {
      ch = [rdr read];
      return [NSString stringWithFormat:@"%C", ch];
    }
    case '\'':
      return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)] _cons:QUOTE];
    case '@':
      return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)] _cons:DEREF];
    case '^':
      return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)] _cons:WITH_META];
    case '`':
      return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)] _cons:SYNTAX_QUOTE];
    case '~': {
      ch = [rdr read];
      if (ch == '@') {
        return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)]
                 _cons:UNQUOTE_SPLICING];
      } else {
        [rdr unread:ch];
        return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)] _cons:UNQUOTE];
      }      
    }      
    case '(': {
      NSArray *arr = readDelimitedList(')', rdr, YES);
      if ([arr count] == 0)
        return [Cons EMPTY];      
      return [Cons create:arr];
    }          
    case ')':
      @throw @"Unmatched delimiter error";
    case '[': {
      NSArray *lst = readDelimitedList(']', rdr, YES);
      return lst;
    }      
    case ']':
      @throw @"Unmatched delimiter error";
    case '{': {
      NSArray *arr = readDelimitedList('}', rdr, YES);
      if ([arr count] == 0)
        return [NSDictionary dictionary];
      if (([arr count] % 2) != 0)
        @throw @"Must initialize a map with an even number of elements";
      NSMutableDictionary *dict = [NSMutableDictionary dictionary];
      for(int i=0;i<arr.count;i+=2){
        [dict setObject:[arr objectAtIndex:i+1]
                 forKey:[arr objectAtIndex:i]];
      }
      return dict;
    }
    case '}':
      @throw @"Unmatched delimiter error";
    case '#':
      ch = [rdr read];
      switch (ch) {
        case '\'':
          return [[[Cons EMPTY] _cons:read(rdr, YES, nil, NO)] _cons:VAR];
        case '"': {
          NSMutableString *str = [NSMutableString string];
          ch = [rdr read];
          while (ch != '"') {
            [str appendFormat:@"%C", ch];
            ch = [rdr read];
          }
          NSString *s = [NSRegularExpression escapedTemplateForString:str];
          NSError *err = NULL;
          return [NSRegularExpression regularExpressionWithPattern:s
                                                           options:1<<4
                                                             error:&err];
        }
        case '(':
          return createFunction();
        case '{': {
          NSArray *arr = readDelimitedList('}', rdr, true);
          if ([arr count] == 0)
            return [NSSet set];
          else
            return [NSSet setWithArray:arr];
        }
        case '_':
          return NIL;
        default:
          return nil;
      }
    default:
      return nil;
  }
}

static NSMutableArray *readDelimitedList(unichar delim, PushbackReader *rdr,
                                         BOOL isRecursive) {
  NSMutableArray *arr = [NSMutableArray array];
  unichar ch = [rdr read];
  while (ch && ch != delim) {
    if (!isspace(ch) || ch != ',') {
      if (isMacro(ch)) {
        [arr addObject:readMacro(rdr, ch)];
      } else {
        [rdr unread:ch];
        [arr addObject:read(rdr, YES, nil, isRecursive)];      
      }     
    }
    ch = [rdr read];
  }
  return arr;
}

static id read (PushbackReader *rdr, BOOL eofIsError, id eofValue,
                BOOL isRecursive) {
  @try {
    for (;;) {
      unichar ch = [rdr read];
      while (isspace(ch) || ch == ',') ch = [rdr read];
      if (isdigit(ch)) {
        return readNumber(rdr, ch);
      }
      id form = readMacro(rdr, ch);
      if (form) return form;
      if (ch == '+' || ch == '-') {
        unichar ch2 = [rdr read];
        if (isdigit(ch2)) {
          [rdr unread:ch2];
          return readNumber(rdr, ch);
        }
        [rdr unread:ch2];
      }
      NSString *token = readToken(rdr, ch);
      return interpretToken(token);
    }
  } @catch (NSString *err) {
    NSLog(@"Exception: %@", err);
    return NIL;
  }
}

static id readString (const char *s) {
  NSString *str = [NSString stringWithUTF8String:s];
  return read([[PushbackReader alloc] initWithString:str], YES, nil, NO);
}
