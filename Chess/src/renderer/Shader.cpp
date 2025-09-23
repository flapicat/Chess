#include "Shader.h"
#include <sstream>
#include <fstream>
#include <iostream>

Ref<Shader> Shader::Create(const std::string& filePath)
{
	return CreateRef<Shader>(filePath);
}

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath)
{
	auto src = ReadFile(filePath);
	auto stages = Preprocess(src);
	m_RendererID = Compile(stages);
}

Shader::~Shader()
{
	Delete();
}

void Shader::Bind()
{
	glUseProgram(m_RendererID);
}
void Shader::UnBind()
{
	glUseProgram(0);
}
void Shader::Delete()
{
	if (m_RendererID)
	{
		glDeleteProgram(m_RendererID);
		m_RendererID = 0;
	}
}


std::string Shader::ReadFile(const std::string& path)
{
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if (!in)
	{
		std::cerr << "Shader::ReadFile: failed to open " << path << "\n";
		return {};
	}

	std::ostringstream ss;
	ss << in.rdbuf();
	return ss.str();
}
GLenum Shader::ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")   return GL_VERTEX_SHADER;
	if (type == "fragment") return GL_FRAGMENT_SHADER;

	std::cerr << "Unknown shader type: " << type << "\n";
	return 0;
}

std::unordered_map<GLenum, std::string> Shader::Preprocess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const std::string typeToken = "#type ";
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		if (eol == std::string::npos)
			throw std::runtime_error("Shader preprocessing error: missing end of line after #type");

		size_t begin = pos + typeToken.size();
		std::string type = source.substr(begin, eol - begin);

		GLenum glType = ShaderTypeFromString(type);
		if (glType == 0)
			throw std::runtime_error("Shader preprocessing error: invalid shader type '" + type + "'");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		size_t nextTypePos = source.find(typeToken, nextLinePos);

		shaderSources[glType] = (nextTypePos == std::string::npos)
			? source.substr(nextLinePos)
			: source.substr(nextLinePos, nextTypePos - nextLinePos);

		pos = nextTypePos;
	}

	return shaderSources;
}
uint32_t Shader::Compile(const std::unordered_map<GLenum, std::string>& sources)
{
	GLuint program = glCreateProgram();
	std::vector<GLuint> shaderIDs;
	shaderIDs.reserve(sources.size());

	for (auto& kv : sources)
	{
		GLenum type = kv.first;
		const std::string& src = kv.second;

		GLuint shader = glCreateShader(type);
		const char* csrc = src.c_str();
		glShaderSource(shader, 1, &csrc, nullptr);
		glCompileShader(shader);

		if (!CheckShader(shader, (type == GL_VERTEX_SHADER ? "VERTEX" :
			type == GL_FRAGMENT_SHADER ? "FRAGMENT" :
			type == GL_GEOMETRY_SHADER ? "GEOMETRY" :
			type == GL_COMPUTE_SHADER ? "COMPUTE" :
			"SHADER")))
		{
			glDeleteShader(shader);
			for (auto id : shaderIDs) glDeleteShader(id);
			glDeleteProgram(program);
			return 0;
		}

		glAttachShader(program, shader);
		shaderIDs.push_back(shader);
	}

	glLinkProgram(program);

	for (auto id : shaderIDs)
		glDeleteShader(id);

	if (!CheckProgram(program, "PROGRAM"))
	{
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

bool Shader::CheckShader(GLuint shader, const std::string& tag)
{
	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint len = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		std::string log(len, '\0');
		glGetShaderInfoLog(shader, len, &len, log.data());
		std::cerr << "ERROR::" << tag << "_COMPILATION_FAILED\n" << log << "\n";
		return false;
	}
	return true;
}
bool Shader::CheckProgram(GLuint program, const std::string& tag)
{
	GLint success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint len = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		std::string log(len, '\0');
		glGetProgramInfoLog(program, len, &len, log.data());
		std::cerr << "ERROR::" << tag << "_LINK_FAILED\n" << log << "\n";
		return false;
	}
	return true;
}


void Shader::SetBool(const std::string& name, bool v)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(loc, (int)v);
}

void Shader::SetInt(const std::string& name, int v)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1i(loc, v);
}

void Shader::SetIntArray(const std::string& name, int count, int* v)
{
	glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, v);
}

void Shader::SetFloat(const std::string& name, float v)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform1f(loc, v);
}

void Shader::SetVec2(const std::string& name, const glm::vec2& v)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform2f(loc, v.x, v.y);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& v)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::SetVec4(const std::string& name, const glm::vec4& v)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::SetMat3(const std::string& name, const glm::mat3& m)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& m)
{
	GLint loc = glGetUniformLocation(m_RendererID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}