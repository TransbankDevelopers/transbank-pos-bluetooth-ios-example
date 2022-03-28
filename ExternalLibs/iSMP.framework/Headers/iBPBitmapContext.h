//
//  iBPBitmapContext.h
//  PCL
//
//  Created by Hichem Boussetta on 01/02/12.
//  Copyright (c) 2012 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


/*!
    @file       iBPBitmapContext.h 
    @brief      Header file for the iBPBitmapContext class
*/



/*!
    @anchor iBPBitmapContext
    @brief  Helper class to create customizable bitmap contexts
    <p>
        This class provides an easy way to render text and/or bitmaps in a bigger bitmap. This gives much more flexibility to the developer when using the Bluetooth Printer API. Since, the first implementation of the printing protocol does not support text formatting (changing font, size and alignment), the @ref iBPBitmapContext class provides a workaround that consists in rendering the text on the iOS side and then sending the resulting bitmap for printing. Moreover, the latter can also be used for other purposes like printing it over the air (AirPrint), sending it by E-mail or MMS.
    </p>
*/
@interface iBPBitmapContext : NSObject


/*!
    @anchor     textFont
    @brief      The name of the font to be used to be render text within the bitmap context
    <p>
        This property should be set before issuing the call to @ref drawText so that the text is formatted accordingly. So, this property does only affect the text that is rendered after it is changed and not the one that was added before.<br />
        All iOS supported fonts can be used. Those can be retrieved by calling [UIFont familyNames] and [UIFont fontNamesForFamilyName:] of the iOS SDK.
    </p>
*/
@property (nonatomic, copy)     NSString *      textFont;


/*!
    @anchor     textSize
    @brief      The size of the text to be drawn on the bitmap context
    <p>
        The size has to be set before drawing the text. The property change will only affect the text that will come after and not the one that was drawn before.
    </p>
*/
@property (nonatomic, assign)   NSUInteger      textSize;


/*!
    @anchor     alignment
    @brief      The alignment of text or bitmaps within the bitmap context
    <p>
        This property is to be set with one of the constants of the NSTextAlignment enumeration. This will only impact the text or bitmaps that are drawn after changing the property.<br />
        The supported alignment configurations are Left, Right and Center.
    </p>
*/
@property (nonatomic, assign)   NSUInteger      alignment;


/*!
    @anchor     characterSpacing
    @brief      The spacing between text characters when they are rendered into the bitmap context
    <p>
        The new character spacing value is applied to the text that follows changing the property.
    </p>
*/
@property (nonatomic, assign)   NSUInteger      characterSpacing;


/*!
    @anchor     lineFeedStep
    @brief      The number of pixels to skip in the Y direction when calling @ref lineFeed
*/
@property (nonatomic, assign)   NSUInteger      lineFeedStep;


/*!
    @anchor     initWithWidth
    @brief      Initialize the bitmap context with the dimensions provided as arguments
    <p>
        This method should be called just after constructing an iBPBitmapContext object and before performing any drawing operation.
    </p>
    @param      width   Width of the bitmap context
    @param      height  Height of the bitmap context
    @result     An initialized iBPBitmapContext object
 
*/
-(id)initWithWidth:(CGFloat)width andHeight:(CGFloat)height;


/*!
    @anchor     drawText
    @brief      Draws a text string on the bitmap context
    <p>
        The text string is renderend using the last configured formatting options. Those can be set by changing the values of the following properties:
        <ul>
            <li>@ref textFont</li>
            <li>@ref textSize</li>
            <li>@ref alignment</li>
            <li>@ref characterSpacing</li>
        </ul>
    </p>
    @param      text    NSString object containing the text string to be drawn
*/
-(void)drawText:(NSString *)text;


/*!
    @anchor     drawBitmap
    @brief      Draws an image on the bitmap context
    <p>
        This methods takes an UIImage object and draws it on the bitmap context. If the bitmap width is larger than the width of the context, a scaling factor is applied to make it fit in its container.<br />
        Bitmaps can also be aligned (left, center or right) based on the @ref alignment property that is also used to align text.
    </p>
    @param      image   UIImage object containing the image to be drawn
*/
-(void)drawBitmapWithImage:(UIImage *)image;


/*!
    @anchor     drawBitmapWithImageAndSize
    @brief      Draws an image on the bitmap context and binds its size to the 
    <p>
        This method behaves exactly the same as @ref drawBitmap, except that it offers the possibility to change the size of the bitmap through the bitmapSize argument. This is useful when having to print bitmap resources that have different sizes while they need to be drawn with the same width to align them.<br />
        When scaling the image object provided as argument according to the bitmapSize parameter, the aspect ratio is preserved and the bitmap is scaled to fit within the width of the context if the provided size exceeds this width.
    </p>
    @param      image   UIImage object containing the image to be drawn
    @param      bitmapSize  A CGSize structure that specifies the preferred bitmap size
*/
-(void)drawBitmapWithImage:(UIImage *)image andSize:(CGSize)bitmapSize;


/*!
    @anchor     clearContext
    @brief      Clear the bitmap context
    <p>
        This function clears the bitmap context and erases every text or bitmap that was drawn.
    </p>
*/
-(void)clearContext;


/*!
    @anchor     jumpLine
    @brief      Move the drawing position a step forward in the vertical direction
    <p>
        This function moves the drawing Y position a step forward. The step can be changed through the @ref lineFeedStep property.
    </p>
*/
-(void)lineFeed;


/*!
    @anchor     getImage
    @brief      Return an UIImage object of the content of the bitmap context from the beginning to the drawing position
    <p>
        This function returns an UIImage object from what has been drawn on the bitmap context, starting from the beginning of the bitmap context to the current drawing position. The image object has the following bitmap configuration:
            <ul>
                <li>Grayscale color space</li>
                <li>No alpha channel</li>
                <li>1 color component</li>
                <li>8 bits per color component</li>
            </ul>
        For more information on iOS supported bitmap configurations, please refer to Apple's documentation here <a href="http://developer.apple.com/library/mac/#qa/qa2001/qa1037.html">here</a>.<br />
        The returned UIImage object can be provided as argument to the @ref iBPPrintBitmap of ICAdministation's iBP category in order to print it using the BT printer. It also can be saved in a database, or sent by E-mail or by MMS.
    </p>
    @result     UIImage object of the content of the bitmap context
*/
-(UIImage *)getImage;


/*!
    @anchor     getImageAt
    @brief      Return an UIImage object of the content of the bitmap context from the beginning to the drawing position
    <p>
        This function returns an UIImage object containing a sub-image from what is drawn on the bitmap context. It picks the part of the context starting at yPosition and that is maxHeight large at maximum. If maxHeight exceeds the current drawing position reached in the context, the returned image is taken from yPosition until the current drawing Y position.<br />
        The image object has the following bitmap configuration:
        <ul>
            <li>Grayscale color space</li>
            <li>No alpha channel</li>
            <li>1 color component</li>
            <li>8 bits per color component</li>
        </ul>
        For more information on iOS supported bitmap configurations, please refer to Apple's documentation here <a href="http://developer.apple.com/library/mac/#qa/qa2001/qa1037.html">here</a>.<br />
        The returned UIImage object can be provided as argument to the @ref iBPPrintBitmap of ICAdministation's iBP category in order to print it using the BT printer. It also can be saved in a database, or sent by E-mail or by MMS.
    </p>
    @param      yPosition   The starting point in the Y-Axis of the image to be returned
    @param      maxHeight   The maximum height of the image to be returned. If the image should go beyond the drawing position reached withinthe bitmap context, only the part before the drawing position is returned.
    @result     UIImage object of the content of the bitmap context
*/
-(UIImage *)getImageAt:(int)yPosition maxHeight:(int)maxHeight;


/*!
    @anchor     cursorPosition
    @brief      Return the current drawing position within the bitmap context
    @result     The drawing Y Position within the bitmap context
*/
-(CGFloat)drawingPosition;

@end
