#ifndef SHADER_H
#define SHADER_H

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm.hpp>
#include <glew.h>
class Shader
{
public:
	//=======================================================================
	//                         Constructor/Destructor
	//=======================================================================
	Shader(const std::string vertexPath, const std::string fragmentPath);
	~Shader();

	//=======================================================================
	//							  Public Methods
	//=======================================================================	
	void readFileCode(std::string path, std::string & binder);
	void activate();
	bool validateShader(int shaderId);
	bool validateShaderProgram();

	// Used for setting variables inside the code (from learnopengl.com)
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string & name, const glm::vec2 & value) const;
	void setVec2(const std::string & name, float x, float y) const;
	void setVec3(const std::string & name, const glm::vec3 & value) const;
	void setVec3(const std::string & name, float x, float y, float z) const;
	void setVec4(const std::string & name, const glm::vec4 & value) const;
	void setVec4(const std::string & name, float x, float y, float z, float w);
	void setMat2(const std::string & name, const glm::mat2 & mat) const;
	void setMat3(const std::string & name, const glm::mat3 & mat) const;
	void setMat4(const std::string & name, const glm::mat4 & mat) const;
	
	//=======================================================================
	//                         Properties
	//=======================================================================
	unsigned int id;
	std::string vertexCode;
	std::string fragmentCode;
};

#endif