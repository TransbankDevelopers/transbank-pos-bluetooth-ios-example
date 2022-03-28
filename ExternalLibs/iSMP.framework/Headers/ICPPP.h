//
//  ICPPP.h
//  PCL Library
//
//  Created by Hichem Boussetta on 24/05/12.
//  Copyright (c) 2012 Ingenico. All rights reserved.
//

/*!
    @file	 ICPPP.h
    @brief   Header file of the ICPPP class
*/


#import <Foundation/Foundation.h>
#import "ICISMPDeviceExtension.h"

/*!
 @defgroup     ICPPPGroup  Communicate using TCP/IP protocol
 @brief        This category involves all required methods and data structures to communicate with a Companion using TCP/IP.
 @{
 */

@protocol ICPPPDelegate;


/*! @anchor ICPPP
 @brief a channel offering a TCP/IP over PPP link.

 This class implements a TCP/IP stack that allows an iOS device to communicate with a terminal using TCP/IP
 over PPP on a serial link.

 ICPPP offers networking features including:
 - allow the terminal to open up to 5 simultaneously TCP connections to remote hosts. In other words the iOS device acts as a proxy.
 - accept incoming connections and map them to ports on which some terminal applications are listening. In this configuration the terminal behaves as a server and the iOS device as a gateway
 - allow the terminal to open TCP connections as a client to the iOS device

 To initialize the ICPPP object, do as following:
 - Instantiate the object by calling sharedChannel().
   The returned instance has to be retained since this method returns an auto-released object.
 - Set the @ref ICPPPDelegateProperty property to a valid object that will receive the events of the class.

 After the ICPPP object is initialized, the TCP/IP stack can be started by calling openChannel().
 This action will trigger a PPP connection to the terminal, after which the TCP/IP communication between the terminal
 and the iOS device becomes ready.

 ICPPP is recomended over ICNetwork and offers similar features.
*/
@interface ICPPP : ICISMPDevice

/*! @brief returns the unique shared instance of ICPPP

 Note: The object returned by this method is autoreleased and must be retained to stay alive.

 @return A pointer to the shared instance
*/
+(ICPPP *)sharedChannel;


/*!
 @anchor     ICPPPDelegateProperty
 @brief      The delegate object. If you use ICPclService, use ICPclService.delegate, and not ICPPP.delegate.

 This property should be assigned the reference of a delegate object that will receive the events of ICPPP - This object should implement the ICISMPDeviceDelegate and ICPPPDelegate protocols.
*/
@property (nonatomic, weak) id<ICISMPDeviceDelegate, ICPPPDelegate> delegate;


/*!
    @anchor     IP
    @brief      The IP address attributed to the iOS device when connected to the terminal using ICPPP
    <p>
        This is the IP address obtained by the iOS device after it connects to the terminal using ICPPP. This property becomes available after the event @ref pppChannelDidOpen is fired.
    </p>
*/
@property (nonatomic, readonly) NSString * IP;

/*!
    @anchor     submask
    @brief      The mask of the sub-network created between the iOS device and the terminal
    <p>
        This is the mask of the sub-network created between the iOS device and the terminal after opening the ICPPP channel. This property becomes available after the event @ref pppChannelDidOpen is fired.
    </p>
*/
@property (nonatomic, readonly) NSString * submask;

/*!
    @anchor     dns
    @brief      The IP address of the DNS server to be used inside the sub-network created between the iOS device and the terminal
    <p>
        This is the IP address of the DNS server to be used inside the sub-network created between the iOS device and the terminal after opening the ICPPP channel. This property becomes available after the event @ref pppChannelDidOpen is fired.
    </p>
*/
@property (nonatomic, readonly) NSString * dns;

/*!
    @anchor     terminalIP
    @brief      The IP address of the terminal when connected to the iOS device using ICPPP
    <p>
        This is the IP address obtained by the terminal after it connects to the iOS device after opening the ICPPP channel. This property becomes available after the event @ref pppChannelDidOpen is fired.
    </p>
*/
@property (nonatomic, readonly) NSString * terminalIP;

/*! @brief Open the channel for the PPP and starts the PPP link.

 This method opens the channel for the PPP, and put in place a TCP/IP over PPP communication link between the
 payment terminal and the iOS device.

 This method is asynchronous. Establishing the PPP link can take a few secconds, so make sure the delegate implements
 the @ref pppChannelDidOpen callback to be informed when the PPP is actually ready. The app won't be able to perform
 TCP communication with the terminal until the link is ready.

 Note that a previous stopping process might still be going on, in which case ICPPP will wait for the previous link to
 completely stop before starting a new link.

 When the link becomes ready, the application can get the attributes of the PPP connection.
 Those can be retrieved using the following properties:

 - @ref IP : IP address of iOS device
 - @ref submask : Submask of the network
 - @ref dns : IP address of the DNS server when available
 - @ref terminalIP : IP address of the terminal

 These attributes are not necessary for the the app to communicate with the terminal. They are just provided for information.

 @return ISMP_Result_Failure if the channel for the PPP failed to open, ISMP_Result_SUCCESS otherwise.
*/
-(iSMPResult)openChannel;

/*! @brief Stop the PPP link and close the channel for the PPP.

 This method disconnects the PPP link, cutting any TCP communication going between the iOS device the terminal, and
 then close the channel for the PPP.

 This function is asynchronous. Stopping the PPP can take a few seconds, so make sure the delegate
 implements the @ref pppChannelDidClose callback to be informed when the PPP is actually stopped.
*/
-(void)closeChannel;

/*! @brief Adds a TCP bridge from the iOS device to the terminal for the provided port number

 Call this method when you need to connect your iOS app or route an incoming call to a server running on the terminal.
 This method will map the iOS network stack for the provided port to the ICPPP stack on the same port of a server
 running on the terminal side.
 Call this method after opening the ICPPP channel.

 This method is useful if an application wants to communicate in TCP with the terminal in the configuration where
 the terminal is the server, and the application is the client.
 This works the same as the port mapping mechanism used on routers.
 The connection requests can originate from a local or remote client.

 Bridges added by calling addiOSToTerminalBridgeOnPort(), addTerminalToiOSBridgeOnPort() and
 addiOSToTerminalBridgeLocalOnPort() are limited to 12.
 This means that these methods should not be called more than 12 times.

 A bridge remains active until the ICPPP channel is closed and won't be started automatically when the channel is
 opened again.
 It is necessary in this case to re-create it when needed.

 There is currently no way to close a bridge, other than closing the PPP.

 The possible errors while opening a bridge are the following:
 - -1: no more available bridges
 - -2: bridge already exists
 - -3: issue during thread creation
 - -4: bridge initialization failed

 @param port the port to forward
 @returns 0 on success, a negative value on failure
*/
-(int)addiOSToTerminalBridgeOnPort:(NSInteger)port;

/*! @brief Adds a TCP bridge from terminal to the iOS device for the provided port number

 Call this method when you need to connect the terminal to a server running on the iOS device. This method will
 map the incoming terminal connection on the ICPPP stack for the provided port, to the same port of a server running on
 the native iOS network stack.
 Call this method after opening the ICPPP channel.

 This method is useful if an iOS application wants to communicate in TCP with the terminal in the configuration where
 the terminal is the client, and the iOS device is the server.

 Bridges added by calling addiOSToTerminalBridgeOnPort(), addTerminalToiOSBridgeOnPort() and
 addiOSToTerminalBridgeLocalOnPort() are limited to 12.
 This means that these methods should not be called more than 12 times.

 A bridge remains active until the ICPPP channel is closed and won't be started automatically when the channel is
 opened again.
 It is necessary in this case to re-create it when needed.

 There is currently no way to close a bridge, other than closing the ICPPP channel.

 The possible errors while opening a bridge are the following:
 - -1: no more available bridges
 - -2: bridge already exists
 - -3: issue during thread creation
 - -4: bridge initialization failed

 @param port the port to forward
 @returns 0 on success, a negative value on failure
 */
-(int)addTerminalToiOSBridgeOnPort:(NSInteger)port;

/*! @brief Adds a TCP bridge from the iOS device to the terminal for the provided port number, forwarding only local connections.

 Call this method when you need to connect your iOS device to a server running on the terminal.
 This method will map the iOS network stack for the provided port to the ICPPP stack on the same port of a server
 running on the terminal side. Call this method after opening the ICPPP channel.
 
 This method is useful if an iOS application wants to communicate in TCP with the terminal in the configuration where
 the terminal is the server, and the iOS device is the client.
 This works the same as the port mapping mechanism used on routers.
 The connection requests shall originate from a local client.

 Bridges added by calling addiOSToTerminalBridgeOnPort(), addTerminalToiOSBridgeOnPort() and
 addiOSToTerminalBridgeLocalOnPort() are limited to 12.
 This means that these methods should not be called more than 12 times.

 A bridge remains active until the ICPPP channel is closed and won't be started automatically when the channel is
 opened again.
 It is necessary in this case to re-create it when needed.

 There is currently no way to close a bridge, other than closing the ICPPP channel.

 The possible errors while opening a bridge are the following:

 - -1: no more available bridges
 - -2: bridge already exists
 - -3: issue during thread creation
 - -4: bridge initialization failed

 @param port the port to forward
 @returns 0 on success, a negative value on failure
 */
-(int)addiOSToTerminalBridgeLocalOnPort:(NSInteger)port;

/*!
 @anchor  setKeepAliveDelayIntervalAndCount
 @brief   Set TCP keep alive parameters
 <p>
 This method allows to configure the different parameters. When keep alive option is set on the Telium device, the internal socks server set this option to connect to the remote using these parameters.
 <ul>
 <li>delay : idle time used when keep alive is enabled, default value 1 second, max value 4 294 967 seconds</li>
 <li>interval : interval between keepalives, default 7200 seconds, max value 4 294 967 seconds</li>
 <li>count : number of keepalives before close, default 5 times, max value 2 147 483 647</li>
 </ul>
 </p>
 @return An integer constant defined with the @ref eISMPResult enumeration. ISMP_Result_SUCCESS on success.
 */
-(iSMPResult)setKeepAliveDelay:(int)keepAliveDelay Interval:(int)keepAliveInterVal andCount:(int)keepAliveCount;

@end

/*!
    @anchor     ICPPPDelegate
    @brief      The ICPPP Channel's delegate methods
    <p>
        These methods should be implemented by @ref ICPPPDelegate's delegate to subscribe to be notified of events.
    </p>
*/
@protocol ICPPPDelegate

/*!
    @anchor  pppChannelDidOpen
    @brief   Method called ICPPP channel is ready
    <p>
        ICPPP channel becomes ready when the TCP/IP stack is initialized and the terminal and iOS device are connected using the IP protocol. At this stage, the connection attributes can be retrieved throught the properties: @ref IP, @ref submask, @ref dns and @ref terminalIP.<br />
        When this event is fired, communication using the bridges to and from the terminal becomes possible.
    </p>
*/
-(void)pppChannelDidOpen;

/*!
    @anchor  pppChannelDidClose
    @brief   Method called ICPPP channel is closed asynchronously
    <p>
        This event is fired when the ICPPP channel is closed without calling @ref ICPPP_closeChannel. This may happen when the terminal disconnects or when the serial link between the two device is broken (when loosing the Bluetooth connection or when unplugging the devices).<br />
        After receiving this event, network communication is no more possible until the channel is opened again by calling openChannel().
    </p>
*/
-(void)pppChannelDidClose;

/*!
 @}
 */

@end

/*!
 @}
 */
