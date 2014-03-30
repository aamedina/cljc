#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <llvm/Support/Threading.h>
#import <editline/readline.h>
#import "LispReader.h"
#import "Symbol.h"
#import "Var.h"
#import "RT.h"

static const Symbol *CLOJURE_MAIN = [[Symbol alloc]
                                      initWithNamespace:@"clojure.main"
                                                andName:nil];
static const Var *REQUIRE = RT::var(@"clojure.core", @"require");
static const Var *MAIN = RT::var(@"clojure.main", @"main");

int main (int argc, const char *argv[]) {
  llvm_start_multithreaded();
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
        fprintf(stdout, "%s\n", [[ret toString] UTF8String]);
      }
    }
    exit(0);
  }
  llvm_stop_multithreaded();
  return 0;
}
