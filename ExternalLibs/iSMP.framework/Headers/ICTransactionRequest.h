//
//  ICTransactionRequest.h
//  iSMP
//
//  Created by Pierre MB on 19/04/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ICAdministration.h"
/** @file ICTransactionRequest.h */

/** @brief Used to describe the parameters of a payment transaction.

 This is the input data object for doTransactionWithRequest
 The application should fill all the necessary fields before issuing the transaction request.

 This is the object counterpart of `ICTransactionRequest`

 @seealso ICPclService
 */
@interface ICTransactionRequestObject : NSObject <NSCopying>

/** The POS number (should be within the range [0 - 255]). */
@property (nonatomic) NSInteger posNumber;
/** The amount for the transaction. If the ISO2 track needs to be read, amount should be 0. */
@property (nonatomic) NSInteger amount;
/** The kind of payment to use @see ICTransactionAccountType. */
@property (nonatomic) ICTransactionAccountType accountType;
/** The type of transaction to use @see ICTransactionType. */
@property (nonatomic) ICTransactionType transactionType;
/** The currency code in ISO4217 format. */
@property (copy, nonatomic) NSString *currency;
/** Application specific data to be passed to payment application. Max size 10 bytes */
@property (copy, nonatomic) NSData *privateData;
/** The authorization that the POS asks the terminal for @see ICTransactionAuthorization */
@property (nonatomic) ICTransactionAuthorization authorization;
/** A NSData object that contains any additional transaction data to be passed to the terminal. The maximum data length that can be passed through this parameter is 64 KB. */
@property (copy, nonatomic) NSData *extendedData;
/** The number of the terminal application that should be invoked to handle the transaction. If this parameter is set to 0, the payment application will be implicitely selected. */
@property (nonatomic) NSUInteger applicationNumber;

/** @brief (designated initializer) init a new instance with `amount`, `currency` and `posNumber` properties set with
 the values given in parameter, and the other properties with relevant default values.

 @param amount the amount in cents (or equivalent for the currency used)
 @param currency the code of the currency
 @param posNumber a POS number
 */
- (instancetype)initWithAmount:(NSInteger)amount currency:(NSString *)currency posNumber:(NSInteger)posNumber;

@end
