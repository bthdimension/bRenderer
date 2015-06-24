#ifndef B_OBJ_LOADER_H
#define B_OBJ_LOADER_H

#include <map>
#include <boost/bind.hpp>

#include "GeometryData.h"
#include "ModelData.h"

#include "vmmlib/matrix.hpp"
#include "../obj_parser/obj_parser.hpp"

/** @brief Loads and processes OBJ models and materials
*	@author David Steiner, Benjamin B�rgisser
*/
class OBJLoader
{
public:
	/* Typedefs */
	typedef std::map< std::string, MaterialData >   Materials;

	/* Structs */
	struct FaceData
	{
		Index       v1;
		Index       v2;
		Index       v3;
		vmml::vec3f normal;
		vmml::vec3f tangent;
		vmml::vec3f bitangent;
	};

	struct VertexData
	{
		std::vector<Index>  faces;
		vmml::vec3f         position;
		vmml::vec3f         normal;
		vmml::vec3f         tangent;
		vmml::vec3f         bitangent;

		float x()   { return position.x(); }
		float y()   { return position.y(); }
		float z()   { return position.z(); }

		VertexData(const vmml::vec3f &position)
			: position(position)
		{}
	};

	/* Functions */

	/**	@brief Constructor
	*	@param[in] flags
	*/
	OBJLoader(obj::obj_parser::flags_type flags)
		: _flags(flags)
	{}

	/**	@brief Constructor
	*	@param[in] data The underlying data of the model
	*	@param[in] flags
	*/
	OBJLoader(ModelData *data, obj::obj_parser::flags_type flags)
		: _data(data)
		, _flags(flags)
	{}

	/**	@brief Loads the model data from an input stream
	*	@param[in] istream
	*/
	bool load(std::istream& istream);

	/**	@brief Loads an obj material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*/
	MaterialData loadMaterial(const std::string &fileName, const std::string &materialName);

	/**	@brief Creates the face normals
	*/
	void createFaceNormals();

	/**	@brief Creates the vertex normals
	*/
	void createVertexNormals();

	/**	@brief Returns the geometry groups
	*/
	ModelData::GroupMap getData() { return _groups; }

private:

	/* Functions */

	void createGroup(const std::string& name)
	{
		_groupName = name;
		auto &group = _groups[_groupName];
		if (!group)
		{
			group = GeometryDataPtr(new GeometryData);
		}
		_group = group;
	}

	void info_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void warning_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void error_callback(const std::string& filename, std::size_t line_number, const std::string& message);
	void geometric_vertex_callback(obj::float_type x, obj::float_type y, obj::float_type z);
	void texture_vertex_callback(obj::float_type u, obj::float_type v);
	void vertex_normal_callback(obj::float_type x, obj::float_type y, obj::float_type z);
	void triangular_face_geometric_vertices_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
	void triangular_face_geometric_vertices_texture_vertices_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
	void triangular_face_geometric_vertices_vertex_normals_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
	void triangular_face_geometric_vertices_texture_vertices_vertex_normals_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
	void polygonal_face_geometric_vertices_begin_callback(obj::index_type v1, obj::index_type v2, obj::index_type v3);
	void polygonal_face_geometric_vertices_vertex_callback(obj::index_type v);
	void polygonal_face_geometric_vertices_end_callback();
	void polygonal_face_geometric_vertices_texture_vertices_begin_callback(const obj::index_2_tuple_type& v1_vt1, const obj::index_2_tuple_type& v2_vt2, const obj::index_2_tuple_type& v3_vt3);
	void polygonal_face_geometric_vertices_texture_vertices_vertex_callback(const obj::index_2_tuple_type& v_vt);
	void polygonal_face_geometric_vertices_texture_vertices_end_callback();
	void polygonal_face_geometric_vertices_vertex_normals_begin_callback(const obj::index_2_tuple_type& v1_vn1, const obj::index_2_tuple_type& v2_vn2, const obj::index_2_tuple_type& v3_vn3);
	void polygonal_face_geometric_vertices_vertex_normals_vertex_callback(const obj::index_2_tuple_type& v_vn);
	void polygonal_face_geometric_vertices_vertex_normals_end_callback();
	void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_begin_callback(const obj::index_3_tuple_type& v1_vt1_vn1, const obj::index_3_tuple_type& v2_vt2_vn2, const obj::index_3_tuple_type& v3_vt3_vn3);
	void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_vertex_callback(const obj::index_3_tuple_type& v_vt_vn);
	void polygonal_face_geometric_vertices_texture_vertices_vertex_normals_end_callback();
	void group_name_callback(const std::string& group_name);
	void smoothing_group_callback(obj::size_type group_number);
	void object_name_callback(const std::string& object_name);
	void material_library_callback(const std::string& filename);
	void material_name_callback(const std::string& material_name);
	void comment_callback(const std::string& comment);

	template< bool POSITION, bool TEX_COORD, bool NORMAL >
	void genVertex(const IndexData &d);

	template< bool NORMAL >
	void genFace(const IndexData &d1, const IndexData &d2, const IndexData &d3);

	/**	@brief Loads an obj material
	*	@param[in] fileName File name including extension
	*	@param[in] materialName Name of the material
	*/
	void loadObjMtl(const std::string &fileName, const std::string &materialName = "");

	/* Variables */

	obj::obj_parser::flags_type _flags;
	ModelData                   *_data;
	Materials                   _materials;
	std::string                 _groupName;
	ModelData::GroupMap         _groups;
	GeometryDataPtr             _group;

	std::vector<FaceData>       _faces;
	std::vector<VertexData>     _vertices;
	std::vector<vmml::vec2f>    _texCoords;
	std::vector<vmml::vec3f>    _normals;
	std::vector<vmml::vec3f>    _tangents;
	std::vector<vmml::vec3f>    _bitangents;
};


#endif /* defined(B_OBJ_LOADER_H) */
