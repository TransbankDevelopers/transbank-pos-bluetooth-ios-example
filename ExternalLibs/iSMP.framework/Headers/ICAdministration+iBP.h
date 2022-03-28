//
//  ICAdministration+iBP.h
//  PCL
//
//  Created by Hichem Boussetta on 02/01/12.
//  Copyright (c) 2012 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ICAdministration.h"

/**
 * An enumeration of the returned constants of the iBP API.
 */
enum eiBPResult {
    /// Request succeed.
    iBPResult_OK,

    /// Request failed due to a wrong parameters passed to the Ingenico's device.
    iBPResult_KO,

    /// Request timeout meaning that no response came from the Ingenico's device.
    iBPResult_TIMEOUT,

    /// Failure because the iOS device and the Ingenico's device are not connected.
    iBPResult_ISMP_NOT_CONNECTED,

    /// Request failure due to the printer not being open. `iBPOpenPrinter` should be called to recover from this error.
    iBPResult_PRINTER_NOT_CONNECTED,

    /// Request failure because the parameters passed to the API are irrelevant (null parameter).
    iBPResult_INVALID_PARAM,

    /// Request failure because the text provided is longer than 512 characters.
    iBPResult_TEXT_TOO_LONG,

    /// Request failure if the provided bitmap can not be converted to monochrome configuration.
    iBPResult_BITMAP_CONVERSION_ERROR,

    /// The logo name passed as argument is inappropriate (number of characters should be in the range [4, 8]).
    iBPResult_WRONG_LOGO_NAME_LENGTH,

    /// Printer error.
    iBPResult_PRINTING_ERROR,

    /// No more paper in the printer.
    iBPResult_PAPER_OUT,

    /// Printer is in low battery condition.
    iBPResult_PRINTER_LOW_BATT
};

/**
 * An enumeration of the returned constants of the iBP API.
 *
 * @see `eiBPResult`
 */
typedef enum eiBPResult iBPResult;


/**
 * An enumeration of the font supported in the Ingenico's device.
 */
enum eiBPFont {
    /// Latin-1 Western European.
    ISO8859_1,

    /// Latin-2 Central European.
    ISO8859_2,

    /// Latin-3 South European.
    ISO8859_3,

    /// Latin/Cyrillic. Covers mostly Slavic languages that use a Cyrillic alphabet, including Belarusian, Bulgarian, Macedonian, Russian, Serbian, and Ukrainian (partial).
    ISO8859_5,

    /// Latin/Arabic. Covers the most common Arabic language characters.
    ISO8859_6,

    /// Latin/Greek. Covers the modern Greek language (monotonic orthography). Can also be used for Ancient Greek written without accents or in monotonic orthography, but lacks the diacritics for polytonic orthography.
    ISO8859_7,

    /// A revision of 8859-1 that removes some little-used symbols, replacing them with the euro sign € and the letters Š, š, Ž, ž, Œ, œ, and Ÿ, which completes the coverage of French, Finnish and Estonian.
    ISO8859_15
};

/**
 * An enumeration of the font supported in the Ingenico's device.
 *
 * @see `eiBPFont`
 */
typedef enum eiBPFont iBPFont;

/**
 * Bluetooth printer API extension of the PCL library.
 *
 * This category extends the API of the `ICAdministration` class provided by the PCL library. The methods defined in this file are used to communicate with the bluetooth printer driver of the Ingenico's device to print certain types of documents composed of text and bitmaps.
 */
@interface ICAdministration (iBP)

/**
 * The selected font to print text.
 */
@property (copy, nonatomic) NSString * font;

/**
 * Tries to open a session to the Ingenico's device's bluetooth printer.
 *
 * This call must succeedto be able to send printing commands to the Ingenico's device's bluetooth printer. If a session is already opened, this methods returns immediately with success as a result.
 * This call is blocking and has a 15 seconds timeout after which it is deemed to have failed.
 * When the printing is done, the session should be closed by calling `[ICAdministration iBPClosePrinter]`.
 *
 * @return A value of `eiBPResult` enumeration.
 */
-(iBPResult)iBPOpenPrinter;

/**
 * Tries to close an already opened printing session.
 *
 * When no session has already been opened to the Ingenico's device's bluetooth printer, this method returns imediately with success as result, otherwise it inquires the Ingenico's device's bluetooth printer to close the existant one.
 * If this method succeeds, all subsequent printing commands will faill if no new session is created by calling `[ICAdministration iBPOpenPrinter]`.
 * This call is blocking and has a 15 seconds timeout after which it is deemed to have failed.
 *
 * @return A value of `eiBPResult` enumeration.
 */
-(iBPResult)iBPClosePrinter;

/**
 * Request to print the text provided as parameter.
 *
 * This call is blocking and has a timeout of 15 seconds after which it is deemed to have failed.
 *
 * @note Before printing text, you should choose the font using `[ICAdministration iBPSetFont]`. If `[ICAdministration iBPSetFont]` is not used, the default font is ISO8859-15.
 *
 * @warning The length of the string to be printed should not exceed 512 characters otherwise the call will fail.
 *
 * @param text A NSString object of the text to print.
 *
 * @return A value of `eiBPResult` enumeration.
 */
-(iBPResult)iBPPrintText:(NSString *)text;


/*!
    @anchor  iBPPrintBitmap
    @brief   Request the print of the bitmap  provided as parameter
    <p align="justify">
        This method accepts all image types that are supported by iOS. The UIImage object undergoes is converted to an indexed black and white monochrome bitmap where each pixel is encoded on a bit of data. When printed, the bitmap keeps its original size if it does not exceed the maximum dimensions, otherwise it is scaled to fit within those.<br />
        A printing session must already be opened before calling this method, otherwise the call returns immediately with a failure result.<br />
        This call is blocking and has a timeout of 30 seconds.<br />
        See @ref iBPPrintBitmapSizeAlignment for more drawing options.
    </p>
    @param      image UIImage object to be printed.
    @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
*/
-(iBPResult)iBPPrintBitmap:(UIImage *)image;

/*!
 @anchor  iBPPrintBitmapLastBitmap
 @brief   Request the print of the bitmap  provided as parameter
 <p align="justify">
 This method is extended from @ref iBPPrintBitmap and added to fix a print issue that occurs when using a CUSTOM BT printer. More generally you should use this method if the bitmap to be printed is higher than 1024 or if you want to print several bitmaps in a consecutive way.  This method accepts all image types that are supported by iOS. The UIImage object undergoes is converted to an indexed black and white monochrome bitmap where each pixel is encoded on a bit of data. When printed, the bitmap keeps its original size if it does not exceed the maximum dimensions, otherwise it is scaled to fit within those.<br />
 A printing session must already be opened before calling this method, otherwise the call returns immediately with a failure result.<br />
 This call is blocking and has a timeout of 30 seconds.<br />
 See @ref iBPPrintBitmapSizeAlignment for more drawing options.
 </p>
 @param      image UIImage object to be printed.
 @param      isLastBitmap BOOL specify if the bitmap to print is the last.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)iBPPrintBitmap:(UIImage *)image lastBitmap:(BOOL)isLastBitmap;

/*!
    @anchor  iBPPrintBitmapSizeAlignment
    @brief   Print a bitmap and scale it according to the provided CGSize parameter
    <p align="justify">
        This methods works exactly the same as @ref iBPPrintBitmap (It is synchronous and has a 15 seconds timeout), except that the bitmap can be scaled using the bitmapSize argument and can be aligned horizontally (left, center or right alignment). The size of the bitmap can be decreased or increased based on the bitmapSize parameter, and is always limited by the maximum dimensions that a bitmap may have to be printed.<br />
            In order to draw the bitmap in its original size, the application may provide a CGSize structure initialized with the bitmap's width and height.
    </p>
    @param      image UIImage object to be printed.
    @param      bitmapSize  A CGSize structure that specifies the preferred bitmap size.
    @param      alignment   The alignment of the bitmap within the printed ticket (left, right or center). This may be one of the constants of *NSTextAlignment*
    @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
*/
-(iBPResult)iBPPrintBitmap:(UIImage *)image size:(CGSize)bitmapSize alignment:(NSTextAlignment)alignment;

/*!
 @anchor  iBPPrintBitmapSizeAlignmentLastBitmap
 @brief   Print a bitmap and scale it according to the provided CGSize parameter
 <p align="justify">
    This method is extended from @ref iBPPrintBitmapSizeAlignment and added to fix a print issue that occurs when using a CUSTOM BT printer. More generally you should use this method if the bitmap to be printed is higher than 1024 or if you want to print several bitmaps in a consecutive way.This methods works exactly the same as @ref iBPPrintBitmapSizeAlignment (It is synchronous and has a 15 seconds timeout), except that the bitmap can be scaled using the bitmapSize argument and can be aligned horizontally (left, center or right alignment). The size of the bitmap can be decreased or increased based on the bitmapSize parameter, and is always limited by the maximum dimensions that a bitmap may have to be printed.<br />
 In order to draw the bitmap in its original size, the application may provide a CGSize structure initialized with the bitmap's width and height.
 </p>
 @param      image UIImage object to be printed.
 @param      bitmapSize  A CGSize structure that specifies the preferred bitmap size.
 @param      alignment   The alignment of the bitmap within the printed ticket (left, right or center). This may be one of the constants of *NSTextAlignment*.
 @param      isLastBitmap BOOL specify if the bitmap to print is the last.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)iBPPrintBitmap:(UIImage *)image size:(CGSize)bitmapSize alignment:(NSTextAlignment)alignment lastBitmap:(BOOL)isLastBitmap;

/*!
    @anchor     iBPStoreLogo
    @brief      Stores logos to the Companion.
    <p>Logos are bitmaps identified by a name composed of 8 characters at most. Logos are also converted to monochrome black and white bitmaps and undergo the same transaformations as for bitmaps printed using @ref iBPPrintBitmap.<br />
       This call is blocking and has a timeout of 30 seconds.
    </p>
    @param      name  The name of the logo (Composed of 8 characters at most)
    @param      logo UIImage object of the logo to be loaded into the terminal.
    @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
*/
-(iBPResult)iBPStoreLogoWithName:(NSString *)name andImage:(UIImage *)logo;


/*!
    @anchor     iBPPrintLogo
    @brief      Request the printing of a logo stored inside the Companion.
    <p>
        Logos are bitmaps identified by a name and store inside the Companion either manually (by using the file transfer tool LLT) or programmatically by calling the @ref iBPStoreLogo method. To print a logo, we just refer to the logo's name and provide as an argument to @ref iBPPrintLogo.
        This call is blocking and has a timeout of 20 seconds.
    </p>
    @param      name The name of the logo.
    @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
*/
-(iBPResult)iBPPrintLogoWithName:(NSString *)name;


/*!
    @anchor     iBPGetPrinterStatus
    @brief      Gets the printer's status
    <p>
        This call is blocking and has a timeout of 10 seconds.
    </p>
    @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
*/
-(iBPResult)iBPGetPrinterStatus;

/**
 @anchor     iBPSetFont
 @brief      Request to set the font provided as parameter
 <p>
 This call permits to select the font used to print text using @ref iBPPrintText.
 </p>
 @param      selectedFontToTelium the encoding format ISO8859 supported by Telium @ref eiBPFont.
 @result     One of the enumerations of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)iBPSetFont:(iBPFont *) selectedFontToTelium;

/**
 @anchor    iBPOpenCashDrawer
 @brief     Request to open the cash drawer connected to a printer
 <p>
 This call allow you to open the cash drawer connected to a printer.
 </p>
 @result    One of the enumeration of @ref eiBPResult. It is iBPResult_OK when the call succeeds.
 */
-(iBPResult)iBPOpenCashDrawer;

/*!
 @anchor     iBPisBatteryLow
 @brief      Gets if the printer is in battery low mode
 <p>
 This call is blocking and has a timeout of 10 seconds.
 </p>
 @result     TRUE if the printer is in low battery, FALSE in other case.
 */
-(BOOL)iBPisBatteryLow;
/*!
 @anchor     iBPisConnected
 @brief      Gets if the printer is connected
 <p>
 This call is blocking and has a timeout of 10 seconds.
 </p>
 @result     TRUE if the printer is connected, FALSE in other case.
 */
-(BOOL)iBPisConnected;

/*!
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


@end
/*!
@}
*/
