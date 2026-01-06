//
//  SSZipArchive.h
//  SSZipArchive
//
//  Created by Sam Soffes on 7/21/10.
//

#ifndef _SSZIPARCHIVE_H
#define _SSZIPARCHIVE_H

#import <Foundation/Foundation.h>

#import "SSZipCommon.h"

NS_ASSUME_NONNULL_BEGIN

extern NSString *const SSZipArchiveErrorDomain;
typedef NS_ENUM(NSInteger, SSZipArchiveErrorCode) {
    SSZipArchiveErrorCodeFailedOpenZipFile             = -1,
    SSZipArchiveErrorCodeFailedOpenFileInZip           = -2,
    SSZipArchiveErrorCodeFileInfoNotLoadable           = -3,
    SSZipArchiveErrorCodeFileContentNotReadable        = -4,
    SSZipArchiveErrorCodeFailedToWriteFile             = -5,
    SSZipArchiveErrorCodeInvalidArguments              = -6,
    SSZipArchiveErrorCodeSymlinkEscapesTargetDirectory = -7,
};

@protocol SSZipArchiveDelegate;

@interface SSZipArchive : NSObject

// Password check
+ (BOOL)isFilePasswordProtectedAtPath:(NSString *)path;
+ (BOOL)isPasswordValidForArchiveAtPath:(NSString *)path password:(NSString *)pw error:(NSError * _Nullable * _Nullable)error NS_SWIFT_NOTHROW;

// Total payload size
+ (NSNumber *)payloadSizeForArchiveAtPath:(NSString *)path error:(NSError **)error;

// Unzip
+ (BOOL)unzipFileAtPath:(NSString *)path toDestination:(NSString *)destination taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;
+ (BOOL)unzipFileAtPath:(NSString *)path toDestination:(NSString *)destination delegate:(nullable id<SSZipArchiveDelegate>)delegate taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
              overwrite:(BOOL)overwrite
               password:(nullable NSString *)password
                  error:(NSError * *)error
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
              overwrite:(BOOL)overwrite
               password:(nullable NSString *)password
                  error:(NSError * *)error
               delegate:(nullable id<SSZipArchiveDelegate>)delegate
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler NS_REFINED_FOR_SWIFT;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
     preserveAttributes:(BOOL)preserveAttributes
              overwrite:(BOOL)overwrite
               password:(nullable NSString *)password
                  error:(NSError * *)error
               delegate:(nullable id<SSZipArchiveDelegate>)delegate
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
        progressHandler:(void (^_Nullable)(NSString *entry, unz_file_info zipInfo, long entryNumber, long total))progressHandler
      completionHandler:(void (^_Nullable)(NSString *path, BOOL succeeded, NSError * _Nullable error))completionHandler
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
              overwrite:(BOOL)overwrite
               password:(nullable NSString *)password
        progressHandler:(void (^_Nullable)(NSString *entry, unz_file_info zipInfo, long entryNumber, long total))progressHandler
      completionHandler:(void (^_Nullable)(NSString *path, BOOL succeeded, NSError * _Nullable error))completionHandler
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
     preserveAttributes:(BOOL)preserveAttributes
              overwrite:(BOOL)overwrite
         nestedZipLevel:(NSInteger)nestedZipLevel
               password:(nullable NSString *)password
                  error:(NSError **)error
               delegate:(nullable id<SSZipArchiveDelegate>)delegate
        progressHandler:(void (^_Nullable)(NSString *entry, unz_file_info zipInfo, long entryNumber, long total))progressHandler
      completionHandler:(void (^_Nullable)(NSString *path, BOOL succeeded, NSError * _Nullable error))completionHandler
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

+ (BOOL)unzipFileAtPath:(NSString *)path
          toDestination:(NSString *)destination
     preserveAttributes:(BOOL)preserveAttributes
              overwrite:(BOOL)overwrite
    symlinksValidWithin:(nullable NSString *)symlinksValidWithin
         nestedZipLevel:(NSInteger)nestedZipLevel
               password:(nullable NSString *)password
                  error:(NSError **)error
               delegate:(nullable id<SSZipArchiveDelegate>)delegate
        progressHandler:(void (^_Nullable)(NSString *entry, unz_file_info zipInfo, long entryNumber, long total))progressHandler
      completionHandler:(void (^_Nullable)(NSString *path, BOOL succeeded, NSError * _Nullable error))completionHandler
            taskHandler:(void (^ _Nullable)(SSZipArchive * _Nonnull))taskHandler;

/// 获取压缩包文件列表
/// - Parameters:
///   - path: 压缩包路径
///   - password: 解压码
///   - error: 错误提示
+ (nullable NSArray<NSString *> *)unzipFileNamesAtPath:(NSString *)path password:(nullable NSString *)password error:(NSError **)error;

/// 从压缩包里解压指定文件到某一路径
/// - Parameters:
///   - path: 压缩包路径
///   - fileName: 文件名
///   - toPath: 解压路径
///   - password: 解压码
///   - error: 错误提示
+ (void)unzipFileAtPath:(NSString *)path withFileName:(NSString *)fileName toPath:(NSString *)toPath password:(nullable NSString*)password error:(NSError **)error;

// Zip
// default compression level is Z_DEFAULT_COMPRESSION (from "zlib.h")
// keepParentDirectory: if YES, then unzipping will give `directoryName/fileName`. If NO, then unzipping will just give `fileName`. Default is NO.

// without password
+ (BOOL)createZipFileAtPath:(NSString *)path withFilesAtPaths:(NSArray<NSString *> *)paths taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath keepParentDirectory:(BOOL)keepParentDirectory taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;

// with optional password
// - default is AES encryption
// - don't use AES if you need compatibility with native macOS unzip and Archive Utility
// - disabling AES will fallback to PKWARE traditional encryption
+ (BOOL)createZipFileAtPath:(NSString *)path withFilesAtPaths:(NSArray<NSString *> *)paths withPassword:(nullable NSString *)password taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path withFilesAtPaths:(NSArray<NSString *> *)paths withPassword:(nullable NSString *)password progressHandler:(void(^ _Nullable)(NSUInteger entryNumber, NSUInteger total))progressHandler taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath withPassword:(nullable NSString *)password taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path withContentsOfDirectory:(NSString *)directoryPath keepParentDirectory:(BOOL)keepParentDirectory withPassword:(nullable NSString *)password taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path
    withContentsOfDirectory:(NSString *)directoryPath
        keepParentDirectory:(BOOL)keepParentDirectory
               withPassword:(nullable NSString *)password
         andProgressHandler:(void(^ _Nullable)(NSUInteger entryNumber, NSUInteger total))progressHandler
                taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
+ (BOOL)createZipFileAtPath:(NSString *)path
    withContentsOfDirectory:(NSString *)directoryPath
        keepParentDirectory:(BOOL)keepParentDirectory
           compressionLevel:(int)compressionLevel
                   password:(nullable NSString *)password
                        AES:(BOOL)aes
            progressHandler:(void(^ _Nullable)(NSUInteger entryNumber, NSUInteger total))progressHandler
                taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
//suport symlink compress --file
+ (BOOL)createZipFileAtPath:(NSString *)path withFilesAtPaths:(NSArray<NSString *> *)paths withPassword:(nullable NSString *)password keepSymlinks:(BOOL)keeplinks taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;
//suport symlink compress --directory
+ (BOOL)createZipFileAtPath:(NSString *)path
    withContentsOfDirectory:(NSString *)directoryPath
        keepParentDirectory:(BOOL)keepParentDirectory
           compressionLevel:(int)compressionLevel
                   password:(nullable NSString *)password
                        AES:(BOOL)aes
            progressHandler:(void(^ _Nullable)(NSUInteger entryNumber, NSUInteger total))progressHandler
               keepSymlinks:(BOOL)keeplinks
                taskHandler:(void(^ _Nullable)(SSZipArchive *))taskHandler;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithPath:(NSString *)path NS_DESIGNATED_INITIALIZER;
- (BOOL)open;
- (BOOL)openForAppending;

/// write empty folder
- (BOOL)writeFolderAtPath:(NSString *)path withFolderName:(NSString *)folderName withPassword:(nullable NSString *)password;
/// write file
- (BOOL)writeFile:(NSString *)path withPassword:(nullable NSString *)password;
- (BOOL)writeFileAtPath:(NSString *)path withFileName:(nullable NSString *)fileName withPassword:(nullable NSString *)password;
- (BOOL)writeFileAtPath:(NSString *)path withFileName:(nullable NSString *)fileName compressionLevel:(int)compressionLevel password:(nullable NSString *)password AES:(BOOL)aes;
/// write symlink files
- (BOOL)writeSymlinkFileAtPath:(NSString *)path withFileName:(nullable NSString *)fileName compressionLevel:(int)compressionLevel password:(nullable NSString *)password AES:(BOOL)aes;
/// write data
- (BOOL)writeData:(NSData *)data filename:(nullable NSString *)filename withPassword:(nullable NSString *)password;
- (BOOL)writeData:(NSData *)data filename:(nullable NSString *)filename compressionLevel:(int)compressionLevel password:(nullable NSString *)password AES:(BOOL)aes;

- (BOOL)close;

- (void)cancel;

@end

@protocol SSZipArchiveDelegate <NSObject>

@optional

- (void)zipArchiveWillUnzipArchiveAtPath:(NSString *)path zipInfo:(unz_global_info)zipInfo;
- (void)zipArchiveDidUnzipArchiveAtPath:(NSString *)path zipInfo:(unz_global_info)zipInfo unzippedPath:(NSString *)unzippedPath;

- (BOOL)zipArchiveShouldUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath fileInfo:(unz_file_info)fileInfo;
- (void)zipArchiveWillUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath fileInfo:(unz_file_info)fileInfo;
- (void)zipArchiveDidUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath fileInfo:(unz_file_info)fileInfo;
- (void)zipArchiveDidUnzipFileAtIndex:(NSInteger)fileIndex totalFiles:(NSInteger)totalFiles archivePath:(NSString *)archivePath unzippedFilePath:(NSString *)unzippedFilePath;

- (void)zipArchiveProgressEvent:(unsigned long long)loaded total:(unsigned long long)total;

@end

NS_ASSUME_NONNULL_END

#endif /* _SSZIPARCHIVE_H */
