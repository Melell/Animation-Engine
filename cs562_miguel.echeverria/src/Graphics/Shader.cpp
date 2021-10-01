/**
* @file Shader.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Contains the opengl shader program interface.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "Shader.h"
#include <GL/glew.h>


namespace cs460
{
#define DEBUG 1

#if DEBUG
    static std::ostream& debug = std::cout;
#else
    static std::ostringstream debug;
#endif // DEBUG

    shader* shader::CreateShaderProgram(const std::string& vertShader, const std::string& fragShader)
    {
        shader* shaderProgram = new shader();
        // First pass shader program
        if (!shaderProgram->CompileShaderFromFile(vertShader.c_str(), GLSLShader::VERTEX))
        {
            std::cout << vertShader << std::endl << shaderProgram->Log() << std::endl;
        }
        if (!shaderProgram->CompileShaderFromFile(fragShader.c_str(), GLSLShader::FRAGMENT))
        {
            std::cout << fragShader << std::endl << shaderProgram->Log() << std::endl;
        }

        if (!shaderProgram->Link())
        {
            std::cout << shaderProgram->Log() << std::endl;
        }

        return shaderProgram;
    }

    shader::shader() : handle_(0), linked_(false) {}

    shader::~shader()
    {
        if (handle_ > 0)
            glDeleteProgram(handle_);
    }

    bool shader::CompileShaderFromFile(const char* fileName, GLSLShader::GLSLShaderType type)
    {
        if (!FileExists(fileName))
        {
            log_string_ = "File not found.";
            return false;
        }

        if (handle_ <= 0)
        {
            handle_ = glCreateProgram();
            if (handle_ == 0)
            {
                log_string_ = "Unable to create shader program.";
                return false;
            }
        }

        std::ifstream inFile(fileName, std::ios::in);
        if (!inFile)
        {
            return false;
        }

        std::ostringstream code;
        while (inFile.good())
        {
            int c = inFile.get();
            if (!inFile.eof())
                code << (char)c;
        }
        inFile.close();

        return CompileShaderFromString(code.str(), type);
    }

    bool shader::CompileShaderFromString(const std::string& source, GLSLShader::GLSLShaderType type)
    {
        if (handle_ <= 0)
        {
            handle_ = glCreateProgram();
            if (handle_ == 0)
            {
                log_string_ = "Unable to create shader program.";
                return false;
            }
        }

        GLuint shaderhandle_ = 0;

        switch (type)
        {
        case GLSLShader::VERTEX:
            shaderhandle_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case GLSLShader::FRAGMENT:
            shaderhandle_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case GLSLShader::GEOMETRY:
            shaderhandle_ = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case GLSLShader::TESS_CONTROL:
            shaderhandle_ = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case GLSLShader::TESS_EVALUATION:
            shaderhandle_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case GLSLShader::COMPUTE:
            shaderhandle_ = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            return false;
        }

        const char* c_code = source.c_str();
        glShaderSource(shaderhandle_, 1, &c_code, NULL);

        // Compile the shader
        glCompileShader(shaderhandle_);

        // Check for errors
        int result;
        glGetShaderiv(shaderhandle_, GL_COMPILE_STATUS, &result);
        if (GL_FALSE == result)
        {
            // Compile failed, store log and return false
            int length = 0;
            log_string_ = "";
            glGetShaderiv(shaderhandle_, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                char* c_log = new char[length];
                int    written = 0;
                glGetShaderInfoLog(shaderhandle_, length, &written, c_log);
                log_string_ = c_log;
                delete[] c_log;
            }

            return false;
        }
        else
        {
            // Compile succeeded, attach shader and return true
            glAttachShader(handle_, shaderhandle_);

            glDeleteShader(shaderhandle_);
            return true;
        }
    }

    bool shader::Link()
    {
        if (linked_)
            return true;
        if (handle_ <= 0)
            return false;

        glLinkProgram(handle_);

        int status = 0;
        glGetProgramiv(handle_, GL_LINK_STATUS, &status);
        if (GL_FALSE == status)
        {
            // Store log and return false
            int length = 0;
            log_string_ = "";

            glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &length);

            if (length > 0)
            {
                char* c_log = new char[length];
                int    written = 0;
                glGetProgramInfoLog(handle_, length, &written, c_log);
                log_string_ = c_log;
                delete[] c_log;
            }

            return false;
        }
        else
        {
            linked_ = true;
            return linked_;
        }
    }

    void shader::Use() const
    {
        if (handle_ <= 0 || (!linked_))
            return;

        glUseProgram(handle_);
    }

    std::string shader::Log() const { return log_string_; }

    int shader::GetHandle() const { return handle_; }

    bool shader::IsLinked() const { return linked_; }

    void shader::BindAttribLocation(GLuint location, const char* name)
    {
        glBindAttribLocation(handle_, location, name);
    }

    void shader::BindFragDataLocation(GLuint location, const char* name)
    {
        glBindFragDataLocation(handle_, location, name);
    }

    void shader::SetUniform(const std::string& name, float x, float y, float z) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniform3f(loc, x, y, z);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, const glm::vec2& v) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniform2f(loc, v.x, v.y);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, const glm::vec3& v) const
    {
        this->SetUniform(name, v.x, v.y, v.z);
    }

    void shader::SetUniform(const std::string& name, const glm::vec4& v) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniform4f(loc, v.x, v.y, v.z, v.w);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, const glm::mat4& m) const
    {
        int loc = GetUniformLocation(name);
        if (loc >= 0)
        {
            glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, const glm::mat3& m) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, float val) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniform1f(loc, val);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, int val) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniform1i(loc, val);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetUniform(const std::string& name, bool val) const
    {
        int loc = GetUniformLocation(name);

        if (loc >= 0)
        {
            glUniform1i(loc, val);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::SetSubroutineUniform(const std::string& name, const std::string& funcName) const
    {
        GLuint funcLoc = glGetSubroutineIndex(handle_, GL_FRAGMENT_SHADER, funcName.c_str());
        GLuint loc = glGetSubroutineUniformLocation(handle_, GL_FRAGMENT_SHADER, name.c_str());

        if (loc >= 0)
        {

            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &funcLoc);
        }
        else
        {
            debug << "Uniform: " << name << " not found." << std::endl;
        }
    }

    void shader::PrintActiveUniforms() const
    {
        GLint    nUniforms, size, location, maxLen;
        GLchar* name;
        GLsizei  written;
        GLenum   type;

        glGetProgramiv(handle_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
        glGetProgramiv(handle_, GL_ACTIVE_UNIFORMS, &nUniforms);

        name = (GLchar*)malloc(maxLen);

        printf(" Location | Name\n");
        printf("------------------------------------------------\n");
        for (int i = 0; i < nUniforms; ++i)
        {
            glGetActiveUniform(handle_, i, maxLen, &written, &size, &type, name);
            location = glGetUniformLocation(handle_, name);
            printf(" %-8d | %s\n", location, name);
        }

        free(name);
    }

    void shader::PrintActiveAttribs() const
    {

        GLint    written, size, location, maxLength, nAttribs;
        GLenum   type;
        GLchar* name;

        glGetProgramiv(handle_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
        glGetProgramiv(handle_, GL_ACTIVE_ATTRIBUTES, &nAttribs);

        name = (GLchar*)malloc(maxLength);

        printf(" Index | Name\n");
        printf("------------------------------------------------\n");
        for (int i = 0; i < nAttribs; i++)
        {
            glGetActiveAttrib(handle_, i, maxLength, &written, &size, &type, name);
            location = glGetAttribLocation(handle_, name);
            printf(" %-5d | %s\n", location, name);
        }

        free(name);
    }

    bool Shader::Validate()
    {
        if (!IsLinked())
            return false;

        GLint status;
        glValidateProgram(m_handle);
        glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &status);

        if (GL_FALSE == status)
        {
            // Store log and return false
            int length = 0;
            m_log = "";

            glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &length);

            if (length > 0)
            {
                char* c_log = new char[length];
                int    written = 0;
                glGetProgramInfoLog(m_handle, length, &written, c_log);
                m_log = c_log;
                delete[] c_log;
            }

            return false;
        }
        else
        {
            return true;
        }
    }

    int Shader::get_uniform_location(const std::string& name) const
    {
        return glGetUniformLocation(m_handle, name.c_str());
    }

    bool Shader::file_exists(const std::string& fileName)
    {
        return fs::exists(fs::path(fileName));
    }
}