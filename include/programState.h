#pragma once
#include <GLEW/glew.h>
#include <unordered_map>
#include <array>

namespace my_gl {
    template<std::size_t ArrSize>
    struct UniformData {
        GLint location;
        const char* name;
        std::array<float, ArrSize> value;
    };

    struct AttribData {
        GLint location;
        const char* name;
    };

    class ProgramState {
    public:
        ProgramState(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
        ~ProgramState();
    
    // program
        const GLuint& getId() const;
        void use() const;
        void unuse() const;

    // uniform manip
        template<std::size_t ArrSize>
        void setUniform(const char* name, const std::array<float, ArrSize>& value);

        template<std::size_t ArrSize>
        const UniformData<ArrSize>* getUniform(const char* name) const;
        
        template<std::size_t ArrSize>
        bool useUniform(const char* name) const;
    
    // attribs manip
        void setAttrib(const char* name);
        const AttribData* getAttrib(const char* name) const;
        //bool useAttrib(const char* name) const;

    private:
        std::unordered_map<const char*, UniformData<1>> m_uniforms_1f;
        std::unordered_map<const char*, UniformData<2>> m_uniforms_2f;
        std::unordered_map<const char*, UniformData<3>> m_uniforms_3f;
        std::unordered_map<const char*, UniformData<4>> m_uniforms_4f;

        std::unordered_map<const char*, AttribData> m_attribs;
        GLuint m_id;
    };
}