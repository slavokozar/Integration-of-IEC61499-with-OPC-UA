/*
 * This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information.
 */

#ifndef NETWORKLAYERUDP_H_
#define NETWORKLAYERUDP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_server.h"
#include "ua_client.h"

/** @brief Create the UDP networklayer and listen to the specified port */
UA_ServerNetworkLayer UA_EXPORT * ServerNetworkLayerUDP_new(UA_ConnectionConfig conf, UA_UInt32 port);

UA_Connection UA_EXPORT
ClientNetworkLayerUDP_connect(UA_ConnectionConfig conf, char endpointUrl[], UA_Logger logger);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* NETWORKLAYERUDP_H_ */
