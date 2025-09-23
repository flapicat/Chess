#pragma once
#include "Core/core.h"
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
public:
	static Ref<Shader> Create(const std::string& filePath);
	Shader(const std::string& filePath);
	~Shader();
	
	void Bind();
	void UnBind();
	void Delete();

	void SetBool(const std::string& name, bool v);
	void SetInt(const std::string& name, int v);
	void SetIntArray(const std::string& name, int count, int* v);
	void SetFloat(const std::string& name, float v);
	void SetVec2(const std::string& name, const glm::vec2& v);
	void SetVec3(const std::string& name, const glm::vec3& v);
	void SetVec4(const std::string& name, const glm::vec4& v);
	void SetMat3(const std::string& name, const glm::mat3& m);
	void SetMat4(const std::string& name, const glm::mat4& m);
private:
	std::string m_FilePath;
	uint32_t m_RendererID = 0;

	static bool CheckShader(GLuint shader, const std::string& tag);
	static bool CheckProgram(GLuint program, const std::string& tag);

	static std::string ReadFile(const std::string& path);
	static GLenum ShaderTypeFromString(const std::string& type);
	static std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
	static uint32_t Compile(const std::unordered_map<GLenum, std::string>& sources);
};