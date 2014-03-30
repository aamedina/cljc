#import <Foundation/Foundation.h>

@protocol Object <NSObject>
- (NSString *)toString;
@end

@protocol ICounted <NSObject>
- (int)_count;
@end

@protocol IIndexed <NSObject>
- (id)_nth:(int)n;
- (id)_nth:(int)n default:(id)not_found;
@end

@protocol ICollection <NSObject>
- (id)_conj:(id)val;
@end

@protocol ISeq <NSObject>
- (id)_first;
- (id)_rest;
- (id)_cons:(id)obj;
@end

@protocol INext <NSObject>
- (id)_next;
@end

@protocol ILookup <NSObject>
- (id)_lookup:(id)key;
- (id)_lookup:(id)key default:(id)not_found;
@end

@protocol IAssociative <NSObject>
- (int)_contains_key:(id)key;
- (id)_assoc:(id)key withVal:(id)val;
@end

@protocol IMap <NSObject>
- (id)_dissoc:(id)key;
@end

@protocol IMapEntry <NSObject>
- (id)_key;
- (id)_val;
@end

@protocol ISet <NSObject>
- (id)_disjoin:(id)val;
@end

@protocol IStack <NSObject>
- (id)_peek;
- (id)_pop;
@end

@protocol IVector <NSObject>
- (id)_assoc_n:(int)n withVal:(id)val;
@end

@protocol ISeqable <NSObject>
- (id)_seq;
@end

@protocol IEmptyableCollection <NSObject>
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

@protocol IEquiv <NSObject>
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

@protocol ISequential <NSObject>
@end

@protocol IList <NSObject>
@end

@protocol IRecord <NSObject>
@end

@protocol IReversible <NSObject>
- (id)_rseq;
@end

@protocol ISorted <NSObject>
- (id)_sorted_seq:(bool)ascending;
- (id)_sorted_seq_from:(id)key isAscending:(bool)ascending;
- (id)_entry_key:(id)entry;
- (id)_comparator;
@end

@protocol IWriter <NSObject>
- (void)_write:(NSString *)str;
- (void)_flush;
@end

@protocol IPrintWithWriter <NSObject>
- (id)_pr_writer:(id<IWriter>)writer withOpts:(id<IMap>)opts;
@end

@protocol IPending <NSObject>
- (bool)_realized;
@end

@protocol IWatchable <NSObject>
- (id)_notify_watches:(id)oldval withNewVal:(id)newval;
- (id)_add_watch:(id)key withFunction:(id)f;
- (id)_remove_watch:(id)key;
@end

@protocol IEditableCollection <NSObject>
- (id)_as_transient;
@end

@protocol ITransientCollection <NSObject>
- (id)_conj:(id)val;
- (id)_persistent;
@end

@protocol ITransientAssociative <NSObject>
- (id)_assoc:(id)key withVal:(id)val;
@end

@protocol ITransientMap <NSObject>
- (id)_dissoc:(id)key;
@end

@protocol ITransientVector <NSObject>
- (id)_assoc_n:(NSValue *)n withVal:(id)val;
- (id)_pop;
@end

@protocol ITransientSet <NSObject>
- (id)_disjoin:(id)val;
@end

@protocol IComparable <NSObject>
- (id)_compare:(id)obj;
@end

@protocol IChunk <NSObject>
- (id)_drop_first;
@end

@protocol IChunkedSeq <NSObject>
- (id)_chunked_first;
- (id)_chunked_rest;
@end

@protocol IChunkedNext <NSObject>
- (id)_chunked_next;
@end

@protocol INamed <NSObject>
- (id)_name;
- (id)_namespace;
@end

@protocol IAtom <NSObject>
@end

@protocol IReset <NSObject>
- (id)_reset:(id)new_value;
@end

@protocol ISwap <NSObject>
- (id)_swap:(id)f;
- (id)_swap:(id)f withArg:(id)a;
- (id)_swap:(id)f withArg:(id)a andArg:(id)b;
- (id)_swap:(id)f withArg:(id)a andArg:(id)b andVarArgs:(NSArray *)xs;
@end

@protocol IMultiFn <NSObject>
- (id)_reset;
- (id)_add_method:(id)dispatch_val withMethod:(id)method;
- (id)_remove_method:(id)dispatch_val;
- (id)_prefer_method:(id)dispatch_val overMethod:(id)method;
- (id)_get_method:(id)dispatch_val;
- (id)_methods;
- (id)_prefers;
- (id)_dispatch:(NSArray *)args;
@end

@protocol AFn <NSObject>
- (void)throwArity:(NSValue *)n;
@end
