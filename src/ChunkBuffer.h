#import <Foundation/Foundation.h>
#import "protocols.h"
#import "RT.h"

@interface ChunkBuffer : NSObject <ICounted>
@property id __strong *buffer;
@property int end;
- (instancetype)initWithCapacity:(int)capacity;
- (id<IChunk>)chunk;
- (void)add:(id)obj;
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
  return _end;
}
- (id<IChunk>)chunk {
  ArrayChunk *ret = [[ArrayChunk alloc] initWithArray:self.buffer
                                               andOff:0
                                               andEnd:self.end];
  self.buffer = nil;
  return ret;
}
- (void)add:(id)obj {
  self.buffer[self.end++] = obj;
}
@end

