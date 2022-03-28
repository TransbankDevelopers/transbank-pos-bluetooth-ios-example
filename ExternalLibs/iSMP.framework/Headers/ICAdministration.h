//
//  ICAdministration.h
//  PCL Library
//
//  Created by Hichem Boussetta on 19/07/10.
//  Copyright 2010 Ingenico. All rights reserved.
//


/*!
    @file       ICAdministration.h
    @brief      Header file for ICAdministration class and its related data structures and protocol definitions
*/


#import <Foundation/Foundation.h>
#import "ICISMPDevice.h"

@class ICTmsInformation;
@class ICSoftwareComponent;

#define PROFILE_NAME_SIZE   11                          /**< The tms  SSL profile name max size is 11 */
#define CONTRAT_NAME_SIZE   10                          /**< The tms identifier name max size is 10 */
#define NB_PROFILE          20                          /**< The tms profile max number is 20 */
#define PORT_MAX            65535                       /**< The tms port can't exceed 65535 */
#define HOSTNAME_NAME_SIZE  257                         /**< The tms hostname lenght can't exceed 257 caracters */

/**
 * The Ingenico's device's information.
 */
struct _ICDeviceInformation {
    /// The device's truncated serial number (8 last digits).
	NSInteger		serialNumber;

    /// The device's part number.
	NSInteger		reference;

    /// The payment protocol used by the device.
	char			protocol[20] NS_REFINED_FOR_SWIFT;
};

/**
 * The Ingenico's device's information.
 */
typedef struct _ICDeviceInformation ICDeviceInformation;

/**
 * Convert the device's protocol as a NSString.
 *
 * @note Mapped in Swift to ICDeviceInformation.protocolString.
 *
 * @return An NSString representing the device's protocol.
 */
NSString *ICDeviceInformationGetProtocolString(const ICDeviceInformation *info) NS_SWIFT_NAME(getter:ICDeviceInformation.protocol(self:));


/**
 * Terminal Keyboard Constants
 */
enum eICDeviceKeys {
    /// Numeric Key 0.
	ICNum0			= '0',

    /// Numeric Key 1.
	ICNum1			= '1',

    /// Numeric Key 2.
	ICNum2			= '2',

    /// Numeric Key 3.
	ICNum3			= '3',

    /// Numeric Key 4.
	ICNum4			= '4',

    /// Numeric Key 5.
	ICNum5			= '5',

    /// Numeric Key 6.
	ICNum6			= '6',

    /// Numeric Key 7 .
	ICNum7			= '7',

    /// Numeric Key 8.
	ICNum8			= '8',

    /// Numeric Key 9.
	ICNum9			= '9',

    /// Dot Key.
	ICKeyDot		= '.',

    /// Paper Feed Key.
	ICKeyPaperFeed	= 0x07,

    /// Green Key.
	ICKeyGreen		= 0x16,

    /// Red Key.
	ICKeyRed		= 0x17,

    /// Yellow Key.
	ICKeyYellow		= 0x18,

    /// F1 Key.
	ICKeyF1			= 0x19,

    /// F2 Key.
	ICKeyF2			= 0x20,

    /// F3 Key.
	ICKeyF3			= 0x21,

    /// F4 Key.
	ICKeyF4			= 0x22,

    /// UP Key.
	ICKeyUp			= 0x23,

    /// Down Key.
	ICKeyDown		= 0x24,

    /// OK Key.
	ICKeyOK			= 0x25,

    /// C Key.
	ICKeyC			= 0x26,

    /// F Key.
	ICKeyF			= 0x28,
};

/**
 * The terminal's software component types
 */
enum eICDeviceSoftwareComponentType {
    /// Application.
	ICDeviceApplication = 0,

    /// Library.
	ICDeviceLibrary,

    /// Driver.
	ICDeviceDriver,

    /// Parameter File.
	ICDeviceParameter
};


/**
 * Enumeration of all peripheral with Ingenico's device.
 */
typedef NS_ENUM(int, iSMPPeripheral) {
    /// SPP Apple.
    SPP_Apple
};

/**
 * The enumeration of transaction account types
 */
typedef NS_ENUM(unsigned char, ICTransactionAccountType) {
    /// All account types.
    ICTransactionAccountTypeAll     				= '0',

    /// Bancaire.
    ICTransactionAccountTypeBancaire				= '1',

    /// American Express.
    ICTransactionAccountTypeAmericanExpress			= '2',

    /// Aurore.
    ICTransactionAccountTypeAurore					= '3',

    /// Cetelem.
    ICTransactionAccountTypeCetelem					= '4',

    /// Cofinoga.
    ICTransactionAccountTypeCofinoga				= '5',

    /// Diner Club.
    ICTransactionAccountTypeDinerClub				= '6',

    /// Pass.
    ICTransactionAccountTypePass					= '7',

    /// Franfinance.
    ICTransactionAccountTypeFranfinance				= '8',

    /// JCP.
    ICTransactionAccountTypeJCB						= '9',

    /// Accord.
    ICTransactionAccountTypeAccord					= 'A',

    /// Cheque.
    ICTransactionAccountTypeCheque					= 'C',

    /// Finaref.
    ICTransactionAccountTypeFinaref					= 'F',

    /// Modeus.
    ICTransactionAccountTypeModeus					= 'M',

    /// Moneo.
    ICTransactionAccountTypeMoneo					= 'O',

    /// Pinault Printemps Redoute.
    ICTransactionAccountTypePinaultPrintempsRedoute = 'P',

    /// Mondex.
    ICTransactionAccountTypeMondex					= 'X',
};

/**
 * The enumeration of transaction types
 */
typedef NS_ENUM(unsigned char, ICTransactionType) {
    /// Debit.
    ICTransactionTypeDebit		= '0',

    /// Credit.
    ICTransactionTypeCredit		= '1',

    /// Cancellation.
    ICTransactionTypeAnnulation	= '2',

    /// Duplicata.
    ICTransactionTypeDuplicata	= '3',

    /// ISO2.
    ICTransactionTypeISO2		= 'A',

    /// Specific.
    ICTransactionTypeSpecific	= 'B',
};

/**
 * The enumeration of transaction authorization types
 */
typedef NS_ENUM(unsigned char, ICTransactionAuthorization) {
    /// Authorization 0.
    ICTransactionAuthorization0	= '0',

    /// Authorization 1.
    ICTransactionAuthorization1	= '1',

    /// Authorization 2.
    ICTransactionAuthorization2	= '2',
};


/**
 * The `ICAdministration`'s delegate methods.
 */
@protocol ICAdministrationDelegate;

/**
 * The Ingenico's device's administration channel management class.
 *
 * This class handles the Ingenico's device configuration including power management, device update and other miscellaneous configuration.
 */
@interface ICAdministration : ICISMPDevice {

#pragma mark - Protected attributes

    /**
     * List of the in progress printer jobs.
     */
	NSMutableArray				* _printerJobs;

    /**
     * A boolean value which indicates whether `ICAdministration` is waiting for a signature to be returned by the application.
     */
	BOOL						  _isWaitingForSignature;

    /**
     * Table of fonts used for printing.
     */
	NSDictionary				* _fontTable;

    /**
     * A boolean value which indicates whether the power management settings should be updated.
     */
	BOOL						_shouldUpdatePowerManagementSettings;

    /**
     * The timeout value which is used for `[ICAdministration doTransaction]`
     */
    NSUInteger                  _doTransactionTimeout;

    /**
     * The SPMCI component version.
     */
    NSString                    * _spmciVersion;
}


/*!
    @defgroup   ICAdministrationInitialization  ICAdministration Initialization
    @ingroup    ICAdministrationGroup
    @brief      Steps in order to initialize the ICAdministration object
    <p>
        All administration routines of the Companion are provided by the @ref ICAdministration class. This can be initialized as following:
        <ul>
            <li>Retain the global class instance by calling @ref sharedICAdministration</li>
            <li>Set the delegate property to a valid object that conforms to the @ref ICISMPDeviceDelegate and @ref ICAdministrationDelegate protocols</li>
            <li>Call the @ref openICAdministration method to open the Administration channel</li>
            <li>Test the @ref isAvailable property to check if the Administration channel is ready to be used</li>
        </ul>
    </p>
    @{
*/

#pragma mark - Properties

/**
 * The Ingenico's device's backlight timeout configuration parameter.
 *
 * @note This property can be changed by using `[ICAdministration setTimeouts]` method.
 */
@property (nonatomic, readonly) NSInteger backlightTimeout;

/**
 * The Ingenico's device's suspend timeout configuration parameter.
 *
 * @note This property can be changed by using `[ICAdministration setTimeouts]` method.
 */
@property (nonatomic, readonly) NSInteger suspendTimeout;

/**
 * Retrieve the current battery level of the Ingenico's device.
 *
 * @note This is an integer value between 0 and 100. If an error occurs (Ingenico's device not connected for example), this value will be equel to -1.
 */
@property (nonatomic, readonly) NSInteger batteryLevel;

/**
 * The delegate object which should implement the `ICAdministrationDelegate` protocol.
 *
 * @note By implementing the protocol, the delegate is notified with `ICAdministration`'s events.
 *
 * @warning If you use `ICPclService`, use `[ICPclService delegate]` instead.
 */
@property (nonatomic, weak) id<ICISMPDeviceDelegate,ICAdministrationDelegate> delegate;

#pragma mark - Class methods

/**
 * Returns the unique shared instance of `ICAdministration`.
 *
 * @note The object returned by this method is autoreleased. It is then necessary to retain it.
 *
 * @return A point to the shared instance.
 */
+ (instancetype)sharedChannel;

#pragma mark - Instance methods

/**
 * Open the administration channel.
 *
 * This is a synchronous method which opens the administration channel. It has a timeout of 15 seconds after which the channel is deemed not to be available.
 *
 * @note The `isAvailable` property of `ICAdministration` is set to YES when the call to open is successful and to NO otherwise.
 *
 * @return This method may return one of the following constants defined within `eISMPResult`:
 *  - *ISMP_Result_SUCCESS*: if the call is successful
 *  - *ISMP_Result_ISMP_NOT_CONNECTED*: if the open fails because the Ingenico's device is not connected
 *  - *ISMP_Result_Failure*: if the call failed for another reason - this may be because the application does not declare the administration protocol or the Ingenico's device does not support this protocol.
 */
-(iSMPResult)open;

/**
 * Close the administration channel.
 *
 * This method closes the `ICAdministration` channel (its input and output streams) without having to release the object.
 *
 * @note After this method is called, the `isAvailable` property of `ICAdministration` is set to NO and all the subsequent commands that are sent to the terminal will fail.
 */
-(void)close;

/**
 * Open the barcode reader on the Ingenico's device.
 *
 * @param inactivityTimeOut Use to set a timeout for the barcode reader inactivity. By default it is 5000 seconds.
 * @return YES if the barcode reader was opened successfully or NO otherwise.
 */
-(BOOL)openBarcodeReader:(NSUInteger)inactivityTimeOut;

/**
 * Close the barcode reader on the Ingenico's device.
 *
 * @return YES if the barcode reader was closed successfully or NO otherwise.
 */
-(BOOL) closeBarcodeReader;

/**
 * Start scanning from the barcode reader on the Ingenico's device.
 *
 * @note The red light on the Ingenico's device should turn on if the barcode scanner start successfully.
 *
 * @return YES if the barcode scanner was started successfully or NO otherwise.
 */
-(BOOL) startBarcodeReaderScan;

/**
 * Stop scanning from the barcode reader on the Ingenico's device.
 *
 * @note The red light on the Ingenico's device should turn off if the barcode scanner stops successfully.
 *
 * @return YES if the barcode scanner was stopped successfully.
 */
-(BOOL) stopBarcodeReaderScan;

/**
 * Enable the barcode reader to be triggered on the Ingenico's device.
 *
 * @note The red light is turned on when the side buttons on the Ingenico's device are pressed.
 *
 * @return YES if the barcode scanner trigger was enabled successfully or NO otherwise.
 */
-(BOOL) enableBarcodeReaderTrigger;

/**
 * Disable the barcode reader to be triggered on the Ingenico's device.
 *
 * @note The red light is not turned on when the side buttons on Ingenico's device are pressed.
 *
 * @return YES if the barcode scanner trigger was disabled successfully or NO otherwise.
 */
-(BOOL) disableBarcodeReaderTrigger;

/**
 * Enable barcode reader symbologies on the Ingenico's device.
 *
 * @param symbologyParams The symbology to enable on the Ingenico's device.
 *
 * @return YES if symbologies were successfully set on the Ingenico's device or NO otherwise.
 */
-(BOOL) enableBarcodeReaderSymbologies :(int)symbologyParams;

/**
 * Configure the barcode reader mode on the Ingenico's device.
 *
 * @param readerMode  Single scan = 0, Multiple scan = 10
 *
 * @return YES if the barcode reader mode was successfully set on the Ingenico's device or NO otherwise.
 */
-(BOOL) setReaderModeForBarcodeReader: (int)readerMode;

/**
 * Configure the barcode reader scan beep on the Ingenico's device.
 *
 * @param beepMode Disable = 0, 1 beep = 1, 2 beeps = 2.
 *
 * @return YES if the barcode reader scan beep was successfully set on the Ingenico's device or NO otherwise.
 */
- (BOOL) setGoodScanBeepForBarcodeReader: (int)beepMode;

/**
 * Configure the barcode reader imager mode on the Ingenico's device.
 *
 * @param imagerMode 1D = 0, 1D2D = 1, 1D2D bright = 2, 1D2D reflective = 3.
 *
 * @return YES if the barcode reader imager mode was successfully set on the Ingenico's device or NO otherwise.
 */
- (BOOL) setImagerModeForBarcodeReader : (int)imagerMode;

/**
 * Set the barcode reader version on the Ingenico's device.
 *
 * @param version The version number (two bytes).
 *
 * @return YES if the barcode reader version was successfully set or NO otherwise.
 */
- (BOOL) setBarcodeReaderVersion :(NSString *)version;

/**
 * Soft reset of the barcode reader on the Ingenico's device.
 *
 * @note These method will reset all barcode reader configurations.
 *
 * @return YES if the barcode reader reset was successfully performed or NO otherwise.
 */
- (BOOL) resetBarcodeReader;

/**
 * Get the barcode reader version from the Ingenico's device.
 *
 * @return The version number (two bytes) or nil if no version was set.
 */
- (NSString *) getBarcodeReaderVersion;

/**
 * Configure the barcode reader beep on the Ingenico's device.
 *
 * @param length The duration of the beep (in ms between 0 and 2500ms).
 * @param frequency The frequency of the beep (in Hz between 1000 and 5110Hz).
 *
 * @return YES if the barcode reader beep was successfully set or NO otherwise.
 */
- (BOOL) setBeepFrequencyAndLengthForBarcodeReader : (int)length freq:(int)frequency;

/**
 * Configure the barcode reader lightning mode on the Ingenico's device.
 *
 * @param mode illuminationLEDPriority = 0, aperturePriority = 1.
 *
 * @return YES if the barcode reader lightning mode was successfully set or NO otherwise.
 */
- (BOOL) setLightningModeForBarcodeReader : (int)params;

/**
 * Get the barcode reader firmware version from the Ingenico's device.
 *
 * @return The firmware version.
 */
-(NSString *) getFirmwareVersionForBarcodeReader;

/**
 * Configure the Ingenico's device screen light timeouts.
 *
 * Change the Ingenico's device suspend and backlight timeouts to optimize power consumption. The new values will not necessarily be applied, depending on whether they are within or outside the valid value range supported by the device. In the latter case, the new timeout values are culled to fit the valid range and the method will return NO. The new actual timeouts can be retrieved by using `[ICAdministration backlightTimeout]` and `[ICAdministration suspendTimeout]`.
 *
 * @param backlightTimeout The backlight timeout value (between 10 and 10000).
 * @param suspendTimeout The suspend timeout value (between 10 and 65535).
 *
 * @return YES if the timeouts was successfully set or NO otherwise.
 */
-(BOOL)setBacklightTimeout:(NSUInteger)backlightTimeout andSuspendTimeout:(NSUInteger)suspendTimeout;

/**
 * Set the system date and time of the Ingenico's device.
 *
 * @note The request is applied by the Ingenico's device only if the difference between the iOS device and the Ingenico's device dates is less than 2 days.
 *
 * @return YES if the date was successfully set or NO otherwise.
 */
-(BOOL)setDate;

/**
 * Get the system date and time of the Ingenico's device.
 *
 * @note This method is synchronous and may be time consuming depending on the load of the Ingenico's device.
 *
 * @return A `NSDate` containing the current date and time of the Ingenico's device.
 */
-(NSDate *)getDate;

/**
 * Retrieve the state of the Ingenico's device.
 *
 * @note The purpose of this message is to poll the Ingenico's device to get its state and turn it off in case of inactivify.
 *
 * @return YES if the Ingenico's device is idle or NO if it is busy.
 */
-(BOOL)isIdle;

/**
 * Get the peripheral status of the Ingenico's device.
 *
 * @note The purpose of this message is to poll the Ingenico's device to get the status of the peripheral connected on the SPP channel.
 *
 * @param device An `iSMPPeripheral` value indicating which link to return the status.
 *
 * @return 0 if not connected, 2 if connected and -1 in case of error.
 */
-(int)getPeripheralStatus:(iSMPPeripheral)device;

/**
 * Retrieve the current serial number and reference code of the Ingenico's device.
 *
 * @note When the `getInformation` method fails due to a timeout, the integer data members of the `ICDeviceInformations` are set to negative values.
 *
 * @return An `ICDeviceInformations` structure containing general information about the Ingenico's device.
 */
-(ICDeviceInformation)getInformation;

/**
 * Retrieve the full serial number of the Ingenico's device.
 *
 * @note When `getFullSerialNumber` method fails due to a timeout, the return value is set to "NULL".
 *
 * @return A NSString containing the full serial number of the Ingenico's device.
 */
-(NSString*)getFullSerialNumber;

/**
 * Reset the Ingenico's device.
 *
 * @note This method asks to the Ingenico's device to reboot. The call to `reset` returns immediately and has no effect when the Ingenico's device is disconnected.
 *
 * @param resetInfo A constant value indicating the reason of the reset.
 */
-(void)reset:(NSUInteger)resetInfo;

/**
 * Simulate a key press on the Ingenico's device's keypad.
 *
 * @note This command simulates a key press event on the Ingenico's device's keypad. It returns immediately and has no effect when the Ingenico's device is disconnected.
 *
 * @param key The key code from `eICDeviceKeys`.
 *
 * @result YES if the key was successfully send or NO otherwise.
 */
-(BOOL)simulateKey:(NSUInteger)key;

/**
 * Retrieve the Ingenico's device's software components informations.
 *
 * @note This method issues a synchronous message to retrieve the software components loaded into the Ingenico's device. It returns an `NSArray` of `ICSoftwareComponent` objects. Each one of these objects has a name, version, type and CRC code. All binary types are enumerated within `eICDeviceSoftwareComponentType`.
 *
 * @return A `NSArray` containing `ICSoftwareComponent` objects.
 */
-(NSArray<ICSoftwareComponent *> *)getSoftwareComponents;

/**
 * Retrieve the Ingenico's device's SPMCI component version.
 *
 * @note This method issues a synchronous message to retrieve the software component version loaded into the Ingenico's device. It returns an `NSString` of `ICSoftwareComponent` objects.
 *
 * @return A `NSString` containing the SPMCI version file.
 */
-(NSString *)getSpmciVersion;

/**
 * Start a remote download session on the Ingenico's device.
 *
 * @note This method starts the remote download process which updates the software components of the Ingenico's device from a remote server. The server's parameters (IP, Port, Logon) must be configured on the Ingenico's device side.
 *
 * @warning A network connection is required for the download and the Ingenico's device should be granted access to the network by properly initializing a `ICPPP` or `ICNetwork` object.
 * This function is blocking and returns as soon as the download is over, or when the 10 minutes timeout is reached. The terminal reboots just after that.
 *
 * @result YES if the update is successful or NO otherwise.
 */
-(BOOL)startRemoteDownload;

/**
 * Update the Ingenico's device's encryption key.
 *
 * @note This methid is synchronous and takes time to return since it requires downloading an encryption key from a remote server. It should be performed in background to avoid freezing the main thread.
 *
 * @param ip The IP address of the remote server where download the key.
 * @param port The port number of the remote server where download the key.
 *
 * @return A return code constant definied with the `eISMPResult` enumeration. `ISMP_Result_SUCCESS` is returned if the call succeeds. The error codes are prefixed with *`ISMP_Result_KEY_INJECTION`*.
 */
-(iSMPResult)updateEncryptionKeyWithServerIP:(NSString *)ip andPort:(NSUInteger)port;

/**
 * Update the Ingenico's device encryption key.
 *
 * @note This method is synchronous and takes time to return since it requires downloading an encryption key from a remote server. It should be performed in background to avoid freezing the main thread.
 *
 * @param hostname The hostname address of the remote server where download the key.
 * @param port The port number of the remote server where download the key.
 *
 * @return A return code constant definied with the `eISMPResult` enumeration. `ISMP_Result_SUCCESS` is returned if the call succeeds. The error codes are prefixed with *`ISMP_Result_KEY_INJECTION`*.
 */
-(iSMPResult)updateEncryptionKeyWithServerByHostName:(NSString *)hostname andPort:(NSUInteger)port;

/**
 * Verify if the encryption key loaded into the Ingenico's device is valid.
 *
 * This method verify if an encryption key previously downloaded into the Ingenico's device through `updateEncryptionKeyWithServerIP` or `updateEncryptionKeyWithServerByHostName` is present and valid.
 *
 * @return A return code constant definied with the `eISMPResult` enumeration. `ISMP_Result_SUCCESS` is returned if the call succeeds.
 */
-(iSMPResult)validateEncryptionKey;

/**
 * Erase an encryption key loaded into the Ingenico's device.
 *
 * This method removes an encryption key previously downloaded into the Ingenico's device through `updateEncryptionKeyWithServerIP` or `updateEncryptionKeyWithServerByHostName`.
 *
 * @return A return code constant definied with the `eISMPResult` enumeration. `ISMP_Result_SUCCESS` is returned if the call succeeds.
 */
-(iSMPResult)eraseEncryptionKey;

/**
 * Update the server connection state of the Ingenico's device.
 *
 * This method is used to inform the Ingenico's device whether the iOS device is connected to a remote host. This will affect the icon on the Ingenico's device's manager which will display an established or a broken link.
 *
 * @param connectionState The connection state to a remote host. Set to YES when connected or NO when disconnected.
 *
 * @return YES if the operation succeeds or NO otherwise.
 */
-(BOOL)setServerConnectionState:(BOOL)connectionState;

/**
 * Configure the TMS server's parameters of the Ingenico's device.
 *
 * This method is used to write one or more parameters in the Ingenico's device using `ICTmsInformation` structure.
 *
 * @return A return code constant definied with the `eISMPResult` enumeration. `ISMP_Result_SUCCESS` is returned if the call succeeds.
 */
-(iSMPResult)setTmsInformation:(ICTmsInformation*)tmsInfos;

/**
 * Retrieve the TMS server's parameters from the Ingenico's device.
 *
 * @note This method is synchronous and may be time consuming depending on the load of the Ingenico's device.
 *
 * @return The parameters set in the Ingenico's device are returned in `ICTmsInformation` structure.
 */
-(ICTmsInformation*)getTmsInformation;

/**
 * Set lock backlight of the Ingenico's device.
 *
 * @note This method is used to manage the backlight of the Ingenico's device. Unlock is automatic when using the keyboard and the lock command is unvalidated.
 *
 * @param lockValue 0 to unlock, 3 to lock the backlight.
 *
 * @return The parameters set in the Ingenico's device are returned in `ICTmsInformation` structure.
 */
-(iSMPResult)setLockBacklight:(NSUInteger)lockValue;

/**
 * Send a Telium's shortcut to the Ingenico's device.
 *
 * @warning This method only works with Ingenico's device under Telium 2.
 *
 * @note This method is used to access directly a specific Telium Manager menu specified by a shortcut number. The shorcut list is available on the Telium Manager user guide.
 *
 * @param shortcutManager The shortcut affected to a specific menu of the Telium Manager.
 *
 * @return YES if the operation succeeds or NO otherwise.
 */
-(BOOL)sendShortcut:(NSString*)shortcutManager;

/**
 * Get the iOS add-on version.
 *
 * @note This method is used to know which version of the PCL for iOS add-on is linked with the application.
 *
 * @return An `NSString` containing the add-on version.
 */
-(NSString*)getAddonVersion;

@end

/**
 * The `ICAdministration`'s delegate methods.
 */
@protocol ICAdministrationDelegate
@optional

/**
 * This event is triggered to tell to the iOS device that it has to awake the payment application at some point in time.
 *
 * @note All the notification data is contained in an `UILocalNotification` object to be scheduled inside this method. This is done by calling `scheduleLocalNotification` on the `UIApplication` shared object. Backward compability with iOS 3.x is maintained and the event is not fired in this case. It is also for this reason that the returned notification object is passed as an `id`.
 *
 * @param wakeUpNotification A pointer to a `UILocalNotification` object which should be registered by the receiving application.
 */
-(void)shouldScheduleWakeUpNotification:(id)wakeUpNotification;

/**
 * Log event handler to be implemented by the delegate of the `ICAdministration` object.
 *
 * The log messages can be filtered based on their severity. They are defined within the `SEVERITY_LOG_LEVELS` enumeration and can be converted to a human readable form using the `-[ICISMPDevice severityLevelString:]` and `-[ICISMPDevice severityLevelStringA:]` methods of the `ICISMPDevice` class.
 *
 * @param message The log message.
 * @param severity The severity of the log message.
 */
-(void)confLogEntry:(NSString*)message withSeverity:(int)severity;

/**
 * Serial data log event handle to be implemented by the delegate of the `ICAdministration` object.
 *
 * @param date The serial data incoming or outgoing on the `ICAdministration` channel.
 * @param isIncoming A boolean for the direction in/out of the serial data.
 */
-(void)confSerialData:(NSData*)data incoming:(BOOL)isIncoming;

@end
