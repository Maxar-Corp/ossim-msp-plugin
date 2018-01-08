//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************

#include "Image.h"
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimString.h>

#include <SensorModel/SensorModelService.h>
#include <SupportData/SupportDataService.h>
using namespace std;

namespace ossimMsp
{

Image::Image(const std::string& imageId,
             const std::string& filename,
             const std::string& modelName,
             unsigned int entryIndex,
             unsigned int band)
:  m_imageId (imageId),
   m_filename (filename),
   m_entryIndex (entryIndex),
   m_activeBand (band),
   m_modelName (modelName),
   m_csmModel (0)
{

}

Image::Image(const Json::Value& json_node)
:  m_entryIndex (0)
   , m_csmModel (0)
{
   loadJSON(json_node);
}

Image::~Image()
{
   //m_handler.reset();
}

void Image::getAvailableModels(std::vector< pair<string, string> >& availableModels) const
{
   // Fetch models from MSP:
   csm::Isd isd (m_filename); // TODO: Note entry index ignored here
   MSP::SMS::SensorModelService sms;
   sms.setPluginPreferencesRigorousBeforeRpc();
   availableModels = sms.getAllSupportedModels(isd);
}

void Image::loadJSON(const Json::Value& json_node)
{
   ostringstream xmsg;
   xmsg<<__FILE__<<": loadJSON(JSON) -- ";

   // Parse JSON. Filename is required:
   if (json_node.isMember("filename"))
   {
      m_filename = json_node["filename"].asString();
   }
   else
   {
      xmsg<<"JSON node missing required field: \"filename\".";
      throw ossimException(xmsg.str());
   }

   // Entry index defaults to 0 if not present:
   if (json_node["entryIndex"].isUInt())
      m_entryIndex = json_node["entryIndex"].asUInt();

   // Band defaults to 1 if not present:
   if (json_node["band"].isUInt())
      m_activeBand = json_node["band"].asUInt();

   // Sensor model defaults to most accurate available if not provided (indicated by blank name):
   if (json_node.isMember("sensorModel"))
      m_modelName = json_node["sensorModel"].asString();

   // Sensor model defaults to most accurate available if not provided (indicated by blank name):
   if (json_node.isMember("imageId"))
      m_imageId = json_node["imageId"].asString();

   // Establish the sensor model. This also sets the official image ID, which will be overwritten
   // if JSON field provided
   string modelState = json_node["modelState"].asString();
   if (modelState.size())
   {
      MSP::SMS::SensorModelService sms;
      csm::Model* base = sms.createModelFromState(modelState.c_str());
      m_csmModel = dynamic_cast<csm::RasterGM*>(base);
      string id = m_csmModel->getImageIdentifier();
      if (id.compare("UNKNOWN"))
         m_imageId = id;
      else
         m_csmModel->setImageIdentifier(m_imageId);
   }
   else
   {
      getCsmSensorModel();
   }
}

void Image::saveJSON(Json::Value& json_node) const
{
   json_node.clear();
   json_node["imageId"] = m_imageId;
   json_node["filename"] = m_filename;
   json_node["entryIndex"] = m_entryIndex;

   if (m_modelName.size())
      json_node["sensorModel"] = m_modelName;

   if (m_csmModel)
   {
      string state = m_csmModel->getModelState();
      json_node["modelState"] = state;
   }
}

void Image::setCsmSensorModel(const csm::RasterGM* model)
{
   ostringstream xmsg;
   xmsg<<__FILE__<<": getCsmSensorModel() -- ";

   try
   {
      MSP::SMS::SensorModelService sms;
      csm::Model* base = sms.createModelFromState(model->getModelState().c_str());
      m_csmModel = dynamic_cast<csm::RasterGM*>(base);
      if (m_csmModel)
      {
         // Fetch the ID according to CSM, checking for "UNKNOWN":
         string id = m_csmModel->getImageIdentifier();
         if (id.compare("UNKNOWN"))
            m_imageId = id;
         else
            m_csmModel->setImageIdentifier(m_imageId);
      }
   }
   catch (exception& e)
   {
      xmsg<<"Caught exception: "<<e.what();
      throw ossimException(xmsg.str());
   }

   m_modelName = model->getModelName();
}

const csm::RasterGM*  Image::getCsmSensorModel()
{
   ostringstream xmsg;
   xmsg<<__FILE__<<": getCsmSensorModel() -- ";

   // May already be instantiated:
   if (m_csmModel)
      return m_csmModel;

   try
   {
      MSP::SMS::SensorModelService sms;
      const char* modelName = 0;
      if (m_modelName.size())
         modelName = m_modelName.c_str();
      MSP::ImageIdentifier entry ("IMAGE_INDEX", ossimString::toString(m_entryIndex).string());
      sms.setPluginPreferencesRigorousBeforeRpc();
      csm::Model* base = sms.createModelFromFile(m_filename.c_str(), modelName, &entry);
      m_csmModel = dynamic_cast<csm::RasterGM*>(base);

      if (m_csmModel)
      {
         // Fetch the ID according to CSM, checking for "UNKNOWN":
         string id = m_csmModel->getImageIdentifier();
         if (id.compare("UNKNOWN"))
            m_imageId = id;
         else
            m_csmModel->setImageIdentifier(m_imageId);
      }
   }
   catch (exception& e)
   {
      xmsg<<"Caught exception: "<<e.what();
      throw ossimException(xmsg.str());
   }
   return m_csmModel;
}

} // end namespace ossimMsp