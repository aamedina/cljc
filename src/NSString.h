#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSString (ASeq) <ISeq, ISeqable, Object>
@end

@implementation NSString (ASeq)
- (NSString *)toString {
  return [NSString stringWithFormat:@"\"%@\"", self];
}
- (id)_seq {
  return [self componentsSeparatedByString:@""];
}
- (id)_first {
  return [[self _seq] _first];
}
- (id)_rest {
  return [[self _seq] _rest];
}
- (id)_cons:(NSString *)obj {
  return [[self _seq] _cons:obj];
}
@end
