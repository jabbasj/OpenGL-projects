#include "all_headers.h"

int Mesh::unique_ID = 0;

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<GLuint> _indices, std::vector<Texture> _textures, Model* _parent, bool _centered)
{
	mID = unique_ID;
	++unique_ID;

	parent = _parent;
	centered = _centered;

	vertices = _vertices;
	indices = _indices;
	textures = _textures;

	SetupMesh();
}

Mesh::Mesh(Mesh * copy_this, int a) {
	mID = unique_ID;
	unique_ID++;

	parent = copy_this->parent;
	vertices = copy_this->vertices;
	textures = copy_this->textures;
	indices = copy_this->indices;
	centered = copy_this->centered;
	SetupMesh();
}

void Mesh::AddVertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 uv) {
	Vertex temp;

	temp.Position = pos;
	temp.Normal = norm;
	temp.TexCoords = uv;

	vertices.push_back(temp);
}

void Mesh::Draw(GLFWwindow * win) {

	for (GLuint i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

}


void Mesh::SetupMesh() {

	// Find Center
	for (int k = 0; k < vertices.size(); k++) {
		if (vertices[k].Position.x > max.x) max.x = vertices[k].Position.x;
		if (vertices[k].Position.x < min.x) min.x = vertices[k].Position.x;

		if (vertices[k].Position.y > max.y) max.y = vertices[k].Position.y;
		if (vertices[k].Position.y < min.x) min.y = vertices[k].Position.y;

		if (vertices[k].Position.z > max.z) max.z = vertices[k].Position.z;
		if (vertices[k].Position.z < min.z) min.z = vertices[k].Position.z;
	}
	centroid = 0.5f * (max + min);
	radius = sqrt(pow(max.x, 2) + pow(max.y, 2) + pow(max.z, 2));

	centroid.w = 1.0f;
	max.w = 1.0f;
	min.w = 1.0f;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO[0]);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),&vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)0);

	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)offsetof(Vertex, Normal));

	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)offsetof(Vertex, TexCoords));
}

/* Artifact from assignment 1, left for future refence if needed
void Mesh::Spin(GLFWwindow * win) {

	if (!centered) {
		ModelMat = glm::translate(ModelMat, centroid.xyz());
	}

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		ModelMat = glm::scale(ModelMat, glm::vec3(1 + change_value, 1 + change_value, 1 + change_value));
	}

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS) {
		ModelMat = glm::scale(ModelMat, glm::vec3(1 - change_value, 1 - change_value, 1 - change_value));
	}

	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, -change_value, glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, change_value, glm::vec3(0, 1, 0));
	}

	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, change_value, glm::vec3(1, 0, 0));
	}

	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, -change_value, glm::vec3(1, 0, 0));
	}

	ModelMat = glm::rotate(ModelMat, change_value, glm::vec3(0, 0, 1));

	if (!centered) {
		ModelMat = glm::translate(ModelMat, -1.0f * centroid.xyz());
	}
}

void Mesh::Revolve(GLFWwindow * win) {

	if (!centered) {
		ModelMat = glm::translate(ModelMat, centroid.xyz());
	}

	ModelMat = glm::translate(ModelMat, glm::vec3(-5.0f* cos(vertical_angle), -5.0f * sin(vertical_angle), 0.0f));	

	if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		ModelMat = glm::scale(ModelMat, glm::vec3(1 + change_value, 1 + change_value, 1 + change_value));
	}

	if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS) {
		ModelMat = glm::scale(ModelMat, glm::vec3(1 - change_value, 1 - change_value, 1 - change_value));
	}

	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, -change_value, glm::vec3(0, 0, 1));
	}

	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, change_value, glm::vec3(0, 0, 1));
	}

	if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, change_value, glm::vec3(1, 0, 0));
	}

	if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
		ModelMat = glm::rotate(ModelMat, -change_value, glm::vec3(1, 0, 0));
	}

	ModelMat = glm::rotate(ModelMat, change_value, glm::vec3(0.0f, 0.0f, 1.0f));
	
	ModelMat = glm::translate(ModelMat, glm::vec3(5.0f * cos(vertical_angle), 5.0f * sin(vertical_angle), 0.0f));


	vertical_angle += change_value;

	ModelMat = glm::rotate(ModelMat, -change_value, glm::vec3(0.0f, 0.0f, 1.0f));

	if (!centered) {
		ModelMat = glm::translate(ModelMat, -1.0f * centroid.xyz());
	}
}
*/