/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPC_UA_READ
 *** Description: Read value from OPC UA server
 *** Version:
 ***     1.0: 2015-11-08/Slavomir Kozar - Certicon -
 *************************************************************************/


#include "OPC_UA_READ.h"

#include <stdint.h>
#include <inttypes.h>

#include "open62541.h"


#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPC_UA_READ_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OPC_UA_READ, g_nStringIdOPC_UA_READ)

const CStringDictionary::TStringId FORTE_OPC_UA_READ::scm_anDataInputNames[] = {g_nStringIdSERVER, g_nStringIdNODE};

    const CStringDictionary::TStringId FORTE_OPC_UA_READ::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_OPC_UA_READ::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_OPC_UA_READ::scm_anDataOutputTypeIds[] = {g_nStringIdINT};

const TForteInt16 FORTE_OPC_UA_READ::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_OPC_UA_READ::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_OPC_UA_READ::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_OPC_UA_READ::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_OPC_UA_READ::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OPC_UA_READ::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_OPC_UA_READ::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_OPC_UA_READ::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:

        UA_Int32 value = 0;
        OUT() = value;

        UA_Client *client = UA_Client_new(UA_ClientConfig_standard, Logger_Stdout_new());

        UA_StatusCode connected = UA_Client_connect(client, ClientNetworkLayerTCP_connect, SERVER());
        if(connected != UA_STATUSCODE_GOOD) {
            UA_Client_delete(client);
            sendOutputEvent(scm_nEventCNFID);
        }

        UA_ReadRequest rReq;
        UA_ReadRequest_init(&rReq);
        rReq.nodesToRead = UA_ReadValueId_new();
        rReq.nodesToReadSize = 1;
        rReq.nodesToRead[0].nodeId = UA_NODEID_STRING_ALLOC(1, NODE()); /* assume this node exists */
        rReq.nodesToRead[0].attributeId = UA_ATTRIBUTEID_VALUE;


        UA_ReadResponse rResp = UA_Client_read(client, &rReq);
        if(rResp.responseHeader.serviceResult == UA_STATUSCODE_GOOD &&
            rResp.resultsSize > 0 && rResp.results[0].hasValue &&
            UA_Variant_isScalar(&rResp.results[0].value) &&
            rResp.results[0].value.type == &UA_TYPES[UA_TYPES_INT32]) {
            value = *(UA_Int32*)rResp.results[0].value.data;
            OUT() = value;
        }else{
            sendOutputEvent(scm_nEventCNFID);

        }

        UA_ReadRequest_deleteMembers(&rReq);
        UA_ReadResponse_deleteMembers(&rResp);



        UA_Client_disconnect(client);
        UA_Client_delete(client);

        sendOutputEvent(scm_nEventCNFID);
        break;

    break;
  }
}



