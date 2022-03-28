//
//  ICCommunicationPeripherals.h
//  PCL Library
//
//  Created by Hichem Boussetta on 02/09/10.
//  Copyright 2010 Ingenico. All rights reserved.
//


/*!
    @file       ICCommunicationPeripherals.h
    @brief      Header file for ICCommunicationPeripherals
*/



#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>



/*!
    @enum		NetworkStatus
    @brief      iOS's Network Status Constants
*/
enum NetworkStatus{
    NotReachable = 0,                                   /**< No network is reachable */
    ReachableViaWiFi,                                   /**< The network is reachable through the wireless interface */
    ReachableViaWWAN                                    /**< The network is reachable through the cellular interface */
};


@protocol ICCommunicationPeripheralsDelegate;



/*!
    @brief    This interface retrieves information about communication peripherals
    <p>Theses include GPRS and WLAN devices</p>
*/
@interface ICCommunicationPeripherals : NSObject {
	
	//GPRS Device Information
	BOOL			  gprsIsConnected;                      /**< GPRS Connection State */
	NSData			* gprsMacAddress;                       /**< GPRS MAC Address */
	NSData			* gprsSimCardNumber;                    /**< GPRS Card Sim Number (There is no iOS API to retrieve this value) */
	
	//WLAN Device Information
	BOOL			  wlanIsConnected;                      /**< WLAN Connection State */
	NSData			* wlanMacAddress;                       /**< WLAN MAC Address */
	
	//Host information
	NSString		* hostName;                             /**< Host Name */
	NSString		* hostIP;                               /**< Host IP */
	
	//Reachability status
	NSUInteger		  reachabilityStatus;                   /**< Reachability Status */
	
	SCNetworkReachabilityRef reachabilityRef;               /**< ReachabilityRef Variable */
	
	id<ICCommunicationPeripheralsDelegate>		__weak delegate;   /**< Delegate Object of ICCommunicationPeripherals */
}


/*!
	@brief      Default Initializer
    <p>When initialized with no parameters, the ICCommunicationPeripherals object will check for the availability of internet connection</p>
	@result     The initialized receiver
*/
-(id)init;



/*!  
	@brief   Initializer with Host Name
    <p>
        When initialized with a host name or address, the ICCommunicationPeripherals object will check if the host is reachable through the available network interfaces.
    </p>
	@result     The initialized receiver
*/
-(id)initWithHostName:(NSString *)host;


/*!
	@brief      Initializer with Host IP
    <p>
        When initialized with a host name or address, the ICCommunicationPeripherals object will check if the host is reachable through the available network interfaces.
    </p>
	@result     The initialized receiver
*/
-(id)initWithHostAddress:(NSString *)ip;



/*!
    @brief   The GPRS's device connection state
*/
@property (nonatomic, readonly) BOOL gprsIsConnected;



/*!
	@brief   The GPRS device's MAC Address
*/
@property (nonatomic, readonly) NSData * gprsMacAddress;



/*!
	@brief   The GPRS device's SIM Card Number
*/
@property (nonatomic, readonly) NSData * gprsSimCardNumber;



/*!
	@brief   The WLAN device's connection state
*/
@property (nonatomic, readonly) BOOL wlanIsConnected;



/*!
	@brief   The WLAN device's MAC Address
*/
@property (nonatomic, readonly) NSData * wlanMacAddress;





/*!
    @brief   The network's reachability status
*/
@property (nonatomic, readonly) NSUInteger reachabilityStatus;


/*!
    @brief      The delegate object that should implement the ICCommunicationPeripheralsDelegate
*/
@property (nonatomic, weak) id<ICCommunicationPeripheralsDelegate> delegate;


/*!
	@brief      This method retrieves the reachability status of the GPRS and WLAN devices
*/
-(void)currentReachabilityStatus;





/*!
	@brief      This method retrieves the MAC addresses of the GPRS and WLAN devices
*/
-(void)getMacAddresses;



@end




/*!
    @brief      Methods to be implemented by delegates of ICCommunicationPeripherals objects 
*/
@protocol ICCommunicationPeripheralsDelegate
@optional

/*!
    @anchor  networkReachabilityDidChanged
    @brief   This method is called whenever the reachability status changes
    <p>
        The reachability status associated with an ICCommunicationPeripherals object may refer to the reachability of a given host, or simply the reachability of the network connection.
    </p>
*/
-(void)networkReachabilityDidChanged;

@end
