//
//  ICISMPDeviceExtension.h
//  PCL Library
//
//  Created by Boris LECLERE on 7/18/12.
//  Copyright (c) 2012 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ICISMPDevice.h"

@protocol ICISMPDeviceExtensionDelegate;

/**
 * This class extends `ICISMPDevice` by adding interface methods to send and receive data.
 *
 * This class has not to be used directly by the applications since it does not open an Ingenico's device's session itself. It provides an API for sending and receiving data to the classes which implement a raw communication channel, like `ICTransaction` and `ICSPP`. These two classes inherit from `ICISMPDeviceExtension` and can therefore call its methods to send and receive data.
 */
@interface ICISMPDeviceExtension : ICISMPDevice
{
#pragma mark - Protected attributes

    /// List to send.
	NSMutableArray * m_SendList;
}

#pragma mark - Properties

/// Number of frame received.
@property(getter = TotalNbFrameReceived) unsigned int m_TotalNbFrameReceived;

/// Size of received buffer.
@property(getter = ReceiveBufferSize, setter = SetReceiveBufferSize:) unsigned int m_ReceiveBufferSize;

/// Number of frame sent.
@property(getter = TotalNbFrameSent) unsigned int m_TotalNbFrameSent;

/**
 * The delegate object which should implement the `ICISMPDeviceDelegate` and `ICISMPDeviceExtensionDelegate` protocols.
 *
 * @note By implement the protocol, the delegate is notified with `ICISMPDevice`'s and `ICISMPDeviceExtension`'s events.
 */
@property(nonatomic, weak) id<ICISMPDeviceDelegate, ICISMPDeviceExtensionDelegate> delegate;

#pragma mark - Instance methods

/**
 * Send data synchronously to the Ingenico's device.
 *
 * This method writes the NSData object provided in argument on the output stream of the SPP channel and returns the number of bytes which have been written. This method is to be used in a context where the data is sent synchronously, like in a loop for example. This means that the application should implement a loop and continue calling `[ICISMPDeviceExtension SendData]` until all the data is written to the terminal.
 *
 * @param Data A NSData object containing the data bytes to send to the Ingenico's device.
 *
 * @return The number of bytes which have been written or -1 if an error occured.
 */
-(int)SendData:(NSData *)Data;

/**
 * Send data asynchronously to the Ingenico's device.
 *
 * This method writes the NSData object provided in argument on the output stram of the SPP channel. It returns immediately whatever the length of the data sent. The data buffer is provided to the `ICSPP` class which sends it asynchronously.
 *
 * @param Data A NSData object containing the data bytes to send to the Ingenico's device.
 *
 * @return YES if the NSData object is valid and the SPP channel is available or NO otherwise.
 */
-(bool)SendDataAsync:(NSData *)Data;

/**
 * Send a text string synchronously to the Ingenico's device.
 *
 * This method writes the NSString object provided in argument on the output stream of the SPP channel and returns the number of bytes which have been written. If the number of bytes written is not equal to the NSString length, other calls to the method will be necessary in order to send the remaining parts.
 *
 * @param String A NSString object containing the data bytes to send to the Ingenico's device.
 *
 * @return The number of bytes which have been written or -1 if an error occured.
 */
-(int)SendString:(NSString *)String;

/**
 * Send a text string asynchronously on the Ingenico's device.
 *
 * This method writes the NSString object provided in argument on the output stream of the SPP channel. The NSString is written in one time and the application has not to call this method more than once.
 *
 * @param String A NSString object containing the data bytes to be written to the Ingenico's device.
 *
 * @return YES if the NSString object is valid and the SPP channel is available or NO otherwise.
 */
-(bool)SendStringAsync:(NSString *)String;

@end

/**
 * The `ICISMPDeviceExtension`'s delegate methods.
 *
 * @note These methods should be implemented by the `[ICISMPDeviceExtension delegate]` in order to be notified of its events.
 */
@protocol ICISMPDeviceExtensionDelegate <ICISMPDeviceDelegate>

@optional

/**
 * Called when the `ICISMPDeviceExtension` object receives some data.
 *
 * This method is required and must be implemented by the delegate of `ICISMPDeviceExtension` object since it is the only way to retrive the received data.
 *
 * @param Data A NSData object containing the received data.
 * @param Sender The sender of the event.
 */
-(void)didReceiveData:(NSData *)Data fromICISMPDevice:(ICISMPDevice *)Sender;

/**
 * Called when the `ICISMPDeviceExtension` object start receiving some data.
 *
 * This method is optional. It is called just before the `[ICISMPDeviceExtensionDelegate didReceiveDataFromICISMPDevice]` to inform that the sender object started receiving some data but that the latter is not yet ready to be provided to the delegate.
 *
 * @param Sender The sender of the event.
 */
-(void)willReceiveData:(ICISMPDevice *)Sender;

/**
 * Called when the `ICISMPDeviceExntension` object is about to send some data.
 *
 * This method is optional. It is called after calling one of the methods used to send data or text. It informs the delegate that the provided data is about to be sent to the Ingenico's device.
 *
 * @param Sender The sender of the event.
 */
-(void)willSendData:(ICISMPDevice *)Sender;

/**
 * Callend when the `ICISMPDeviceExtension` object succesfully sent a chunk of data to the Ingenico's device.
 *
 * This method is optional. It is called each time a data buffer is sent on the `ICISMPDeviceExtension` output stream.
 *
 * @param Data A NSData buffer of the data which is written on the output stream.
 * @param NbBytesSent The length of the data which is written on the output stream.
 * @param Sender The sender of the event.
 */
-(void)didSendData:(NSData *)Data withNumberOfBytesSent:(unsigned int) NbBytesSent fromICISMPDevice:(ICISMPDevice *)Sender;

@end
