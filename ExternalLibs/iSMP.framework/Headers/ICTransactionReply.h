//
//  ICTransactionReply.h
//  iSMP
//
//  Created by Pierre MB on 19/04/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ICAdministration.h"
/** @file ICTransactionReply.h */

/** @brief Used to describe the result of a payment transaction.

 You don't create instances, you get one through ICPclService and ICAdministration delegate methods, after issueing
 a transaction with, respectively, ICPclService::doTransaction() methods and ICAdministration::doTransaction() methods
 The application should analyze the content of the structure in order to know whether the transaction succeeded or failed.

 This is the object counterpart of ICTransactionReply.
 */
@interface ICTransactionReplyObject : NSObject

/** (the same as in the transaction request) */
@property (readonly, nonatomic) NSInteger posNumber;
/** The status of the payment process */
@property (readonly, nonatomic) unsigned char operationStatus;
/** The real amount used for the transaction */
@property (readonly, nonatomic) NSInteger amount;
/** The account type used for the transaction */
@property (readonly, nonatomic) ICTransactionAccountType accountType;
/** The currency code in ISO4217 format (the same as in the transaction request) */
@property (readonly, nonatomic) NSString *currency;
/** Application specific data to be passed to POS application */
@property (readonly, nonatomic) NSData *privateData;
/** The authorization number */
@property (readonly, nonatomic) NSString *authorizationNumber;
/** Response of the cash register connection */
@property (readonly, nonatomic) NSData *zoneRep;
/** Private area */
@property (readonly, nonatomic) NSData *zonePriv;
/** Extended data */
@property (readonly, nonatomic) NSData *extendedData;

@end
