#include "Renderer.h"
#include "Globals.h"
#include "MeshLoader.h"

float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};
Shader* Renderer::MakeDebugTriangle() {

	MeshLoader ml;
	ml.CreateMesh("");

	auto shader = new Shader();

	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}";



	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);


	shader->ID = glCreateProgram();
	glAttachShader(shader->ID, vertexShader);
	glAttachShader(shader->ID, fragmentShader);
	glLinkProgram(shader->ID);
	glUseProgram(shader->ID);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glGenBuffers(1, &shader->VBO);
	glGenVertexArrays(1, &shader->VAO);
	glGenBuffers(1, &shader->EBO);

	glBindVertexArray(shader->VAO);

	glBindBuffer(GL_ARRAY_BUFFER, shader->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shader->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	MLOG(LOG_INFO, "Built Debug Triangle");
	return shader;
}

void Renderer::Render(Shader* shader) {

	glUseProgram(shader->ID);
	glBindVertexArray(shader->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}