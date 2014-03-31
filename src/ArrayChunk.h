#import <Foundation/Foundation.h>
#import "protocols.h"

@interface ArrayChunk : NSObject <IChunk, IIndexed>
@property (nonatomic, readonly) id array[];
@property (nonatomic, readonly) int off;
@property (nonatomic, readonly) int end;
- (instancetype)initWithArray:(id [])array;
- (instancetype)initWithArray:(id [])array andOff:(int)off;
- (instancetype)initWithArray:(id [])array andOff:(int)off andEnd:(int)end;
@end

@implementation ArrayChunk
- (instancetype)initWithArray:(id [])array {}
- (instancetype)initWithArray:(id [])array andOff:(int)off {}
- (instancetype)initWithArray:(id [])array andOff:(int)off andEnd:(int)end {}
- (id)nth:(int)n {}
- (id)nth:(int)n default:(id)notFound {}
- (id<IChunk>)dropFirst {}
- (id)reduce:(id<IFn>) f start:(id)start {}
@end
