#include "ModelMesh.h"
#include <vector>

ModelMesh::ModelMesh()
{
	drawCount = NULL;
}

ModelMesh::~ModelMesh()
{
	glDeleteVertexArrays(1, &VAO); 
}

void ModelMesh::initGameModel(const IndexedModel& model)
{
	//set draw counter equal to the nubmer of model's indices size
	drawCount = model.indices.size();
	//generate and store vertex array into vertex array object
	glGenVertexArrays(1, &VAO);

	//bind the vertex array object
	glBindVertexArray(VAO);

	//taking the NUM_BUFFERS and the array that store buffers and generate buffer object 
	glGenBuffers(NUM_BUFFERS, VAB);

	//bind the buffer object by taking the type of buffer data and the specified buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VAB[POSITION_VERTEXBUFFER]);
	//store data onto the gpu with specified type, size, pointer of data and the usage pattern of the data store
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
	//enable vertex attribute array that specified by the index number
	glEnableVertexAttribArray(POSITION_VERTEXBUFFER);
	//define vertex attribute data that included index, size, type of data, normalize or not, stride and pointer
	glVertexAttribPointer(POSITION_VERTEXBUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VAB[TEXCOORD_VB]); 
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW); 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VAB[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAB[INDEX_VB]); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	//unbind the vertex array object
	glBindVertexArray(0); 
}

//takes a file path of a model, store it into the indexed model variable and pass it to the initModel function
void ModelMesh::loadGameModel(const std::string& filename)
{
	IndexedModel model = OBJModel(filename).ToIndexedModel();
	initGameModel(model);
	Sphere meshSphere();
}

void ModelMesh::drawModel()
{
	//bind the vertex array object
	glBindVertexArray(VAO);
	//draw primitives based on the data from the VAO
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	//unbind the vertex array object
	glBindVertexArray(0);
}

//pass in position and radius to create a sphere around the object 
void ModelMesh::setupSphere(glm::vec3 position, float radius)
{
	meshSphere.SetPosition(position);
	meshSphere.SetRadius(radius);
}

