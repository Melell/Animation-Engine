/**
* @file Shader.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the opengl shader program interface.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Shader
	{
    public:
        static Shader* CreateShaderProgram(const std::string& vertShader,
            const std::string& fragShader);

        Shader();
        ~Shader();

        bool CompileShaderFromFile(const char* fileName, GLSLShader::GLSLShaderType type);
        bool CompileShaderFromString(const std::string& source, GLSLShader::GLSLShaderType type);
        bool Link();
        bool Validate();
        void Use() const;

        std::string Log() const;

        int  GetHandle() const;
        bool IsLinked() const;

        void BindAttribLocation(GLuint location, const char* name);
        void BindFragDataLocation(GLuint location, const char* name);

        void SetUniform(const std::string& name, float x, float y, float z) const;
        void SetUniform(const std::string& name, const glm::vec2& v) const;
        void SetUniform(const std::string& name, const glm::vec3& v) const;
        void SetUniform(const std::string& name, const glm::vec4& v) const;
        void SetUniform(const std::string& name, const glm::mat4& m) const;
        void SetUniform(const std::string& name, const glm::mat3& m) const;
        void SetUniform(const std::string& name, float val) const;
        void SetUniform(const std::string& name, int val) const;
        void SetUniform(const std::string& name, bool val) const;
        void SetSubroutineUniform(const std::string& name, const std::string& funcName) const;

        void PrintActiveUniforms() const;
        void PrintActiveAttribs() const;

    private:
        int  get_uniform_location(const std::string& name) const;
        bool file_exists(const std::string& fileName);

        unsigned m_handle;
        bool m_linked;
        std::string m_log;
	};
}