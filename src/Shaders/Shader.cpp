#include"Shader.h"
#include<stdlib.h>
#include<glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

void CheckShaderCompilationError(GLuint shader, std::string name)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << name << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}
}

void CheckShaderProgramLinkingError(GLuint shaderProgram)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::LINKING_FAILED\n" << infoLog << std::endl;
		exit(EXIT_FAILURE);
	}
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{


	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	// Check for compilation errors
	CheckShaderCompilationError(vertexShader, "VERTEX");

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	// Check for compilation errors
	CheckShaderCompilationError(fragmentShader, "FRAGMENT");


	// Create Shader Program Object and get its reference
	ID = glCreateProgram();

	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);

	glAttachShader(ID, fragmentShader);

	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);

	// Check for Linking errors
	CheckShaderProgramLinkingError(ID);


	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}


void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void CheckUniformSetError(int uniformLoc, std::string uniformName)
{
	if (uniformLoc == -1)
	{
		//std::cout << "Uniform name: " << uniformName << " not found" << std::endl;
		//exit(EXIT_FAILURE);
	}
}

void Shader::setMat4(std::string uniformName, glm::mat4 uniformValue)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
	CheckUniformSetError(uniformLoc, uniformName);
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(uniformValue));
}
void Shader::setFloat(std::string uniformName, float uniformValue)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
	CheckUniformSetError(uniformLoc, uniformName);
	glUniform1f(uniformLoc, uniformValue);
}

void Shader::setInt(std::string uniformName, int uniformValue)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
	CheckUniformSetError(uniformLoc, uniformName);
	glUniform1i(uniformLoc, uniformValue);
}

void Shader::setVec3(std::string uniformName, float x, float y, float z)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
	CheckUniformSetError(uniformLoc, uniformName);
	glUniform3f(uniformLoc, x, y, z);
}

void Shader::setVec3(std::string uniformName, glm::vec3 value)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
	CheckUniformSetError(uniformLoc, uniformName);
	glUniform3f(uniformLoc, value.x, value.y, value.z);
}

void Shader::setBool(std::string uniformName, bool value)
{
	int uniformLoc = glGetUniformLocation(ID, uniformName.c_str());
	CheckUniformSetError(uniformLoc, uniformName);
	glUniform1i(uniformLoc, value);
}

void printError() {
	GLenum code;
	const GLubyte* string;
	code = glGetError();
	switch(code)
	{
		// opengl 2 errors (8)
    case GL_NO_ERROR:
		fprintf(stderr, "GL_NO_ERROR\n");
		break;

	case GL_INVALID_ENUM:
		fprintf(stderr, "GL_INVALID_ENUM\n");
		break;

	case GL_INVALID_VALUE:
		fprintf(stderr, "GL_INVALID_VALUE\n");
		break;

	case GL_INVALID_OPERATION:
		fprintf(stderr, "GL_INVALID_OPERATION\n");
		break;

	/*case GL_STACK_OVERFLOW:
		fprintf(stderr, "GL_STACK_OVERFLOW");

	case GL_STACK_UNDERFLOW:
		fprintf(stderr, "GL_STACK_UNDERFLOW");*/

	case GL_OUT_OF_MEMORY:
		fprintf(stderr, "GL_OUT_OF_MEMORY\n");
		break;

	/*case GL_TABLE_TOO_LARGE:
		fprintf(stderr, "GL_TABLE_TOO_LARGE");*/

		// opengl 3 errors (1)
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		fprintf(stderr, "GL_INVALID_FRAMEBUFFER_OPERATION\n");
		break;

		// gles 2, 3 and gl 4 error are handled by the switch above
	default:
		fprintf(stderr, "unknown error\n");
		break;
	}
}



