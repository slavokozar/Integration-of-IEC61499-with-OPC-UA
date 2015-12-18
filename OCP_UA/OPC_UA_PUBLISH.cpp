/*************************************************************************
 *** FORTE Library Element
 ***
 *** This file was generated using the 4DIAC FORTE Export Filter V1.0.x!
 ***
 *** Name: OPC_UA_PUBLISH
 *** Description: Read value from OPC UA server
 *** Version:
 ***     1.0: 2015-11-08/Slavomir Kozar - Certicon -
 *************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "open62541.h"

#include "OPC_UA_PUBLISH.h"
#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "OPC_UA_PUBLISH_gen.cpp"
#endif


DEFINE_FIRMWARE_FB(FORTE_OPC_UA_PUBLISH, g_nStringIdOPC_UA_PUBLISH)

const CStringDictionary::TStringId FORTE_OPC_UA_PUBLISH::scm_anDataInputNames[] = {g_nStringIdIN};

const CStringDictionary::TStringId FORTE_OPC_UA_PUBLISH::scm_anDataInputTypeIds[] = {g_nStringIdANY};

const TForteInt16 FORTE_OPC_UA_PUBLISH::scm_anEIWithIndexes[] = {0};
const TDataIOID FORTE_OPC_UA_PUBLISH::scm_anEIWith[] = {0, 255};
const CStringDictionary::TStringId FORTE_OPC_UA_PUBLISH::scm_anEventInputNames[] = {g_nStringIdREQ};

const TForteInt16 FORTE_OPC_UA_PUBLISH::scm_anEOWithIndexes[] = {-1, -1};
const CStringDictionary::TStringId FORTE_OPC_UA_PUBLISH::scm_anEventOutputNames[] = {g_nStringIdCNF};

const SFBInterfaceSpec FORTE_OPC_UA_PUBLISH::scm_stFBInterfaceSpec = {
  1,  scm_anEventInputNames,  scm_anEIWith,  scm_anEIWithIndexes,
  1,  scm_anEventOutputNames,  0, 0,  1,  scm_anDataInputNames, scm_anDataInputTypeIds,
  0,  0, 0,
  0, 0
};




UA_Boolean write_value = 0;

int write_type = UA_NS0ID_BOOLEAN;
//int write_type = UA_NS0ID_SBYTE;
//int write_type = UA_NS0ID_BYTE;
//int write_type = UA_NS0ID_INT16;
//int write_type = UA_NS0ID_UINT16;
//int write_type = UA_NS0ID_INT32;
//int write_type = UA_NS0ID_UINT32;
//int write_type = UA_NS0ID_INT64;
//int write_type = UA_NS0ID_UINT64;
//int write_type = UA_NS0ID_FLOAT;
//int write_type = UA_NS0ID_DOUBLE;
//int write_type = UA_NS0ID_STRING;



UA_NodeId createFBNode(UA_Client *client, char *fb_id){
        UA_NodeId obj_id;
        UA_ObjectAttributes obj_attr;
        UA_ObjectAttributes_init(&obj_attr);

        char display[32];
        sprintf(display, "FB-%s", fb_id);

        obj_attr.displayName = UA_LOCALIZEDTEXT("en_US", display);

        UA_StatusCode retval = UA_Client_addObjectNode(
                                            client,
                                            UA_NODEID_STRING_ALLOC(1, fb_id),
                                            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                            UA_QUALIFIEDNAME(0, fb_id),
                                            UA_NODEID_NULL, // no variable type
                                            obj_attr, &obj_id
                                        );
        if(retval == UA_STATUSCODE_GOOD )
            printf("Created new object %s \n", fb_id);
        else
            printf("Error creating object : %x\n", retval);

        return obj_id;
}

UA_NodeId createVariableNode(UA_Client *client, UA_NodeId parentNodeId, char *fb_id, char *var_id){
        UA_NodeId obj_id;
        UA_VariableAttributes var_attr;
        UA_VariableAttributes_init(&var_attr);

        char display[32];
        sprintf(display, "Variable-%s", var_id);
        var_attr.displayName = UA_LOCALIZEDTEXT("en_US", display);

        char browse[32];
        sprintf(browse, "%s.%s", fb_id, var_id);

        UA_Variant_setScalarCopy(&var_attr.value, &write_value, &UA_TYPES[write_type]);

        UA_StatusCode retval = UA_Client_addVariableNode(
                                            client,
                                            UA_NODEID_STRING_ALLOC(1, browse),
                                            UA_NODEID_STRING_ALLOC(1, fb_id),
                                            UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
                                            UA_QUALIFIEDNAME(0, browse),
                                            UA_NODEID_NULL, // no variable type
                                            var_attr, &obj_id
                                        );
        if(retval == UA_STATUSCODE_GOOD )
            printf("Created new object %s \n", browse);
        else
            printf("Error creating object : %x\n", retval);

        return obj_id;
}




void FORTE_OPC_UA_PUBLISH::executeEvent(int pa_nEIID){
  switch(pa_nEIID){
    case scm_nEventREQID:

        TDataConnectionPtr connection = m_apoDIConns[0];
        if(connection->isConnected()){
            DEVLOG_INFO("OPC UA connected\n");

            const SConnectionPoint& sourcePoint = static_cast<TDataConnectionPtr>(connection)->getSourceId();

            CFunctionBlock *sourceFB = sourcePoint.mFB;

            CStringDictionary::TStringId sourceFBTypeId = sourceFB->getFBTypeId();

            CStringDictionary::TStringId sourceFBNameId = sourceFB->getInstanceNameId();
            const char *sourceFBName = CStringDictionary::getInstance().get(sourceFBNameId);

            const SFBInterfaceSpec* interfaceSpec = sourceFB->getFBInterfaceSpec();
            CStringDictionary::TStringId sourceDataPointNameId  = interfaceSpec->m_aunDONames[sourcePoint.mPortId];
            const char *sourceDataPointName = CStringDictionary::getInstance().get(sourceDataPointNameId);

            CStringDictionary::TStringId sourceDataPointTypeNameId  = interfaceSpec->m_aunDODataTypeNames[sourcePoint.mPortId];
            const char *sourceDataPointTypeName = CStringDictionary::getInstance().get(sourceDataPointTypeNameId);

            char message[128];
            sprintf(message,"%s %s %s\n", sourceFBName, sourceDataPointName, sourceDataPointTypeName);
            DEVLOG_INFO(message);

            char *fb_id;
            strcpy(fb_id, sourceFBName);

            char *var_id;
            strcpy(var_id, sourceDataPointName);

            char varBrowse_id[64];
            sprintf(varBrowse_id, "%s.%s", fb_id, var_id);

            UA_Client *client = UA_Client_new(UA_ClientConfig_standard, Logger_Stdout);
            UA_StatusCode retval = UA_Client_connect(client, ClientNetworkLayerTCP_connect, "opc.tcp://localhost:16664");

            if(retval != UA_STATUSCODE_GOOD) {
                UA_Client_delete(client);
                sendOutputEvent(scm_nEventCNFID);
                break;
            }

            char node_id_string[64];
            int node_found = 0;


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

                    sprintf(node_id_string, "%.*s", ref->nodeId.nodeId.identifier.string.length, ref->nodeId.nodeId.identifier.string.data);
                    if(!strcmp(fb_id, node_id_string)){
                        node_found = 1;
                    }
                }
            }

            UA_BrowseRequest_deleteMembers(&bReq);
            UA_BrowseResponse_deleteMembers(&bResp);

            if(!node_found){
                DEVLOG_INFO("OPC UA FB OBJECT create\n");

                /* New Object */
                UA_NodeId fbNode_id = createFBNode(client, fb_id);
                /* New Variable */
                UA_NodeId varNode_id = createVariableNode(client, fbNode_id, fb_id, var_id);
            }else{
                UA_NodeId fbNode_id = UA_NODEID_STRING_ALLOC(1, fb_id);

                int var_found = 0;

                UA_BrowseRequest bReq;
                UA_BrowseRequest_init(&bReq);
                bReq.requestedMaxReferencesPerNode = 0;
                bReq.nodesToBrowse = UA_BrowseDescription_new();
                bReq.nodesToBrowseSize = 1;
                bReq.nodesToBrowse[0].nodeId = fbNode_id;
                bReq.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_ALL; //revarBrowse_idturn everything

                UA_BrowseResponse bResp = UA_Client_Service_browse(client, bReq);

                for (int i = 0; i < bResp.resultsSize; ++i) {
                    for (int j = 0; j < bResp.results[i].referencesSize; ++j) {
                        UA_ReferenceDescription *ref = &(bResp.results[i].references[j]);

                        sprintf(node_id_string, "%.*s", ref->nodeId.nodeId.identifier.string.length, ref->nodeId.nodeId.identifier.string.data);
                        if(!strcmp(varBrowse_id, node_id_string)){
                            var_found = 1;
                        }
                    }
                }

                UA_BrowseRequest_deleteMembers(&bReq);
                UA_BrowseResponse_deleteMembers(&bResp);

                if(var_found){
                    printf("Variable node found\n");

                    UA_StatusCode retval = UA_Client_deleteNode(
                                                                client,
                                                                UA_NODEID_STRING_ALLOC(1, varBrowse_id),
                                                                false
                                                                );

                    if(retval == UA_STATUSCODE_GOOD )
                        printf("Variable removed %s \n", varBrowse_id);
                    else
                        printf("Error removing variable : %x\n", retval);
                }
                 UA_NodeId varNode_id = createVariableNode(client, fbNode_id, fb_id, var_id);
            }



            UA_Client_disconnect(client);
            UA_Client_delete(client);


        }else{
            DEVLOG_INFO("OPC UA not - connected\n");
        }

      sendOutputEvent(scm_nEventCNFID);
      break;
  }
}



