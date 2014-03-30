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

@class PushbackReader;

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

static id macros[256];
static id dispatchMacros[256];

static id (^string_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^comment_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^discard_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^wrapping_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^var_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^dispatch_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^fn_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^arg_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};


static id (^meta_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^syntax_quote_reader)(PushbackReader *rdr) =
    ^id (PushbackReader *rdr) {
  return nil;
};

static id (^unquote_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^char_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^list_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^eval_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^vector_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^map_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^set_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^regex_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static id (^unmatched_delimiter_reader)(PushbackReader *rdr) =
    ^id (PushbackReader *rdr) {
  return nil;
};

static id (^unreadable_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr)
{
  return nil;
};

static id (^ctor_reader)(PushbackReader *rdr) = ^id (PushbackReader *rdr) {
  return nil;
};

static void init_reader_macros () {
  macros['"'] = string_reader;
  macros[';'] = comment_reader;
  macros['\''] = wrapping_reader;
  macros['@'] = wrapping_reader;
  macros['^'] = meta_reader;
  macros['`'] = syntax_quote_reader;
  macros['~'] = unquote_reader;
  macros['('] = list_reader;
  macros[')'] = unmatched_delimiter_reader;
  macros['['] = vector_reader;
  macros[']'] = unmatched_delimiter_reader;
  macros['{'] = map_reader;
  macros['}'] = unmatched_delimiter_reader;
  macros['\\'] = char_reader;
  macros['%'] = arg_reader;
  macros['#'] = dispatch_reader;

  dispatchMacros['^'] = meta_reader;
  dispatchMacros['\''] = var_reader;
  dispatchMacros['"'] = regex_reader;    
  dispatchMacros['('] = fn_reader;
  dispatchMacros['{'] = set_reader;
  dispatchMacros['='] = eval_reader;
  dispatchMacros['!'] = comment_reader;
  dispatchMacros['<'] = unreadable_reader;
  dispatchMacros['_'] = discard_reader;
}

static BOOL isMacro (unichar ch) {
  return (ch < 256 && macros[ch]);
}

static BOOL isTerminatingMacro (unichar ch) {
  return (ch != '#' && ch != '\'' && ch != '%' && isMacro(ch));
}

