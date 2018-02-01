//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************
#ifndef MspImage_HEADER
#define MspImage_HEADER 1

#include <Config.h>
#include <map>
#include <memory>
#include <string>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/JsonInterface.h>
#include <ossim/reg/Image.h>
#include <csm/RasterGM.h>
namespace ossimMsp
{

/**
 * Class representing an Image as used by ossim-msp services.
 */
class OSSIM_DLL MspImage : public ossim::Image
{
public:
   static unsigned int UNASSIGNED_PHOTOBLOCK_INDEX;

   MspImage(const std::string& imageId,
         const std::string& filename,
         const std::string& modelName="",
         unsigned int entryIndex=0,
         unsigned int band=1);

   MspImage(const Json::Value& image_json_node);

   ~MspImage();

   /**
    * Returns all available sensor model plugins and model names for this image:
    * @param availableModels List of <plugin-name, model-name> pairs.
    */
    virtual void getAvailableModels(std::vector< pair<std::string, std::string> >& availableModels) const;

    /**
    * Refer to <a href="https://docs.google.com/document/d/1DXekmYm7wyo-uveM7mEu80Q7hQv40fYbtwZq-g0uKBs/edit?usp=sharing">3DISA API document</a>
    * for JSON format used.
    */
    virtual void loadJSON(const Json::Value& json);

    /**
    * Refer to <a href="https://docs.google.com/document/d/1DXekmYm7wyo-uveM7mEu80Q7hQv40fYbtwZq-g0uKBs/edit?usp=sharing">3DISA API document</a>
    * for JSON format used.
    */
    virtual void saveJSON(Json::Value& json) const;

    /**
     * Replaces the MSP CSM sensor model with updated model.
     */
    void setCsmSensorModel(const csm::RasterGM* model);

    /**
     * Returns the MSP CSM sensor model associated with the image. If the sensor model name is not
     * defined, the first (most accurate) model will be selected.
     */
    const csm::RasterGM* getCsmSensorModel();

    /**
     * Returns the ISD portion
     */
    static void getISD(std::string isd, const std::string& filename, unsigned int entryIndex=0)
    {
       // TODO
    }

private:
   csm::RasterGM* m_csmModel;
};

} // End namespace ossimMsp

#endif
