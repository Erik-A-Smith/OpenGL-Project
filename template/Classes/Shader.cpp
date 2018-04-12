#include "Shader.h"

Shader::Shader(const std::string vertexPath, const std::string fragmentPath)
{
	//////////////////////////////////
	//		Compiling vrtx shader
	//////////////////////////////////
	readFileCode(vertexPath, vertexCode);
	int vertexShader = glCreateShader(GL_VERTEX_SHADER); // assign an ID
	char const * vertexCodePointer = vertexCode.c_str(); // get the pointer to the code
	glShaderSource(vertexShader, 1, &vertexCodePointer, NULL);
	glCompileShader(vertexShader);
	validateShader(vertexShader);

	//////////////////////////////////
	//		Compiling frg shader
	//////////////////////////////////
	readFileCode(fragmentPath, fragmentCode);
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // assign an ID
	char const * fragmentCodePointer = fragmentCode.c_str(); // get the pointer to the code
	glShaderSource(fragmentShader, 1, &fragmentCodePointer, NULL);
	glCompileShader(fragmentShader);
	validateShader(fragmentShader);

	//////////////////////////////////
	//		Link shaders
	//////////////////////////////////
	id = glCreateProgram(); // assign an ID
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	validateShaderProgram();

	// Free up memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

Shader::~Shader()
{
}

void Shader::activate() {
	glUseProgram(id);
}

bool Shader::validateShader(int shaderId)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return success;
}

void Shader::readFileCode(std::string path, std::string &binder)
{
	std::string results = "";
	std::ifstream readStream(path, std::ios::in);
	if (readStream.is_open()) {
		std::string Line = "";
		while (getline(readStream, Line))
			results += "\n" + Line;
		readStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", path.c_str());
		getchar();
		exit(-1);
	}
	binder = results;
}

bool Shader::validateShaderProgram()
{
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	return success;
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}