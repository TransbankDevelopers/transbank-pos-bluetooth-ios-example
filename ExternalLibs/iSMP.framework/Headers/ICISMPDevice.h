//
//  ICISMPDevice.h
//  PCL Library
//
//  Created by Christophe Fontaine on 21/06/10.
//  Copyright 2010 Ingenico. All rights reserved.
//

/*!
	@file		ICISMPDevice.h
	@brief      Header file for ICISMPDevice class and its related data structures and protocol definitions
*/

#import <Foundation/Foundation.h>
#import <ExternalAccessory/ExternalAccessory.h>

@protocol ICISMPDeviceDelegate;

/**
 * The enumeration of all error codes that can be thrown by the PCL library.
 */
typedef NS_ENUM(int, iSMPResult) {
    /// The function called is not supported by this type of Ingenico's device.
    ISMP_Result_Function_Not_Supported = -1,

    /// The call succeeded.
    ISMP_Result_SUCCESS = 0,

    /// The call failed because the Ingenico's device is not connected.
    ISMP_Result_ISMP_NOT_CONNECTED,

    /// The call failed for an unknown reason.
    ISMP_Result_Failure,

    /// The call failed because the timeout was reached. No response was received from the Ingenico's device.
    ISMP_Result_TIMEOUT,

    /// Key injection aborted.
    ISMP_Result_KEY_INJECTION_ABORTED,

    /// Key injection failed because no key was found on the server.
    ISMP_Result_KEY_INJECTION_KEY_NOT_FOUND,

    /// Key injection failed because the returned HTTP file is invalid.
    ISMP_Result_KEY_INJECTION_INVALID_HTTP_FILE,

    /// Key injection failed because the returned HTTP response is not 200 OK.
    ISMP_Result_KEY_INJECTION_INVALID_HTTP_RESPONSE,

    /// Key injection failed because the returned HTTP header is invalid.
    ISMP_Result_KEY_INJECTION_INVALID_HTTP_HEADER,

    /// Key injection failed because of an SSL initialization failure.
    ISMP_Result_KEY_INJECTION_SSL_NEW_ERROR,

    /// Key injection failed because the connection to the server can not be established.
    ISMP_Result_KEY_INJECTION_SSL_CONNECT_ERROR,

    /// Key injection failed because of an SSL reading error.
    ISMP_Result_KEY_INJECTION_SSL_READ_ERROR,

    /// Key injection failed because of an SSL writing error.
    ISMP_Result_KEY_INJECTION_SSL_WRITE_ERROR,

    /// Key injection failed because of an SSL profile error.
    ISMP_Result_KEY_INJECTION_SSL_PROFILE_ERROR,

    /// Key injection failed because of an internal error.
    ISMP_Result_KEY_INJECTION_INTERNAL_ERROR,

    /// The encryption key does not exist within the Ingenico's device.
    ISMP_Result_ENCRYPTION_KEY_NOT_FOUND,

    /// The encryption key is not valid.
    ISMP_Result_ENCRYPTION_KEY_INVALID,

    /// The encryption DLL is missing within the Ingenico's device.
    ISMP_Result_ENCRYPTION_DLL_MISSING
};


/**
 * Base class of an Ingenico device's communication channel.
 *
 * The application's developer should not deal directly with this class but with its sub-classes instead, otherwise it throws an exception when initialized (Abstract Class Exception).
 *
 * For instance, the `ICAdministration` class provides all the necessary features to launch configuration commands and retrieve the parameters of the terminal. The `ICBarCodeReader` class provides the interface to configure and communicate with the barcode reader embedded into the Ingenico's device, the `ICTransaction` channel works, in the other heand, as a gateway forwarding the payment transactions to the bank's servers.
 *
 * The `ICISMPDevice` class provides, however, some attributes about the connection state of the iOS application to the terminal (`[ICISMPDevice isAvailable]`), the name of the protocol used by an ICISMPDevice (`[ICISMPDevice protocolName]`), and gives also access to the input and ouput streams, which can be handy for debug or test purposes in case we decide to go without the external accessory protocol and to initialize the device with streams created manually.
 *
 * This class was named ICDevice in the library version prior to 3.2, and was renamed, since, to avoid conflicts with another classhaving the name from Apple's Image Capture Framework. It implements the basic communication login between the iOS device and the Ingenico's device and provides some class methods and callbacks which help to retrieve information about the library or the Ingenico's device. This class can be considered as the core of the PCL library.
 *
 * @throws "Abstract Class Exception"
 */
@interface ICISMPDevice : NSObject <NSStreamDelegate> {
#pragma mark - Protected attributes
	// Companion management

    /**
     * The name of the external accessory protocol implemented by `ICISMPDevice`.
     *
     * @note The procotol names conform to the reverse DNS format.
     */
	NSString				* protocolName;

    /**
     * The IAP session opened by the `ICISMPDevice` to the Ingenico's device.
     */
	EASession				* _cradleSession;

    /**
     * Ingenico's device connection state.
     */
	BOOL					  isAvailable;

    /**
     * Serial output stream.
     */
	NSOutputStream			* outStream;

    /**
     * Serial input stream.
     */
	NSInputStream			* inStream;
	
	// iSMP Messages Processing

    /**
     * Used to synchronize access to received data buffer.
     */
	NSRecursiveLock			* _inDataLock;

    /**
     * Data received from Ingenico's device.
     */
	NSMutableData			* _inStreamData;

    /**
     * Map TLV tags to selectors.
     */
	NSMutableDictionary		* _actionLookupTable;

    /**
     * Indicates whether data should be processed in the communication thread (NO by default, messages are processed on the main thread).
     */
	BOOL					  mustProcessReceivedDataOnCurrentThread;

    /**
     * List of all Ingenico's device's response Tags on a given channel.
     */
	NSArray					* _spmResponseTags;

    /**
     * This operation queue serializes the send operations to the Companion
     */
	NSOperationQueue		* _requestOperationQueue DEPRECATED_ATTRIBUTE;

    /**
     * Condition variable  used by subclassed of `ICISMPDevice` to synchronize their calls.
     */
	NSCondition				* _waitingForResultCondition;

    /**
     * Array of requests sent but that were not responded to by the terminal.
     */
	NSMutableArray			* _pendingRequests;

    /**
     * Selector called to process received data.
     */
	SEL _processReceivedDataSEL DEPRECATED_ATTRIBUTE;

    /**
     * Used to optimize the call of _processReceivedDataSEL.
     */
	IMP _processReceivedDataIMP DEPRECATED_ATTRIBUTE;

    /**
     * Selector called to simulate a byte available event.
     */
	SEL _simulateEventBytesAvailableforStreamSEL DEPRECATED_ATTRIBUTE;
}

#pragma mark - Properties

/**
 * Check and get notified about connection changes between the iDevice and Companion
 */

/**
 * The name of the external accessory protocol implemented by `ICISMPDevice`.
 *
 * @note The protocol names conform to the reverse DNS format.
 */
@property (nonatomic, readonly) NSString    *protocolName;

/**
 * The state of the `ICISMPDevice`.
 *
 * This attribute indicates the state of the `ICISMPDevice`.
 * If the channel corresponding to the protocol's name is not open, it will be set to NO. Instead, if the channel is opened, it will be set to YES.
 */
@property (readonly) BOOL isAvailable;

/**
 * This property is a pointer to the input stream opened by `ICISMPDevice` to read data from the Ingenico's device.
 *
 * This can be an input stream provided by the Ingenico's device session opened by the iOS application to the terminal, or can be an input stream set manually (for testing purpose). In both situations, it can be used to spy the state of the stream.
 */
@property (readonly) NSInputStream   *inStream;

/**
 * This property is a point to the ouput stream opened by the `ICISMPDevice` to send data to the Ingenico's device.
 *
 * This can be an output stream provided by the Ingenico's device session opened by the iOS application to the terminal, or can be an ouput stream set manually (for testing purpose). In both situations, it can be used to spy the state of the stream.
 */
@property (readonly) NSOutputStream  *outStream;

/**
 * The delegate object which should implement the `ICISMPDeviceDelegate` protocol.
 *
 * @note By implementing the protocol, the delegate is notified with `ICISMPDevice`'s events.
 */
@property(nonatomic, weak) id<ICISMPDeviceDelegate> delegate;

#pragma mark - Instance methods

/**
 * Initialize a new object (the receiver) with a definied protocol string.
 *
 * @note An init message is generally coupled with an alloc.
 *
 * @param protocolString A string identifying the channel.
 * @return The initialized receiver.
 */
-(id)initWithProtocolString:(NSString*)protocolString;

/**
 * Initialize a new object (the receiver) with input and output streams.
 *
 * @note This method initializes the `ICISMPDevice` without resorting to the Ingenico's device's protocol.
 *
 * @param inStream An input stream.
 * @param outStream An output stream.
 *
 * @return The initialized receiver.
 */
-(id)initWithStreamIn:(NSInputStream*)inStream 
		 andStreamOut:(NSOutputStream*)outStream DEPRECATED_ATTRIBUTE;

#pragma mark - Class methods

// The following methods reads values returned in the authentication process

/**
 * The connection state to the device.
 *
 * @result YES if the Ingenico's device is connected, NO otherwise.
 */
+(BOOL) isAvailable;

/**
 * Returns the SVN revision of the library.
 *
 * @return The revision string.
 */
+(NSString *)getRevisionString;

/**
 * Returns the official version of the PCL library in the form of "@(#)PROGRAM:iSMP PROJECT:iSMP-x.y". This is the function to use to check the version of the PCL library. To just get the version number, get the substring of the returned `NSString` object starting from index 30.
 *
 * @return The version string.
 */
+(NSString *)getVersionString;

/**
 * The truncated serial number of the device.
 *
 * @return Returns nil if the device is not connected or the 8 last digits of the serial number otherwise.
 * @seeAlso `[ICISMPDevice getFullSerialNumber]`
 */
+(NSString*) serialNumber;

/**
 * The model number of the Ingenico's device.
 *
 * @return Returns nil if the device is not connected or the model number otherwise.
 */
+(NSString*) modelNumber;

/**
 * The firmware revision of the Ingenico's device.
 *
 * @return Returns nil if the device is not connected or the firmware revision otherwise.
 */
+(NSString*) firmwareRevision;

/**
 * The hardware revision of the Ingenico's device.
 *
 * @return Returns nil if the device is not connected or the hardware revision otherwise.
 */
+(NSString*) hardwareRevision;

/**
 * Bluetooth's name of the Ingenico's device.
 *
 * @example IMP352-01234567
 * @return Returns nil if the device is not connected or the name and serial number separated by a - otherwise.
 */
+(NSString*) name;

/**
 * Set the Ingenico's device to work with.
 *
 * @param wantedDevice The Ingenico's device which you wnant to use.
 */
+(void)setWantedDevice:(NSString *)wantedDevice;

/**
 * Return the device to work with.
 *
 * @return Returns nil if no Ingenico's device is already selected with `[ICISMPDevice setWantedDevice]` method.
 */
+(NSString*) getWantedDevice;

/**
 * Get the Ingenico's devices connected in Bluetooth.
 *
 * @return Returns an `NSMutableArray` containing the serial number (8 last digits) of each connected Ingenico's IAP device.
 */
+(NSMutableArray *)getConnectedTerminals;

/**
 * Severity levels enum.
 */
enum SEVERITY_LOG_LEVELS {
    /// Debug message
	SEV_DEBUG = 0,
    /// Information message
	SEV_INFO,
    /// Trace message
	SEV_TRACE,
    /// Warning message
	SEV_WARN,
    /// Error message
	SEV_ERROR,
    /// Fatal error message
	SEV_FATAL,
    /// Unknown-Severity message
	SEV_UNKOWN
};

/**
 * Return a static string corresponding to the severity level.
 *
 * The string returned will have one of the following format:
 *  - SEV_DEBUG
 *  - SEV_INFO
 *  - SEV_TRACE
 *  - SEV_WARN
 *  - SEV_ERROR
 *  - SEV_FATAL
 *  - SEV_UNKOWN
 */
+(NSString*) severityLevelString:(int)level;

/**
 * Return a static char * corresponding to the severity level.
 */
+(const char*) severityLevelStringA:(int)level;

@end

#pragma mark -

/**
 * The `ICISMPDevice`'s delegate methods.
 *
 * @note These methods should be implemented by the `[ICISMPDevice delegate]` in order to be notified of its events.
 */
@protocol ICISMPDeviceDelegate
@optional

/**
 * Called when the Ingenico's device is available. Use `isKindOf` method to know which Ingenico's device is connected.
 *
 * When this method is triggered, the iOS device is ready to communicate with the Ingenico's device.
 *
 * The `-(void)accessoryDidConnect:(ICICMPDevice *)sender` event is fired each time that the channel is opened. It is not necessary to do any further initialization of the device, since it connects automatically to an external accessory using the same protocol whenever it is detected. The *sender* parameter informs the delegate about the channel which has just been activated soit can perform any related process accordingly.
 *
 * @param sender A pointer to the object which fired the `-(void)accessoryDidConnect:(ICICMPDevice *)sender` method.
 */
-(void)accessoryDidConnect:(ICISMPDevice*)sender;

/**
 * Called when the Ingenico's device is disconnected. Use ìsKindOf` method to know which Ingenico's device is disconnected.
 *
 * This event is fired once even if many `ICISMPDevice` channels have been opened to the external Ingenico's device. It notifies the delegate that the communication with the Ingenico's device will be interrupted. It is, then, up to the application to decide what should happen thereafter; display a disconnection notification to the user and tell him to reconnect and reboot the device, or just tell him to wait until the Ingenico's device resets if it is the case.
 *
 * @param sender A pointer to the object which fired `-(void)accessoryDidDisconnect:(ICISMPDevice *)sender` event.
 */
-(void)accessoryDidDisconnect:(ICISMPDevice *)sender;

@optional 

// log : you may implement either logEntry / logSerialData or implement the device specific --> only one call is made.

/**
 * Provides the log traces and their severities.
 *
 * @warning This method may be called by the library from an arbitrary thread. So the application should not perform a long or a blocking task within this delegate method.
 *
 * @param message The log message.
 * @param severity The severity of the log message (one of the values definied within `SEVERITY_LOG_LEVELS`).
 */
-(void) logEntry:(NSString*)message withSeverity:(int)severity;

/**
 * Provides traces of serial data exchanged with the Ingenico's device.
 *
 * @warning This method may be called by the library from an arbitrary thread. So the application should not perform a long or a blocking task within this delegate method.
 *
 * @param data The serial data.
 * @param isIncoming Will be YES if the data comes to the iOS device or NO in the other case.
 */
-(void) logSerialData:(NSData*)data incomming:(BOOL)isIncoming;

@end
