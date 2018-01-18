//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************



// TODO: Consider consolidating MSP and CSM plugins into one, and putting all
// MSP interface functionality there. Because I need CK's code to create CSM model from image
// filename.


#include <services/VersionService.h>
#include <Config.h>
#include <ossim/ossimVersion.h>
#include <MSPVersionUtils/MSPVersionInformation.h>
#include <sstream>

using namespace std;

namespace ossimMsp
{
void VersionService::saveJSON(Json::Value& responseJson) const
{
   string MSPServicesVersion = MSP::MSPVersionInformation::getMSPServicesVersionsString();
   string MSPPluginsVersions = MSP::MSPVersionInformation::getMSPPluginsVersionsString();

   // Represent results in response JSON:
   responseJson["ossimVersion"] = OSSIM_VERSION;
   responseJson["ossimGitRevision"] = OSSIM_REVISION;
   responseJson["ossimBuildDate"] = OSSIM_BUILD_DATE;
   responseJson["ossimMspPluginGitRevision"] = OSSIM_MSP_GIT_REVISION;
   responseJson["ossimMspPluginBuildDate"] = OSSIM_MSP_BUILD_DATE;
   responseJson["MSPServicesVersion"] = MSPServicesVersion;
   responseJson["MSPPluginsVersions"] = MSPPluginsVersions;
}
}

