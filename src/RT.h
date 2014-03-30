#import <Foundation/Foundation.h>
#import "protocols.h"

id (^createFunction())() {
  return ^id () { return nil; };
}
