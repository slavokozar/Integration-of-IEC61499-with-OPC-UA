/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPC_UA_WRITE
 *** Description: Read value from OPC UA server
 *** Version:
 ***     1.0: 2015-11-08/Slavomir Kozar - Certicon -
 *************************************************************************/

#include "OPC_UA_WRITE.h"

#include <stdint.h>
#include <inttypes.h>

#include "open62541.h"


#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPC_UA_WRITE_gen.cpp"
#endif

DEFINE_FIRMWARE_FB(FORTE_OPC_UA_WRITE, g_nStringIdOPC_UA_WRITE)

const CStringDictionary::TStringId FORTE_OPC_UA_WRITE::scm_anDataInputNames[] = {g_nStringIdSERVER, g_nStringIdNODE, g_nStringIdIN};

const CStringDictionary::TStringId FORTE_OPC_UA_WRITE::scm_anDataInputTypeIds[] = {g_nStringIdWSTRING, g_nStringIdWSTRING, g_nStringIdINT};

const TForteInt16 FORTE_OPC_UA_WRITE::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_OPC_UA_WRITE::scm_anEIWith[] = {0, 1, 2, 255};
const CStringDictionary::TStringId FORTE_OPC_UA_WRITE::scm_anEventInputNames[] = {g_nStringIdREQ};

const TForteInt16 FORTE_OPC_UA_WRITE::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_OPC_UA_WRITE::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_OPC_UA_WRITE::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  3,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};


void FORTE_OPC_UA_WRITE::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:
        UA_Client *client = UA_Client_new(UA_ClientConfig_standard, Logger_Stdout_new());
        UA_StatusCode retval = UA_Client_connect(client, ClientNetworkLayerTCP_connect, "opc.tcp://localhost:16664");
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Client_delete(client);
            break;
        }

        //variable to store data
        UA_Int32 value = IN();


        UA_WriteRequest wReq;
        UA_WriteRequest_init(&wReq);
        wReq.nodesToWrite = UA_WriteValue_new();
        wReq.nodesToWriteSize = 1;
        wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(1, "the.answer"); /* assume this node exists */
        wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
        wReq.nodesToWrite[0].value.hasValue = UA_TRUE;
        wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT32];
        //wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; //do not free the integer on deletion
        wReq.nodesToWrite[0].value.value.data = &value;

        UA_WriteResponse wResp = UA_Client_write(client, &wReq);



        UA_WriteRequest_deleteMembers(&wReq);
        UA_WriteResponse_deleteMembers(&wResp);


        UA_Client_disconnect(client);
        UA_Client_delete(client);



        sendOutputEvent(scm_nEventCNFID);
        break;
  }
}



