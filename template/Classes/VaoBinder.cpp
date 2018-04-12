#include "VaoBinder.h"

VaoBinder::VaoBinder()
{
}


VaoBinder::~VaoBinder()
{
}

//void VaoBinder::bind(Complex* object, std::vector<unsigned int>& VBOs, std::vector<unsigned int>& EBOs)
void VaoBinder::bind(Part* object)
{
	if (object->partRepresentation.vaoIndex == NULL && object->partRepresentation.vertices.size() != 0 && object->partRepresentation.indices.size() != 0) {
		unsigned int VBO_vertex, vaoIdx, VBO_uv, VBO_normal;
		glGenVertexArrays(1, &vaoIdx);
		glGenBuffers(1, &VBO_vertex);
		glGenBuffers(1, &VBO_normal);
		glGenBuffers(1, &VBO_uv);

		glBindVertexArray(vaoIdx);

		// position attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
		glBufferData(GL_ARRAY_BUFFER, (object->partRepresentation.vertices).size() * sizeof(unsigned int), &(object->partRepresentation.vertices)[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// normal attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
		glBufferData(GL_ARRAY_BUFFER, (object->partRepresentation.normals).size() * sizeof(unsigned int), &(object->partRepresentation.normals)[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// textures
		if ((object->partRepresentation.uv).size() > 0){
			glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*(object->partRepresentation.uv).size(), &(object->partRepresentation.uv)[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(2);
		}

		object->partRepresentation.vaoIndex = vaoIdx;
	}

	for (std::vector<Part*>::iterator prt = object->subParts.begin(); prt != object->subParts.end(); ++prt) {
		bind((*prt));
	}
}
