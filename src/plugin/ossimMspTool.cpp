//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************

#include "ossimMspTool.h"
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimException.h>
#include <services/SensorModelService.h>
#include <services/VersionService.h>
#include <services/SourceSelectionService.h>
#include <services/TriangulationService.h>
#include <services/MensurationService.h>

#include <iostream>
#include <memory>
#include <sstream>

using namespace std;

namespace ossimMsp
{
const char* ossimMspTool::DESCRIPTION =
      "Provides access to MSP functionality (source selection, sensor models, triangulation, mensuration).";

ossimMspTool::ossimMspTool()
: m_outputStream (0),
  m_verbose (false)
{
}

ossimMspTool::~ossimMspTool()
{
}

void ossimMspTool::setUsage(ossimArgumentParser& ap)
{
   // Add global usage options. Don't include ossimChipProcUtil options as not appropriate.
   ossimTool::setUsage(ap);

   // Set the general usage:
   ossimApplicationUsage* au = ap.getApplicationUsage();
   ossimString usageString = ap.getApplicationName();
   usageString += " msp [options] \n\n";
   usageString +=
         "Accesses all ossim-msp functionality given JSON request on stdin (or input file if\n"
         "-i specified). The response JSON is output to stdout unless -o option is specified.\n";
   au->setCommandLineUsage(usageString);

   // Set the command line options:
   au->addCommandLineOption("-i <filename>",
         "Reads request JSON from the input file specified instead of stdin.");
   au->addCommandLineOption("-o <filename>",
         "Outputs response JSON to the output file instead of stdout.");
   au->addCommandLineOption("-v",
         "Verbose. All non-response (debug) output to stdout is enabled.");
}

bool ossimMspTool::initialize(ossimArgumentParser& ap)
{
   string ts1;
   ossimArgumentParser::ossimParameter sp1(ts1);

   if (!ossimTool::initialize(ap))
      return false;
   if (m_helpRequested)
      return true;

   if ( ap.read("-v"))
      m_verbose = true;

   if ( ap.read("-i", sp1))
   {
      ifstream s (ts1);
      if (s.fail())
      {
         ossimNotify(ossimNotifyLevel_FATAL)<<__FILE__<<" Could not open input file <"<<ts1<<">";
         return false;
      }
      try
      {
         Json::Value queryJson;
         s >> queryJson;
         loadJSON(queryJson);
      }
      catch (exception& e)
      {
         ossimNotify(ossimNotifyLevel_FATAL)<<__FILE__<<" Could not parse input JSON <"<<ts1<<">";
         return false;
      }
   }

   if ( ap.read("-o", sp1))
   {
      ofstream* s = new ofstream (ts1);
      if (s->fail())
      {
         ossimNotify(ossimNotifyLevel_FATAL)<<__FILE__<<" Could not open output file <"<<ts1<<">";
         return false;
      }
      m_outputStream = s;
   }
   else
      m_outputStream = &cout;

   ap.reportRemainingOptionsAsUnrecognized();
   if (ap.errors())
   {
      ap.writeErrorMessages(clog);
      setUsage(ap);
      ap.getApplicationUsage()->write(ossimNotify(ossimNotifyLevel_INFO));
      return false;
   }

   return true;
}

void ossimMspTool::initialize(const ossimKeywordlist& kwl)
{
   ossimString value;
   ostringstream xmsg;

   // Don't copy KWL if member KWL passed in:
   if (&kwl != &m_kwl)
   {
      // Start with clean options keyword list.
      m_kwl.clear();
      m_kwl.addList( kwl, true );
   }

   ossimTool::initialize(kwl);
}

void ossimMspTool::loadJSON(const Json::Value& queryRoot)
{
   ostringstream xmsg;
   xmsg<<"ossimMspTool::loadJSON()  ";

   try
   {
      // Fetch service name and route accordingly:
      string serviceName = queryRoot["service"].asString();
      if (serviceName      == "getVersion")
         m_mspService.reset(new VersionService);
      else if (serviceName == "sensorModels")
         m_mspService.reset(new SensorModelService);
      else if (serviceName == "sourceSelection")
         m_mspService.reset(new SourceSelectionService);
      else if (serviceName == "triangulation")
         m_mspService.reset(new TriangulationService);
      else if (serviceName == "mensuration")
         m_mspService.reset(new MensurationService);
      else
      {
         xmsg<<"Unsupported service <"<<serviceName<<"> requested."<<endl;
         throw ossimException(xmsg.str());
      }

      m_mspService->loadJSON(queryRoot);
   }
   catch(ossimException &e)
   {
      cerr<<"Exception: "<<e.what()<<endl;
      *m_outputStream<<"{ \"ERROR\": \"" << e.what() << "\" }\n"<<endl;
   }
}

bool ossimMspTool::execute()
{
   ostringstream xmsg;
   xmsg<<"ossimMspTool::execute()  ";

   if (helpRequested())
      return true;

   if (!m_mspService)
      return false;

   try
   {
      // Call actual service and get response:
      m_mspService->execute();
      m_responseJSON.clear();
      m_mspService->saveJSON(m_responseJSON);

      // Serialize JSON object for return:
      if (m_outputStream)
         (*m_outputStream) << m_responseJSON;
   }
   catch(ossimException &e)
   {
      cerr<<"Exception: "<<e.what()<<endl;
      if (m_outputStream)
         *m_outputStream<<"{ \"ERROR\": \"" << e.what() << "\" }\n"<<endl;
   }

   // close any open file streams:
   if (m_outputStream)
   {
      ofstream* so = dynamic_cast<ofstream*>(m_outputStream);
      if (so)
      {
         so->close();
         delete so;
      }
   }
   return true;
}

void ossimMspTool::getKwlTemplate(ossimKeywordlist& kwl)
{
}

}


