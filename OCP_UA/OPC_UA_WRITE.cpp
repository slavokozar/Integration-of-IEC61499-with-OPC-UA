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
#include <string.h>
#include <inttypes.h>

# include "ua_types.h"
# include "ua_server.h"
# include "ua_client.h"
# include "ua_client_highlevel.h"
# include "ua_nodeids.h"
# include "networklayer_tcp.h"
# include "logger_stdout.h"


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

        UA_Client *client = UA_Client_new(UA_ClientConfig_standard, Logger_Stdout);
        UA_StatusCode retval = UA_Client_connect(client, ClientNetworkLayerTCP_connect,"opc.tcp://localhost:16664");

        if(retval != UA_STATUSCODE_GOOD) {
            UA_Client_delete(client);
            return retval;
        }

        UA_Int32 write_value = IN();

        int node_found = 0;
        int node_type_numeric = 0;
        int node_type_string = 1;

        int node_reference_numeric = 0;
        const char *node_reference_string = "theanswer";

        char node_id_string[16];

        UA_BrowseRequest bReq;
        UA_BrowseRequest_init(&bReq);
        bReq.requestedMaxReferencesPerNode = 0;
        bReq.nodesToBrowse = UA_BrowseDescription_new();
        bReq.nodesToBrowseSize = 1;
        bReq.nodesToBrowse[0].nodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER); //browse objects folder
        bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; //return everything

        UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

        for (int i = 0; i < bResp.resultsSize; ++i) {
            for (int j = 0; j < bResp.results[i].referencesSize; ++j) {
                UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);

                if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_NUMERIC) {
                    if(node_type_numeric == 1 && node_reference_numeric == ref->nodeId.nodeId.identifier.numeric){
                        node_found = 1;
                    }
                }else if(ref->nodeId.nodeId.identifierType == UA_NODEIDTYPE_STRING) {
                    sprintf(node_id_string, "%.*s", ref->nodeId.nodeId.identifier.string.length, ref->nodeId.nodeId.identifier.string.data);
                    if(node_type_string == 1 && !strcmp(node_reference_string, node_id_string)){
                        node_found = 1;
                    }
                }
            }
        }
        UA_BrowseRequest_deleteMembers(&bReq);
        UA_BrowseResponse_deleteMembers(&bResp);

        if(!node_found){

            /* New Integer Variable */
            UA_NodeId var_id;
            UA_VariableAttributes var_attr;
            UA_VariableAttributes_init(&var_attr);
            var_attr.displayName = UA_LOCALIZEDTEXT("en_US", "IntegerVariableNode");
            var_attr.description = UA_LOCALIZEDTEXT("en_US", "Integer variable node");

            /* This does not copy the value */
            UA_Variant_setScalar(&var_attr.value, &write_value, &UA_TYPES[UA_TYPES_INT32]);
            var_attr.dataType = UA_TYPES[UA_TYPES_INT32].typeId;

            if(node_type_numeric == 1){
                retval = UA_Client_addVariableNode(client,
                    UA_NODEID_NUMERIC(1, node_reference_numeric),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                    UA_QUALIFIEDNAME(0, "VariableNode"),
                    UA_NODEID_NULL, // no variable type
                    var_attr, &var_id);


            }else if(node_type_string == 1 ){
                retval = UA_Client_addVariableNode(client,
                    UA_NODEID_STRING_ALLOC(1, node_reference_string),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                    UA_QUALIFIEDNAME(0, "VariableNode"),
                    UA_NODEID_NULL, // no variable type
                    var_attr, &var_id);

            }
        }else{
            UA_WriteRequest wReq;
            UA_WriteRequest_init(&wReq);
            wReq.nodesToWrite = UA_WriteValue_new();
            wReq.nodesToWriteSize = 1;

            if(node_type_numeric == 1){
                wReq.nodesToWrite[0].nodeId = UA_NODEID_NUMERIC(1, node_reference_numeric);
            }else if(node_type_string == 1){
                wReq.nodesToWrite[0].nodeId = UA_NODEID_NUMERIC(1, node_reference_numeric);
            }


            wReq.nodesToWrite[0].nodeId = UA_NODEID_NUMERIC(1, node_reference_numeric)
        }else if(node_type_string == 1 ){
            wReq.nodesToWrite[0].nodeId = UA_NODEID_STRING_ALLOC(1, node_reference_string);
        }
            wReq.nodesToWrite[0].attributeId = UA_ATTRIBUTEID_VALUE;
            wReq.nodesToWrite[0].value.hasValue = UA_TRUE;
            wReq.nodesToWrite[0].value.value.type = &UA_TYPES[UA_TYPES_INT32];
            wReq.nodesToWrite[0].value.value.storageType = UA_VARIANT_DATA_NODELETE; //do not free the integer on deletion
            wReq.nodesToWrite[0].value.value.data = &write_value;

            UA_WriteResponse wResp = UA_Client_Service_write(client, wReq);

            UA_WriteRequest_deleteMembers(&wReq);
            UA_WriteResponse_deleteMembers(&wResp);
        }

        UA_Client_disconnect(client);
        UA_Client_delete(client);

        sendOutputEvent(scm_nEventCNFID);
        break;
  }
}



