//
//  ICSPP.h
//  PCL Library
//
//  Created by Hichem Boussetta on 24/05/12.
//  Copyright (c) 2012 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ICISMPDeviceExtension.h"

/**
 * This class provides a transparent SPP (Bluetooth Serial Port Profile) channel for iOS application which require to communicate with a Bluetooth device paired with the Ingenico's device.
 *
 * This class inherits from `ICISMPDeviceExtension` which provides all the required methods to send data or text synchronously or asynchronously.
 */
@interface ICSPP : ICISMPDeviceExtension

#pragma mark - Properties

/**
 * The delegate object which implement the `ICISMPDeviceDelegate` and `ICISMPDeviceExtensionDelegate` protocols.
 *
 * @note By implementing the protocols, the delegate is notified with `ICSPP`'s events.
 */
@property (nonatomic, weak) id<ICISMPDeviceDelegate, ICISMPDeviceExtensionDelegate> delegate;

#pragma mark - Class methods

/**
 * Returns the unique shared instance of `ICSPP`.
 *
 * @note The object returned by this method is autoreleased. It is then necessary to retain it.
 *
 * @return A pointer to the shared instance.
 */
+(ICSPP *)sharedChannel;

@end
