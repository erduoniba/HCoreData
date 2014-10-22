//
//  CoreDataEnvir.h
//  CoreDataLab
//
//	CoreData enviroment light wrapper.
//	Support CoreData operating methods.
//
//	Create record item.
//	Support concurrency operating.
//
//  Created by NicholasXu on 11-5-25.
//
//  mailto:dehengxu@outlook.com
//
//  Copyright 2011 NicholasXu. All rights reserved.
//


#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

#define CORE_DATA_ENVIR_SHOW_LOG        0

@class CoreDataEnvir;

#pragma mark - CoreDataEnvirObserver (Not be used temporarily)

@protocol CoreDataEnvirObserver

@optional
- (void)didFetchingFinished:(NSArray *) aItems;
- (void)didUpdateContext:(id)sender;
- (void)didDeleteObjects:(id)sender;
- (void)didInsertObjects:(id)sender;
- (void)didUpdateObjects:(id)sender;

@end


#pragma mark - CoreDataRescureDelegate

/**
 CoreData rescure delegate.
 While core data envirement init fails occured.
 */
@protocol CoreDataRescureDelegate <NSObject>

@optional

/**
 Reture if need rescure or abort directly.
 */
- (BOOL)shouldRescureCoreData;

/**
 Return if abort while rescure failed.
 */
- (BOOL)shouldAbortWhileRescureFailed;

/**
 Did start rescure core data.
 
 @param cde A CoreDataEnvir instance.
 */
- (void)didStartRescureCoreData:(CoreDataEnvir *)cde;

/**
 Did finished rescuring work.
 
 @param cde A CoreDataEnvir instance.
 */
- (void)didFinishedRescuringCoreData:(CoreDataEnvir *)cde;

/**
 Rescure failed.
 
 @param cde A CoreDataEnvir instance.
 */
- (void)rescureFailed:(CoreDataEnvir *)cde;

@end


#pragma mark - CoreDataEnvirement

typedef enum
{
    CDEErrorInstanceCreateTooMutch = 1000
}CoreDataEnvirError;

@interface CoreDataEnvir : NSObject {
    NSRecursiveLock *__recursiveLock;
}

/**
 A model object.
 */
@property (nonatomic, readonly) NSManagedObjectModel	*model;

/**
 A context object.
 */
@property (nonatomic, readonly) NSManagedObjectContext *context;

/**
 A persistance coordinator object.
 */
@property (nonatomic, retain) NSPersistentStoreCoordinator *storeCoordinator;

/**
 A NSFetchedResultsController object, not be used by now.
 */
@property (nonatomic, retain) NSFetchedResultsController * fetchedResultsCtrl;

/**
 Model file name. It normally be name.momd
 */
@property (nonatomic, copy, setter = registModelFileName:, getter = modelFileName) NSString *modelFileName;

/**
 Data base file name. It can be whatever you want.
 */
@property (nonatomic, copy, setter = registDatabaseFileName:, getter = databaseFileName) NSString *databaseFileName;

/**
 Data file root path.
 */
@property (nonatomic, copy, setter = registDataFileRootPath:, getter = dataRootPath) NSString *dataRootPath;

/**
 Data rescure when CoreData envirement init occurs error.
 */
//@property (nonatomic, assign) id<CoreDataRescureDelegate> rescureDelegate;

/**
 Triggle to enable persistance shared.
 
 If you wanna create another db file storage, you should
 set this flag to YES or set to NO.
 
 Commonly , you should set this flag to YES.
 
 YES: Multi context shared same persistence file.
 NO: Every context has own persistence file.

 If share persistence coordinator.
 Default is YES;
 
 */
@property (nonatomic) BOOL sharePersistence;

/**
 Regist the specified model file name.
 
 @prarm name    xcdatamodeld file name.(Except file extension.)
 */
+ (void)registDefaultModelFileName:(NSString *)name;

/**
 Regist the specified data file name.
 
 @param name    Data file name.(Except path.)
 */
+ (void)registDefaultDataFileName:(NSString *)name;

/**
 Regist the specified path as data file root.
 
 @param path    Data file root path.
 */
+ (void)registDefaultDataFileRootPath:(NSString *)path;

/**
 Regist resucrer, recommend using UIApplicationDelegate instance.
 
 @param delegate    Rescure delegate
 */
+ (void)registRescureDelegate:(id<CoreDataRescureDelegate>)delegate;

/**
 Get model file name.(Name likes: xxxx.mmod in sandbox.)
 */
+ (NSString *)defaultModelFileName;

/**
 Get data base file name.
 */
+ (NSString *)defaultDatabaseFileName;

/**
 Creating instance conditionally.
 If current thread is main thread return single main instance,else return an temporary new instance.
 */
+ (CoreDataEnvir *)instance;

/**
 Create an CoreDataEnvir instance by specified db file name and momd file name.
 The momd file name is a middle file generated by XCode handle xcdatamodeld file.
 
 @param databaseFileName    A specified db file name.
 @param modelFileName       A specified momd file name.
 */
+ (CoreDataEnvir *)createInstanceWithDatabaseFileName:(NSString *)databaseFileName modelFileName:(NSString *)modelFileName;

/**
 Only returen a single instance runs on main thread.
 */
+ (CoreDataEnvir *)mainInstance;

/**
 Main queue.
 */
+ (dispatch_queue_t)mainQueue;

/**
 Creating a new instance by default db, momd file name.
 */
+ (CoreDataEnvir *)createInstance;

+ (CoreDataEnvir *)createInstanceShareingPersistence:(BOOL)isSharePersistence;

/**
 Release the main instance.
 */
//+ (void)deleteInstance;

/**
 Return data root path
 */
+ (NSString *)dataRootPath;

+ (dispatch_queue_t)backgroundQueue;

+ (CoreDataEnvir *)backgroundInstance;

/**
 Init instance with specified db , model file name.
 
 @param databaseFileName    db file name.
 @param modelFileName       Model mapping file name.
 @return CoreDataEnvir instance.
 */
- (id)initWithDatabaseFileName:(NSString *)databaseFileName modelFileName:(NSString *)modelFileName;

/**
 Init instance with specified db , model file name.
 
 @param databaseFileName    db file name.
 @param modelFileName       Model mapping file name.
 @param isSharePersistence  Share persistence or not.
 @return CoreDataEnvir instance.
 */
- (id)initWithDatabaseFileName:(NSString *)databaseFileName modelFileName:(NSString *)modelFileName sharingPersistence:(BOOL)isSharePersistence;

/**
 Save
 */
- (BOOL)saveDataBase;

@end

#pragma mark -  NSObject (Debug_Ext)

/**
 NSObject (Debug_Ext)
 */
@interface NSObject (Debug_Ext)

/**
 Get current dispatch queue label string.
 */
- (NSString *)currentDispatchQueueLabel;

@end

#pragma mark - NSManagedObject convinent methods.

@interface NSManagedObject (CONVENIENT)

#pragma mark - Operation on main thread.
/**
 Creating managed object on main thread.
 */
+ (id)insertItem;

/**
 Creating managed object in main context by filling 'block'
 */
+ (id)insertItemWithBlock:(void(^)(id item))settingBlock;

/**
 Just fetching record items by the predicate in main context.
 */
+ (NSArray *)items;


/**
 *  获取从startIndex开始的itemCount数据
 *
 *  @param itemCount    需要获取的数据个数
 *  @param startIndex   从第几个开始
 *  @param questionType 数据的类型
 *
 *  @return 获取从startIndex开始的itemCount数据数组
 */
+ (NSArray *)getItemsCount:(NSInteger)itemCount andStartIndex:(NSInteger)startIndex andType:(QUESTION_TYPE)questionType;



/**
 Fetch record items in main context by predicate.
 */
+ (NSArray *)itemsWithPredicate:(NSPredicate *)predicate;

/**
 Fetch record items in main context by formated string.
 */
+ (NSArray *)itemsWithFormat:(NSString *)fmt,...;

/**
 *  Fetch record items in main context by predicate format string more simpler.
 *
 *  @param sortDescriptions SortDescriptions
 *  @param fmt              Predicate format string.
 *
 *  @return Array of items match the condition.
 */
+ (NSArray *)itemsSortDescriptions:(NSArray *)sortDescriptions withFormat:(NSString *)fmt,...;

/**
 *  Fetch record items in main context by predicate format string more simpler.
 *
 *  @param sortDescriptions SortDescriptions
 *  @param offset           offset
 *  @param limtNumber       limit number
 *  @param fmt              predicate format string.
 *
 *  @return Array of items match the condition.
 */
+ (NSArray *)itemsSortDescriptions:(NSArray *)sortDescriptions fromOffset:(NSUInteger)offset limitedBy:(NSUInteger)limitNumber withFormat:(NSString *)fmt,...;

/**
 * Fetching last record item.
 */
+ (id)lastItem;

/**
 *  Fetch record item by predicate in main context.
 *
 *  @param predicate Predicate object.s
 *
 *  @return Last item of the managed object in context.
 */
+ (id)lastItemWithPredicate:(NSPredicate *)predicate;

/**
 
 */
/**
 *  Fetch record item by formated string in main context.
 *
 *  @param fmt Predicate format.
 *
 *  @return Last item of the managed object in context.
 */
+ (id)lastItemWithFormat:(NSString *)fmt,...;

/**
 *  Remove item.
 */
- (void)remove;

/**
 Save db on main thread.
 */
- (BOOL)save;


#pragma mark - Operation on background thread.

/**
 Creating managed object on background thread.
 */
+ (id)insertItemInContext:(CoreDataEnvir *)cde;

/**
 Createing managed object in specified context with filling 'block'
 */
+ (id)insertItemInContext:(CoreDataEnvir *)cde fillData:(void (^)(id item))settingBlock;

/**
 Fetching record items in specified context.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde;

/**
 Fetch record items by predicate.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde usingPredicate:(NSPredicate *)predicate;

/**
 Fetch record items by format string.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde withFormat:(NSString *)fmt,...;

/**
 *  Fetch items by sort descriptions.
 *
 *  @param cde              CoreDataEnvir instance.
 *  @param sortDescriptions SortDescriptions
 *  @param fmt              Predicate format.
 *
 *  @return Array of items match the condition.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde sortDescriptions:(NSArray *)sortDescriptions withFormat:(NSString *)fmt,...;

/**
 *  Fetch items addition by limited range.
 *
 *  @param cde              CoreDataEnvir instance
 *  @param sortDescriptions SortDescriptions
 *  @param offset           offset
 *  @param limtNumber       limted number
 *  @param fmt              Predicate format.
 *
 *  @return Array of items match the condition.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde sortDescriptions:(NSArray *)sortDescriptions fromOffset:(NSUInteger)offset limitedBy:(NSUInteger)limitNumber withFormat:(NSString *)fmt,...;

/**
 *  Fetch item in specified context.
 *
 *  @param cde CoreDataEnvir instance
 *
 *  @return Last item of the managed object in context.
 */
+ (id)lastItemInContext:(CoreDataEnvir *)cde;

/**
 *  Fetch item in specified context through predicate.
 *
 *  @param cde       CoreDataEnvir instance
 *  @param predicate Predicate.
 *
 *  @return Last item of the managed object in context.
 */
+ (id)lastItemInContext:(CoreDataEnvir *)cde usingPredicate:(NSPredicate *)predicate;

/**
 *  Fetch item in specified context through format string.
 *
 *  @param cde
 *  @param fmt Predicate format string.
 *
 *  @return Last item of the managed object in context.
 */
+ (id)lastItemInContext:(CoreDataEnvir *)cde withFormat:(NSString *)fmt,...;

/**
 *  Update NSManagedObject if faulted.
 *
 *  @return Updated object.
 */
- (id)update;

/**
 Update NSManagedObject in specified context if faulted.
 
 @param cde     CoreDataEnvir object.
 */
- (id)updateInContext:(CoreDataEnvir *)cde;

/**
 *  Remove item.
 *
 *  @param cde
 */
- (void)removeFrom:(CoreDataEnvir *)cde;

/**
 *  Save db on main thread.
 *
 *  @param cde
 *
 *  @return Success(YES) or failed(NO).
 */
- (BOOL)saveTo:(CoreDataEnvir *)cde;

@end


