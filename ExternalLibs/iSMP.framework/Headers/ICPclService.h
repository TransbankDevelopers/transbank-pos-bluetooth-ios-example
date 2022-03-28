//
//  ICPclService.h
//  PCLService Library
//
//  Created by Stephane RABILLER on 04/12/14.
//  Copyright (c) 2014 Ingenico. All rights reserved.
//


/*!
 @file	 ICPclService.h
 @brief   Header file of the ICPclService class and its related data structures and protocol definitions
 */

#import <Foundation/Foundation.h>
#import "ICAdministration.h"
#import "ICAdministration+StandAlone.h"
#import "ICAdministration+iBP.h"
#import "ICPPP.h"

@class ICTerminal;
@class ICSSLParameters;


@interface NSString (NSStringHexToBytes)

/*!
 @anchor hexToBytes
 @brief  This function converts hexadecimal string to bytes
 @return Returns an NSData in bytes of the hexadecimal string
 */
-(NSData*) hexToBytes;

@end

/*!
 @defgroup   ICPclServiceGroup PCL Services
 @brief      Methods, structures and callbacks of ICPclService class, that can be used with all Ingenico terminals.
  <p><br>When pclService is started and connected, a TCP/IP link is automatically created between the iDevice and the Telium device (for Terminal connected via Bluetooth) </p>
 @{
 */

/*!
 @anchor     ICPclServiceDelegate
 @brief      The ICPclServiceDelegate method
 <p>
 These methods should be implemented by @ref ICPclServiceDelegate's delegate to subscribe to be notified of events.
 </p>
 */

@protocol ICPclServiceDelegate;

/*!
 @enum		 pclServiceState
 @brief      Enumerate the possible states of the PCL Service
 */
typedef enum {
    PCL_SERVICE_STOPPED = 0,                        /**< The service is stopped */
    PCL_SERVICE_STARTED,                            /**< The service is started but not connected to the Telium device */
    PCL_SERVICE_CONNECTED,                          /**< The service is started and an iDevice is connected to Telium device */
    PCL_SERVICE_FAILED_NO_CNX,                      /**< The service can't start due to no Wi-Fi or local hotspot available */
    PCL_SERVICE_FAILED_INTERNAL                     /**< The service can't start due to internal error */
} pclServiceState;

/*!
 @brief    The ICPclService management class
 <p>
This class provides access to the Terminal configuration and services, including payment transaction, printing, power management, device update and other miscellaneous services.
 </p>
 */

@interface ICPclService : NSObject <ICAdministrationDelegate, ICISMPDeviceDelegate>

/*!
 @anchor     sharedICPclService
	@brief      Returns the unique shared instance of @ref ICPclService
 <p>The object returned by this method is autoreleased. It is then necessary to retain it.</p>
	@result		A pointer to the shared instance
 */
+ (instancetype)sharedICPclService;

/*!
	@brief      The delegate object that should implement the @ref ICPclServiceDelegate protocol
 <p>By implementing the protocol, the delegate is notified with @ref ICPclServiceDelegate events</p>
 */
@property (nonatomic, weak) id<ICPclServiceDelegate> delegate;

/*!
 @anchor  startPclServiceWithandSecurity
 @brief   Start the connection with the Telium device selected

 This method performs a connection to the selected Telium device. It is recommended (but optionnal) to secure the connection using TLS.
 Note that Ingenico does not provide any TLS certificate. Certificates must be managed by the application.
 Warning: Once the service is started, it may take a few seconds for the connection with the Telium device to be established. Make sure to implement the @ref notifyConnection to know when the connection is established.

 Note: before Add-on 2.22.00 it was required to call +[ICPclService selectTerminal:] prior to calling this method.

 Note: you should never call +[ICISMPDevice setWantedDevice:] if you are using ICPclService.

 @param terminal The Terminal to connect to
 @param sslParams The TLS information of the .p12 file included in the application.
 @result One of the enumerations of @ref pclServiceState. It is PCL_SERVICE_STARTED when the call succeeds.
 */
-(pclServiceState)startPclServiceWith:(ICTerminal*) terminal andSecurity:(ICSSLParameters *) sslParams;

/*!
 @anchor getPclServiceState
 @brief   The status of the pclService
 @result  One of the value from @ref pclServiceState
 */
-(pclServiceState)getPclServiceState;

/*!
 @anchor stopPclService
 @brief Stop the connection between the terminal and the iDevice
 <p>This method closes the ICPclService (its input and output streams) without having to release the object. After this method is called, the @ref notifyDisconnection callback is fired, and all the subsequent commands sent to the terminal fail.
 </p>
 */
-(void)stopPclService;


/*! @brief the terminal currently connected to */
@property (readonly, nonatomic) ICTerminal *terminal;

/**
 This methods allows you to paired an new Ingenico's device direclty from your app and connect it through PCL.

 This methods use `[EAAccessoryManager showBluetoothAccessoryPickerWithNameFilter:completion:]` to display the available Ingenico's device corresponding to the protocols declared in the plist file. The use of this method can be long (up to 3 minutes) to display an Ingenico's device and the UI is not customizable.

 Once you select an Ingenico's device, it will ask you to compare the PIN code on the iOS device and the Ingenico's device for PCI certification.

 Once the Ingenico's device and iOS device are paired in Bluetooth, ICPclService will automatically detect the new paired device and start a new PCL connection to it. You can be notified of the connection by using `[ICPclServiceDelegate notifyConnection:(ICPclService *)sender]`.

 @note The Ingenico's device must be set to Phone Pairing mode.

 @warning Only the Ingenico's device which are not already paired with the iOS device will appear in this method. For all already paired devices, you should use the `[ICPclService getAvailableTerminals] methods.
 */
+(void)easyBluetoothPairing;

/*!
 @anchor selectTerminal
 @brief  Set the device to work with
 @param wantedDevice The device to use
 @deprecated use -startPclServiceWith:andSecurity: to select a terminal and connect to it
 */
+(void)selectTerminal:(ICTerminal *)wantedDevice DEPRECATED_ATTRIBUTE;

/*!
 @anchor  getSelectedTerminal
 @brief   Return the device to work with
 @result returns nil if no device already selected with the @ref selectTerminal function
 @deprecated you can use -terminal to get the terminal currently connected
 */
+(ICTerminal *)getSelectedTerminal DEPRECATED_ATTRIBUTE;

/*!
 @anchor  getAvailableTerminals
 @brief   Return the list of terminals that can be used by the iDevice. The list will include both the Bluetooth one (paired and connected one) and the ones available throught the network
 @result returns nil if no device is found or the list of @ref ICTerminal
 */
-(NSArray<ICTerminal *> *)getAvailableTerminals;

/*!
 @anchor setTerminalTime
 @brief  Set the system date and time of the Terminal module
 <p>The setDate request is applied by the Terminal only if the difference between the iDevice and the Terminal dates is less than 2 days</p>
 @result		YES if the request succeeds, NO otherwise
 */
-(BOOL)setTerminalTime;


/*!
 @anchor getTerminalTime
 @brief  Get the system date and time of the Terminal module
 <p>This method is synchronous and may be time consuming depending on the load of the Terminal</p>
 @result		A NSDate containing the current date of the Terminal
 */
-(NSDate *)getTerminalTime;

/*!
 @anchor getTerminalInfo
 @brief Ask the Terminal module for its current serial number and reference code
 <p>
 When the <b>getInformation</b> request fails due to a timeout, the integer data members of @ref ICDeviceInformation are set to a negative value.
 </p>
 @result A @ref ICDeviceInformation structure containing general information about the Terminal
 */
-(ICDeviceInformation)getTerminalInfo;

/*!
 @anchor getFullSerialNumber
 @brief  Ask the Terminal module for its full serial number
 <p>
 When the <b>getFullSerialNumber</b> request fails due to a timeout, the return is set to "NULL".
 </p>
 @result An NSString containing the full serial number of the Terminal
 */
-(NSString*)getFullSerialNumber;

/*!
 @anchor resetTerminal
 @brief Reset the terminal.
 <p>
 This request asks the terminal to reboot.<br />
 The call to @ref resetTerminal returns immediately and it has no effect when the terminal is disconnected.
 </p>
	@param resetInfo A constant indicating the reason of the reset
 */
-(void)resetTerminal:(NSUInteger)resetInfo;

/*!
 @anchor inputSimul
 @brief Simulate a key press on the terminal's keypad
	<p>
 This command simulates a key press event on the terminal's keypad. It returns immediately and has no effect when the terminal is disconnected.
 </p>
	@param key The key code (refer to @ref eICDeviceKeys for all possible values)
	@result YES if the command was successful, NO otherwise.
 */
-(BOOL)inputSimul:(NSUInteger)key;

/*!
 @anchor     getTerminalComponents
	@brief      Get the terminal's software components information
 <p>This method issues a synchronous message to retrieve the software components loaded into the terminal. It returns an array of @ref ICSoftwareComponent objects. Each one of these objects has a name, version, type and CRC code. All binary types are enumerated within @ref eICDeviceSoftwareComponentType</p>
	@result		NSArray containing the components' file names.
 */
-(NSArray<ICSoftwareComponent *> *)getTerminalComponents;

/*!
 @anchor     getSpmciVersion
 @brief      Get the terminal's SPMCI component version
 <p>This method issues a synchronous message to retrieve the software component version loaded into the terminal. It returns an NSString of @ref ICSoftwareComponent objects.
 @result		NSString containing the SPMCI version file.
 */
-(NSString *)getSpmciVersion;

/*!
 @brief   getAddonVersion
 <p>The getAddonversion request is used to know what is the version of the iOS PCL add-on linked with the application.</p>
 @result        An NSString containing add-on version
 */
-(NSString*)getAddonVersion;

/*!
 @anchor     doUpdate
 @brief      Start a remote download session on the Terminal
 <p>
 This method starts the remote download process that updates the software components of the Terminal terminal from a remote server. The server's parameters (IP, Port, Logon) have to be configured on the terminal side or by using the @ref setTmsInformation.<br />
 A network connection is required for the download, and the Terminal should be granted access to the network.<br />
 This function is blocking and returns as soon as the download is over, or when the 10-minuts timeout is reached. The terminal reboots just after that.
 </p>
 @result YES if the update is successful, NO if it failed.
 */
-(BOOL)doUpdate;

/*!
 @anchor setTmsInformation
 @brief Set the TMS server parameters
 <p>The setTmsInformation request is used to write one or more parameters in the terminal using the @ref ICTmsInformation structure</p>
 @result		A return code constant defined within the @ref eISMPResult enumeration. ISMP_Result_SUCCESS is returned if the call succeeds.
 */
-(iSMPResult)setTmsInformation:(ICTmsInformation*)tmsInfos;


/*!
 @anchor getTmsInformation
 @brief Get the TMS server parameters from the Terminal
 <p>This method is synchronous and may be time consuming depending on the load of the Terminal</p>
 @result The parameters set in the terminal are returns in @ref ICTmsInformation structure
 */
-(ICTmsInformation*)getTmsInformation;

/*!
 @anchor setLockBacklight
 @brief Lock or unlock the backlight.
 <p>When the backlight is locked, it will not switch on when a PCL request is received and only a keyboard action will switch it on (and invalidate the lock configuration).
 <br>When the backlight is unlocked, each PCL command will switch the backlight on.</p>
 @param lockValue 0 to unlock, 3 to lock the backlight.
 @result		A return code constant defined within the @ref eISMPResult enumeration. ISMP_Result_SUCCESS is returned if the call succeeds.
 */
-(iSMPResult)setLockBacklight:(NSUInteger)lockValue;

/*!
 @anchor launchM2OSshorcut
 @brief Send a Telium Manager menu shortcut
 <p>The sendShortcut function is used to do a direct call to a Telium Manager menu, specified by its shortcut number. The list of default shortcuts is defined in the Telium Manager user guide. Note that depending on the application loaded on the Telium device, additional shortcuts might be available.</p>
 @param shortcutManager The shortcut assigned to the Telium Manager menu to reach
 @result		YES if the operation succeeds, NO if it fails
 */
-(BOOL)launchM2OSshorcut:(NSString*)shortcutManager;

/*!
 @anchor     sendMessage
 @brief      Send a message to the Terminal
 <p align="justify">
 The UnderCover messaging makes it possible to wrap any third-party protocol with the protocol used on the administration channel.
 Before using the UnderCover messaging, the iOS application and the Terminal's application must agree on the protocol or the messages that would be exchanged.<br />
 Refer to @ref messageReceivedWithData on how to receive messages from the Terminal.
 </p>
 @param		data An NSData object containg the message to be sent. The maximum allowed data length is 1KB.
 @result		YES if the message was received
 */
-(BOOL)sendMessage:(NSData *)data;

/*!
 @anchor     doTransactionWithRequest
 @brief      Starts a transaction in standalone payment mode.

 In standalone payment mode, the terminal communicates directly with the bank server without having any intermediate
 application server.

 doTransactionWithRequest is an asynchronous method that returns immediately. As such the result is not returned, but
 given back to the delegate, through the method @ref transactionDidEnd declared in protocol @ref ICPclServiceDelegate.
 Since doTransactionWithRequest is asynchronous, communication can take place
 between the terminal and the application during that time. This makes it possible for example to exchange some messages
 with an application on the terminal or to handle a signature capture request.

 This method is meant to replace both -doTransaction: and -doTransaction:withData:andApplicationNumber:.

 @param      request An @ref ICTransactionRequestObject instance describing the parameters of the transaction.
 */
-(void)doTransactionWithRequest:(ICTransactionRequestObject *)request;

/*!
 @anchor     doTransaction
 @brief      Start a transaction in standalone payment mode.
 <p>This method is called to start a transaction in standalone payment mode. In this mode, the terminal communicates directly with the bank server without having any intermediate application server. The iDevice application should act as a gateway by initializing an ICNetwork object before calling @ref doTransaction.<br />
 @ref doTransaction is an asynchronous method that returns immediately. The application should conform to @ref ICPclServiceDelegate protocol and implement @ref transactionDidEnd to be notified when the transaction is finished. The maximum allowed time for the transaction to be performed is 60 seconds, after which the transaction is deemed to have failed.<br />
 Since @ref doTransaction is asynchronous, it is possible to send other @ref ICPclService requests to the iDevice before the transaction is over. This makes it possible for example to exchange some messages with an Terminal application or to handle a signature capture request.<br />
 </p>
	@param		request An @ref ICTransactionRequest structure that should be filled with the parameters required to perform the transaction.
 */
-(void)doTransaction:(ICTransactionRequest)request;

/*!
 @anchor     doTransactionEx
 @brief      Start a transaction in standalone payment mode with additional parameters.
 <p>
 This method acts exactly as @ref doTransaction. The only difference is that it comes with additional parameters to customize the transaction by passing extra data structures and specifying the terminal application that should be invoked. This function is also asynchronous and returns immediately. The result is provided to the iOS application through the callback @ref transactionDidEnd.
 </p>
 @param		request         An @ref ICTransactionRequest structure that should be filled with the parameters required to perform the transaction.
 @param      extendedData    A NSData object that contains any additional transaction data to be passed to the terminal. The maximum data length that can be passed through this parameter is 16 KB.
 @param      appNum          The number of the terminal application that should be invoked to handle the transaction. If this parameter is set to 0, the payment application will be implicitely selected.
 */
-(void)doTransactionEx:(ICTransactionRequest)request withData:(NSData *)extendedData andApplicationNumber:(NSUInteger)appNum;


/*!
 @brief   Change the transaction timeout
 @param	 timeout The new timeout value in milliseconds
 */
-(void)setDoTransactionTimeout:(NSUInteger)timeout DEPRECATED_ATTRIBUTE;


/*!
 @brief Get the doTransaction timeout
 @result The doTransaction timeout value
 */
-(NSUInteger)getDoTransactionTimeout DEPRECATED_ATTRIBUTE;

/*!
	@anchor		submitSignatureWithImage
	@brief      Submit the signature on the iDevice screen to the Terminal
 <p align="justify">
 This method should be called within the @ref shouldDoSignatureCapture event before the signature timeout would expire, otherwise it
 will be ignored
 </p>
	@param		image <p align="justify">A UIImage object containing the signature to be submitted. The image must be in black and white, and have any iOS supported bitmap configuration. More on supported bitmap configurations <a href="http://developer.apple.com/library/mac/#qa/qa2001/qa1037.html">here</a>.<br />
	@result		A boolean value equal to YES if the signature was successfully submitted to the Terminal
 */
-(BOOL)submitSignatureWithImage:(UIImage *)image;

/*!
 @brief Tries to open a session to the Terminal bluetooth printer
 @anchor openPrinter
 <p>
 This call must succeed to be able to send printing commands to the Terminal's bluetooth printer. If a session is already opened, this method returns immediately success as a result.<br />
 This call is blocking and has a 15 seconds timeout, after which it is deemed to have failed.<br />
 When the printing is done, the session should be closed by calling @ref closePrinter.
 </p>
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)openPrinter;

/*!
 @brief Tries to close an already opened printing session
 @anchor closePrinter
 <p>
 When no session has already been opened to the Terminal's bluetooth printer, this method returns imediately with a success, otherwise it inquires the Terminal printer to close the existant one.<br />
 If this command succeeds, all subsequent printing commands will fail if no new session is created by calling @ref openPrinter.<br />
 This call is blocking and has a 15 seconds timeout.
 </p>
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)closePrinter;

/**
 @anchor     printText
 @brief      Request to print the text provided as parameter
 <p>
 The length of the string to be printed should not exceed 512 characters otherwise the call will fail.<br />
 This call is blocking and has a timeout of 15 seconds. Before print text you should choose the font using @ref iBPSetFont. If @ref setPrinterFont is not used the default font is ISO8859-15.
 </p>
 @param      text NSString object of the text to be printed. The  length of this string must be 512 characters at most.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)printText:(NSString *)text;

/*!
 @anchor  printBitmap
 @brief   Request the print of the bitmap  provided as parameter
 <p align="justify">
 This method accepts all image types that are supported by iOS. The UIImage object undergoes is converted to an indexed black and white monochrome bitmap where each pixel is encoded on a bit of data. When printed, the bitmap keeps its original size if it does not exceed the maximum dimensions, otherwise it is scaled to fit within those.<br />
 A printing session must already be opened before calling this method, otherwise the call returns immediately with a failure result.<br />
 This call is blocking and has a timeout of 30 seconds.<br />
 See @ref printBitmapSizeAlignment for more drawing options.
 </p>
 @param      image UIImage object to be printed.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)printBitmap:(UIImage *)image;

/*!
 @anchor  printBitmapLastBitmap
 @brief   Request the print of the bitmap  provided as parameter
 <p align="justify">
 This method is extended from @ref printBitmap and added to fix a print issue that occurs when using a CUSTOM BT printer. More generally you should use this method if the bitmap to be printed is higher than 1024 or if you want to print several bitmaps in a consecutive way.  This method accepts all image types that are supported by iOS. The UIImage object undergoes is converted to an indexed black and white monochrome bitmap where each pixel is encoded on a bit of data. When printed, the bitmap keeps its original size if it does not exceed the maximum dimensions, otherwise it is scaled to fit within those.<br />
 A printing session must already be opened before calling this method, otherwise the call returns immediately with a failure result.<br />
 This call is blocking and has a timeout of 30 seconds.<br />
 See @ref printBitmapSizeAlignment for more drawing options.
 </p>
 @param      image UIImage object to be printed.
 @param      isLastBitmap BOOL specify if the bitmap to print is the last.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)printBitmap:(UIImage *)image lastBitmap:(BOOL)isLastBitmap;

/*!
 @anchor  printBitmapSizeAlignment
 @brief   Print a bitmap and scale it according to the provided CGSize parameter
 <p align="justify">
 This methods works exactly the same as @ref iBPPrintBitmap (It is synchronous and has a 15 seconds timeout), except that the bitmap can be scaled using the bitmapSize argument and can be aligned horizontally (left, center or right alignment). The size of the bitmap can be decreased or increased based on the bitmapSize parameter, and is always limited by the maximum dimensions that a bitmap may have to be printed.<br />
 In order to draw the bitmap in its original size, the application may provide a CGSize structure initialized with the bitmap's width and height.
 </p>
 @param      image UIImage object to be printed.
 @param      bitmapSize  A CGSize structure that specifies the preferred bitmap size.
 @param      alignment   The alignment of the bitmap within the printed ticket (left, right or center). This may be one of the constants of <b>NSTextAlignment</b>
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)printBitmap:(UIImage *)image size:(CGSize)bitmapSize alignment:(NSTextAlignment)alignment;

/*!
 @anchor  printBitmapSizeAlignmentLastBitmap
 @brief   Print a bitmap and scale it according to the provided CGSize parameter
 <p align="justify">
 This method is extended from @ref printBitmapSizeAlignment and added to fix a print issue that occurs when using a CUSTOM BT printer. More generally you should use this method if the bitmap to be printed is higher than 1024 or if you want to print several bitmaps in a consecutive way.This methods works exactly the same as @ref printBitmapSizeAlignment (It is synchronous and has a 15 seconds timeout), except that the bitmap can be scaled using the bitmapSize argument and can be aligned horizontally (left, center or right alignment). The size of the bitmap can be decreased or increased based on the bitmapSize parameter, and is always limited by the maximum dimensions that a bitmap may have to be printed.<br />
 In order to draw the bitmap in its original size, the application may provide a CGSize structure initialized with the bitmap's width and height.
 </p>
 @param      image UIImage object to be printed.
 @param      bitmapSize  A CGSize structure that specifies the preferred bitmap size.
 @param      alignment   The alignment of the bitmap within the printed ticket (left, right or center). This may be one of the constants of <b>NSTextAlignment</b>
 @param      isLastBitmap BOOL specify if the bitmap to print is the last.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)printBitmap:(UIImage *)image size:(CGSize)bitmapSize alignment:(NSTextAlignment)alignment lastBitmap:(BOOL)isLastBitmap;

/*!
 @anchor     storeLogo
 @brief      Stores logos to the Terminal.
 <p>Logos are bitmaps identified by a name composed of 8 characters at most. Logos are also converted to monochrome black and white bitmaps and undergo the same transaformations as for bitmaps printed using @ref printBitmap.<br />
 This call is blocking and has a timeout of 30 seconds.
 </p>
 @param      name  The name of the logo (Composed of 8 characters at most)
 @param      logo UIImage object of the logo to be loaded into the terminal.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)storeLogoWithName:(NSString *)name andImage:(UIImage *)logo;

/*!
 @anchor     printLogo
 @brief      Request the printing of a logo stored inside the Terminal.
 <p>
 Logos are bitmaps identified by a name and store inside the Terminal either manually (by using the file transfer tool LLT) or programmatically by calling the @ref storeLogo method. To print a logo, we just refer to the logo's name and provide as an argument to @ref printLogo.
 This call is blocking and has a timeout of 20 seconds.
 </p>
 @param      name The name of the logo.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)printLogoWithName:(NSString *)name;

/*!
 @anchor     getPrinterStatus
 @brief      Gets the printer's status
 <p>
 This call is blocking and has a timeout of 10 seconds.
 </p>
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)getPrinterStatus;

/**
 @anchor     setPrinterFont
 @brief      Request to set the font provided as parameter
 <p>
 This call permits to select the font used to print text using @ref printText.
 </p>
 @param      selectedFontToTelium the encoding format ISO8859 supported by Telium @ref eiBPFont.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)setPrinterFont:(iBPFont *) selectedFontToTelium;

/*!
 @anchor    openCashDrawer
 @brief     Request to open the cash drawer connected to a printer
 <p>
 This call allow you to open the cash drawer connected to a printer.
 </p>
 @result    One of the enumeration of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)openCashDrawer;

/**
 @anchor     iBPMaxBitmapWidth
 @brief      Gets the maximum authorized width of a bitmap to be printed
 @result     Unsigned integer value of the maximum authorized width for a bitmap
 */
@property (nonatomic, readonly, getter = iBPMaxBitmapWidth) NSUInteger iBPMaxBitmapWidth;

/*!
 @anchor     iBPMaxBitmapHeight
 @brief      Gets the maximum authorized height of a bitmap to be printed
 @result     Unsigned integer value of the maximum authorized height for a bitmap
 */
@property (nonatomic, readonly, getter = iBPMaxBitmapHeight) NSUInteger iBPMaxBitmapHeight;

/*!
	@anchor		severityLevelString
	@brief      Return a static string corresponding to the severity level
 <p>
 The string returned will have the form
 <ul>
 <li>"SEV_DEBUG"</li>
 <li>"SEV_INFO "</li>
 <li>"SEV_TRACE"</li>
 <li>"SEV_WARN "</li>
 <li>"SEV_ERROR"</li>
 <li>"SEV_FATAL"</li>
 <li>"SEV_UNKOWN"</li>
 </ul>
 </p>
 */
+(NSString*) severityLevelString:(int)level;

/*!
 @}
 */

/*!
	@anchor		setBacklightTimeout
	@brief      Configure the Terminal's screen light timeouts
 <p>
 Change the Terminal's backlight timeout to optimize power consumption. The new values will not necessarily be applied,
 depending on whether they are within or outside the valid value range supported by the device. In the latter case, the new timeout
 values are culled to fit the valid range and the @ref setBacklightTimeout request will return NO. The new actual
 timeouts can then be retrieved from the @ref getBacklightTimeout property.
 </p>
	@param		backlightTimeout The backlight timeout value. Value between 10 and 1000.
	@result		YES if the request was executed by the device, NO in the other case.
 */
-(BOOL)setBacklightTimeout:(NSUInteger)backlightTimeout;

/*!
	@anchor		setSuspendTimeout
	@brief      Configure the Terminal's screen light timeouts
 <p>
 Change the Terminal's suspend timeout to optimize power consumption. The new values will not necessarily be applied,
 depending on whether they are within or outside the valid value range supported by the device. In the latter case, the new timeout
 values are culled to fit the valid range and the @ref setBacklightTimeout request will return NO. The new actual
 timeouts can then be retrieved from the @ref getBacklightTimeout property.
 </p>
	@param		suspendTimeout The suspend timeout value. Value between 10 and 65535.
	@result		YES if the request was executed by the device, NO in the other case.
 */
-(BOOL)setSuspendTimeout:(NSUInteger)suspendTimeout;

/*!
 @anchor     getBacklightTimeout
 @brief      Get the terminal's backlight timeout
 <p>This method issues a synchronous message to retrieve the backlight timeout loaded into the terminal.
 @result		NSInteger containing the backlight timeout value.
 */
-(NSInteger)getBacklightTimeout;

/*!
 @anchor     getSuspendTimeout
 @brief      Get the terminal's suspend timeout
 <p>This method issues a synchronous message to retrieve the software component version loaded into the terminal.
 @result		NSInteger containing the suspend timeout value.
 */
-(NSInteger)getSuspendTimeout;

/*!
 @anchor     getBatteryLevel
 @brief      Get the terminal's battery level value
 <p>This method issues a synchronous message to retrieve the software component version loaded into the terminal.
 @result		NSInteger containing the battery level value.
 */
-(NSInteger)getBatteryLevel;

/*!
 @anchor    addDynamicBridge
 @brief     Add TCP/IP bridge between iOS device and Telium device
 @param     port TCP port to connect to or to listen(depending on redirection parameter)
 @param     redirection Bridge direction: 0 means iOS connects to Telium TCP port, 1 means Telium connects to iOS TCP port
 @returns   0 on success, a negative value on error

 The possible errors while opening a bridge are the following:

 - -1: no more available bridges
 - -2: bridge already exists
 - -3: issue during thread creation
 - -4: bridge initialization failed
 */
-(int)addDynamicBridge:(NSInteger)port :(int)redirection;

/*!
 @anchor    addDynamicBridge
 @brief     Add TCP/IP bridge between iOS device and Telium device. In case of bridge from iOS to Telium, listen is done only on localhost
 @param     port TCP port to connect to or to listen(depending on redirection parameter)
 @param     redirection Bridge direction: 0 means iOS connects to Telium TCP port, 1 means Telium connects to iOS TCP port
 @returns   0 on success, a negative value on error

 The possible errors while opening a bridge are the following:

 - -1: no more available bridges
 - -2: bridge already exists
 - -3: issue during thread creation
 - -4: bridge initialization failed
 */
-(int)addDynamicBridgeLocal:(NSInteger)port :(int)redirection;

/*!
 @brief   Update the server connection state
 <p>
 This method is used to inform the Companion on whether the iDevice is connected to a remote host. This will affect the icon on the Companion manager, that will display an established or a broken link.
 </p>
 @param		connectionState The connection state to a remote host. Put YES when connected, NO when disconnected.
 @result		YES if the operation succeeds, NO if it fails
 */
-(BOOL)setServerConnectionState:(BOOL)connectionState;

/*!
 @anchor  setKeepAliveDelayIntervalAndCount
 @brief   Set TCP keep alive parameters
 <p>
 This method allows to configure the different parameters. When keep alive option is set on the Telium device, the internal socks server set this option to connect to the remote using these parameters.
 <ul>
 <li>delay : idle time used when keep alive is enabled, default value 1 second, max value 4 294 967 seconds</li>
 <li>interval : interval between keepalives, default 7200 seconds, max value 4 294 967 seconds</li>
 <li>count : number of keepalives before close, default 5 times, max value 2 147 483 647</li>
 </ul>
 </p>
 @return An integer constant defined with the @ref eISMPResult enumeration. ISMP_Result_SUCCESS on success.
 */
-(iSMPResult)setKeepAliveDelay:(int)keepAliveDelay Interval:(int)keepAliveInterVal andCount:(int)keepAliveCount;

-(BOOL)isSelectedTerminalAvailable;

-(BOOL)setExchangeWaitTime1us:(int)time;

/* !
 
 @ brief Open / Close Barcode Reader on the Terminal
 @ result  YES if barcode was closed / opened successfully
 
 */

-(BOOL) closeBarcode;

-(BOOL) openBarcode;

/* !
 
 @ brief OpenBarcodeWithInactivity - Opens Barcode Reader on the Terminal
 @ param InactivityTo is the parameter used to close barcode after specified time of inactivity,  parameter specified by user,
 @ result  YES if barcode was closed / opened successfully
 
 */

-(BOOL) openBarcodeWithInactivity : (NSUInteger)inactivityTo;


/* !
 
 @ brief StartBarcodeScanning on the Terminal
 @ result  YES if barcode was Started Barcode Scanning successfully
 
 */
-(BOOL) startBarcodeScan;

/* !
 
 @ brief StopBarcodeScan on the Terminal
 @ result  YES if barcode was Stopped Barcode Scanning successfully
 
 */
-(BOOL) stopBarcodeScan;

/* !
 
 @ brief EnableBarcodeReaderTrigger on the Terminal
 @ param isEnable  Disable = 0, Enable = 1
 @ param result  Result of the operation (0=OK, 1=KO)
 @ result  YES if Trigger was enabled successfully
 <li> Expected Result : The red light is turned on when the side buttons on terminal are clicked.
 
 */
-(BOOL) enableAutoBarcodeTrigger;

/* !
 
 @ brief DisableBarcodeReaderTrigger on the Terminal
 @ param isEnable  Disable = 0, Enable = 1
 @ param result  Result of the operation (0=OK, 1=KO)
 @ result  YES if Trigger was disbaled successfully
 <li> Expected Result : The red light is not turned off when the side buttons on terminal are clicked.
 
 */

-(BOOL) disableAutoBarcodeTrigger;

/* !
 EnableBeepSoundForBarcodeReader Configure barcode reader mode on remote device.
 
 @param beepMode  Disable = 0, 1 beep = 1, 2 beeps = 2
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded.
 false if the method failed.
 <li> Expected Result : Beep enabled/disabled
 */


-(BOOL) enableBeepSoundForBarcodeReader : (int)numberOfBeeps;


/* !
 SetReaderModeForBarcodeReader Configure barcode reader mode on remote device.
 
 @param mode      Single scan = 0, Multiple scan = 1
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded.
 false if the method failed.
 <li> Expected Result : Symbologies enabled
 */
-(BOOL) setReaderModeForBarcodeReader : (int)readerMode ;


/* !
 @ brief SetImagerModeForBarcodeReader Configure barcode reader imager mode on remote device.
 
 @param imagerMode  1D = 0, 1D2D = 1, 1D2D bright = 2, 1D2D reflective = 3
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded , false if the method failed.
 <li> Expected Result : ImagerMode set based on selection
 */
-(BOOL) setImagerModeForBarcodeReader : (int)imagerMode;


-(BOOL) setIlluminatorModeForBarcodeReader : (int)illuminatorMode;

/* !
 
 @ brief EnableBarcodeReaderSymbologies on the Terminal
 @ param mode send the symbology to be enabled
 @ result  YES if symbologies were set successfully
 <li> Expected Result : Enables barcode scan
 
 */
-(BOOL) enableSymbologyForBarcodeReader : (int)parameter;

/* !
 @ brief SetBarcodeReaderVersion
 @param version           Version number (two bytes)
 @param result            Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded false if the method failed.
 <li> Expected Result : Barcode version set
 */

-(BOOL) setBarcodeVersion :(NSString *) versionNumber;

/* !
 @ brief GetBarcodeVersion
 @param result            barcodeverison received
 @return        true if the method succeeded false if the method failed.
 <li> Expected Result : Barcode version set
 */

-(NSString *) getBarcodeVersion;

/* !
 @ brief SetBeepScanLengthAndFrequency Configure barcode reader beep on remote device.
 
 @param freq    Frequency in Hz (between 1000 and 5110 Hz)
 @param length  Duration in ms (between 0 and 2550 ms)
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded
 false if the method failed.
 <li> Expected Result : Beep length and frequency set
 */

-(BOOL) setBeepScanLengthAndFrequency : (int)length freq:(int)frequency;


/* !
 @brief ResetBarcodeReader Barcode reader soft reset on remote device.
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded.
 false if the method failed.
 <li> Expected Result : Reset all barcode configurations
 */

-(BOOL) resetBarcode;

/* !
 @ brief SetLightningModeForBarcodeReader Configure barcode reader lighting mode on remote device.
 
 @param mode   illuminationLEDPriority = 0, aperturePriority = 1
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded
 false if the method failed.
 <li> Expected Result : Light mode set based on choice
 */

-(BOOL) setBarcodeLightningMode :(int) mode;


/* !
 @ brief GetFirmwareVersionForBarcodeReader - Get barcode reader firmware version.
 
 @param result  Result of the operation (0=OK, 1=KO)
 @return        true if the method succeeded
 false if the method failed.
 <li> Expected Result : Receive Barcode Firmware version number from the terminal
 */
-(NSString *) getBarcodeReaderFirmwareVersion;

@end

/*!
 @anchor     ICPclServiceDelegate
 @brief      The ICPclServiceDelegate method
 <p>
 These methods should be implemented by @ref ICPclServiceDelegate's delegate to subscribe to be notified of events.
 </p>
 */
@protocol ICPclServiceDelegate <NSObject>

@optional

/*!
 @anchor     notifyConnection
 @brief      This delegate method is called when the Terminal is just connected
 */
-(void)notifyConnection:(ICPclService *)sender;

/*!
 @anchor     notifyDisconnection
 @brief      This delegate method is called when the Terminal is just disconnected 
 */
-(void)notifyDisconnection:(ICPclService *)sender;

/*!
 @anchor pclLogEntrywithSeverity
 @brief      Log event handler to be implemented by the delegate of the @ref ICPclService object
 <p>
 The log messages can be filtered based on their severities. These are defined within the @ref SEVERITY_LOG_LEVELS enumeration and can be converted to a human readable form using the @ref severityLevelString and @ref severityLevelStringA methods of the @ref ICPclService class.
 </p>
 @param		 message The log message
 @param		 severity The severity of the log message
 */
-(void)pclLogEntry:(NSString*)message withSeverity:(int)severity;

/*!
	@brief       Serial data log event handler to be implemented by the delegate of the @ref ICAdministration object
	@param		 data The serial data incoming or outgoing on the administration channel
	@param		 isIncoming A boolean for the direction in/out of the serial data
 */
-(void)pclLogSerialData:(NSData*)data incoming:(BOOL)isIncoming;

/*!
	@anchor		shouldDoSignatureCapture
 @brief      This event is triggered when the iOS receives a Do Signature Capture request
 <p align="justify">
 Upon this event, the application should create a graphics context according to the dimension specified within the @ref ICSignatureData structure. The signature is then drawn on this context and should be submitted before the signature capture timeout is expired.<br />
 A helper class <b>ICSignatureView</b> is provided as a sample. This class derives from UIView and demonstrates how to use the UIView's cocoa touch events to draw a signature. This class may be used directly or replaced in case it does not fulfill the application developer's needs in terms of GUI design.
 </p>
	@param		signatureData A C-structure containing the parameters of the signature capture
 */
-(void)shouldDoSignatureCapture:(ICSignatureData)signatureData;

/*!
 @anchor     signatureTimeoutExceeded
	@brief      This event is triggered if the signature is not captured within the time limits specified within the @ref shouldDoSignatureCapture event's parameters
 <p>
 When this method is called, the application should abort performing the signature capture. Calling @ref submitSignatureWithImage to submit the signature will then have no effect and will be ignored by the PCL library.
 </p>
 */
-(void)signatureTimeoutExceeded;

/*!
 @anchor     transactionDidEnd
 @brief      This method is called on the delegate when the terminal is done processing a transaction started through
 either @ref doTransactionWithRequest, @ref doTransaction or @ref doTransactionEx
 @param      replyReceived YES if the terminal processed the transaction in time, NO if the timeout is reached.
 @param      transactionReply An ICTransactionReplyObject instance holding all the information about the transaction.
 */
-(void)transactionDidEndWithTimeoutFlag:(BOOL)replyReceived reply:(ICTransactionReplyObject *)transactionReply;

/*!
 @anchor     transactionDidEndResultAndData
 @brief      This method is called on the delegate when the terminal is done processing a transaction started through either @ref doTransactionWithRequest, @ref doTransaction or @ref doTransactionEx
 @param      replyReceived    YES if the Terminal processed the transaction in time, NO if the timeout of 60 seconds is reached.
 @param      transactionReply An ICTransactionReply structure holding all the information about the transaction.
 @param      extendedData     A NSData object containing additional data about the outcome of the transaction. This data is only present and not nil when the transaction was performed by calling @ref doTransactionExtended.
 */
-(void)transactionDidEndWithTimeoutFlag:(BOOL)replyReceived result:(ICTransactionReply)transactionReply andData:(NSData *)extendedData;

/*!
 @anchor     receiveMessage
 @brief      This event is triggered when the iDevice receives a message from the Terminal
 <p>The iDevice and the Terminal applications should agree beforehand on the messages to be exchanged and on how they will be used.</p>
 @param		 data An NSData containing the message
 */
-(void)receiveMessage:(NSData *)data;




/*!
 @anchor     handleBarcodeEvent
 @brief      This event is triggered when the iDevice receives a Barcode Event  from the Terminal
 <p> The iDevice and the Terminal applications should agree beforehand on the messages to be exchanged and on how they will be used.</p>
 @param		 data An NSData containing the message
 */
//-(void)handleBarcodeEvent : (NSData *)Symboloydata event:(NSData *)eventData;
-(void)handleBarcodeEvent : (NSArray *)dataObjects;

/*!
 @defgroup   ICAdministrationPrinting    Printing over the Administration Channel
 @ingroup    ICAdministrationGroup
 @brief      Print receipts composed of text and bitmaps from the Terminal on the iDevice applications.
 <p>
 The callbacks that provide the information to be printed are defined within the @ref ICPclServiceDelegate protocol. The @ref ICPclServiceDelegate property of the @ref ICPclService object must be set to a valid object that conforms to the mentioned protocol in order to be notified with the printing callbacks. Those provide the raw data and all the information needed to render the receipt.
 </p>
 @{
 */
-(void)handleBarcodeClosedEvent:(BOOL)data;


/*!
 @anchor     shouldPrintTextWithFontAlignmentScalingUnderlineBold
 @brief      This delegate method is called whenever a text string is available for printing
 <p>
 This callback is called when formatted text is to be printed. It provides information on how the text should be rendered. This involves the following attributes:
 <ul>
 <li>The text font - The size of the font is not set by the terminal. This means that the iOS application is free to choose the appropriate text size.</li>
 <li>The alignment of the text (left, right or center)</li>
 <li>The X scale ratio (this may be one of the following values: 1, 2 or 4)</li>
 <li>The Y scale ratio (may be one of the values: 1, 2 or 4)</li>
 <li>underlining</li>
 </ul>
 <br />
 </p>
 @param		text        A NSString containing the text to be printed
 @param		font        An UIFont object including the name of the font that should be used to render the text string
 @param		alignment   The alignment of the text
 @param      xFactor     The scaling factor to be applied to the text in the X direction
 @param      yFactor     The scaling factor to be applied to the text in the Y direction
 @param      underline   Specifies whether the text should be underlined or not
 @param      bold        Boolean that specifies whether the text should have a bold style
 */
-(void)shouldPrintText:(NSString *)text withFont:(UIFont *)font alignment:(NSTextAlignment)alignment XScaling:(NSInteger)xFactor YScaling:(NSInteger)yFactor underline:(BOOL)underline bold:(BOOL)bold;

/*!
 @anchor     shouldPrintRawTextWithCharsetWithFontAlignmentScalingUnderlineBold
 @brief      This delegate method is called whenever a text string is available for printing
 <p>
 This callback is called when formatted text is to be printed. It provides information on how the text should be rendered. This involves the following attributes:
 <ul>
 <li>The text font - The size of the font is not set by the terminal. This means that the iOS application is free to choose the appropriate text size.</li>
 <li>The text charset - The charset of the text to display. This means that the iOS application shall apply the corresponding charset.</li>
 <li>The alignment of the text (left, right or center)</li>
 <li>The X scale ratio (this may be one of the following values: 1, 2 or 4)</li>
 <li>The Y scale ratio (may be one of the values: 1, 2 or 4)</li>
 <li>underlining</li>
 </ul>
 <br />
 </p>
 @param		text        A char* containing the text to be printed
 @param     charset     The charset used by the text
 @param		font        An UIFont object including the name of the font that should be used to render the text string
 @param		alignment   The alignment of the text
 @param     xFactor     The scaling factor to be applied to the text in the X direction
 @param     yFactor     The scaling factor to be applied to the text in the Y direction
 @param     underline   Specifies whether the text should be underlined or not
 @param     bold        Boolean that specifies whether the text should have a bold style
 */
-(void)shouldPrintRawText:(char *)text withCharset:(NSInteger)charset withFont:(UIFont *)font alignment:(NSTextAlignment)alignment XScaling:(NSInteger)xFactor YScaling:(NSInteger)yFactor underline:(BOOL)underline bold:(BOOL)bold;


/*!
 @anchor      shouldPrintImage
	@brief       This delegate method is called whenever an image is available for printing
	@param		 image An UIImage object containing the image
 */
-(void)shouldPrintImage:(UIImage *)image;

/*!
 @anchor        shouldFeedPaperWithLines
 @brief         This delegate method is called whenever the Terminal needs to add empty lines when printing
 @param         lines The number of lines to add
 */
-(void)shouldFeedPaperWithLines:(NSUInteger)lines;

/*!
 @anchor     shouldCutPaper
 @brief      This delegate method is called whenever the Terminal needs to cut the paper
 */
-(void)shouldCutPaper;


/*!
 @anchor     shouldStartReceipt
 @brief      This delegate method is called whenever the Terminal needs to start printing a receipt
 @param      type The receipt type
 */
-(NSInteger)shouldStartReceipt:(NSInteger)type;

/*!
 @anchor     shouldEndReceipt
 @brief      This delegate method is called whenever the Terminal needs to end printing a receipt
 */
-(NSInteger)shouldEndReceipt;

/*!
 @anchor     shouldAddSignature
 @brief      This delegate method is called whenever the Terminal needs to add a signature.
 
 */
-(NSInteger)shouldAddSignature;



@end
/*!
 @}
 */
