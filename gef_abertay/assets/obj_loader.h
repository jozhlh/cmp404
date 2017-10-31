#ifndef _GEF_OBJ_LOADER_H
#define _GEF_OBJ_LOADER_H

#include <gef.h>
#include <map>
#include <string>
#include <vector>

namespace gef
{
	class Platform;
	class Model;
	class Texture;
	class Mesh;

	class OBJLoader
	{
	public:
		Mesh* LoadOBJToMesh(const char* filename, Platform& platform, Model* model);
		bool Load(const char* filename, Platform& platform, Model& model);
	private:
		bool LoadMaterials(Platform& platform, const char* filename, std::map<std::string, Int32>& materials, std::vector<Texture*>& textures);
	};
}


#endif // _OBJ_LOADER_H