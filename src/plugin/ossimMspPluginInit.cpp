//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************

#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossim/plugin/ossimPluginConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/util/ossimToolRegistry.h>
#include "ossimMspToolFactory.h"

extern "C"
{
ossimSharedObjectInfo  mspInfo;

const char* getMspDescription()
{
   return "ossim-msp tool plugin\n\n";
}

int getMspNumberOfClassNames()
{
   return 1;
}

const char* getMspClassName(int idx)
{
   if (idx == 0)
   {
      return "ossimMspTool";
   }
   return (const char*)0;
}

/* Note symbols need to be exported on windoze... */
OSSIM_PLUGINS_DLL void ossimSharedLibraryInitialize(ossimSharedObjectInfo** info)
{
   mspInfo.getDescription = getMspDescription;
   mspInfo.getNumberOfClassNames = getMspNumberOfClassNames;
   mspInfo.getClassName = getMspClassName;

   *info = &mspInfo;

   /* Register the utility... */
   ossimToolRegistry::instance()->
         registerFactory(ossimMsp::ossimMspToolFactory::instance());

}

/* Note symbols need to be exported on windoze... */
OSSIM_PLUGINS_DLL void ossimSharedLibraryFinalize()
{
   ossimToolRegistry::instance()->unregisterFactory(ossimMsp::ossimMspToolFactory::instance());
}
}
