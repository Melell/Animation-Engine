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
    // This class was mainly adapted from cs350
	class Shader
	{
    public:

        enum class Type
        {
            VERTEX,
            FRAGMENT,
            TESSELATION_CONTROL,
            TESSELATION_EVALUATION,
            GEOMETRY,
            COMPUTE
        };

        // Uploads the source, compiles and links the provided vertex and fragment shaders into an opengl shader program
        void create_shader_program(const std::string& vertShader, const std::string& fragShader);

        Shader();
        ~Shader();

        bool compile_shader_from_file(const char* fileName, Shader::Type type);
        bool compile_shader_from_string(const std::string& source, Shader::Type type);
        bool link();
        bool validate();
        void use() const;

        std::string log() const;

        // Getters for the opengl shader program handle, and whether the program has been linked
        int  get_handle() const;
        bool is_linked() const;

        // Bind an attribute or frag data at location, to the variable "name"
        void bind_attrib_location(unsigned int location, const char* name);
        void bind_frag_data_location(unsigned int location, const char* name);

        // Uniform/s setting function
        void set_uniform(const std::string& name, float x, float y, float z) const;
        void set_uniform(const std::string& name, const glm::vec2& v) const;
        void set_uniform(const std::string& name, const glm::vec3& v) const;
        void set_uniform(const std::string& name, const glm::vec4& v) const;
        void set_uniform(const std::string& name, const glm::mat4& m) const;
        void set_uniform(const std::string& name, const glm::mat3& m) const;
        void set_uniform(const std::string& name, float val) const;
        void set_uniform(const std::string& name, int val) const;
        void set_uniform(const std::string& name, bool val) const;
        void set_subroutine_uniform(const std::string& name, const std::string& funcName) const;

        // Print all the active uniforms and attributes
        void print_active_uniforms() const;
        void print_active_attribs() const;

    private:
        int  get_uniform_location(const std::string& name) const;   // Helper function to get the uniform location of the variable "name"
        bool file_exists(const std::string& fileName);              // Helper function to check whether the given path leads to an existing file

        unsigned m_handle;
        bool m_linked;
        std::string m_log;
	};
}