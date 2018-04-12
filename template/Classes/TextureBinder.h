#pragma once

#ifndef TextureBinder_H
#define TextureBinder_H

#pragma once

#include <glm.hpp>
#include <vector>
#include <string>
#include <glm.hpp>
#include "../stb_image.h"
#include <iostream>
#include <glew.h>

struct texture {
	std::string name;
	std::string path;
	texture(std::string _name, std::string _path) {
		name = _name;
		path = _path;
	}
};

struct TextureObjects
{
	int id;
	std::string name;
	std::string path;

	TextureObjects(int _id, std::string _name, std::string _path) {
		id = _id;
		name = _name;
		path = _path;
	}
};

class TextureBinder
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	TextureBinder();
	void init();
	~TextureBinder();

	//=======================================================================
	//                         Methods
	//=======================================================================
	void bind(std::string path, std::string name);

	int textureByName(std::string name);
	std::vector<texture> textures_raw; // not bound to open gl, just a local repository that will eventually be bound (see constructor)
	//=======================================================================
	//							Properties
	//=======================================================================
	std::vector<TextureObjects> bound;
};

#endif