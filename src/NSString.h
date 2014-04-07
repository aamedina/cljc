#import <Foundation/Foundation.h>
#import "protocols.h"

@interface NSString (ASeq) <ISeq, ISeqable>
@end

@implementation NSString (ASeq)
- (NSString *)description {
  return [NSString stringWithFormat:@"\"%@\"", self];
}
- (id)seq {
  return [self componentsSeparatedByString:@""];
}
- (id)first {
  return [[self seq] first];
}
- (id)rest {
  return [[self seq] rest];
}
- (id)cons:(NSString *)obj {
  return [[self seq] cons:obj];
}
@end
