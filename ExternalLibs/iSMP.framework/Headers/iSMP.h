//
//  iSMP.h
//  PCL Library
//
//  Created by Christophe Fontaine on 21/06/10.
//  Copyright 2010 Ingenico. All rights reserved.
//
/*!
    @file       iSMP.h
    @brief      Umbrella header of the framework.

    Includes all PCL Library's public header files.

    The included files are :

      - ICISMPDevice.h
      - ICBarCodeReader.h
      - ICAdministration.h
      - ICTransaction.h
      - ICNetwork.h
      - ICPrinter.h
      - ICSPP.h
      - ICAdministration+StandAlone.h
      - iBP.h
      - ICPPP.h
      - ICPclService.h
      - PCLFileSharing.h
      - PCLTeliumFile.h

    You must link your target against the following frameworks:

      - UIKit
      - Foundation
      - CoreGraphics
      - ExternalAccessory
      - SystemConfiguration
      - CFNetwork
*/

#import "ICISMPDevice.h"
#import "ICBarCodeReader.h"
#import "ICAdministration.h"
#import "ICTransaction.h"
#import "ICNetwork.h"
#import "ICPrinter.h"
#import "ICSPP.h"
#import "ICAdministration+StandAlone.h"
#import "iBP.h"
#import "ICPPP.h"
#import "ICPclService.h"
#import "ICTmsInformation.h"
#import "ICTransactionRequest.h"
#import "ICTransactionReply.h"
#import "ICTerminal.h"
#import "ICSoftwareComponent.h"
#import "ICSSLParameters.h"
#import "ICPclError.h"
#import "PCLFileSharing.h"
#import "PCLTeliumFile.h"
