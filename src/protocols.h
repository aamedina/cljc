#import <Foundation/Foundation.h>

@protocol Object <NSObject>
- (NSString *)toString;
@end

@protocol ICounted <NSObject>
- (int)_count;
@end

@protocol IIndexed <NSObject>
- (id)nth:(int)n;
- (id)nth:(int)n default:(id)notFound;
@end

@protocol ICollection <NSObject>
- (id)conj:(id)val;
@end

@protocol ISeq <NSObject>
- (id)first;
- (id)rest;
- (id)cons:(id)obj;
@end

@protocol INext <NSObject>
- (id)next;
@end

@protocol ILookup <NSObject>
- (id)lookup:(id)key;
- (id)lookup:(id)key default:(id)not_found;
@end

@protocol IAssociative <NSObject>
- (int)containsKey:(id)key;
- (id)assoc:(id)key withVal:(id)val;
@end

@protocol IMap <NSObject>
- (id)dissoc:(id)key;
@end

@protocol IMapEntry <NSObject>
- (id)key;
- (id)val;
@end

@protocol ISet <NSObject>
- (id)disjoin:(id)val;
@end

@protocol IStack <NSObject>
- (id)peek;
- (id)pop;
@end

@protocol IVector <NSObject>
- (id)assocN:(int)n withVal:(id)val;
@end

@protocol ISeqable <NSObject>
- (id)seq;
@end

@protocol IEmptyableCollection <NSObject>
- (id<ICollection>)empty;
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

@protocol IEquiv <NSObject>
- (BOOLEAN)equiv:(id)other;
@end

@protocol IMeta <NSObject>
- (id<IMap>)meta;
@end

@protocol IWithMeta <NSObject>
- (id)withMeta:(id<IMap>)meta;
@end

@interface NSNull (ASeq) <ISeq, ISeqable, Object, ICounted>
@end

@implementation NSNull (ASeq)
- (NSString *)toString {
  return @"nil";
}
- (id)seq {
  return self;
}
- (id)first {
  return self;
}
- (id)rest {
  return @[];
}
- (id)cons:(id)obj {
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
- (id)invoke:(id)a with:(id)b;
- (id)invoke:(id)a with:(id)b with:(id)c;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o with:(id)p;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o with:(id)p with:(id)q;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o with:(id)p with:(id)q with:(id)r;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o with:(id)p with:(id)q with:(id)r with:(id)s;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o with:(id)p with:(id)q with:(id)r with:(id)s with:(id)t;
- (id)invoke:(id)a with:(id)b with:(id)c with:(id)d with:(id)e with:(id)f with:(id)g
         with:(id)h with:(id)i with:(id)j with:(id)k with:(id)l with:(id)m with:(id)n
         with:(id)o with:(id)p with:(id)q with:(id)r with:(id)s with:(id)t
         with:(id<ISeq>)xs;
- (id)applyTo:(id<ISeq>)arglist;
@end

@protocol ISequential <NSObject>
@end

@protocol IList <NSObject>
@end

@protocol IRecord <NSObject>
@end

@protocol IReversible <NSObject>
- (id)rseq;
@end

@protocol IReduce <NSObject>
- (id)reduce:(id<IFn>)f;
- (id)reduce:(id<IFn>)f withInit:(id)start;
@end

@protocol ISorted <NSObject>
- (id)sortedSeq:(bool)ascending;
- (id)sortedSeqFrom:(id)key isAscending:(bool)ascending;
- (id)entryKey:(id)entry;
- (id)comparator;
@end

@protocol IWriter <NSObject>
- (void)write:(NSString *)str;
- (void)flush;
@end

@protocol IPrintWithWriter <NSObject>
- (id)prWriter:(id<IWriter>)writer withOpts:(id<IMap>)opts;
@end

@protocol IPending <NSObject>
- (bool)isRealized;
@end

@protocol IDeref <NSObject>
- (id)deref;
@end

@protocol IWatchable <NSObject>
- (id)notifyWatches:(id)oldval withNewVal:(id)newval;
- (id)addWatch:(id)key withFunction:(id)f;
- (id)removeWatch:(id)key;
@end

@protocol IEditableCollection <NSObject>
- (id)asTransient;
@end

@protocol ITransientCollection <NSObject>
- (id)conj:(id)val;
- (id)persistent;
@end

@protocol ITransientAssociative <NSObject>
- (id)assoc:(id)key withVal:(id)val;
@end

@protocol ITransientMap <NSObject>
- (id)dissoc:(id)key;
@end

@protocol ITransientVector <NSObject>
- (id)assocN:(NSValue *)n withVal:(id)val;
- (id)pop;
@end

@protocol ITransientSet <NSObject>
- (id)disjoin:(id)val;
@end

@protocol IComparable <NSObject>
- (id)compare:(id)obj;
@end

@protocol IChunk <NSObject>
- (id<IChunk>)dropFirst;
- (id)reduce:(id<IFn>) f start:(id)start;
@end

@protocol IChunkedSeq <NSObject>
- (id)chunkedFirst;
- (id)chunkedRest;
@end

@protocol IChunkedNext <NSObject>
- (id)chunkedNext;
@end

@protocol INamed <NSObject>
- (id)name;
- (id)namespace;
@end

@protocol IProxy <NSObject>
- (id)initClojureFnMappings:(id<IMap>)m;
- (id)updateClojureFnMappings:(id<IMap>)m;
- (id<IMap>)getClojureFnMappings;
@end

@protocol IReference <NSObject>
- (id<IMap>)alterMeta:(id<IFn>)alter withArgs:(id<ISeq>)args;
- (id<IMap>)resetMeta:(id<IMap>)m;
@end

@protocol IAtom <NSObject>
@end

@protocol IReset <NSObject>
- (id)reset:(id)new_value;
@end

@protocol ISwap <NSObject>
- (id)swap:(id)f;
- (id)swap:(id)f withArg:(id)a;
- (id)swap:(id)f withArg:(id)a andArg:(id)b;
- (id)swap:(id)f withArg:(id)a andArg:(id)b andVarArgs:(NSArray *)xs;
@end

@protocol IMultiFn <NSObject>
- (id)reset;
- (id)addMethod:(id)dispatch_val withMethod:(id)method;
- (id)removeMethod:(id)dispatch_val;
- (id)preferMethod:(id)dispatch_val overMethod:(id)method;
- (id)getMethod:(id)dispatch_val;
- (id)methods;
- (id)prefers;
- (id)dispatch:(NSArray *)args;
@end

@protocol AFn <NSObject>
- (void)throwArity:(NSValue *)n;
@end

@interface Reduced : NSObject <IDeref>
@property id val;
- (instancetype)initWithVal:(id)val;
@end

@implementation Reduced
- (instancetype)initWithVal:(id)val {
  self = [super init];
  if (self)
    _val = val;
  return self;
}
- (id)deref {
  return _val;
}
@end

@interface Box : NSObject
@property id val;
+ (instancetype)withVal:(id)val;
- (instancetype)initWithVal:(id)val;
@end

@implementation Box
+ (instancetype)withVal:(id)val {
  return [[Box alloc] initWithVal:val];
}
- (instancetype)initWithVal:(id)val {
  self = [super init];
  if (self)
    self.val = val;
  return self;
}
@end
