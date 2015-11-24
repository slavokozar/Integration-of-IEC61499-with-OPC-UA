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
        UA_Client *client = UA_Client_new(UA_ClientConfig_standard, Logger_Stdout_new());
        UA_StatusCode retval = UA_Client_connect(client, ClientNetworkLayerTCP_connect, "opc.tcp://localhost:16664");
        if(retval != UA_STATUSCODE_GOOD) {
            UA_Client_delete(client);
        }

        UA_Client_disconnect(client);
        UA_Client_delete(client);

     sendOutputEvent(scm_nEventCNFID);

    break;
  }
}



