#pragma once
#include "std.h"

GLvoid ortho(GLuint shaderProgramID, GLfloat space)
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(-space, space, -space, space, -space * 10, space * 10);

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}


GLvoid perspective(GLuint shaderProgramID, GLfloat space)
{
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(80.0f), 1.0f, 0.1f, 1000.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, space));

	unsigned int projectionLocation = glGetUniformLocation(shaderProgramID, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

}

glm::mat4 Translate(glm::vec3 T) {

	glm::mat4 translateMatrix(1.0f);

	return glm::translate(translateMatrix, T);
}

glm::mat4 Rotation(GLfloat RX, GLfloat RY, GLfloat RZ) {

	glm::mat4 rotateMatrixX(1.0f);
	glm::mat4 rotateMatrixY(1.0f);
	glm::mat4 rotateMatrixZ(1.0f);

	rotateMatrixX = glm::rotate(rotateMatrixX, glm::radians(RX), glm::vec3(1.0, 0.0, 0.0));
	rotateMatrixY = glm::rotate(rotateMatrixY, glm::radians(RY), glm::vec3(0.0, 1.0, 0.0));
	rotateMatrixZ = glm::rotate(rotateMatrixZ, glm::radians(RZ), glm::vec3(0.0, 0.0, 1.0));

	return (rotateMatrixX * rotateMatrixY * rotateMatrixZ);
}

glm::mat4 Scale(glm::vec3 S) {

	glm::mat4 scaleMatrix(1.0f);
	return glm::scale(scaleMatrix, S);
}

void topview(GLuint shaderProgramID)
{
	glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0, 70, 0), glm::vec3(0, 0, 0), cameraUp);
	unsigned int viewLocation = glGetUniformLocation(shaderProgramID, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
}
