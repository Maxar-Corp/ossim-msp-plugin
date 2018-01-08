//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************

#ifndef ossimIsaTool_HEADER
#define ossimIsaTool_HEADER 1

#include <ossim/plugin/ossimPluginConstants.h>
#include <ossim/imaging/ossimImageHandler.h>
#include <ossim/util/ossimTool.h>
#include <services/ServiceBase.h>
#include <iostream>

namespace ossimMsp
{
class OSSIM_DLL ossimMspTool : public ossimTool
{
public:
   static const char* DESCRIPTION;

   ossimMspTool();

   virtual ~ossimMspTool();

   virtual void setUsage(ossimArgumentParser& ap);

   virtual bool initialize(ossimArgumentParser& ap);

   virtual void initialize(const ossimKeywordlist& ap);

   virtual bool execute();

   virtual ossimString getClassName() const { return "ossimMspTool"; }

   virtual void getKwlTemplate(ossimKeywordlist& kwl);

   virtual void loadJSON(const Json::Value& json);

   virtual void saveJSON(Json::Value& json) const { json = m_responseJSON; }

private:
   std::ostream* m_outputStream;
   bool m_verbose;
   shared_ptr<ServiceBase> m_mspService;
   Json::Value m_responseJSON;
};
}
#endif /* #ifndef ossimIsaTool_HEADER */
