#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace amund::graphics
{

class SpirvSpecification
{
    std::string entrypoint;
    std::vector<unsigned int> constant_indices;
    std::vector<unsigned int> constant_values;

public:

    explicit SpirvSpecification (std::string entrypoint_use) : entrypoint (entrypoint_use)
    {
    }

    void add_constant(unsigned int index, unsigned int value)
    {
        constant_indices.push_back(index);
        constant_values.push_back(value);
    }

    void specialize_shader (unsigned int shader_id) const
    {     
        int spirv_status = 0;
        glGetShaderiv(shader_id, GL_SPIR_V_BINARY, &spirv_status);
        if (spirv_status == GL_FALSE) 
        {
            throw std::runtime_error("SPIR-V shader does not match binary format.");
        }

        assert(constant_indices.size() == constant_values.size());   
        if (constant_indices.empty ())
        {
            glSpecializeShader(shader_id, entrypoint.c_str (),
                0, nullptr, nullptr);
        }
        else
        {
            glSpecializeShader(shader_id, entrypoint.c_str (), 
                constant_indices.size(), constant_indices.data(), constant_values.data());
        }

        int compile_status;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
        if (compile_status == GL_FALSE) 
        {
            throw std::runtime_error("SPIR-V shader did not specialize properly.");
        }
        
    }
};

class Shader
{
unsigned int id;

public:

Shader(GLenum shader_type, const SpirvSpecification& spec, std::filesystem::path loc)
{
    id = glCreateShader (shader_type);

    auto stream = std::ifstream(loc.c_str(), std::ios::binary|std::ios::ate|std::ios::in);

    if (!stream.is_open())
    {
        throw std::runtime_error("Could not open " + loc.generic_string ());
    }
    
    auto file_size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    auto file_binary = std::vector<char> (file_size);
    stream.read(file_binary.data(), file_size);
    stream.close();

    glShaderBinary(1, &id, GL_SHADER_BINARY_FORMAT_SPIR_V, file_binary.data(), file_binary.size());

    spec.specialize_shader(id);
}

void attach (unsigned int program)
{
    glAttachShader(program, id);
}

void detach (unsigned int program)
{
    glDetachShader(program, id);
}

~Shader()
{
    glDeleteShader(id);
}

};

class AttachedShader
{
    Shader& shader;
    unsigned int program_id;

public:

AttachedShader (Shader& shader_use, unsigned int program) :
    shader(shader_use),
    program_id(program)
{
    shader.attach(program_id);
}

~AttachedShader()
{
    shader.detach(program_id);
}

};

class ShaderProgram
{
protected:
    
    unsigned int id;

public:

    explicit ShaderProgram ()
    {
        id = glCreateProgram();
    }

    virtual ~ShaderProgram()
    {
        glDeleteProgram(id);
    }

    void use()
    {
        glUseProgram(id);
    }

    unsigned int get_id()
    {
        return id;
    }

    void set_bool(const std::string& name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value); 
    }
    void set_int(const std::string& name, int value) const
    { 
        glUniform1i(glGetUniformLocation(id, name.c_str()), value); 
    }
    void set_float(const std::string& name, float value) const
    { 
        glUniform1f(glGetUniformLocation(id, name.c_str()), value); 
    } 

    void set_vec3(const std::string& name, const glm::vec3& value) const
    { 
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, glm::value_ptr(value)); 
    } 

    void set_mat3(const std::string& name, const glm::mat3& value) const
    { 
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
    } 
    
    void set_mat4(const std::string& name, const glm::mat4& value) const
    { 
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value)); 
    } 
};

class GraphicsShaderProgram : public ShaderProgram
{
    std::unique_ptr<Shader> vertex_shader;
    std::unique_ptr<Shader> fragment_shader;

public:

    GraphicsShaderProgram(std::filesystem::path vertex_shader_loc, std::filesystem::path fragment_shader_loc)
    {
        //Set up shaders
        std::cout << "Setup shaders" << std::endl;
        auto const spirv_spec = SpirvSpecification("main");
        vertex_shader = std::make_unique<Shader> (GL_VERTEX_SHADER, spirv_spec, vertex_shader_loc);
        fragment_shader = std::make_unique<Shader> (GL_FRAGMENT_SHADER, spirv_spec, fragment_shader_loc);

        //Attach shaders
        std::cout << "Attach shaders" << std::endl;
        {
            assert(vertex_shader != nullptr && fragment_shader != nullptr);
            AttachedShader attached_vs (*vertex_shader, id); 
            AttachedShader attached_fs (*fragment_shader, id); 

            std::cout << "Link Program" << std::endl;
            glLinkProgram(id);

            int link_status = 0;
            glGetProgramiv (id, GL_LINK_STATUS, &link_status);

            if(link_status == GL_FALSE)
            {
                throw std::runtime_error("Unable to link shader program properly.");
            }
        }

    }
};

}