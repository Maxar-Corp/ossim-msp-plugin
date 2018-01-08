//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************
#ifndef PhotoBlock_HEADER
#define PhotoBlock_HEADER 1

#include <Config.h>
#include <string>
#include <vector>
#include <memory>
#include <ossim/base/ossimConstants.h>
#include <common/Image.h>
#include <common/TiePoint.h>
#include <common/GroundControlPoint.h>
#include <ossim/base/JsonInterface.h>
#include <csmutil/JointCovMatrix.h>
#include <csmutil/CsmSensorModelList.h>

namespace ossimMsp
{

/**
 * Class for representing MSP PhotoBlock.
 */
class PhotoBlock : public ossim::JsonInterface,
                   public std::enable_shared_from_this<PhotoBlock>
{
public:
   /**
    * Initialize the photoblock from a prior saved session. If none found, assumes a new, blank
    * session is being started.
    */
   PhotoBlock();
   PhotoBlock(const Json::Value& pb_json_node);
   PhotoBlock(const PhotoBlock& copyThis);

   ~PhotoBlock();

   PhotoBlock& operator=(const PhotoBlock& copythis);

   std::shared_ptr<Image> getImage(const std::string& imageId);
   std::shared_ptr<TiePoint> getTiePoint(unsigned int tpId);
   std::shared_ptr<GroundControlPoint> getGroundPoint(const std::string& gpId);

   std::vector<std::shared_ptr<Image> >&              getImageList()       { return m_imageList; }
   std::vector<std::shared_ptr<TiePoint> >&           getTiePointList()    { return m_tiePointList; }
   std::vector<std::shared_ptr<GroundControlPoint> >& getGroundPointList() { return m_gcpList; }

   // TODO: Add of individual components not valid until proper management of the JCM can be
   //       provided

   /**
    * Adds the image to the photoblock at last position. Sets the image's pb index member.
    * @param images To be added to PB
    * @return The PB index of the image in the photoblock.
    */
   unsigned int addImage(std::shared_ptr<Image> image);

   /**
    * Adds the image to the photoblock at last position. Sets the image's pb index member.
    * @param images To be added to PB
    * @return The PB index of the image in the photoblock.
    */
   unsigned int addGroundPoint(std::shared_ptr<GroundControlPoint> groundPoint);

   unsigned int addTiePoint(shared_ptr<TiePoint> tiepoint);

   /**
    * Adds the list of tiepoints to the PB
    * @param tiepointList To be added to PB
    */
  void addTiePoints(TiePointList& tiepointList);

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
   void fixTpImageIds();

   std::vector<std::shared_ptr<Image> > m_imageList;
   std::vector<std::shared_ptr<TiePoint> > m_tiePointList;
   std::vector<std::shared_ptr<GroundControlPoint> > m_gcpList;

   MSP::JointCovMatrix m_mspJCM;
};

} // End namespace ossimMsp

#endif
