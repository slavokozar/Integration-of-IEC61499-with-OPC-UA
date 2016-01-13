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

#ifndef _OP_UA_SUBSCRIBE_H_
#define _OP_UA_SUBSCRIBE_H_

#include <funcbloc.h>
#include <forte_any.h>

#ERROR type contains variables of type ANY. Please check the usage of these variables as we can not gurantee correct usage on export!

#include <forte_wstring.h>

class FORTE_OP_UA_SUBSCRIBE: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_OP_UA_SUBSCRIBE)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_WSTRING &SERVER() {
    return *static_cast<CIEC_WSTRING*>(getDI(0));
  };

  CIEC_WSTRING &NODE() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  static const CStringDictionary::TStringId scm_anDataOutputNames[];
  static const CStringDictionary::TStringId scm_anDataOutputTypeIds[];
  CIEC_ANY &OUT() {
    return *static_cast<CIEC_ANY*>(getDO(0));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventRESPID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const TDataIOID scm_anEOWith[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 2, 1, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_OP_UA_SUBSCRIBE){
  };

  virtual ~FORTE_OP_UA_SUBSCRIBE(){};

};

#endif //close the ifdef sequence from the beginning of the file

