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

static id readNumber (PushbackReader *rdr, unichar init) {
  BOOL isIntegral = YES;
  BOOL isNegative = NO;
  BOOL isRational = NO;
  NSMutableString *number = [NSMutableString string];
  NSMutableString *numerator = [NSMutableString string];
  NSMutableString *denominator = [NSMutableString string];
  unichar ch = init;
  for (;;) {
    if (!ch) break;
    if (isdigit(ch) || ch == '.' || ch == '/') {
      if ((ch == '.' && !isIntegral) || (ch == '/' && isRational))
        @throw @"malformed number";
      if (ch == '.' && isIntegral)
        isIntegral = NO;
      if (ch == '/' && !isRational) {
        isRational = YES;
        [numerator appendString:number];
      } else if (isRational) {
        [denominator appendFormat:@"%C", ch];
      } else {
        [number appendFormat:@"%C", ch];
      }
      ch = [rdr read];
    } else {
      [rdr unread:ch];
      break;
    }
  }
  StringRef n([number UTF8String]);
  if (isRational) {
    StringRef num([numerator UTF8String]);
    StringRef d([denominator UTF8String]);
    unsigned bitsForNumerator = APInt::getBitsNeeded(num, 10) + 1;
    unsigned bitsForDenominator = APInt::getBitsNeeded(d, 10) + 1;
    return [[Ratio alloc] initWithNumerator:APInt(bitsForNumerator, num, 10)
                             andDenominator:APInt(bitsForDenominator, d, 10)];
  } else if (isIntegral) {
    unsigned bitsNeeded = APInt::getBitsNeeded(n, 10) + 1;
    APInt ap_int(bitsNeeded, n, 10);
    if (bitsNeeded > 64) {
      return [BigInt fromAPInt:ap_int];
    } else {
      return @(ap_int.getLimitedValue());
    }
  } else {
    APFloat ap_float(APFloat::IEEEdouble, n);
    return @(ap_float.convertToDouble());
  }
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
      NSArray *arr = readDelimitedList(')', rdr, true);
      if ([arr count] == 0)
        return [Cons EMPTY];      
      return [Cons create:arr];
    }          
    case ')':
      @throw @"Unmatched delimiter error";
    case '[': {
      NSArray *lst = readDelimitedList(']', rdr, true);
      return lst;
    }      
    case ']':
      @throw @"Unmatched delimiter error";
    case '{': {
      NSArray *lst = readDelimitedList('}', rdr, true);
      return lst;
    }      
    case '}':
      @throw @"Unmatched delimiter error";
    case '#':
      ch = [rdr read];
      switch (ch) {
        case '^':
          return @"#^";
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
          return [NSRegularExpression regularExpressionWithPattern:s
                                                           options:nil
                                                             error:nil];
        }
        case '(':
          return @"#()";
        case '{': {
          NSArray *lst = readDelimitedList('}', rdr, true);
          return lst;
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
  for (;;) {
    unichar ch = [rdr read];
    while (isspace(ch) || ch == ',')
      ch = [rdr read];
    
    if (!ch) @throw @"EOF while reading";
    
    if (ch == delim) break;
    
    if (isMacro(ch))
      [arr addObject:readMacro(rdr, ch)];
    else {
      [rdr unread:ch];
      [arr addObject:read(rdr, true, nil, isRecursive)];      
    }    
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
