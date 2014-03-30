#import <Foundation/Foundation.h>
#import <CoreFoundation/CoreFoundation.h>
#import <editline/readline.h>
#import "LispReader.h"

int main (int argc, const char *argv[]) {
  @autoreleasepool {
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
  return 0;
}
