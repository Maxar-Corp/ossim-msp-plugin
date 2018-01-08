//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************
#include <common/PhotoBlock.h>
#include <iostream>
#include <ossim/base/ossimException.h>

using namespace std;
using namespace ossimMsp;

int main(int argc, char** argv)
{
	clog << "JSON Test" << endl;
	if (argc < 2)
	{
	   clog<<"\nUsage: "<<argv[0]<<" <json-file>\n"<<endl;
	   return 0;
	}

	ostringstream xmsg;
   try
   {
      Json::Value queryRoot;
      string fname_in (argv[1]);
      ifstream jsonFile (fname_in);
      if (jsonFile.fail())
      {
         xmsg<<"Error opening JSON input file <"<<fname_in<<">.";
         throw ossimException(xmsg.str());
      }
      jsonFile>>queryRoot;

      // Fetch photoblock node:
      Json::Value& pbJson = queryRoot["photoblock"];
      if (pbJson.isNull())
      {
         xmsg<<"Failed to parse JSON photoblock node. \n"<<endl;
         throw ossimException(xmsg.str());
      }

      PhotoBlock pb (pbJson);
      Json::Value regurg;
      pb.saveJSON(regurg);

      // Serialize JSON object for return:
      clog << "ECHO:\n"<<regurg<<endl;
      string fname_out ("regurgitated-"+fname_in);
      ofstream outfile (fname_out);
      if (outfile.fail())
      {
         xmsg<<"Error opening JSON output file <"<<fname_out<<">.";
         throw ossimException(xmsg.str());
      }
      outfile<<regurg<<endl;

      outfile.close();
      jsonFile.close();
   }
   catch(exception &mspError)
   {
       clog<<"Exception: "<<mspError.what()<<endl;;
   }

	return 0;
}
