//
//  ICSSLParameters.h
//  iSMP
//
//  Created by Pierre MB on 13/06/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>

/*! @brief The TLS connection parameters */
@interface ICSSLParameters : NSObject <NSCopying>

/*! @brief Wether the connection is secured by TLS or not */
@property(nonatomic) BOOL isSSL;

/*! @brief The p12 file name */
@property(nonatomic, copy) NSString *sslCertificateName;

/*! @brief The p12 file password */
@property(nonatomic, copy) NSString *sslCertificatePassword;

@end

