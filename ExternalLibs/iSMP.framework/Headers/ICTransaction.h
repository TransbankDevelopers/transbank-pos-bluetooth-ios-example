//
//  ICTransaction.h
//  PCL Library
//
//  Created by Christophe Fontaine on 21/06/10.
//  Copyright 2010 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ICISMPDeviceExtension.h"

/**
 * This class provides a raw channel to be used by other third-party Ingenico payment libraries.
 */
@interface ICTransaction : ICISMPDeviceExtension {
#pragma mark - Protected attributes
    /// Delegates of ICTransaction.
	NSMutableSet	*streamObjects __deprecated_msg("Not used anymore since the class supports only one delegate now.");
}

#pragma mark - Properties

/**
 * The delegate object of `ICTransaction` class. This property is readonly and to set it properly you need to call `[ICTransaction forwardStreamEvents]`.
 */
@property (nonatomic, readonly) id streamEventDelegate;

/**
 * The delegate object which should implement the `ICISMPDeviceDelegate` and `ICISMPDeviceExtensionDelegate` protocols.
 *
 * @note By implement the protocols, the delegate is notified with `ICTransaction`'s events.
 */
@property (nonatomic, weak) id<ICISMPDeviceDelegate, ICISMPDeviceExtensionDelegate> delegate;

#pragma mark - Class methods

/**
 * Returns the unique shared instance of `ICTransaction`.
 *
 * @note The object returned by this method is autoreleased. It is then necessary to retain it.
 *
 * @return A pointer to the shared instance.
 */
+(id) sharedChannel;

#pragma mark - Instance methods

/**
 * Initializes an `ICTransaction` object with the protocol string `kICProtoNameSPMTransaction`.
 *
 * @return The initialized receiver.
 */
-(id)init;

/**
 * Manage delegates of this stream.
 *
 * To be notified of stream events, implement `NSStreamDelegate` protocol and subscribe to them by calling this methods.
 *
 * @param enabled YES to enable the stream delegate or NO otherwise.
 * @param anObject An object which imlements the `NSStreamDelegate` protocol.
 */
-(void)forwardStreamEvents:(BOOL)enabled to:(id<NSStreamDelegate>)anObject;

@end
