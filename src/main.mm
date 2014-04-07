#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <llvm/Support/Threading.h>
#import <editline/readline.h>
#import "LispReader.h"
#import "Compiler.h"
#import "Symbol.h"
#import "Var.h"
#import "RT.h"
#import "ArrayChunk.h"
#import "ArraySeq.h"
#import "ChunkBuffer.h"

static const Symbol *CLOJURE_MAIN = [[Symbol alloc]
                                      initWithNamespace:@"clojure.main"
                                                andName:nil];
static const Var *REQUIRE = var(@"clojure.core", @"require");
static const Var *MAIN = var(@"clojure.main", @"main");

int main (int argc, const char *argv[]) {
  @autoreleasepool {
    init_reader_macros();
    static char *line = (char *)NULL;
    while (1) {
      if (line) {
        free(line);
        line = (char *)NULL;
      }
      line = readline("=> ");
      if (line && *line) {
        add_history(line);
        id ret = readString(line);
        if (ret || [ret isKindOfClass:NSNull.class])          
          fprintf(stdout, "%s\n", [[ret description] UTF8String]);
      }
    }
    exit(0);
  }
  return 0;
}
