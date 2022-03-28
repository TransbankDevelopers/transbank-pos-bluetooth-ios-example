//
//  ICTmsInformation.h
//  iSMP
//
//  Created by Pierre MB on 13/04/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifndef ICTmsDeprecated
#define ICTmsDeprecated(msg) DEPRECATED_MSG_ATTRIBUTE(msg)
#endif

/*! @file ICTmsInformation.h */

/*!
 @anchor     tmsParameters
 @brief      The TMS's parameters
 */

@interface ICTmsInformation : NSObject <NSCopying>

/*! @brief      The TMS's server IP address or hostname (hostname can't exceed 257 caracters)
 *  set to nil to keep the current value.
 */
@property (nonatomic, copy) NSString *host;

/*! @brief      The TMS's server Port (can't exceed 65535)
 *  set to nil to keep the current value.
 */
@property (nonatomic, copy) NSString *port;

/*! @brief      The TMS's server Identifier (9 caracters max)
 *  set to nil to keep the current value.
 */
@property (nonatomic, copy) NSString *identifier;

/*! @brief      The TMS's SSL profiles list */
@property (nonatomic, copy) NSArray<NSString *> *availableSslProfiles;

/*! @brief      The TMS's SSL current profile
 *  set to
 *  - nil: to keep current value
 *  - empty string: to disable SSL mode
 */
@property (nonatomic, copy) NSString *currentSslProfile;

/*! @brief      default init: with host, port, identifier and SSL profile */
- (instancetype)initWithHost:(NSString *)host port:(NSString *)port identifier:(NSString *)identifier sslProfile:(NSString *)profile;

// deprecated

/*! @brief      The TMS's server IP address or hostname (hostname can't exceed 257 caracters)
 *  @deprecated use `host` instead.
 */
@property (nonatomic, retain) NSMutableString * TmsIP DEPRECATED_MSG_ATTRIBUTE("use `host` instead");

/*! @brief      The TMS's server Port (can't exceed 65535)
 *  @deprecated use `port` instead
 */
@property (nonatomic, retain) NSMutableString * TmsPort DEPRECATED_MSG_ATTRIBUTE("use `port` instead");

/*! @brief      The TMS's server Identifier (9 caracters max)
 *  @deprecated use `port` instead
 */
@property (nonatomic, retain) NSMutableString * TmsIdentifier DEPRECATED_MSG_ATTRIBUTE("use `identifier` instead");

/*! @brief      The TMS's SSL profiles list
 *  @deprecated use `port` instead
 */
@property (nonatomic, retain) NSMutableArray * TmsArraySSLProfile DEPRECATED_MSG_ATTRIBUTE("use `availableSslProfiles` instead");

/*! @brief      The TMS's SSL current profile
 *  @deprecated use `port` instead
 */
@property (nonatomic, retain) NSMutableString * TmsCurrentSSLProfile DEPRECATED_MSG_ATTRIBUTE("use `currentSslProfile` instead");

@end

