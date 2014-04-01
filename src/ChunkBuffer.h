#import <Foundation/Foundation.h>
#import "protocols.h"
#import "RT.h"

@interface ChunkBuffer : NSObject <ICounted>
@property id __strong *buffer;
@property int end;
- (instancetype)initWithCapacity:(int)capacity;
@end

@implementation ChunkBuffer
- (instancetype)initWithCapacity:(int)capacity {
  self = [super init];
  if (self) {
    self.buffer = new id[capacity];
  }  
  return self;
}
- (int)_count {
  return 0;
}
@end
