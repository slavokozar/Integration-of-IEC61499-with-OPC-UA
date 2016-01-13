/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OP_UA_SUBSCRIBE
 *** Description: Read value from OPC UA server
 *** Version:
 ***     1.0: 2015-11-08/Slavomir Kozar - Certicon -
 *************************************************************************/

#include "OP_UA_SUBSCRIBE.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OP_UA_SUBSCRIBE_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OP_UA_SUBSCRIBE, g_nStringIdOP_UA_SUBSCRIBE)

const CStringDictionary::TStringId FORTE_OP_UA_SUBSCRIBE::scm_anDataInputNames[] = {g_nStringIdSERVER, g_nStringIdNODE};

const CStringDictionary::TStringId FORTE_OP_UA_SUBSCRIBE::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING};

const CStringDictionary::TStringId FORTE_OP_UA_SUBSCRIBE::scm_anDataOutputNames[] = {g_nStringIdOUT};

const CStringDictionary::TStringId FORTE_OP_UA_SUBSCRIBE::scm_anDataOutputTypeIds[] = {g_nStringIdANY};

const TForteInt16 FORTE_OP_UA_SUBSCRIBE::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_OP_UA_SUBSCRIBE::scm_anEIWith[] = {0, 1, 255};
const CStringDictionary::TStringId FORTE_OP_UA_SUBSCRIBE::scm_anEventInputNames[] = {g_nStringIdREQ};

const TDataIOID FORTE_OP_UA_SUBSCRIBE::scm_anEOWith[] = {0, 255};
const TForteInt16 FORTE_OP_UA_SUBSCRIBE::scm_anEOWithIndexes[] = {0, -1};
const CStringDictionary::TStringId FORTE_OP_UA_SUBSCRIBE::scm_anEventOutputNames[] = {g_nStringIdRESP};

const SFBInterfaceSpec FORTE_OP_UA_SUBSCRIBE::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  scm_anEOWith, scm_anEOWithIndexes,  2,  scm_anDataInputNames, scm_anDataInputTypeIds,
  1,  scm_anDataOutputNames, scm_anDataOutputTypeIds,
  0, 0
};


void FORTE_OP_UA_SUBSCRIBE::executeEvent(int pa_nEIID){
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
  }
}



