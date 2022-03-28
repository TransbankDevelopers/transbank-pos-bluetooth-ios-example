//
//  ICSoftwareComponent.h
//  iSMP
//
//  Created by Pierre MB on 22/05/2017.
//  Copyright © 2017 Ingenico. All rights reserved.
//

#import <Foundation/Foundation.h>

/*! @brief The terminal's software components

 Each software component loaded into the terminal has a name, version and a CRC code. The components can be retrieved by calling
 @ref ICAdministration's @ref getSoftwareComponents method when the Companion is ready.
 */
@interface ICSoftwareComponent : NSObject

/*! @brief Software Component Name

 A software component name is a number with 4 or 6 digits.
 */
@property (nonatomic, readonly) NSString *name;

/*! @brief Software Component Version */
@property (nonatomic, copy) NSString *version;

/*! @brief Software Component CRC

 This is a NSString representation of the 2-byte CRC code of the software component binary.
 */
@property (nonatomic, readonly) NSString *crc;

/*! @brief The type of a software component

 A software component loaded into the terminal may be one of the following:

 - Application: .AGN
 - Library: .LGN
 - Driver: .DGN
 - Parameter: .PGN

 The component type constants are declared with @ref eICDeviceSoftwareComponentType
 */
@property (nonatomic, readonly) NSUInteger type;

@end
