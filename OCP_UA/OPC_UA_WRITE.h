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

#ifndef _OPC_UA_WRITE_H_
#define _OPC_UA_WRITE_H_

#include <funcbloc.h>
#include <forte_wstring.h>
#include <forte_int.h>

class FORTE_OPC_UA_WRITE: public CFunctionBlock{
  DECLARE_FIRMWARE_FB(FORTE_OPC_UA_WRITE)

private:
  static const CStringDictionary::TStringId scm_anDataInputNames[];
  static const CStringDictionary::TStringId scm_anDataInputTypeIds[];
  CIEC_WSTRING &SERVER() {
    return *static_cast<CIEC_WSTRING*>(getDI(0));
  };

  CIEC_WSTRING &NODE() {
    return *static_cast<CIEC_WSTRING*>(getDI(1));
  };

  CIEC_INT &IN() {
    return *static_cast<CIEC_INT*>(getDI(2));
  };

  static const TEventID scm_nEventREQID = 0;
  static const TForteInt16 scm_anEIWithIndexes[];
  static const TDataIOID scm_anEIWith[];
  static const CStringDictionary::TStringId scm_anEventInputNames[];

  static const TEventID scm_nEventCNFID = 0;
  static const TForteInt16 scm_anEOWithIndexes[];
  static const CStringDictionary::TStringId scm_anEventOutputNames[];

  static const SFBInterfaceSpec scm_stFBInterfaceSpec;

   FORTE_FB_DATA_ARRAY(1, 3, 0, 0);

  void executeEvent(int pa_nEIID);

public:
  FUNCTION_BLOCK_CTOR(FORTE_OPC_UA_WRITE){
  };

  virtual ~FORTE_OPC_UA_WRITE(){};

};

#endif //close the ifdef sequence from the beginning of the file

