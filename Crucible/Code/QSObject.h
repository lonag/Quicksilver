
#import <Foundation/Foundation.h>
#import <QSCrucible/QSBasicObject.h>

@protocol QSObject, QSObjectRanker;
@class QSObject, QSBasicObject;

extern NSSize QSMaxIconSize;

@protocol QSObjectHandler <NSObject>
- (BOOL)loadIconForObject:(QSObject *)object;
- (void)setQuickIconForObject:(QSObject *)object;
- (BOOL)objectHasChildren:(QSObject *)object;
- (BOOL)objectHasValidChildren:(QSObject *)object;
- (NSArray *)childrenForObject:(QSObject *)object;
- (QSObject *)parentOfObject:(QSObject *)object;
- (NSString *)detailsOfObject:(QSObject *)object;
- (NSString *)identifierForObject:(QSObject *)object;
- (BOOL)drawIconForObject:(QSObject *)object inRect:(NSRect)rect flipped:(BOOL)flipped;
- (NSDragOperation)operationForDrag:(id <NSDraggingInfo>)sender ontoObject:(QSObject *)dObject withObject:(QSBasicObject *)iObject;
- (NSString *)actionForDragMask:(NSDragOperation)operation ontoObject:(QSObject *)dObject withObject:(QSBasicObject *)iObject;
- (BOOL)loadChildrenForObject:(QSObject *)object;
- (NSString *)kindOfObject:(id <QSObject>)object;
- (id)dataForObject:(QSObject *)object pasteboardType:(NSString *)type;
@end

// meta dictionary keys
#define kQSObjectPrimaryName ((NSString *)kMDItemTitle)

#define kQSObjectAlternateName ((NSString *)kMDItemHeadline)
#define kQSObjectPrimaryType @"QSObjectType"
#define kQSObjectIconName @"QSObjectIconName"

#define kQSObjectDefaultAction @"QSObjectDefaultAction"

#define kQSObjectObjectID ((NSString *)kMDItemIdentifier)
#define kQSObjectParentID @"QSObjectParentID"
#define kQSObjectDetails @"QSObjectDetails"
#define kQSObjectKind @"QSObjectKind"
#define kQSObjectSource @"QSObjectSource"
#define kQSObjectCreationDate ((NSString *)kMDItemContentCreationDate)
#define kQSStringEncoding @"QSStringEncoding"
//#define METAKEYS [NSArray arrayWithObjects:

#define kMeta @"metadata"
#define kData @"data"

// cache dictionary keys
#define kQSObjectIcon @"QSObjectIcon"
#define kQSObjectChildren @"QSObjectChildren"
#define kQSObjectAltChildren @"QSObjectAltChildren"
#define kQSObjectChildrenLoadDate @"QSObjectChildrenLoadDate"
#define kQSContents @"QSObjectContents"
#define kQSObjectComponents @"QSObjectComponents"

typedef struct _QSObjectFlags {
    unsigned int        multiTyped:1;
    unsigned int        iconLoaded:1;
    unsigned int        childrenLoaded:1;
    unsigned int        contentsLoaded:1;
    unsigned int        noIdentifier:1;
    unsigned int        isProxy:1;
    unsigned int        retainsIcon:1;
	//  NSCellType          type:2;
} QSObjectFlags;

@interface QSObject : QSBasicObject <NSCopying> {
	NSString *name;
	NSString *label;
	NSString *identifier;
	NSImage *icon;
	NSString *primaryType;
	id primaryObject;
	
	NSMutableDictionary *	meta;		//Name, Label, Type, Identifier, Source, embedded details
	NSMutableDictionary *	data;		//Data or typed dictionary (multiTyped Object)
	NSMutableDictionary *	cache;		//Icons, children, alias data, 
	QSObjectFlags			flags;
    NSTimeInterval			lastAccess;
}
+ (void)cleanObjectDictionary;
+ (void)purgeOldImagesAndChildren;
+ (void)purgeAllImagesAndChildren;
+ (void)purgeImagesAndChildrenOlderThan:(NSTimeInterval)interval;
+ (void)purgeIdentifiers;

+ (void)registerObject:(QSBasicObject *)object withIdentifier:(NSString *)anIdentifier;

+ (id)objectWithName:(NSString *)aName;
+ (id)objectWithIdentifier:(NSString *)anIdentifier;
+ (id)makeObjectWithIdentifier:(NSString *)anIdentifier;
+ (id)objectByMergingObjects:(NSArray *)objects;
+ (id)objectByMergingObjects:(NSArray *)objects withObject:(QSObject *)object;

- (id)init;
- (void)dealloc;
- (BOOL)isEqual:(id)anObject;
- (NSString *)guessPrimaryType;
- (NSArray *)splitObjects;
- (NSString *)displayName ;
- (NSString *)toolTip;
- (NSString *)descriptionWithLocale:(NSDictionary *)locale indent:(unsigned)level;
- (NSString *)details;
- (id)primaryObject;

- (int)count;
- (int)primaryCount;
- (NSArray *)allKeys;

- (NSArray *)types;
- (NSArray *)decodedTypes;

- (int)primaryCount;

- (id)handler;
- (id)handlerForType:(NSString *)type selector:(SEL)selector;

- (id)objectForType:(id)aKey;
- (void)setObject:(id)object forType:(id)aKey;

- (id)objectForCache:(id)aKey;
- (void)setObject:(id)object forCache:(id)aKey;

- (id)objectForMeta:(id)aKey;
- (void)setObject:(id)object forMeta:(id)aKey;

- (void)setDetails:(NSString *)newDetails;

- (NSMutableDictionary *)cache;
- (void)setCache:(NSMutableDictionary *)aCache;


@end

@interface QSObject (Icon)
- (BOOL)loadIcon;
- (BOOL)unloadIcon;
- (NSImage *)icon;
- (void)setIcon:(NSImage *)newIcon;
@end

@interface QSObject (Hierarchy)
- (QSBasicObject *)parent;
- (void)setParentID:(NSString *)parentID;
- (BOOL)childrenValid;
- (BOOL)unloadChildren;
- (void)loadChildren;
- (BOOL)hasChildren;
@end

@interface QSObject (Archiving)
+ (id)objectFromFile:(NSString *)path;
- (id)initWithCoder:(NSCoder *)coder;
- (void)encodeWithCoder:(NSCoder *)coder;

- (id)initFromFile:(NSString *)path;
- (void)writeToFile:(NSString *)path;

- (NSMutableDictionary *)archiveDictionary;

- (void)extractMetadata;

- (id)findDuplicateOrRegisterID;
@end



//Standard Accessors
@interface QSObject (Accessors)
- (NSString *)identifier;
- (void)setIdentifier:(NSString *)newIdentifier;
- (NSString *)name;
- (void)setName:(NSString *)newName;
- (NSArray *)children;
- (void)setChildren:(NSArray *)newChildren;
- (NSArray *)altChildren;
- (void)setAltChildren:(NSArray *)newAltChildren;
- (NSString *)label;
- (void)setLabel:(NSString *)newLabel;
- (NSString *)primaryType;
- (void)setPrimaryType:(NSString *)newPrimaryType;
- (NSMutableDictionary *)dataDictionary;
- (void)setDataDictionary:(NSMutableDictionary *)newDataDictionary;
- (BOOL)iconLoaded;
- (void)setIconLoaded:(BOOL)flag;
- (BOOL)retainsIcon;
- (void)setRetainsIcon:(BOOL)flag;
- (BOOL)childrenLoaded;
- (void)setChildrenLoaded:(BOOL)flag;
- (BOOL)contentsLoaded;
- (void)setContentsLoaded:(BOOL)flag;
- (NSTimeInterval)childrenLoadedDate;
- (void)setChildrenLoadedDate:(NSTimeInterval)newChildrenLoadedDate;
///- (id)contents;
///- (void)setContents:(id)newContents;
//- (NSTimeInterval)lastUseDate;
//- (void)setLastUseDate:(NSTimeInterval)newLastUseDate;
@end
