#ifndef B_MODEL_DATA_H
#define B_MODEL_DATA_H

#include "GeometryData.h"


/** @brief The underlying data of a model
*	@author David Steiner
*/
class OBJLoader;

class ModelData
{
	friend OBJLoader;
    
public:
	/* Typedefs */
    typedef std::map< std::string, GeometryDataPtr >    GroupMap;

	/* Functions */
    
	/**	@brief Constructor
	*	@param[in] fileName The name of the obj file
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*/
    ModelData(const std::string &fileName, bool flipT = false, bool flipZ = false);

	/**	@brief Constructor
	*	@param[in] flipT Flip T axis of texture
	*	@param[in] flipZ Flip Z axis of the geometry
	*/
    ModelData(bool flipT = false, bool flipZ = false);

	/**	@brief Destructor
	*/
    ~ModelData();
    
	/**	@brief Loads the model from a file
	*	@param[in] fileName The name of the obj file
	*/
    ModelData &load(const std::string &fileName);
    
	/**	@brief Returns the geometry groups
	*/
    GroupMap getData() const;
    
private:
	
	/* Variables */
	
	OBJLoader		*_objLoader;
    bool            _flipT;
    bool            _flipZ;
};

#endif /* defined(B_MODEL_DATA_H) */
