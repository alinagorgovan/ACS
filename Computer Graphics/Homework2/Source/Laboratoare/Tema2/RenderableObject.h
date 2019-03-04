#pragma once

#include <Core/Engine.h>
#include "Transform3DH.h"

class RenderableObject
{
public:
	RenderableObject()
	{
		baseColor = glm::vec3(1);
		modelMatrix = glm::mat4(1);
		translate = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		scale = glm::vec3(1, 1, 1);
	}

	RenderableObject(Mesh *mesh, Shader *shader, glm::vec3 color, glm::vec3 position, glm::vec3 scaleFactor)
	{
		this->mesh = mesh;
		this->baseColor = color;
		this->shader = shader;
		this->translate = position;
		this->scale = scaleFactor;
		modelMatrix = glm::mat4(1);
		rotation = glm::vec3(0, 0, 0);
	}

	Mesh* GetMesh() { return mesh; }
	void SetMesh(Mesh *mesh) { this->mesh = mesh; }

	Shader* GetShader() { return shader; }
	void SetShader(Shader *shader) { this->shader = shader; }

	glm::vec3 GetBaseColor() { return baseColor; }
	void SetBaseColor(const glm::vec3 &color) { baseColor = color; }

	glm::mat4 GetModel() {
		modelMatrix = glm::mat4(1);
		modelMatrix *= Transform3DH::Translate(translate.x, translate.y, translate.z);
		modelMatrix *= Transform3DH::RotateOX(rotation.x);
		modelMatrix *= Transform3DH::RotateOY(rotation.y);
		modelMatrix *= Transform3DH::RotateOZ(rotation.z);
		modelMatrix *= Transform3DH::Scale(scale.x, scale.y, scale.z);

		return modelMatrix;
	}
	void SetModel(glm::mat4 &model) { modelMatrix = model; }

	glm::vec3 GetTranslate() { return translate; }
	void SetTranslate(const glm::vec3 &position) { translate = position; }

	glm::vec3 GetRotation() { return rotation; }
	void SetRotation(const glm::vec3 &radians) { rotation = radians; }

	glm::vec3 GetScale() { return scale; }
	void SetScale(const glm::vec3 &scaleFactor) { scale = scaleFactor; }

public:
	glm::vec3 translate;
	glm::vec3 rotation;
	glm::vec3 scale;
protected:
	Mesh *mesh;
	Shader *shader;
	glm::vec3 baseColor;
	glm::mat4 modelMatrix;
};
