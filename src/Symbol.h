#import <Foundation/Foundation.h>
#import "protocols.h"

@interface Symbol : NSObject <Object, IMeta, IWithMeta>
@property (nonatomic, readonly) NSString *ns;
@property (nonatomic, readonly) NSString *name;
@property (nonatomic, readonly) id<IMap> _meta;
+ (instancetype)intern:(NSString *)name;
- (instancetype)initWithName:(NSString *)nsname;
- (instancetype)initWithNamespace:(NSString *)ns andName:(NSString *)name;
- (instancetype)initWithMeta:(id<IMap>)meta
                andNamespace:(NSString *)ns
                     andName:(NSString *)name;
@end

@implementation Symbol
+ (instancetype)intern:(NSString *)name {
  return [[Symbol alloc] initWithName:name];
}
- (instancetype)initWithName:(NSString *)nsname {
  self = [super init];
  if (self)
    _name = nsname;
  return self;
}
- (instancetype)initWithNamespace:(NSString *)ns andName:(NSString *)name {
  self = [self initWithName:name];
  if (self)
    _ns = ns;
  return self;
}
- (instancetype)initWithMeta:(id<IMap>)meta
                andNamespace:(NSString *)ns
                     andName:(NSString *)name {
  self = [self initWithNamespace:ns andName:name];
  if (self)
    __meta = meta; 
  return self;
}
- (NSString *)toString {
  if (_ns && _name)
    return [NSString stringWithFormat:@"%@/%@", _ns, _name];
  return [NSString stringWithFormat:@"%@", _name];    
}
- (id<IMap>)meta {
  return self.meta;
}
- (id)withMeta:(id<IMap>)meta {  
  return [[Symbol alloc] initWithMeta:meta
                         andNamespace:self.ns
                              andName:self.name];
}
@end
