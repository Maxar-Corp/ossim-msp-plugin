//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************

#include "MspPhotoBlock.h"
#include "MspImage.h"
#include <ossim/base/ossimException.h>

using namespace ossim;

namespace ossimMsp
{
MspPhotoBlock::MspPhotoBlock()
{
}

MspPhotoBlock::MspPhotoBlock(const Json::Value& pb_json_node)
{
   loadJSON(pb_json_node);
}

MspPhotoBlock::MspPhotoBlock(const MspPhotoBlock& copyThis)
: PhotoBlock (copyThis)
{
   *this = copyThis;
}

MspPhotoBlock::~MspPhotoBlock()
{
}

void MspPhotoBlock::loadJSON(const Json::Value& pb_json_node)
{
   ostringstream xmsg;
   xmsg<<__FILE__<<": loadJSON() -- ";

   // Attempt to read the MSP-formatted JSON photoblock header first. If not successful, assume it
   // is in ossim format:
   if (!pb_json_node.isMember("photoBlockHeader"))
   {
      ossim::PhotoBlock::loadJSON(pb_json_node);
      return;
   }

   const Json::Value& pbheaderJson = pb_json_node["photoBlockHeader"];
   m_name = pbheaderJson["name"].asString();
   m_type = pbheaderJson["type"].asString();
   m_date = pbheaderJson["date"].asString();
   m_description = pbheaderJson["description"].asString();
   m_ownerProducer = pbheaderJson["ownerProducer"].asString();
   m_classification = pbheaderJson["classification"].asString();
   m_derivedFrom = pbheaderJson["derivedFrom"].asString();
   m_disseminationCtrls = pbheaderJson["disseminationControls"].asString();

   // Always do images first, as tiepoints will be using the image list to:
   if (pb_json_node.isMember("imageList"))
   {
      Json::Value imageListJson = pb_json_node["imageList"];
      const Json::Value& stateListJson = pb_json_node["sensorModelStateList"];

      // IMPORTANT NOTE: Assuming that the array entry order for images and sensor model states
      // correspond:
      unsigned int count = imageListJson.size();
      if (count != stateListJson.size())
      {
         xmsg<<"The number of images and sensor model states provided do not correspond!. Cannot"
               " load MSP PhotoBlock.";
         throw ossimException(xmsg.str());
      }
      for (unsigned int i=0; i<count; ++i)
      {
         Json::Value& imageJson = imageListJson[i];
         imageJson.append(stateListJson[i]);

         shared_ptr<ossim::Image> item (new MspImage(imageJson));
         m_imageList.push_back(item);
      }
   }

   // TODO: Implement GCP cross covariance capability

   if (pb_json_node.isMember("groundPointList"))
   {
      const Json::Value& listJson = pb_json_node["groundPointList"];
      unsigned int count = listJson.size();
      for (unsigned int i=0; i<count; ++i)
      {
         Json::Value jsonItem = listJson[i];
         jsonItem.append(pb_json_node["gpCrossCovList"]);
         shared_ptr<ossim::GroundControlPoint> item (new ossim::GroundControlPoint(jsonItem));
         m_gcpList.push_back(item);
      }
   }


   if (pb_json_node.isMember("imagePointList"))
   {
      // This is a sequential list of image points, correlated only by point ID to other points.
      // So need a multi map to store using pointID as key:
      std::multimap<string, Json::Value> ipMap;
      vector<string> pointIds;
      const Json::Value& ipListJson = pb_json_node["imagePointList"];
      unsigned int count = ipListJson.size();
      for (unsigned int i=0; i<count; ++i)
      {
         const Json::Value& ipJson = ipListJson[i];
         string pointId = ipJson["pointId"].asString();
         pointIds.push_back(pointId);
         ipMap.emplace(pointId, ipJson);
      }

      string imageId;
      ossimDpt xy;
      double sigmaX, sigmaY, rho;
      NEWMAT::SymmetricMatrix cov;
      typedef std::multimap<string, Json::Value>::iterator ipIterType;
      std::pair < ipIterType, ipIterType > tpSet;
      for (auto &pointId : pointIds)
      {
         shared_ptr<TiePoint> tp (new TiePoint);
         tp->setTiePointId(pointId);
         tpSet = ipMap.equal_range(pointId);
         for (ipIterType ipIter=tpSet.first; ipIter!=tpSet.second; ++ipIter)
         {
            Json::Value& ipJson = ipIter->second;
            shared_ptr<Image> image = findImage(ipJson["imageId"].asString());
            if (!image)
            {
               tp.reset();
               continue;
            }
            xy.x = ipJson["column"].asDouble();
            xy.y = ipJson["row"].asDouble();
            sigmaX = ipJson["sigmaColumn"].asDouble();
            sigmaY = ipJson["sigmaRow"].asDouble();
            rho = ipJson["rho"].asDouble();
            cov(0,0) = sigmaX*sigmaX;
            cov(1,1) = sigmaY*sigmaY;
            cov(0,1) = rho*sigmaX*sigmaY;
            tp->setImagePoint(image, xy, cov);
         }
         if (tp->getImageCount() >= 2)
            m_tiePointList.push_back(tp);
      }
   }
}

void MspPhotoBlock::saveJSON(Json::Value& pbJSON) const
{
   ossim::PhotoBlock::saveJSON(pbJSON);
}

shared_ptr<ossim::Image>  MspPhotoBlock::findImage(const std::string& imageId)
{
   // Loop over all images represented in this tiepoint:
   shared_ptr<ossim::Image> image;
   ossimString tpiid = ossimString(imageId).trim();
   for (int t=0; t<m_imageList.size(); ++t)
   {
      ossimString pbiid = ossimString(m_imageList[t]->getImageId()).trim();
      if (pbiid != tpiid)
         continue;
      image = m_imageList[t];
      break;
   }
   return image;
}

void MspPhotoBlock::getCsmModels(MSP::CsmSensorModelList& csmModelList)
{
   csmModelList.clear();
   for (int i=0; i<m_imageList.size(); ++i)
   {
      shared_ptr<MspImage> image = dynamic_pointer_cast<MspImage>(m_imageList[i]);
      if (image)
      {
         const csm::RasterGM* model = image->getCsmSensorModel();
         csmModelList.push_back(model);
         //clog<<"imageId: <"<<model->getImageIdentifier()<<">"<<endl;//###TODO
      }
   }
}

void MspPhotoBlock::setCsmModels(MSP::CsmSensorModelList& csmModelList)
{
   ostringstream xmsg;
   xmsg<<__FILE__<<":"<<__LINE__<<" CSM models list provided is not synchronized with image list in"
         " MspPhotoBlock. This should not happen.";

   if (m_imageList.size() != csmModelList.size())
      throw ossimException(xmsg.str());

   for (int i=0; i<m_imageList.size(); ++i)
   {
      if (m_imageList[i]->getImageId() != csmModelList[i]->getImageIdentifier())
         throw ossimException(xmsg.str());

      shared_ptr<MspImage> image = dynamic_pointer_cast<MspImage>(m_imageList[i]);
      if (image)
         image->setCsmSensorModel(csmModelList[i]);
   }
}

} // end namespace ossimMsp
