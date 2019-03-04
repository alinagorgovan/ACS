#include "Cue.h"

void Cue::RenderCue(Tema::Camera* camera, glm::mat4 projectionMatrix, glm::vec3 whiteBallPos) {
	if (this->aimed) {
		if (this->translateCueX > 0) {
			this->translateCueX -= 0.02f;
		}
		else {
			this->aimed = false;
			this->renderCue = false;
			this->shoot = true;
			this->translateCueX = 0;
		}
	}
	this->angle = this->deltaX * 0.001;
	this->modelMatrix = glm::mat4(1);
	this->modelMatrix *= Transform3DH::Translate(whiteBallPos.x + 1 * sin(this->angle), whiteBallPos.y, whiteBallPos.z + 1 * cos(this->angle));
	this->modelMatrix *= Transform3DH::RotateOY(this->angle);
	this->modelMatrix *= Transform3DH::Scale(scale.x, scale.y, scale.z);
	this->SetTranslate(glm::vec3(whiteBallPos.x + 1 * sin(this->angle), whiteBallPos.y, whiteBallPos.z + 1 * cos(this->angle)));
	
	if (!mesh || !shader || !shader->program)
		return;

	glUseProgram(shader->program);
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	//glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	glUniform3fv(glGetUniformLocation(shader->program, "color"), 1, glm::value_ptr(this->GetBaseColor()));
	glUniform1f(glGetUniformLocation(shader->GetProgramID(), "dx"), translateCueX);
	glUniform1f(glGetUniformLocation(shader->program, "alpha"), this->angle);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}
void Cue::Aim()
{
	if (!this->aimed) {
		this->translateCueX += sign * 0.01;
		if (this->translateCueX < 0 || this->translateCueX > maxAimDistance) {
			this->sign = -this->sign;
		}
	}
}