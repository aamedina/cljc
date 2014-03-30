#import <Foundation/Foundation.h>

@protocol Object <NSObject>
- (NSString *)toString;
@end

@protocol ICounted
- (int)_count;
@end

@protocol IIndexed
- (id)_nth:(int)n;
- (id)_nth:(int)n default:(id)not_found;
@end

@protocol ICollection
- (id)_conj:(id)val;
@end

@protocol ISeq
- (id)_first;
- (id)_rest;
- (id)_cons:(id)obj;
@end

@protocol INext
- (id)_next;
@end

@protocol ILookup
- (id)_lookup:(id)key;
- (id)_lookup:(id)key default:(id)not_found;
@end

@protocol IAssociative
- (int)_contains_key:(id)key;
- (id)_assoc:(id)key withVal:(id)val;
@end

@protocol IMap
- (id)_dissoc:(id)key;
@end

@protocol IMapEntry
- (id)_key;
- (id)_val;
@end

@protocol ISet
- (id)_disjoin:(id)val;
@end

@protocol IStack
- (id)_peek;
- (id)_pop;
@end

@protocol IVector
- (id)_assoc_n:(int)n withVal:(id)val;
@end

@protocol ISeqable
- (id)_seq;
@end

@protocol IEmptyableCollection
- (id<ICollection>)_empty;
@end

@interface CLJBool : NSObject <Object>
@property (nonatomic, readonly) BOOL truth;
+ (instancetype)true;
+ (instancetype)false;
- (instancetype)initWithBool:(BOOL)truth;
@end

@implementation CLJBool
+ (instancetype)true {
  return [[CLJBool alloc] initWithBool:YES];
}
+ (instancetype)false {
  return [[CLJBool alloc] initWithBool:NO];
}
- (instancetype)initWithBool:(BOOL)truth {
  self = [super init];
  if (self)
    _truth = truth;
  return self;
}
- (NSString *)toString {
  return _truth ? @"true" : @"false";
}
@end

static const CLJBool *T = [CLJBool true];
static const CLJBool *F = [CLJBool false];

#define BOOLEAN const CLJBool *__strong

@protocol IEquiv
- (BOOLEAN)_equiv:(id)other;
@end

@protocol IMeta <NSObject>
- (id<IMap>)_meta;
@end

@protocol IWithMeta <NSObject>
- (id)_with_meta:(id<IMap>)meta;
@end

@interface NSNull (ASeq) <ISeq, ISeqable, Object, ICounted>
@end

@implementation NSNull (ASeq)
- (NSString *)toString {
  return @"nil";
}
- (id)_seq {
  return self;
}
- (id)_first {
  return self;
}
- (id)_rest {
  return @[];
}
- (id)_cons:(id)obj {
  return @[obj];
}
- (int)_count {
  return 0;
}
@end

static const NSNull *NIL = [NSNull null];

@interface NSNumber (Printable) <Object>
@end

@implementation NSNumber (Printable)
- (NSString *)toString {
  return [self description];
}
@end

@interface NSRegularExpression (Printable) <Object>
@end

@implementation NSRegularExpression (Printable)
- (NSString *)toString {
  return [NSString stringWithFormat:@"#\"%@\"", [self pattern]];
}
@end

@interface NSObject (Printable) <Object>
@end

@implementation NSObject (Printable)
- (NSString *)toString {
  if ([[[self class] description] isEqual:@"__NSGlobalBlock__"])    
    return [NSString stringWithFormat:@"#%@", [self description]];
  else
    return [self description];
}
@end

@protocol IFn <NSObject>
@optional
- (id)invoke;
- (id)invoke:(id)a;
- (id)invoke:(id)a and:(id)b;
- (id)invoke:(id)a and:(id)b and:(id)c;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o and:(id)p;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o and:(id)p and:(id)q;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o and:(id)p and:(id)q and:(id)r;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o and:(id)p and:(id)q and:(id)r and:(id)s;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o and:(id)p and:(id)q and:(id)r and:(id)s and:(id)t;
- (id)invoke:(id)a and:(id)b and:(id)c and:(id)d and:(id)e and:(id)f and:(id)g
         and:(id)h and:(id)i and:(id)j and:(id)k and:(id)l and:(id)m and:(id)n
         and:(id)o and:(id)p and:(id)q and:(id)r and:(id)s and:(id)t
         and:(id<ISeq>)xs;
- (id)applyTo:(id<ISeq>)arglist;
@end
