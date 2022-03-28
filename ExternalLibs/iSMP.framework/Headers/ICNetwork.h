//
//  ICNetwork.h
//  PCL Library
//
//  Created by Christophe Fontaine on 21/06/10.
//  Copyright 2010 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ICISMPDevice.h"

@protocol ICNetworkDelegate;

/**
 * A generic Ingenico's device's network management.
 */
@interface ICNetwork : ICISMPDevice <NSStreamDelegate>
{
#pragma mark - Protected attributes

    /// An array of all open connections.
	NSMutableArray	* openConnections;

    /// An operation queue on which the write operations are scheduled.
	NSOperationQueue	* spmToHostWriteOperations;
}

#pragma mark - Properties

/**
 * The delegate object which should implement `ICISMPDeviceDelegate` and `ICNetworkDelegate` protocols.
 *
 * @note By implementing the protocols, the delegate is notified with `ICNetwork`'s events.
 */
@property (nonatomic, weak) id<ICISMPDeviceDelegate,ICNetworkDelegate> delegate;

#pragma mark - Class methods

/**
 * Returns the unique shared instance of `ICNetwork`.
 *
 * @note The object returned by this method is autoreleased. It is then necessary to retain it.
 *
 * @return A pointer to the shared instance.
 */
+(id) sharedChannel;

@end

/**
 * The `ICNetwork`'s delegate methods.
 *
 * These methods should be implemented by the `[ICNetwork delegate]` in order to be notified of its events.
 */
@protocol ICNetworkDelegate
@optional

/**
 * Method to be implemented by the delegate of the `ICNetwork` in order to receive all data between the iOS device and the Ingenico's device.
 *
 * @param data The logged network data.
 * @param isIncoming YES if the data comes to the iOS device or NO otherwise.
 */
-(void)networkData:(NSData *)data incoming:(BOOL)isIncoming;

/**
 * Called when the network channel tries to connect to a remote host.
 *
 * `ICNetwork`'s delegate may implement this method to be notified when the Ingenico's device attempts to open a connection to a remove server.
 *
 * @param host The host name or IP addres of the remote host.
 * @param port The destination port.
 */
-(void)networkWillConnectToHost:(NSString *)host onPort:(NSUInteger)port;

/**
 * Called when a connection to a remote host is opened.
 *
 * `ICNetwork`'s delegate may implement this method to be notified when a network connection is established between the Ingenico's device and a remote host.
 *
 * @param host The host name or IP address of the remote host.
 * @param port The destination port.
 */
-(void)networkDidConnectToHost:(NSString *)host onPort:(NSUInteger)port;

/**
 * Called when a connection to a remote host has failed.
 *
 * `ICNetwork`'s delegate may implement this method to be notified of connection failures to a remote host.
 *
 * @param host The host name or IP address of the remote host.
 * @param port The destination port.
 */
-(void)networkFailedToConnectToHost:(NSString *)host onPort:(NSUInteger)port;

/**
 * Called when the Ingenico's device is disconnected from a remote host.
 *
 * `ICNetwork`'s delegate may implement this method to be notified when the Ingenico's device is disconnected from a remote server.
 *
 * @param host The host name or IP address of the remote host.
 * @param port The destination port.
 */
-(void)networkDidDisconnectFromHost:(NSString *)host onPort:(NSUInteger)port;

/**
 * Called if a network errors occur during a communication with a remote host.
 *
 * `ICNetwork`'s delegate may implement htis method to be notified if there are some network errors while communicating with the remote host.
 *
 * @param host The host name of IP address of the remote host.
 * @param port The destination port.
 */
-(void)networkDidReceiveErrorWithHost:(NSString *)host andPort:(NSUInteger)port;

@end
