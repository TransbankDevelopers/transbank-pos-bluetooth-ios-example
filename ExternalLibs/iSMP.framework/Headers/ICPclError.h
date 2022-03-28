//
//  ICPclError.h
//  iSMP
//
//  Created by Pierre MB on 18/08/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
/** the error domain used for iSMP errors */
extern NSErrorDomain const ICPclErrorDomain;
NS_ASSUME_NONNULL_END

/** the codes for the error in the ICPclErrorDomain */
typedef NS_ENUM(NSInteger, ICPclError) {
    ICPclErrorGeneric = 1, /**< a generic code when no specific code exists */
    ICPclErrorNotImplementedOnTerminal, /**< the terminal responded it doesn't know the requested command */
};
