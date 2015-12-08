/**
* @file ua_transport_generated.h
*
* @brief Autogenerated data types
*
* Generated from Custom.Opc.Ua.Transport.bsd with script C:/open62541/tools/generate_datatypes.py
* on host opcua-PC by user opcua at 2015-11-18 11:21:45
*/

#ifndef UA_TRANSPORT_GENERATED_H_
#define UA_TRANSPORT_GENERATED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ua_types.h"
#ifdef UA_INTERNAL
#include "ua_types_encoding_binary.h"
#endif
#include "ua_types_generated.h"


/**
* @ingroup types
*
* @defgroup ua_transport_generated Autogenerated ua_transport Types
*
* @brief Data structures that are autogenerated from an XML-Schema.
*
* @{
*/

#define UA_TRANSPORT_COUNT 10

extern UA_EXPORT const UA_DataType *UA_TRANSPORT;


/** @brief Message Type and whether the message contains the final chunk */
typedef enum { 
    UA_MESSAGETYPEANDFINAL_ACKF = 1179337537,
    UA_MESSAGETYPEANDFINAL_CLOF = 1179601987,
    UA_MESSAGETYPEANDFINAL_HELF = 1179403592,
    UA_MESSAGETYPEANDFINAL_MSGF = 1179079501,
    UA_MESSAGETYPEANDFINAL_OPNF = 1179537487
} UA_MessageTypeAndFinal;
#define UA_TRANSPORT_MESSAGETYPEANDFINAL 0
static UA_INLINE UA_MessageTypeAndFinal * UA_MessageTypeAndFinal_new(void) { UA_MessageTypeAndFinal *v = (UA_MessageTypeAndFinal*)UA_Int32_new(); return v; }
static UA_INLINE void UA_MessageTypeAndFinal_init(UA_MessageTypeAndFinal *p) { UA_Int32_init((UA_Int32*)p); }
static UA_INLINE void UA_MessageTypeAndFinal_delete(UA_MessageTypeAndFinal *p) { UA_Int32_delete((UA_Int32*)p); }
static UA_INLINE void UA_MessageTypeAndFinal_deleteMembers(UA_MessageTypeAndFinal *p) { }
static UA_INLINE UA_StatusCode UA_MessageTypeAndFinal_copy(const UA_MessageTypeAndFinal *src, UA_MessageTypeAndFinal *dst) { return UA_Int32_copy((const UA_Int32*)src, (UA_Int32*)dst); }

/** @brief TCP Header */
typedef struct {
    UA_UInt32 messageTypeAndFinal;
    UA_UInt32 messageSize;
} UA_TcpMessageHeader;
#define UA_TRANSPORT_TCPMESSAGEHEADER 1
static UA_INLINE void UA_TcpMessageHeader_init(UA_TcpMessageHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE void UA_TcpMessageHeader_delete(UA_TcpMessageHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE void UA_TcpMessageHeader_deleteMembers(UA_TcpMessageHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE UA_TcpMessageHeader * UA_TcpMessageHeader_new(void) { return (UA_TcpMessageHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }
static UA_INLINE UA_StatusCode UA_TcpMessageHeader_copy(const UA_TcpMessageHeader *src, UA_TcpMessageHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPMESSAGEHEADER]); }

/** @brief Hello Message */
typedef struct {
    UA_UInt32 protocolVersion;
    UA_UInt32 receiveBufferSize;
    UA_UInt32 sendBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
    UA_String endpointUrl;
} UA_TcpHelloMessage;
#define UA_TRANSPORT_TCPHELLOMESSAGE 2
static UA_INLINE void UA_TcpHelloMessage_init(UA_TcpHelloMessage *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE void UA_TcpHelloMessage_delete(UA_TcpHelloMessage *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE void UA_TcpHelloMessage_deleteMembers(UA_TcpHelloMessage *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE UA_TcpHelloMessage * UA_TcpHelloMessage_new(void) { return (UA_TcpHelloMessage *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }
static UA_INLINE UA_StatusCode UA_TcpHelloMessage_copy(const UA_TcpHelloMessage *src, UA_TcpHelloMessage *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPHELLOMESSAGE]); }

/** @brief Acknowledge Message */
typedef struct {
    UA_UInt32 protocolVersion;
    UA_UInt32 receiveBufferSize;
    UA_UInt32 sendBufferSize;
    UA_UInt32 maxMessageSize;
    UA_UInt32 maxChunkCount;
} UA_TcpAcknowledgeMessage;
#define UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE 3
static UA_INLINE void UA_TcpAcknowledgeMessage_init(UA_TcpAcknowledgeMessage *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE void UA_TcpAcknowledgeMessage_delete(UA_TcpAcknowledgeMessage *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE void UA_TcpAcknowledgeMessage_deleteMembers(UA_TcpAcknowledgeMessage *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE UA_TcpAcknowledgeMessage * UA_TcpAcknowledgeMessage_new(void) { return (UA_TcpAcknowledgeMessage *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }
static UA_INLINE UA_StatusCode UA_TcpAcknowledgeMessage_copy(const UA_TcpAcknowledgeMessage *src, UA_TcpAcknowledgeMessage *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_TCPACKNOWLEDGEMESSAGE]); }

/** @brief Secure Layer Sequence Header */
typedef struct {
    UA_TcpMessageHeader messageHeader;
    UA_UInt32 secureChannelId;
} UA_SecureConversationMessageHeader;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER 4
static UA_INLINE void UA_SecureConversationMessageHeader_init(UA_SecureConversationMessageHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE void UA_SecureConversationMessageHeader_delete(UA_SecureConversationMessageHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE void UA_SecureConversationMessageHeader_deleteMembers(UA_SecureConversationMessageHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE UA_SecureConversationMessageHeader * UA_SecureConversationMessageHeader_new(void) { return (UA_SecureConversationMessageHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageHeader_copy(const UA_SecureConversationMessageHeader *src, UA_SecureConversationMessageHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEHEADER]); }

/** @brief Security Header */
typedef struct {
    UA_ByteString securityPolicyUri;
    UA_ByteString senderCertificate;
    UA_ByteString receiverCertificateThumbprint;
} UA_AsymmetricAlgorithmSecurityHeader;
#define UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER 5
static UA_INLINE void UA_AsymmetricAlgorithmSecurityHeader_init(UA_AsymmetricAlgorithmSecurityHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_AsymmetricAlgorithmSecurityHeader_delete(UA_AsymmetricAlgorithmSecurityHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_AsymmetricAlgorithmSecurityHeader_deleteMembers(UA_AsymmetricAlgorithmSecurityHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_AsymmetricAlgorithmSecurityHeader * UA_AsymmetricAlgorithmSecurityHeader_new(void) { return (UA_AsymmetricAlgorithmSecurityHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_StatusCode UA_AsymmetricAlgorithmSecurityHeader_copy(const UA_AsymmetricAlgorithmSecurityHeader *src, UA_AsymmetricAlgorithmSecurityHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_ASYMMETRICALGORITHMSECURITYHEADER]); }

/** @brief Secure Layer Symmetric Algorithm Header */
typedef struct {
    UA_UInt32 tokenId;
} UA_SymmetricAlgorithmSecurityHeader;
#define UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER 6
static UA_INLINE void UA_SymmetricAlgorithmSecurityHeader_init(UA_SymmetricAlgorithmSecurityHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_SymmetricAlgorithmSecurityHeader_delete(UA_SymmetricAlgorithmSecurityHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE void UA_SymmetricAlgorithmSecurityHeader_deleteMembers(UA_SymmetricAlgorithmSecurityHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_SymmetricAlgorithmSecurityHeader * UA_SymmetricAlgorithmSecurityHeader_new(void) { return (UA_SymmetricAlgorithmSecurityHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }
static UA_INLINE UA_StatusCode UA_SymmetricAlgorithmSecurityHeader_copy(const UA_SymmetricAlgorithmSecurityHeader *src, UA_SymmetricAlgorithmSecurityHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SYMMETRICALGORITHMSECURITYHEADER]); }

/** @brief Secure Layer Sequence Header */
typedef struct {
    UA_UInt32 sequenceNumber;
    UA_UInt32 requestId;
} UA_SequenceHeader;
#define UA_TRANSPORT_SEQUENCEHEADER 7
static UA_INLINE void UA_SequenceHeader_init(UA_SequenceHeader *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE void UA_SequenceHeader_delete(UA_SequenceHeader *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE void UA_SequenceHeader_deleteMembers(UA_SequenceHeader *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE UA_SequenceHeader * UA_SequenceHeader_new(void) { return (UA_SequenceHeader *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }
static UA_INLINE UA_StatusCode UA_SequenceHeader_copy(const UA_SequenceHeader *src, UA_SequenceHeader *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SEQUENCEHEADER]); }

/** @brief Secure Conversation Message Footer */
typedef struct {
    UA_Int32 paddingSize;
    UA_Byte *padding;
    UA_Byte signature;
} UA_SecureConversationMessageFooter;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER 8
static UA_INLINE void UA_SecureConversationMessageFooter_init(UA_SecureConversationMessageFooter *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE void UA_SecureConversationMessageFooter_delete(UA_SecureConversationMessageFooter *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE void UA_SecureConversationMessageFooter_deleteMembers(UA_SecureConversationMessageFooter *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE UA_SecureConversationMessageFooter * UA_SecureConversationMessageFooter_new(void) { return (UA_SecureConversationMessageFooter *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageFooter_copy(const UA_SecureConversationMessageFooter *src, UA_SecureConversationMessageFooter *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEFOOTER]); }

/** @brief Secure Conversation Message Abort Body */
typedef struct {
    UA_UInt32 error;
    UA_String reason;
} UA_SecureConversationMessageAbortBody;
#define UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY 9
static UA_INLINE void UA_SecureConversationMessageAbortBody_init(UA_SecureConversationMessageAbortBody *p) { UA_init(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE void UA_SecureConversationMessageAbortBody_delete(UA_SecureConversationMessageAbortBody *p) { UA_delete(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE void UA_SecureConversationMessageAbortBody_deleteMembers(UA_SecureConversationMessageAbortBody *p) { UA_deleteMembers(p, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE UA_SecureConversationMessageAbortBody * UA_SecureConversationMessageAbortBody_new(void) { return (UA_SecureConversationMessageAbortBody *) UA_new(&UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }
static UA_INLINE UA_StatusCode UA_SecureConversationMessageAbortBody_copy(const UA_SecureConversationMessageAbortBody *src, UA_SecureConversationMessageAbortBody *dst) { return UA_copy(src, dst, &UA_TRANSPORT[UA_TRANSPORT_SECURECONVERSATIONMESSAGEABORTBODY]); }

/// @} /* end of group */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* UA_TRANSPORT_GENERATED_H_ */
