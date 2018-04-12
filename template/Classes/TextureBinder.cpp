#include "TextureBinder.h"

TextureBinder::TextureBinder()
{
	textures_raw.push_back(texture("grass", "Textures/grass.jpg"));
	textures_raw.push_back(texture("horse", "Textures/horse.jpg"));
}

void TextureBinder::init() {
	for (std::vector<texture>::iterator it = textures_raw.begin(); it != textures_raw.end(); ++it) {
		bind(it->path, it->name);
	}
}

TextureBinder::~TextureBinder()
{
}

void TextureBinder::bind(std::string path, std::string name)
{
	// Texture 1
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	bound.push_back(TextureObjects(texture, name, path));
}

int TextureBinder::textureByName(std::string name) {
	for (std::vector<TextureObjects>::iterator it = bound.begin(); it != bound.end(); ++it) {
		if (it->name == name) {
			return it->id;
		}
	}
}
