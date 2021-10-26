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


    // Uploads the source, compiles and links the provided vertex and fragment shaders into an opengl shader program
    void Shader::create_shader_program(const std::string& vertShader, const std::string& fragShader)
    {
        // First pass shader program
        if (!compile_shader_from_file(vertShader.c_str(), Type::VERTEX))
            std::cout << vertShader << std::endl << log() << std::endl;

        if (!compile_shader_from_file(fragShader.c_str(), Type::FRAGMENT))
            std::cout << fragShader << std::endl << log() << std::endl;

        if (!link())
            std::cout << log() << std::endl;
    }

    Shader::Shader()
        :   m_handle(0),
            m_linked(false)
    {
    }

    Shader::~Shader()
    {
        if (m_handle > 0)
            glDeleteProgram(m_handle);
    }

    bool Shader::compile_shader_from_file(const char* fileName, Shader::Type type)
    {
        if (!file_exists(fileName))
        {
            m_log = "File not found.";
            return false;
        }

        if (m_handle <= 0)
        {
            m_handle = glCreateProgram();
            if (m_handle == 0)
            {
                m_log = "Unable to create shader program.";
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

        return compile_shader_from_string(code.str(), type);
    }

    bool Shader::compile_shader_from_string(const std::string& source, Shader::Type type)
    {
        if (m_handle <= 0)
        {
            m_handle = glCreateProgram();
            if (m_handle == 0)
            {
                m_log = "Unable to create shader program.";
                return false;
            }
        }

        GLuint shaderhandle_ = 0;

        switch (type)
        {
        case Type::VERTEX:
            shaderhandle_ = glCreateShader(GL_VERTEX_SHADER);
            break;
        case Type::FRAGMENT:
            shaderhandle_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case Type::GEOMETRY:
            shaderhandle_ = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case Type::TESSELATION_CONTROL:
            shaderhandle_ = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case Type::TESSELATION_EVALUATION:
            shaderhandle_ = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case Type::COMPUTE:
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
            m_log = "";
            glGetShaderiv(shaderhandle_, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                char* c_log = new char[length];
                int    written = 0;
                glGetShaderInfoLog(shaderhandle_, length, &written, c_log);
                m_log = c_log;
                delete[] c_log;
            }

            return false;
        }
        else
        {
            // Compile succeeded, attach shader and return true
            glAttachShader(m_handle, shaderhandle_);

            glDeleteShader(shaderhandle_);
            return true;
        }
    }

    bool Shader::link()
    {
        if (m_linked)
            return true;
        if (m_handle <= 0)
            return false;

        glLinkProgram(m_handle);

        int status = 0;
        glGetProgramiv(m_handle, GL_LINK_STATUS, &status);
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
            m_linked = true;
            return m_linked;
        }
    }

    void Shader::use() const
    {
        if (m_handle <= 0 || (!m_linked))
            return;

        glUseProgram(m_handle);
    }

    std::string Shader::log() const
    {
        return m_log;
    }

    int Shader::get_handle() const
    {
        return m_handle;
    }

    bool Shader::is_linked() const
    {
        return m_linked;
    }

    void Shader::bind_attrib_location(unsigned int location, const char* name)
    {
        glBindAttribLocation(m_handle, location, name);
    }

    void Shader::bind_frag_data_location(unsigned int location, const char* name)
    {
        glBindFragDataLocation(m_handle, location, name);
    }

    void Shader::set_uniform(const std::string& name, float x, float y, float z) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniform3f(loc, x, y, z);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, const glm::vec2& v) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniform2f(loc, v.x, v.y);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, const glm::vec3& v) const
    {
        set_uniform(name, v.x, v.y, v.z);
    }

    void Shader::set_uniform(const std::string& name, const glm::vec4& v) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniform4f(loc, v.x, v.y, v.z, v.w);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, const glm::mat4& m) const
    {
        int loc = get_uniform_location(name);
        if (loc >= 0)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, const glm::mat3& m) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, float val) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniform1f(loc, val);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, int val) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniform1i(loc, val);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_uniform(const std::string& name, bool val) const
    {
        int loc = get_uniform_location(name);

        if (loc >= 0)
            glUniform1i(loc, val);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::set_subroutine_uniform(const std::string& name, const std::string& funcName) const
    {
        GLuint funcLoc = glGetSubroutineIndex(m_handle, GL_FRAGMENT_SHADER, funcName.c_str());
        GLuint loc = glGetSubroutineUniformLocation(m_handle, GL_FRAGMENT_SHADER, name.c_str());

        if (loc >= 0)
            glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &funcLoc);
        else
            debug << "Uniform: " << name << " not found." << std::endl;
    }

    void Shader::print_active_uniforms() const
    {
        GLint    nUniforms, size, location, maxLen;
        GLchar* name;
        GLsizei  written;
        GLenum   type;

        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
        glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &nUniforms);

        name = (GLchar*)malloc(maxLen);

        printf(" Location | Name\n");
        printf("------------------------------------------------\n");
        for (int i = 0; i < nUniforms; ++i)
        {
            glGetActiveUniform(m_handle, i, maxLen, &written, &size, &type, name);
            location = glGetUniformLocation(m_handle, name);
            printf(" %-8d | %s\n", location, name);
        }

        free(name);
    }

    void Shader::print_active_attribs() const
    {

        GLint    written, size, location, maxLength, nAttribs;
        GLenum   type;
        GLchar* name;

        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
        glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

        name = (GLchar*)malloc(maxLength);

        printf(" Index | Name\n");
        printf("------------------------------------------------\n");
        for (int i = 0; i < nAttribs; i++)
        {
            glGetActiveAttrib(m_handle, i, maxLength, &written, &size, &type, name);
            location = glGetAttribLocation(m_handle, name);
            printf(" %-5d | %s\n", location, name);
        }

        free(name);
    }

    bool Shader::validate()
    {
        if (!is_linked())
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