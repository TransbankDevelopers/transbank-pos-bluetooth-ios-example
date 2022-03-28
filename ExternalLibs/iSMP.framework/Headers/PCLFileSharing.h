//
//  PCLFileSharing.h
//  iSMP
//
//  Created by Jeremy PELTIER (Ingenico Partner) on 21/11/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "PCLTeliumFile.h"

@protocol PCLFileSharingDelegate <NSObject>

@optional
-(void)fileSharingServerDisconnect;

@end

/*!
 @defgroup   PCLFileSharingGroup PCL File Sharing
 @brief      Methods, structures and callbacks of PCLFileSharing class, that can be used with all Ingenico terminals under Telium 3 operating system.
 */

/*!
 @anchor        PCLFileSharingResult
 @brief         The returned result by the PCLFileSharing methods
 @ingroup       PCLFileSharingGroup
 */
typedef enum {
    kPCLFileSharingResultOk = 0,                                /**< Everything work perfectly */
    kPCLFileSharingResultInternalResourceError = -10000,        /**< An internal resource error has occured */
    kPCLFileSharingResultInvalidParameterError = -10001,        /**< Invalid parameter */
    kPCLFileSharingResultInvalidHandleError = -10002,           /**< Invalid handle */
    kPCLFileSharingResultInvalidRightsError = -10003,           /**< Invalid rights */
    kPCLFileSharingResultAbortedError = -10004,                 /**< Aborted */
    kPCLFileSharingResultCommunicationError = -10005,           /**< There is a communication error between your app and our device */
    kPCLFileSharingResultTimeoutError = -10006,                 /**< The request has timeout */
    kPCLFileSharingResultProtocolError = -10007,                /**< There is a protocol error */
    kPCLFileSharingResultInterruptedError = -10008,             /**< The request has been interrupted */
    kPCLFileSharingResultCompressionError = -10009,             /**< Compression error */
    kPCLFileSharingResultInvalidPackageError = -10010,          /**< Invalid package */
    kPCLFileSharingResultIdentificationError = -10011,          /**< Identification error */
    kPCLFileSharingResultInjectionError = -10012,               /**< Injection error */
    kPCLFileSharingResultNothingToInstallError = -10013,        /**< There is nothing to install */
    kPCLFileSharingResultInstallationError = -10014,            /**< There is an error during the installation */
    kPCLFileSharingResultInvalidPackageListError = -10015,      /**< Invalid package list */
    kPCLFileSharingResultInvalidCallError = -10016,             /**< Invalid call */
    kPCLFileSharingResultConnectionError = -10017,              /**< Connection error */
    kPCLFileSharingResultPostponedJob = -10018,                 /**< Postponed job */
    kPCLFileSharingResultServerError = -10019,                  /**< Server error */
    kPCLFileSharingResultTerminalDisconnected = -20000          /**< Terminal not connected */
} PCLFileSharingResult;

/*!
 @anchor PCLFileSharingServerState
 @brief The server states
 @ingroup       PCLFileSharingGroup
 */
typedef enum {
    kServerStateDisconnected,   /**< The server is disconnected */
    kServerStateConnecting,     /**< The server is connecting */
    kServerStateConnected,      /**< The server is connected */
    kServerStateDisconnecting   /**< The server is disconnecting */
} PCLFileSharingServerState;

/*!
 @class PCLFileSharing
 @brief The PCLFileSharing class.<br>This class was designed to help you to create a PCLFileSharing server on your apps and send files to our devices
 @ingroup       PCLFileSharingGroup
 */
@interface PCLFileSharing: NSObject
{
    void (^_completionHandler)(PCLFileSharingResult result);
    void (^_completionHandlerForMultipleSending)(int sent, int totalToSend, PCLFileSharingResult result);
    void (^_completionHandlerForFilesList)(int result, NSArray<PCLTeliumFile *> *files);
}

@property (nonatomic, weak) id<PCLFileSharingDelegate> delegate;

/**
 @brief Create and keep in memory an instance of the PCLFileSharing class in order to use it from everywhere in your code
 @return PCLFileSharing The instance of the PCLFileSharing
 */
+(PCLFileSharing *)sharedInstance;


/**
 @brief Start the File Sharing server.<br>You need to specify the port where you want to start the File Sharing server.
 @param port The port where start the server
 @param handler A completion handler in order to get information about the result
 */
-(void)start:(int)port andCompletionHandler:(void(^)(PCLFileSharingResult))handler;

/**
 @brief Start the File Sharing server. <br>This method allow you to start the File Sharing server on your iOS device and start the remote upgrade of you terminal with the given parameters.
 @important The PCL connection between the iOS device and the Telium 3 device must be started before calling this method.
 @param host The host where the terminal will try to connect
 @param port The port where the terminal will try to connect
 @param handler A completion handler in order to get information about the result
 */
-(void)startAndDoRemoteToHost:(NSString *)host andPort:(int)port withCompletion:(void(^)(PCLFileSharingResult))handler;

/**
 @brief Stop the File Sharing server.
 @param handler A completion handler in order to get information about the result
 */
-(void)stopWithCompletionHandler:(void(^)(PCLFileSharingResult))handler;

/**
 @brief Stop the File Sharing server.<br>This method allow you to give some parameter to do action after the server stop.
 @param reset A boolean to order or not to the terminal to reset
 @param restore A boolean to order to restore the previous TMS parameters
 @param handler A completion handler in order to get information about the result
 */
-(void)stopWithReset:(BOOL)reset andRestore:(BOOL)restore withCompletion:(void(^)(PCLFileSharingResult))handler;

/**
 @brief Send a file.<br>It will allow you to send a file from your app to the directory that you want on our devices.
 @param filepath The path to the file that you want to send
 @param toDirectory The path of the directory where you want send the file, for example: /import
 @param handler A completion handler in order to get information about the result
 */
-(void)send:(NSString *)filepath to:(NSString *)toDirectory andCompletionHandler:(void(^)(PCLFileSharingResult))handler;

/**
 @brief Send multiple files.<br>It will allow you to send multiple files from your app to the directory that you want on our devices.
 @param files An array of file pathes that you want to send
 @param toDirectory The path of the directory where you want send the file, for example: /import
 @param handler A completion handler in order to get information about the result
 */
-(void)sendMultiple:(NSArray<NSString *> *)files to:(NSString *)toDirectory andCompletionHandler:(void (^)(int, int, PCLFileSharingResult))handler;

/**
 @brief List content of a directory path.<br>It will allow you to get the files and directories list for a specific directory path on our devices.
 @param filepath The directory path that you want to get.
 @param handler A completion handler in order to get information about the result and an array of PCLTeliumFile that will allow you to display information about the files in the directory that you want to get.
 */
-(void)list:(NSString *)filepath andCompletionHandler:(void (^)(PCLFileSharingResult, NSArray<PCLTeliumFile *> *))handler;

/**
 @brief Download a file.<br>It will allow you to download a file from our devices to your application. It use the default NSFileManager to save the file in a specific directory.
 @param filepath The path of the file to get on our devices.
 @param toDirectory The directory path where you want to save the downloaded file.
 @param handler A completion handler in order to get information about the result.
 */
-(void)download:(NSString *)filepath toDirectory:(NSString *)directory andCompletionHandler:(void (^)(PCLFileSharingResult))handler;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
-(void)delete:(NSString *)filename andCompletionHandler:(void (^)(PCLFileSharingResult))handler;

-(void)deletePackage:(NSString *)filename andCompletionHandler:(void (^)(PCLFileSharingResult))handler;
    
#endif
    
/**
 @brief Get current server state
 @return PCLFileSharingServerState The current state of the server
 */
-(PCLFileSharingServerState)currentState;

/**
 @brief This method allows you to convert an extension to a file type name for Telium devices. By give as parameter the extension of a file, we will return to you a file type name if the extension match one of the official extension of a Telium device.
 @param extension The extension to check
 @return NSString A file type name string, for example Custom Legacy Application for .p3a files
 */
-(NSString *)TeliumFileTypeForExtension:(NSString *)extension;

@end
