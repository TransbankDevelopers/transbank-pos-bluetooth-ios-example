//
//  ICBarCodeReader.h
//  PCL Library
//
//  Created by Christophe Fontaine on 21/06/10.
//  Copyright 2010 Ingenico. All rights reserved.
//

/*!
    @file       ICBarCodeReader.h
    @brief      Header file for ICBarCodeReader class and related data structures and protocols
 */

#import <Foundation/Foundation.h>
#import "ICISMPDevice.h"

#define kICBarCodeReaderPclVersion @CURRENT_PROJECT_VERSION

/**
 * The `ICBarCodeReader`'s delegate methods.
 *
 * These methods should be implemented by the `[ICBarCodeReader delegate]` in order to be notified of its events.
 *
 * @note Only `[ICBarCodeReaderDelegate barcodeData]` method is required. You should also implement `[ICBarcodeReaderDelegate unsuccessfulDecode]` method to alert the user when the scanner fails to decode a barcode.
 */
@protocol ICBarCodeReaderDelegate
@required

/**
 * This method is triggered each time the scanner successfully decodes a barcode.
 *
 * @note The data objects will contain ONLY the successfully decoded barcodes.
 *
 * @param data An object of type `NSString` if the scanner is configured to read barcodes or of type `UIImage` if the scanner is used in snapshot mode.
 * @param type The type of the decoded barcode. This is a value of the constants defined within `eICBarCodeSymbologies`.
 */
-(void)barcodeData:(id)data ofType:(int)type;

/**
 * This method is triggered when the scanner must be configured.
 *
 * @note When the channel is opened (after the powerOn method or a soft reset), the scanner sends the power up event and requires to be configured.
 */
-(void)configurationRequest;

@optional

/**
 * This method is triggered when a barcode is read but the decoding fail.
 */
-(void)unsuccessfulDecode;

/**
 * This method is triggered when the hardware line of the trigger is pressed.
 */
-(void)triggerPulled;

/**
 * This method is triggered when the hardware line of the trigger is released.
 */
-(void)triggerReleased;

/**
 * This method is trigger when other events are coming from the barcode reader of the Ingenico's device.
 *
 * These events have not been defined yet, they include:
 *  - Decoding events: unsuccessful decoding, start of read session, end of read session
 *  - Hardware events: start-up, setup modification by reading a configuration barcode, configuration barcode rejected
 *
 * @param eventCode An event code constant [NOT USED]
 */
-(void)onScanMiscEvent:(int)eventCode;

/**
 * Method to be implemented by the delegate of the `ICBarCodeReader` in order to receive all non-serial log messages.
 *
 * @note Log messages can be filered based on their severity.
 *
 * @param logEntry The log message.
 * @param severity The severity of the log message.
 */
- (void)barcodeLogEntry:(NSString *)logEntry withSeverity:(int)severity;

/**
 * Method to be implemented by the delegate of `ICBarCodeReader` in order to receive the serial data passing through the barcode reader channel.
 *
 * @param data The serial data ingoing or outgoing.
 * @param isIncoming YES if the data comes from the Ingenico's device or NO otherwise.
 */
- (void)barcodeSerialData:(NSData *)data incoming:(BOOL)isIncoming;

@end

/**
 * The Ingenico's device's barcode reader channel management class.
 *
 * This class handles all the configuration and events of the device's barcode scanner.
 */
@interface ICBarCodeReader : ICISMPDevice {
#pragma mark - Protected attributes

    /**
     * List of the scanned barcodes in scan mode *multiScan*.
     */
	NSMutableArray					*multiCodeStrings;

    /**
     * Condition for received result.
     */
    NSCondition						*iscpResultReceived;

    /**
     * Condition for reply result.
     */
	NSCondition						*iscpReplyReceived;

    /**
     * Condition for received startup event from barcode reader.
     */
	NSCondition						*scannerStartedCondition;

    /**
     * ISCP last result data.
     */
	NSData							*lastResultData;

    /**
     * Operation queue.
     */
	NSOperationQueue				*operationQueue;

    /**
     * Command counter.
     */
	int								bufferingSetupWrites;

    /**
     * Buffer of ISCP commands.
     */
	NSMutableArray					*setupWriteList;
	
@private
    /// :nodoc:
	NSInteger						currentScanMode;

    /// :nodoc:
	NSInteger						multiScan;

    /// :nodoc:
	NSInteger						triggerEnabled;

    /// :nodoc:
	NSInteger						turboModeEnabled;

    /// :nodoc:
	NSInteger						currentImagerMode;

    /// :nodoc:
	NSString						*firmwareVersion;

    /// :nodoc:
    BOOL                            shouldRequestConfiguration;

    /// :nodoc:
    NSMutableData                   *picture;

    /// :nodoc:
    NSInteger                       sizeOfPicture;
}

/**
 * An enumeration of the symbologies supported by the Ingenico's device's barcode scanner.
 *
 * These constants are used by the `[ICBarCodeReader enableSymboly]`, `[ICBarCodeReader enableSymbologies]`, `[ICBarCodeReader isSymbologyEnabled]` and `[ICBarCodeReader isSymbologySupported]` methods.
 */
enum eICBarCodeSymbologies {

    /// Unknown Symbology.
    ICBarCode_Unknown               = -1,

    /// All Symbologies.
    ICBarCode_AllSymbologies        = 0,

    /// EAN13 Barcode Type.
    ICBarCode_EAN13                 = 1,
    /// EAN8 Barcode Type.
    ICBarCode_EAN8                  = 2,
    /// UPCA Barcode Type.
    ICBarCode_UPCA                  = 3,
    /// UPCE Barcode Type.
    ICBarCode_UPCE                  = 4,

    /// EAN13_2 Barcode Type.
    ICBarCode_EAN13_2               = 5,
    /// EAN8_2 Barcode Type.
    ICBarCode_EAN8_2                = 6,
    /// UPCA_2 Barcode Type.
    ICBarCode_UPCA_2                = 7,
    /// UPCE_2 Barcode Type.
    ICBarCode_UPCE_2                = 8,

    /// EAN13_5 Barcode Type.
    ICBarCode_EAN13_5               = 9,
    /// EAN8_5 Barcode Type.
    ICBarCode_EAN8_5                = 10,
    /// UPCA_5 Barcode Type.
    ICBarCode_UPCA_5                = 11,
    /// UPCE_5 Barcode Type.
    ICBarCode_UPCE_5                = 12,

    /// Code39 Barcode Type.
    ICBarCode_Code39                = 13,

    /// Interleaved2of5 Barcode Type.
    ICBarCode_Interleaved2of5       = 15,
    /// Standard2of5 Barcode Type.
    ICBarCode_Standard2of5          = 16,
    /// Matrix2of5 Barcode Type.
    ICBarCode_Matrix2of5            = 17,

    /// CodeBar Barcode Type.
    ICBarCode_CodaBar               = 19,
    /// MSI Barcode Type.
    ICBarCode_MSI                   = 21,
    /// Pleassey Barcode Type.
    ICBarCode_Plessey               = 22,

    /// Code128 Barcode Type.
    ICBarCode_Code128               = 23,
    /// Code93 Barcode Type.
    ICBarCode_93                    = 25,
    /// Code11 Barcode Type.
    ICBarCode_11                    = 26,

    /// Telepen Barcode Type.
    ICBarCode_Telepen               = 27,
    /// Code39_ItalianCPI Barcode Type.
    ICBarCode_Code39_ItalianCPI     = 29,

    /// CodeBlockA Barcode Type.
    ICBarCode_CodaBlockA            = 30,
    /// CodaBlockF Barcode Type.
    ICBarCode_CodaBlockF            = 31,

    /// PDF417 Barcode Type.
    ICBarCode_PDF417                = 33,
    /**
     * GS1_128 Barcode Type.
     *
     * Used to replace EAN128.
     */
    ICBarCode_GS1_128               = 34,
    /// ISBT128 Barcode Type.
    ICBarCode_ISBT128               = 35,
    /// MicroPDF Barcode Type.
    ICBarCode_MicroPDF              = 36,

    /// GS1_DataBarOmni Barcode Type.
    ICBarCode_GS1_DataBarOmni       = 37,
    /// GS1_DataBarLimited Barcode Type.
    ICBarCode_GS1_DataBarLimited    = 38,
    /// GS1_DataBarExpanded Barcode Type.
    ICBarCode_GS1_DataBarExpanded   = 39,

    /// DataMatrix Barcode Type.
    ICBarCode_DataMatrix            = 40,
    /// QRCode Barcode Type.
    ICBarCode_QRCode                = 41,
    /// Maxicode Barcode Type.
    ICBarCode_Maxicode              = 42,
    /// UPC-E1 Barcode Type.
    ICBarCode_UPCE1                 = 43,

    /// Aztec Barcode Type.
    ICBarCode_Aztec                 = 74,

    /// MaxIndex Barcode Type.
    ICBarCode_MaxIndex
};

/**
 * An enumeration of the available illumination modes.
 */
enum eICBarCode_IlluminationMode {
    /// Enable both the aimer and the leds.
    aimerAndIlluminiationLeds,
    /// Enable only the aimer.
    aimerOnly,
    /// Enable only the leds.
    illuminationLedsOnly,
    /// Disable illumination.
    noIllumination
};

/**
 * An enumeration of the scan modes.
 *
 * These constants are used by the `[ICBarCodeReader configureBarCodeReaderMode]` method.
 */
enum eICBarCode_ScanMode {
    /// Single scan mode.
    ICBarCodeScanMode_SingleScan,
    /// Multi scan mode.
    ICBarCodeScanMode_MultiScan
};

/**
 * An enumeration of the predefined imager mode.
 *
 * These constants are used by the `[ICBarCodeReader configureImagerMode]` method.
 */
enum eICBarCode_ImagerMode {
    /// 1D.
    ICBarCodeImagerMode_1D,
    /// 1D and 2D standard.
    ICBarCodeImagerMode_1D2D,
    /// 1D and 2D bright environment.
    ICBarCodeImagerMode_1D2D_bright,
    /// 1D and 2D reflective surface.
    ICBarCodeImagerMode_1D2D_reflective
};

/**
 * An enumeration of the available aimer states.
 *
 * These constants are used by the `[ICBarCodeReader aimerState]` method.
 */
enum eICBarCode_AimerState {
    /// Enable the aimer
    aimerOn,
    /// Enable the aimer with flashing mode.
    aimerFlashing,
    /// Disable the aimer.
    aimerOff
};

/**
 * An enumeration of the Ingenico's device's barcode scanner's lighting modes.
 *
 * These constants are used by the `[ICBarCodeReader lightingMode]` method.
 */
enum eICBarCode_LightingMode {
    /// Shorter exposure time.
    illuminiationLEDPriority,
    /// Use aperture priority if you have a shiny barcode label.
    aperturePriority
};

/**
 * An enumeration of the Ingenico's device power on status.
 *
 * These constants are used by the `[ICBarCodeReader powerOn]` method.
 */
enum eICBarCode_PowerOnStatus {
    /// The powerOn command was successful.
    ICBarCodeReader_PowerOnSuccess,

    /// The powerOn command failed due to a synchronization problem.
    ICBarCodeReader_PowerOnFailed,

    /// The powerOn command was forbidden. This happens when the device is charging on the craddle.
    ICBarCodeReader_PowerOnDenied
};

#pragma mark - Properties

/**
 * The delegate object which should implement the `ICBarCodeReaderDelegate` protocol.
 *
 * @note By implementing the protocol, the delegate is notified with `ICBarCodeReader`'s events.
 */
@property(nonatomic, weak) id<ICISMPDeviceDelegate, ICBarCodeReaderDelegate> delegate;

/**
 * Defines the number of barcode frame resent.
 *
 * - timeout of ACK frame is 2s
 * - default value is DEFAULT_ISCP_RETRY_COUNT (5)
 */
@property(nonatomic, assign) int iscpRetryCount;

#pragma mark - Class methods

/**
 * Returns the unique shared instance of `ICBarCodeReader`.
 *
 * The scanner is not started when initialized an `ICBarCodeReader` object. This can be done by explicitely calling `[ICBarCodeReader powerOn]` method which is responsible for opening the barcode channel to the connected Ingenico's device. The is closed when calling `[ICBarCodeReader powerOff]` or when the object is released.
 *
 * Scanner state, whether it is reader or not, can be determined at any time by examing the value of `[ICBarCodeReader isAvailable]` property. If the scanner is not available, all the other calls to control or configuration commands will fail. Therefore, the application should always check the `[ICBarCodeReader isAvailable]` property before sending any command.
 *
 * @note The object returned by this method is autoreleased. It is then necessary to retain it.
 *
 * @return A pointer to the shared instance.
 */
+(ICBarCodeReader *)sharedICBarCodeReader;

/**
 * Test whether a symbology is supported or not.
 *
 * @param type The symbology to test. One of the constant value of `eICBarCodeSymbologies`.
 *
 * @return Returns YES if the symbology is supported or NO otherwise.
 */
+(BOOL)isSymbologySupported:(int)type;

/**
 * Converts a symbology code to its text representation.
 *
 * @param type An integer value corresponding to `eICBarCodeSymbologies` enumeration.
 *
 * @return An NSString containing the name of the symbology.
 */
+(NSString*)symbologyToText:(int)type;

#pragma mark - Instance methods

/**
 * Start a decoding session.
 *
 * @note The decoding session will remain active until a successful decode, a timeout or when you call `[ICBarCodeReader stopScan]`.
 */
-(void)startScan;

/**
 * Stop a decoding session.
 *
 * @note This method cancels the decoding session before reaching the timeout.
 *
 * @seeAlso [ICBarCodeReader setScanTimeout]
 */
-(void)stopScan;

/**
 * Start a snapshot image.
 *
 * @warning This method is not implemented.
 */
-(void)startSnapshot;

/**
 * Disable the setup write message and buffer the configuration instead.
 *
 * @warning The application MUST call `[ICBarCodeReader unbufferSetupCommands]` when it wishes to send the whole configuration.
 */
-(void)bufferWriteCommands;

/**
 * Enable the setup write message and send ALL buffered setup command in one message.
 *
 * @note A call to this method without a previous call to `[ICBarCodeReader bufferWriteCommands]` has no effet.
 */
-(void)unbufferSetupCommands;

/**
 * To switch from single scan, multi scan or raw mode.
 *
 * @note In single scan mode, the delegate will be callend when a barcode is read. In multi scan mode, the scanner will decode barcodes until the trigger is released or `[ICBarCodeReader stopScan]` is called.
 *
 * @param mode The barcode reader mode to activate. One of the constants value of `eICBarCode_ScanMode`.
 */
-(void)configureBarCodeReaderMode:(int)mode;

/**
 * Get the current barcode reader mode.
 *
 * @seeAlso `[ICBarCodeReader configureBarCodeReaderMode]`
 *
 * @return An integer of the `eICBarCode_ScanMode` constants.
 */
-(int)getBarCodeReaderMode;

/**
 * Enable a list of symbologies.
 *
 * @note The array must constains NSInteger corresponding to the symbologies you wish to enable (`eICBarCodeSymbologies`). Set symbologies to NULL or defined count to 0 to disable all symbologies.
 *
 * @param symbologies An array of integer value of scanner barcode types defined within `eICBarCodeSymbologies` enumeration.
 * @param count The total number of barcode types contained within the symbologies array.
 */
-(void)enableSymbologies:(int *)symbologies symbologyCount:(int)count; // symbologies: int[] containing eICBarCodeSymbologies

/**
 * Enable or disable a symbology.
 *
 * @param type The symbology to enable / disable. Refer to `eICBarCodeSymbologies` enumeration for all possible values.
 * @param enabled The new state to apply to the symbology. YES for enable and NO for disable.
 */
-(void)enableSymbology:(int)type enabled:(BOOL)enabled;

/**
 * Determine if the symbology is enable.
 *
 * @note This method is synchronous and may be time-consuming in case where the scanner is busy doing other tasks. It should be better to perform this method in a background thread.
 *
 * @param type The symbology which you want to know the state. Refer to `eICBarCodeSymbologies` enumeration for all possible values.
 *
 * @return Returns YES if the symbology is enabled or NO otherwise.
 */
-(BOOL)isSymbologyEnabled:(int)type;

/**
 * Enable transmission of UPC-A barcodes as EAN-13 format.
 *
 * EAN-13 is an extended form of UPC-A and adds a country code. The Ingenico's device barcode scanner can be configured using this method to convert and transmit UPC-A barcodes as EAN-13 format.
 *
 * @note By default, the Ingenico's device barcode scanner is configured to not transmit UPC-A barcodes as EAN-13 format.
 *
 * @param enabled YES to enable transmission of UPC-A barcodes as EAN-13 format or NO otherwise.
 */
-(void)enableTransmitUPCABarcodesAsEAN13:(BOOL)enabled;

/**
 * Enable transmission of UPC-E barcodes as UPC-A format.
 *
 * This method allows decoded UPC-E barcodes to be converted and transmitted as UPC-A.
 *
 * UPC-E is a variation of UPC-A which allows for a more compact barcode by eliminating "extra" zeros. Since the resulting UPC-E barcode is about half the size of an UPC-A barcode, UPC-E is generally used on products with very small packaging where a full UPC-A barcode couldn't reasonably fit.
 *
 * @param enabled YES to enable transmission of UPC-E barcodes as UPC-A format or NO otherwise.
 */
-(void)enableTransmitUPCEBarcodesAsUPCA:(BOOL)enabled;

/**
 * Enable transmission of EAN-8 barcodes as EAN-13 format.
 *
 * This method allows decoded EAN-8 to be converted and transmistted as EAN-13.
 *
 * EAN-8 is the EAN equivalent of UPC-E in the sense that it provides a "short" barcode for small packages. An EAN-8 barcode is shorter than an EAN-13 barcode, although somewhat longer than an UPC-E barcode.
 *
 * @param enabled YES to enable transmission of EAN-8 barcodes as EAN-13 format or NO otherwise.
 */
-(void)enableTransmitEAN8BarcodesAsEAN13:(BOOL)enabled;

/**
 * Select a predefined imager mode.
 *
 * @param mode The barcode reader imager mode to activate. One of the constants value of `eICBarCode_ImagerMode`.
 */
-(void)configureImagerMode:(int)mode;

/**
 * Enable or disable the flashing of the aimer.
 *
 * @param enabled YES to enable the flashing of the aimer or NO otherwise.
 */
-(void)enableAimerFlashing:(BOOL)enabled;

/**
 * Configures the barcode reader aimer state.
 *
 * @param state The barcode reader aimer state to configure. One of the constants value of `eICBarCode_AimerState`.
 */
-(void)aimerState:(int)state;

/**
 * Configure the barcode reader illumination mode.
 *
 * @param mode The barcode reader illumination mode to configure. One of the constants value of `eICBarCode_IlluminationMode`.
 */
-(void)illuminationMode:(int)mode __deprecated_msg("Use -(void)aimerState:(int)state and -(void)illuminationLevel:(int)level instead.");

/**
 * Defines the maximum brightness of the barcode reader.
 *
 * Set the maximum brightness level of the illumination LEDs which can be used to achieve the lightning goal. The level must be between 0% (off) and 100% (brightest).
 *
 * @note Set the illumination level on a lower setting when reading shiny barcode labels where high intensity lightning will be too bright to decode the label.
 *
 * @param level An integer value in between 0 and 100.
 */
-(void)illuminationLevel:(int)level;

/**
 * Configure the average light intensity.
 *
 * The lighting goal is the average light intensity which the scanner tries to achieve when capturing an image.
 * If the lighting goal is set too low, the image will be dark (underexposed).
 * If it is set too high, the image will be too bright (overexposed).
 * Use the lighting mode setting to determine how the scanner tries to achieve the lighting goal.
 *
 * @param goal An integer value between 0 and 255.
 */
-(void)lightingGoal:(int)goal;

/**
 * Configure the lighting goal mode.
 *
 * The lighting goal mode determines how the scanner tries to achieve the lighting goal. Priority can be given to the illumination LEDs or to aperture (exposure time).
 * Using the illumination LED priority setting will result in a shorter exposure time, therefore less risk of bluerred images.
 * Use the aperture priority setting when you have a shiny barcode label where high intensity lighting will be too bright to decode it.
 *
 * @param priorityType The barcode reader lighting goal mode to configure. One of the constants value of `eICBarCode_LightingMode`.
 */
-(void)lightingMode:(int)priorityType;

/**
 * Returns the barcode reader aimer flashing state.
 *
 * @return YES if the aimer flashing is enable or NO otherwise.
 */
-(BOOL)aimerFlashing;

/**
 * Return the barcode reader aimer state.
 *
 * @return An integer value corresponding to the `eICBarCode_AimerState` enumeration.
 */
-(int)aimerState;

/**
 * Retrieve the current illumination mode of the barcode scanner.
 *
 * @return An integer value corresponding to the `eICBarCode_IlluminationMode` enumeration.
 */
-(int)illuminationMode __deprecated_msg("Use -(int)aimerState and -(int)illuminationLevel instead.");

/**
 * Retrieve the current illumination level of the barcode scanner.
 *
 * @return An integer value between 0 and 100 representing the illumination level of the barcode reader's light.
 */
-(int)illuminationLevel;

/**
 * Retrieve the current lighting goal of the barcode scanner.
 *
 * @return An integer value between 0 and 255 representing the lighting goal of the barcoder reader.
 */
-(int)lightingGoal;

/**
 * Retrieve the lighting mode of the barcode scanner.
 *
 * @return An integer value corresponding to the `eICBarCode_LightingMode` enumeration.
 */
-(int)lightingMode;

/**
 * Enable or disable the Damaged 1D Codes options.
 *
 * Enable this setting when scanning a damaged or badly printed 1D barcodes. This settings enhances the ability to read these types of barcodes.
 *
 * @param enabled YES to enable the Damaged 1D Codes setting or NO otherwise.
 */
-(void)useEnhancedBCRSensitivity:(BOOL)enabled;

/**
 * Returns if the Damaged 1D Codes setting is enable or disable.
 *
 * @return YES if the Damaged 1D Codes setting is enabled or NO otherwise.
 */
-(BOOL)isEnhancedBCRSensitivityEnabled;

/**
 * Power on the Ingenico's device's barcode scanner.
 *
 * This method powers on the Ingenico's device's barcode scanner. If may take up to 1 second for this method to return.
 * After the scanner is started, the `[ICBarCodeReader configurationRequest]` callback is called to invite the application's user to apply its barcode scanner configuration (set single or multi-scanning, choose the barcode symbologies, choose the imager mode 1D/2D, ...).
 *
 * @warning The power on request is not always successful. It may fail if there is any synchronization problem or when the Ingenico's device is being charged on the craddle.
 *
 * @return An integer value corresponding to the `eICBarCode_PowerOnStatus` enumeration.
 */
-(int)powerOn;

/**
 * Power off the Ingenico's device's barcode scanner.
 *
 * @note The barcode scanner should be turned off when not in use to save the device's battery life.
 */
-(void)powerOff;

/**
 * Perform a soft reset of the barcode scanner.
 *
 * @note The soft reset reinitializes the scanner parameters.
 */
-(void)softReset;

/**
 * Apply the default barcode scanner's configuration.
 *
 * @note This method reinitializes the scanner parameters to the default configuration.
 */
-(void)applyDefaultConfiguration;

/**
 * Configure the delay after which the barcode scanner will abort the barcode decoding.
 *
 * @param timeout The timeout to apply between 0 (timeout disable) to 60 seconds.
 */
-(void)setScanTimeout:(int)timeout;

/**
 * Retrieve the barcode reader's scan timeout.
 *
 * It returns the timeout value in seconds. The returned value is between 0 (timeout disable) and 60 seconds.
 *
 * @note This method is synchronous and blocking.
 *
 * @return An integer value between 0 and 60 or -1 if an error occured.
 */
-(int)getScanTimeout;

/**
 * Enable or disable non volatile mode.
 *
 * @note When using non volatile mode, parameters are restored when the barcode reader is opened.
 *
 * @param enable YES to enable non volatile mode or NO otherwise.
 */
-(void)setNonVolatileMode:(BOOL)enabled;

/**
 * Enable or disable beep on scan.
 *
 * Use this method if you wish to enable or disable the beep when a barcode is successfully read by the barcode scanner.
 *
 * @param enable YES to enable the beep on scan or NO otherwise.
 */
-(void)goodScanBeepEnable:(BOOL)enabled;

/**
 * Configure the beep on scan.
 *
 * This method changes the scan and error beep frequency and length.
 *
 * @param enabled YES to enable the beep sound or NO otherwise.
 * @param frequency An integer between 1000 to 4095 kHz.
 * @param length An integer value representing the beep duration in millisecond.
 */
-(void)setBeep:(BOOL)enabled frequency:(int)frequency andLength:(int)length;

/**
 * Play the beep at the chosen frequency for a duration and then wait for a duration.
 *
 * @note This is a non-bloking method. Call it sequencially in order to play a sequence of "beeps".
 *
 * @param frequency An integer between 1000 to 4095 kHz.
 * @param onTimeMs The duration in milliseconds of the beep.
 * @param offTimeMs The duration to wait in milliseconds (between 0 and 4095ms) after the beep.
 */
-(void)playBeep:(int)frequency during:(int)onTimeMs andWait:(int)offTimeMs;

/**
 * Enable or disable the trigger's buttons of the Ingenico's device's.
 *
 * @param YES to enable the trigger's buttons or NO otherwise.
 *
 * @return YES if the setting has been set successfully or NO otherwise.
 */
-(BOOL)enableTrigger:(BOOL)enabled;

/**
 * Return if the trigger's buttons are enable or disable on the Ingenico's device.
 *
 * @warning This is a synchronous and blocking method. It should be better to run it in background since it may be time-consuming.
 *
 * @return YES if the trigger's buttons are enable or NO otherwise.
 */
-(BOOL)isTriggerEnabled;

/**
 * Retrieve the barcode reader's firmware version.
 *
 * @warning This method is synchronous and may take some time before it gets the result. It should be run in a separate thread in order to not to freeze the main thread.
 *
 * @return A NSString containing the version string of the barcode reader's firmware.
 */
-(NSString *)getFirmwareVersion; // returns the barcode firmware version

/**
 * Retrieve the barcode reader's name.
 *
 * @return A NSString containing the barcode reader's name.
 */
-(NSString *)scannerName;

/**
 * Retrieve the barcode reader's model.
 *
 * @return A NSString containing the barcode reader's model.
 */
-(NSString *)scannerModel;

@end
