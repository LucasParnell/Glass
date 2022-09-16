#include "Renderer.h"
#include "Texture.h"
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
std::vector <std::pair<std::string, uint32_t>> Renderer::loadedShaders;

Shader Renderer::LoadObject(Mesh* meshToLoad, std::string shaderName) {


	mesh = meshToLoad;

	auto shader = new Shader();

	int loadedId = -1;

	for (int i = 0; i < loadedShaders.size(); i++) {
		if (loadedShaders[i].first == shaderName)
			loadedId = loadedShaders[i].second;
	}

	if (loadedId == -1) {
		std::ifstream frag("CoolWorld/" + shaderName + ".frag");
		std::string fragTmp((std::istreambuf_iterator<char>(frag)),
			(std::istreambuf_iterator<char>()));

		std::ifstream vert("CoolWorld/" + shaderName + ".vert");
		std::string vertTmp((std::istreambuf_iterator<char>(vert)),
			(std::istreambuf_iterator<char>()));

		const char* vertStr = vertTmp.c_str();
		const char* fragStr = fragTmp.c_str();

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertStr, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragStr, NULL);
		glCompileShader(fragmentShader);


		shader->ID = glCreateProgram();

		glAttachShader(shader->ID, vertexShader);
		glAttachShader(shader->ID, fragmentShader);
		glLinkProgram(shader->ID);

		GLint success;

		glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);

		if (!success) {
			std::vector<GLchar> errorLog(512);
			glGetProgramInfoLog(shader->ID, 512, NULL, &errorLog[0]);
			MLOG(LOG_ERROR, "\n" + std::string(&errorLog[0]));
		}

		glUseProgram(shader->ID);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		loadedShaders.push_back(std::pair<std::string, uint32_t>(shaderName, shader->ID));
	}
	else {
		shader->ID = loadedId;
	}

	ParseCamera(shader->ID);


	return *shader;
}

void Renderer::ParseCamera(uint32_t shaderID) {
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)(Globals::SCR_WIDTH) / (float)(Globals::SCR_HEIGHT), 0.1f, 100.0f);
	SetMat4("projection", projection, shaderID);

	Camera* mainCam = Camera::GetMainCamera();
	SetMat4("view", mainCam->GetView(), shaderID);


}


void drawMesh(const std::map<int, GLuint>& vbos,
	tinygltf::Model& model, tinygltf::Mesh& mesh) {
	for (size_t i = 0; i < mesh.primitives.size(); ++i) {
		tinygltf::Primitive primitive = mesh.primitives[i];
		tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

		glDrawElements(primitive.mode, indexAccessor.count,
			indexAccessor.componentType,
			BUFFER_OFFSET(indexAccessor.byteOffset));
	}
}

// recursively draw node and children nodes of model
void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
	tinygltf::Model& model, tinygltf::Node& node) {
	if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
		drawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
	}
	for (size_t i = 0; i < node.children.size(); i++) {
		drawModelNodes(vaoAndEbos, model, model.nodes[node.children[i]]);
	}
}
void drawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos,
	tinygltf::Model& model) {
	glBindVertexArray(vaoAndEbos.first);

	const tinygltf::Scene& scene = model.scenes[model.defaultScene];
	for (size_t i = 0; i < scene.nodes.size(); ++i) {
		drawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
	}

	glBindVertexArray(0);
}


void Renderer::Render() {
		//Make this better please
		for(int i=0; i<Globals::mEntityManager->entities.size(); i++){
			auto entity = Globals::mEntityManager->entities[i];
			auto components = &Globals::mComponentManager->entityComponents[entity];

			glm::mat4 model = glm::mat4(1.0f);
			Shader* shader = nullptr;
			bool textureLoaded = false;
			GLuint texture;
			for (int x = 0; x < components->size(); x++) {
				auto lComponent = Globals::mComponentManager->GetComponentPtr((Component)components->at(x));
				switch (lComponent->componentType) {
					case(IComponent::TRANSFORM):
					{
						Transform* transform = (Transform*)lComponent;
						glm::translate(model, transform->position); //<< fix this too
					}
						break;
					case(IComponent::SHADER):
					{
						shader = (Shader*)lComponent;
						break;
					}
					case(IComponent::TEXTURE):
					{
						auto texobj = (Texture*)lComponent;
						if (texobj->loaded) {
							textureLoaded = true;
							texture = texobj->texture;
						}
						break;
					}
					default:
						continue;
						break;
				}
			}
			if (!shader) {
				continue;
			}


			glUseProgram(shader->ID);

			if (textureLoaded) {
				glBindTexture(GL_TEXTURE_2D, texture);
			}

			ParseCamera(shader->ID);
			SetMat4("model", model, shader->ID);

			drawModel(mesh->vaoAndEbos, mesh->model);
			glBindVertexArray(0);


		
	}





}