//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************
#ifndef MspPhotoBlock_HEADER
#define MspPhotoBlock_HEADER 1

#include <Config.h>
#include <string>
#include <vector>
#include <memory>
#include <ossim/base/ossimConstants.h>
#include <ossim/reg/TiePoint.h>
#include <ossim/reg/GroundControlPoint.h>
#include <ossim/reg/PhotoBlock.h>
#include <csmutil/JointCovMatrix.h>
#include <csmutil/CsmSensorModelList.h>

namespace ossimMsp
{

/**
 * Class for representing MSP PhotoBlock.
 */
class MspPhotoBlock : public ossim::PhotoBlock
{
public:
   /**
    * Initialize the photoblock from a prior saved session. If none found, assumes a new, blank
    * session is being started.
    */
   MspPhotoBlock();
   MspPhotoBlock(const Json::Value& pb_json_node);
   MspPhotoBlock(const MspPhotoBlock& copyThis);

   ~MspPhotoBlock();

   /*
   * Refer to <a href="https://docs.google.com/document/d/1DXekmYm7wyo-uveM7mEu80Q7hQv40fYbtwZq-g0uKBs/edit?usp=sharing">3DISA API document</a>
   * for JSON format used.
   */
   virtual void loadJSON(const Json::Value& json);

   /*
   * Refer to <a href="https://docs.google.com/document/d/1DXekmYm7wyo-uveM7mEu80Q7hQv40fYbtwZq-g0uKBs/edit?usp=sharing">3DISA API document</a>
   * for JSON format used.
   */
   virtual void saveJSON(Json::Value& json) const;

   /**
    * Convenience method to assemble the list of CSM sensor models corresponding to the images in
    * the photoblock
    */
   void getCsmModels(MSP::CsmSensorModelList& csmModelList);

   /**
    * Convenience method to assign all CSM models for images in the photoblock:
    */
   void setCsmModels(MSP::CsmSensorModelList& csmModelList);

   MSP::JointCovMatrix& getJointCovariance() {return m_mspJCM;}

   void setJointCovariance(const MSP::JointCovMatrix& cov) { m_mspJCM = cov; }

private:
   std::shared_ptr<ossim::Image> findImage(const std::string& imageId);

   std::string m_name;
   std::string m_type;
   std::string m_date;
   std::string m_description;
   std::string m_ownerProducer;
   std::string m_classification;
   std::string m_derivedFrom;
   std::string m_disseminationCtrls;

   MSP::JointCovMatrix m_mspJCM;
};

} // End namespace ossimMsp

#endif
