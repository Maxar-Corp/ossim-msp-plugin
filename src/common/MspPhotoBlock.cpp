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
   // Always do images first, as tiepoints will be using the image list to correct image ID:
   if (pb_json_node.isMember("images"))
   {
      const Json::Value& listJson = pb_json_node["images"];
      unsigned int count = listJson.size();
      for (unsigned int i=0; i<count; ++i)
      {
         const Json::Value& jsonItem = listJson[i];
         shared_ptr<ossim::Image> item (new MspImage(jsonItem));
         m_imageList.push_back(item);
      }
   }

   if (pb_json_node.isMember("groundPoints"))
   {
      const Json::Value& listJson = pb_json_node["groundPoints"];
      unsigned int count = listJson.size();
      for (unsigned int i=0; i<count; ++i)
      {
         const Json::Value& jsonItem = listJson[i];
         shared_ptr<ossim::GroundControlPoint> item (new ossim::GroundControlPoint(jsonItem));
         m_gcpList.push_back(item);
      }
   }

   if (pb_json_node.isMember("tiePoints"))
   {
      const Json::Value& listJson = pb_json_node["tiePoints"];
      unsigned int count = listJson.size();
      for (unsigned int i=0; i<count; ++i)
      {
         const Json::Value& jsonItem = listJson[i];
         shared_ptr<ossim::TiePoint> item (new ossim::TiePoint(jsonItem));
         m_tiePointList.push_back(item);
      }
   }

   fixTpImageIds();
}

void MspPhotoBlock::saveJSON(Json::Value& pbJSON) const
{
   ossim::PhotoBlock::saveJSON(pbJSON);
}

void MspPhotoBlock::fixTpImageIds()
{
   unsigned int count = m_tiePointList.size();
   for (unsigned int t=0; t<count; ++t)
   {
      m_tiePointList[t]->fixTpImageIds(m_imageList);
      unsigned int num_images = m_tiePointList[t]->getImageCount();
      for (unsigned int i=0; i<num_images; ++i)
      {

      }
   }
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
