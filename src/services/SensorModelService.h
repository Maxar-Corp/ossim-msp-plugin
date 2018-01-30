//**************************************************************************************************
//
//     OSSIM Open Source Geospatial Data Processing Library
//     See top level LICENSE.txt file for license information
//
//**************************************************************************************************
#ifndef SensorModelService_HEADER
#define SensorModelService_HEADER 1

#include <services/ServiceBase.h>
#include <memory>
#include "../common/MspImage.h"

namespace ossimMsp
{

/**
 * Top-level class for interfacing to MSP sensor model service from 3DISA client.
 */
class SensorModelService : public ServiceBase
{
public:
   SensorModelService();
   virtual ~SensorModelService();

   /**
    * Does nothing. The work is done in saveJSON().
    */
   virtual void execute();

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
    * Useful for invoking functionality without JSON. Prints out JSON response to the console.
    */
   void execute(const std::string& imageFile);

private:
   std::string m_method;
   std::string m_application;
   std::shared_ptr<MspImage> m_image;
};

} // End namespace ossimMsp

#endif
