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

#ifndef _OPC_UA_PUBLISH_H_
#define _OPC_UA_PUBLISH_H_

#include <funcbloc.h>
#include <forte_any.h>


class FORTE_OPC_UA_PUBLISH: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_OPC_UA_PUBLISH)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_ANY &IN() {
    return *static_cast<CIEC_ANY*>(getDI(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 1, 0, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_OPC_UA_PUBLISH){
  };

  virtual ~FORTE_OPC_UA_PUBLISH(){};

};

#endif //close the ifdef sequence from the beginning of the file

