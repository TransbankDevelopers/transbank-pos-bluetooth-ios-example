//
//  ICPrinter.h
//  PCL Library
//
//  Created by Christophe Fontaine on 22/06/10.
//  Copyright 2010 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ICISMPDevice.h"

@protocol ICPrinterDelegate;

/**
 * This class provides native printing support for the Ingenico's device's applications.
 */
@interface ICPrinter : ICISMPDevice {
#pragma mark - Protected attributes

    /// The number of received microlines during a print session.
	NSUInteger			  _microlineCount;

    /// The microline number sent by the Companion (contains the microline number in the first 3 bytes, and the printing session in the last byte).
	NSUInteger			  _microlineNumber;
}

#pragma mark - Properties

/**
 * The delegate object which should implement `ICISMPDeviceDelegate` and `ICPrinterDelegate` protocols.
 *
 * @note By implementing the protocols, the delegate is notified with `ICPrinter`'s events.
 */
@property(nonatomic, weak) id<ICISMPDeviceDelegate,ICPrinterDelegate> delegate;

#pragma mark - Class methods

/**
 * Returns the unique shared instance of `ICPrinter`.
 *
 * @note The object returned by this method is autoreleased. It is then necessary to retain it.
 */
+(id)sharedPrinter;

@end

/**
 * The `ICPrinter`'s delegate methods.
 *
 * These methods should be implemented by the `[ICPrinter delegate]` in order to be notified of its events.
 */
@protocol ICPrinterDelegate
@optional

/**
 * Called every time the printer channel receives data from the Ingenico's device.
 *
 * All the data passed through calls to this method must be buffered until `[ICPrinterDelegate printingDidEndWithRowNumber` is called. The receiver should then generate the receipt using the data it gathered sor far.
 *
 * @param data A NSData containing a set of line dots to be printed.
 */
-(void)receivedPrinterData:(NSData *)data;

/**
 * Called every time the printer channel receives data from the Ingenico's device.
 *
 * The application should bufferize the data and print it sequencially. This method provides the number of received lines since it is meant to be used when the end of printing is not known in advance.
 *
 * @param data A NSData containing a set of line dots to be printed.
 * @param count The number of printed lines contained within the data.
 */
-(void)receivedPrinterData:(NSData *)data numberOfLines:(NSInteger)count;

/**
 * Called when the `ICPrinter` object has received all the data needed for printing a receipt.
 *
 * Each pixel of the receipt is encoded using one bit of data and the number of rows is given by the count parameter. The width of the graphics to be rendered is then equal to 8 times the size of the received data divided by the number of rows.
 *
 * @note Please note that this method should be used separately of the `[ICPrinterDelegate receivedPrinterData]` method (must not be implementing if you use this one).
 *
 * @param count The number of rows of the receipt to be printed.
 */
-(void)printingDidEndWithRowNumber:(NSUInteger)count;

@end
