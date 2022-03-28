//
//  ICTerminal.h
//  iSMP
//
//  Created by Pierre MB on 17/05/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 @brief      The terminal parameters
 */
@interface ICTerminal : NSObject <NSCopying>

/** @brief the terminal name (ie: taking the form Model-SerialNumber) */
@property(nonatomic, copy) NSString *name;

/** @brief the terminal connection interface */
@property(nonatomic) BOOL isBluetooth;

/** @brief the terminal MAC address, if it has one */
@property(nonatomic, copy) NSString *macAddress;

/** @brief the terminal IP address, if it has one */
@property(nonatomic, copy) NSString *ipAddress;

/** @brief checks the equality of two terminals */
- (BOOL)isEqualToTerminal:(ICTerminal *)other;

/** @brief get a special instance representing direct connect */
+ (ICTerminal *)directConnect;

// Deprecated

/** @brief the terminal name (Type-SerialNumber). Use `name` instead.
 @deprecated use `name` instead
 */
@property(nonatomic, copy) NSMutableString *terminalName DEPRECATED_MSG_ATTRIBUTE("use `name` instead");

/** @brief the terminal MAC address, if it has one. Use `macAddress` instead.
 @deprecated use `macAddress` instead
 */
@property(nonatomic, copy) NSMutableString *terminalMacAddress DEPRECATED_MSG_ATTRIBUTE("use `macAddress` instead");

/** @brief the terminal IP address, if it has one. Use `ipAddress` instead.
 @deprecated use `ipAddress` instead
 */
@property(nonatomic, copy) NSMutableString *terminalIPAddress DEPRECATED_MSG_ATTRIBUTE("use `ipAddress` instead");

@end
