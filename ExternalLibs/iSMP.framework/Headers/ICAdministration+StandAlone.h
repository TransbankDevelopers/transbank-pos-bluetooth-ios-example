//
//  ICAdministration+StandAlone.h
//  PCL Library
//
//  Created by Christophe Fontaine on 23/02/2011.
//  Copyright 2010 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ICISMPDevice.h"
#import "ICAdministration.h"

@class ICTransactionRequestObject, ICTransactionReplyObject;

/**
 * Transaction request structure in Igenico's device standalone payment mode.
 *
 * This is the input data structure for `[ICAdministrationStandAlone doTransaction]`. The application should fill all the necessary fields before issuing the transaction request.
 */
struct _ICTransactionRequest {
    /// The POS number (should be within the range [0 - 255]).
	unsigned short	posNumber;

    /// The amount for the transaction (left completed with '0'). If the ISO2 track needs to be read, amount should be '0'.
	char			amount[8];

    /// No longer to be used.
    unsigned char	specificField;

    /// The kind of payment the POS wishes to use `ICTransactionAccountType`.
	unsigned char	accountType;

    /// The type of transaction `ICTransactionType`.
	unsigned char	transactionType;

    /// The currency code in ISO4217 format.
	char			currency[3];

    /// Application specific data to be passed to payment application.
	char			privateData[10];

	/// Deprecated and ignored.
	unsigned char	delay __deprecated;

    /// The authorization that the POS asks the Ingenico's device for `ICTransactionAuthorization`.
	unsigned char	authorization;
};

/**
 * Transaction request structure in Igenico's device standalone payment mode.
 *
 * @see `_ICTransactionRequest`
 */
typedef struct _ICTransactionRequest ICTransactionRequest;

/**
 * The output data structure of `[ICAdministrationStandAlone doTransaction]`.
 *
 * This structure is the result of the call to `[ICAdministrationStandAlone doTransaction]`. It is returned witing `ICAdministrationStandAloneDelegate` method `[ICAdministrationStandAloneDelegate transactionDidEnd]`.
 * The application should analyze the content of the structure in order to know whether the transaction succeeded or failed.
 */
struct _ICTransactionReply {
    /// The POS number (should be within the range [0 - 255]).
	unsigned short	posNumber;

    /// The status of the payment process.
	unsigned char	operationStatus;

    /// The real amount used for the transaction.
	char			amount[8];

    /// The account type used for the transaction `ICTransactionAccountType`.
	unsigned char	accountType;

    /// The currency code in ISO4217 format (the same as in the transaction request).
	char			currency[3];

    /// Application specific data to be passed to POS application.
	char			privateData[10];

    /// No longer to be used.
	char			PAN[19] __deprecated_msg("No longer to be used.");

    /// No longer to be used.
	char			cardValidity[4] __deprecated_msg("No longer to be used.");

    /// The authorization number.
	char			authorizationNumber[9];

    /// No longer to be used.
	char			CMC7[35] __deprecated_msg("No longer to be used.");

    /// No longer to be used.
	char			ISO2[38] __deprecated_msg("No longer to be used.");

    /// No longer to be used.
	char			FNCI[10] __deprecated_msg("No longer to be used.");

    /// No longer to be used.
	char			guarantor[10] __deprecated_msg("No longer to be used.");

    /// Response of the cash register connection.
    char            zoneRep[55];

    /// Private area.
    char            zonePriv[10];
};

/**
 * The output data structure of `[ICAdministrationStandAlone doTransaction]`.
 *
 * @see `_ICTransactionReply`
 */
typedef struct _ICTransactionReply ICTransactionReply;

/**
 * This structure is given to the iOS device's application within the `[ICAdministrationStandAloneDelegate shouldDoSignatureCapture]`.
 */
struct _ICSignatureData {
    /// The X position of the screen.
	NSUInteger		screenX;

    /// The Y position of the screen.
	NSUInteger		screenY;

    /// The width of the capture screen.
	NSUInteger		screenWidth;

    /// The height of the capture screen.
	NSUInteger		screenHeight;

    /// The timeout for the signature to be captured and sent to the Ingenico's device.
	NSUInteger		userSignTimeout;
};

/**
 * The signature's data.
 *
 * @see `_ICSignatureData`
 */
typedef struct _ICSignatureData ICSignatureData;


@protocol ICAdministrationDelegate;

/**
 *
 */
@interface ICAdministration (StandAlone)

/**
 * Starts a transaction in standalone payment mode.
 *
 * In standalone payment mode, the Ingenico's device communicates directly with the bank's server without having any intermediate application server.
 * The app should setup the `ICPPP`channel before issueing the transaction in order to ensure the Ingenico's device has access to the network.
 *
 * This method is an asynchronous method which returns immediately. As such the result is not returned yet, but given back to the delegate, through the method `[ICAdministrationStandAloneDelegate transactionDidEnd]` declared in `ICPclServiceDelegate` protocol.
 *
 * The timeout delay is configurable with `[ICAdministrationStandAlone setDoTransactionTimeout]`. After that delay, the transaction is deemed to have failed.
 *
 * Since this method is asynchronous, communication can take place between the Ingenico's device and the application during that time. This makes it possible for example to exchange some messages with an application on the Ingenico's device or to handle a signature capture request.
 *
 * This method is meant to replace both `[ICAdministrationStandAlone doTransaction]` and `[ICAdministrationStandAlone doTransaction:withData:andApplicationNumber:]`.
 *
 * @param request An `ICTransactionRequest` object describing the parameters of the transaction.
 */
-(void)doTransactionWithRequest:(ICTransactionRequestObject *)request;

/**
 * Starts a transaction in standalone payment mode.
 *
 * In standalone payment mode, the Ingenico's device communicates directly with the bank's server without having any intermediate application server.
 * The app should setup the `ICPPP`channel before issueing the transaction in order to ensure the Ingenico's device has access to the network.
 *
 * This method is asynchronous method which returns immediately. The application should conform to `ICAdministrationStandAloneDelegate` protocol and implement `[ICAdministrationStandAloneDelegate transactionDidEnd]` to be notified when the transaction is finished.
 *
 * The maximum allowed time for the transaction to be performed is 60 seconds, after which the transaction is deemed to have failed.
 *
 * Since this method is asynchronous, communication can take place between the Ingenico's device and the application during that time. This makes it possible for example to exchange some messages with an application on the Ingenico's device or to handle a signature capture request.
 *
* @param request An `ICTransactionRequest` object describing the parameters of the transaction.
 */
-(void)doTransaction:(ICTransactionRequest)request;

/**
 * Start a transaction in standalone payment mode with additional parameters.
 *
 * This methods acts exactly as `[ICAdministrationStandAlone doTransaction:]`. The only difference is that it comes with additional parameters to customize the transaction by passing extra data structurs and specifying the Ingenico's device's application which should be invoked.
 *
 * This method is also asynchronous and returns immediately. The result is provided to the iOS application through the method `[ICAdministrationStandAloneDelegate transactionDidEnd:]`.
 *
 * @param request An `ICTransactionRequest` object describing the parameters of the transaction.
 * @param extendedData A NSData object which contains any additional transaction data to be passed to the Ingenico's device. The maximum data lendth which can be passed through this parameter is 16kB.
 * @param appNum The number of the Ingenico's device's application which should be invoked to handle the transaction. If this parameter is set to 0, the payment application will be implicitely selected.
 */
-(void)doTransaction:(ICTransactionRequest)request withData:(NSData *)extendedData andApplicationNumber:(NSUInteger)appNum;

/**
 * Configure the transaction timeout setting.
 *
 * @param timeout The new timeout to set in milliseconds.
 */
-(void)setDoTransactionTimeout:(NSUInteger)timeout __deprecated;

/**
 * Retrieve the transaction timeout setting.
 *
 * @return The timeout value in milliseconds.
 */
-(NSUInteger)getDoTransactionTimeout __deprecated;

/**
 * Submit the signature on the iOS screen to the Ingenico's device.
 *
 * This method should be called within the `[ICAdministrationStandAloneDelegate shouldDoSignatureCapture]` event before the signature timeout expires, otherwise it will be ignored.
 *
 * @param image An UIImage object containing the signature to submit. The image must be in black and white, and have any iOS supported bitmap configuration.
 *
 * @result YES if the signature was successfully submitted to the Ingenico's device or NO otherwise.
 */
-(BOOL)submitSignatureWithImage:(UIImage *)image;

/**
 * Send a message to the Ingenico's device.
 *
 * The undercover messaging makes it possible to wrap any third-party protocol with the protocol used on the `ICAdministration` channel.
 * Before using the undercover messaging, the iOS application and the Ingenico's device's application must agree on the protocol or the messages which will be exchanged.
 * Refer to `[ICAdministrationStandAloneDelegate messageReceivedWithData:] to see how to receive messages from the Ingenico's device.
 *
 * @param data An NSData object containing the message to be sent. The maximul allowed data length is 1kB.
 *
 * @return YES if the message was received by the Ingenico's device or NO otherwise.
 */
-(BOOL)sendMessage:(NSData *)data;

@end

/**
 * The `ICAdministration`'s stand alone delegate methods.
 *
 * The methods should be implemented by the `[ICAdministration delegate]` in order to be notified of its events.
 */
@protocol ICAdministrationStandAloneDelegate <NSObject, ICAdministrationDelegate>
@optional

/**
 * Called when the Ingenico's device has finished processing a transaction started through either `[ICAdministrationStandAlone doTransactionWithRequest]`, `[ICAdministrationStandAlone doTransaction]` or `[ICAdministrationStandAlone doTransactionEx]`.
 *
 * @param replyReceived YES if the Ingenico's device processed the transaction in time or NO if the timeout is reached.
 * @param transactionReply An `ICTransactionReply` object containing all the information about the transaction.
 */
-(void)transactionDidEndWithTimeoutFlag:(BOOL)replyReceived reply:(ICTransactionReplyObject *)transactionReply;

/**
 * Called when the Ingenico's device has finished processing a transaction started either `[ICAdministrationStandAlone doTransactionWithRequest]`, `[ICAdministrationStandAlone doTransaction]` or `[ICAdministrationStandAlone doTransactionEx]`.
 *
 * @param replyReceived YES if the Ingenico's device processed the transaction in time or NO if the timeout is reached.
 * @param transactionReply An `ICTransactionReply` object containing all the information about the transaction.
 * @param extendedData A NSData object containing additional data about the outcome of the transaction. This data is only present and not nil when the transaction was performed by calling `[ICAdministrationStandAlone doTransactionEx]` method.
 */
-(void)transactionDidEndWithTimeoutFlag:(BOOL)replyReceived result:(ICTransactionReply)transactionReply andData:(NSData *)extendedData;

/**
 * Called when the Ingenico's device ask for a signature capture in the iOS application.
 *
 * Upon this event, the application should create a graphics context according to the dimension specified within the `ICSignatureData` structure. The signature is then draws on this context and should be submitted before the signature capture timeout is reached.
 *
 * A helper class *ICSignatureView* is provided as a sample. This class derives from UIView and demonstrates how to use the UIView's Cocoa touch events to draw a signature. This class may be used directly or replace in case it does not fulfill the application developer's needs in terms of UI / UX requirements.
 *
 * @param signatureData An `ICSignatureData` C-structure containing the parameters for the signature capture.
 */
-(void)shouldDoSignatureCapture:(ICSignatureData)signatureData;

/**
 * Called when the signature is not captured within the time limits specified within `[ICAdministrationStandAloneDelegate shouldDoSignatureCapture]` method's parameters.
 *
 * When this method is called, the application should abort performing the signature capture. Calling `[ICAdministrationStandAlone submitSignatureWithImage]` method to submit the signature will have no effect and will be ignore by the PCL library.
 */
-(void)signatureTimeoutExceeded;

/**
 * Called when the iOS device receives a message from the Ingenico's device.
 *
 * @note The iOS device's and the Ingenico's device's applications should agree beforehand on the messages to be exhanged and on how they will be used.
 *
 * @param data A NSData containing the message.
 */
-(void)messageReceivedWithData:(NSData *)data;

/**
 * Called when the iOS device receives a message from the Ingenico's device.
 *
 * @note The iOS device's and the Ingenico's device's applications should agree beforehand on the messages to be exhanged and on how they will be used.
 *
 * @param symbologyData A NSData containing the symbology elements from the Ingenico's device's barcode reader.
 */
-(void)barcodeEventReceivedWithData:(NSString *)symbologyData barcodeEvent: (NSString *) barcodeData;

/**
 * Called when the iOS device receives a message from the Ingenico's device.
 *
 * @note The iOS device's and the Ingenico's device's applications should agree beforehand on the messages to be exhanged and on how they will be used.
 *
 * @param eventData A boolean value containing the status if the barcode reader is open or closed.
 */
-(void)barcodeClosedEventReceivedData : (BOOL)eventData;

/**
 * Called whenever a text string is available for printing.
 *
 * @note The font, size and alignement of the text are those used by default by the Ingenico's device. It is however possible to override them and use other values.
 *
 * @param text A NSString containing the text to be printed.
 * @param font An UIFont object including the font and size to be applied to the text when priting.
 * @param alignment The alignment of the text.
 */
-(void)shouldPrintText:(NSString *)text withFont:(UIFont *)font andAlignment:(NSTextAlignment)alignment;

/**
 * Called whenever a text string is available for printing.
 *
 * This method is called when formatted text is to be printed. It provides information on how the text should be rendered. This involves the following attributes:
 * - The text font: The size of the font is not set by the terminal. This means that the iOS application is free to choose the appropriate text size,
 * - The alignement of the text (left, right or center),
 * - The X scale ratio (this may be one of the following values: 1, 2 or 4),
 * - The Y scale ratio (may be one of the values: 1, 2 or 4),
 * - underlining.
 *
 * @param text A NSString containing the text to be printed.
 * @param font An UIFont object including the name of the font that should be used to render the text string.
 * @param alignment The alignment of the text.
 * @param xFactor The scaling factor to be applied to the text in the X direction.
 * @param yFactor The scaling factor to be applied to the text in the Y direction.
 * @param underline Specifies whether the text should be underlined or not.
 */
-(void)shouldPrintText:(NSString *)text withFont:(UIFont *)font alignment:(NSTextAlignment)alignment XScaling:(NSInteger)xFactor YScaling:(NSInteger)yFactor underline:(BOOL)underline;

/**
 * Called whenever a text string is available for printing.
 *
 * This method is called when formatted text is to be printed. It provides information on how the text should be rendered. This involves the following attributes:
 * - The text font: The size of the font is not set by the terminal. This means that the iOS application is free to choose the appropriate text size,
 * - The alignement of the text (left, right or center),
 * - The X scale ratio (this may be one of the following values: 1, 2 or 4),
 * - The Y scale ratio (may be one of the values: 1, 2 or 4),
 * - underlining.
 *
 * @param text A NSString containing the text to be printed.
 * @param font An UIFont object including the name of the font that should be used to render the text string.
 * @param alignment The alignment of the text.
 * @param xFactor The scaling factor to be applied to the text in the X direction.
 * @param yFactor The scaling factor to be applied to the text in the Y direction.
 * @param underline Specifies whether the text should be underlined or not.
 * @param bold Specifies whether the text should be bolded or not.
 */
-(void)shouldPrintText:(NSString *)text withFont:(UIFont *)font alignment:(NSTextAlignment)alignment XScaling:(NSInteger)xFactor YScaling:(NSInteger)yFactor underline:(BOOL)underline bold:(BOOL)bold;

/**
 * Called whenever a text string is available for printing.
 *
 * This method is called when formatted text is to be printed. It provides information on how the text should be rendered. This involves the following attributes:
 * - The text font: The size of the font is not set by the terminal. This means that the iOS application is free to choose the appropriate text size,
 * - The alignement of the text (left, right or center),
 * - The X scale ratio (this may be one of the following values: 1, 2 or 4),
 * - The Y scale ratio (may be one of the values: 1, 2 or 4),
 * - underlining.
 *
 * @param text A NSString containing the text to be printed.
 * @param charset The charset used by the text.
 * @param font An UIFont object including the name of the font that should be used to render the text string.
 * @param alignment The alignment of the text.
 * @param xFactor The scaling factor to be applied to the text in the X direction.
 * @param yFactor The scaling factor to be applied to the text in the Y direction.
 * @param underline Specifies whether the text should be underlined or not.
 * @param bold Specifies whether the text should be bolded or not.
 */
-(void)shouldPrintRawText:(char *)text withCharset:(NSInteger)charset withFont:(UIFont *)font alignment:(NSTextAlignment)alignment XScaling:(NSInteger)xFactor YScaling:(NSInteger)yFactor underline:(BOOL)underline bold:(BOOL)bold;

/**
 * Called whenever an image is available for printing.
 *
 * @param image An UIImage object containing the image to print.
 */
-(void)shouldPrintImage:(UIImage *)image;

/**
 * Called whenever the Ingenico's device needs to add empty lines when printing.
 *
 * @param lines The number of lines to add.
 */
-(void)shouldFeedPaperWithLines:(NSUInteger)lines;

/**
 * Called whenever the Ingenico's device needs to cut the paper.
 */
-(void)shouldCutPaper;

/**
 * Called whenever the Ingenico's device needs to start printing a receipt.
 *
 * @param type The receipt type.
 */
-(NSInteger)shouldStartReceipt:(NSInteger)type;

/**
 * Called whenever the Ingenico's device needs to end the printing of a receipt.
 */
-(NSInteger)shouldEndReceipt;

/**
 * Called whenever the Ingenico's device needs to add a signature.
 */
-(NSInteger)shouldAddSignature;

/**
 * Called when the iOS device receives a PCL informations request from the SPM.
 */
-(void)shouldSendPclAddonInfos;

@end
