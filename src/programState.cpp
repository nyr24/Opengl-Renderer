#include "utils.h"
#include "programState.h"

template void my_gl::ProgramState::setUniform<1>(const char*, const std::array<float, 1>&);
template void my_gl::ProgramState::setUniform<2>(const char*, const std::array<float, 2>&);
template void my_gl::ProgramState::setUniform<3>(const char*, const std::array<float, 3>&);
template void my_gl::ProgramState::setUniform<4>(const char*, const std::array<float, 4>&);

template const my_gl::UniformData<1>* my_gl::ProgramState::getUniform(const char*) const;
template const my_gl::UniformData<2>* my_gl::ProgramState::getUniform(const char*) const;
template const my_gl::UniformData<3>* my_gl::ProgramState::getUniform(const char*) const;
template const my_gl::UniformData<4>* my_gl::ProgramState::getUniform(const char*) const;

template bool my_gl::ProgramState::useUniform<1>(const char*) const;
template bool my_gl::ProgramState::useUniform<2>(const char*) const;
template bool my_gl::ProgramState::useUniform<3>(const char*) const;
template bool my_gl::ProgramState::useUniform<4>(const char*) const; 


my_gl::ProgramState::ProgramState(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) 
    : m_id{ my_gl::createProgram(vertexShaderFilePath, fragmentShaderFilePath) }
{}

my_gl::ProgramState::~ProgramState() {
    unuse();
}

const GLuint& my_gl::ProgramState::getId() const {
    return m_id;
}

// Uniforms
template<std::size_t ArrSize>
void my_gl::ProgramState::setUniform(const char* name, const std::array<float, ArrSize>& value) {
    static_assert((ArrSize >= 1 && ArrSize <= 4) && "Invalid uniform arr size");

    my_gl::UniformData<ArrSize> uniformData{
        .location = glGetUniformLocation(m_id, name),
        .name = name,
        .value = value,
    };

    switch (ArrSize) {
    case 1:
        m_uniforms_1f[name] = std::move(uniformData);
        break;
    case 2:
        m_uniforms_2f[name] = std::move(uniformData);
        break;
    case 3:
        m_uniforms_3f[name] = std::move(uniformData);
        break;
    case 4:
        m_uniforms_4f[name] = std::move(uniformData);
        break;
    }
}


template<std::size_t ArrSize>
const my_gl::UniformData<ArrSize>* my_gl::ProgramState::getUniform(const char* name) const {
    static_assert((ArrSize >= 1 && ArrSize <= 4) && "Invalid uniform arr size");

    switch (static_cast<int>(ArrSize)) {
    case 1: {
        auto val = m_uniforms_1f.find(name);
        return val != m_uniforms_1f.end() 
                ? &val->second
                : nullptr;
    }
    case 2: {
        auto val = m_uniforms_2f.find(name);
        return val != m_uniforms_2f.end() 
                ? &val->second
                : nullptr;
    }
    case 3: {
        auto val = m_uniforms_3f.find(name);
        return val != m_uniforms_3f.end() 
                ? &val->second
                : nullptr;
    }
    case 4: {
        auto val = m_uniforms_4f.find(name);
        return val != m_uniforms_4f.end() 
                ? &val->second
                : nullptr;
    }
    }
}

template<std::size_t ArrSize>
bool my_gl::ProgramState::useUniform(const char* name) const {
    static_assert((ArrSize >= 1 && ArrSize <= 4) && "Invalid uniform arr size");

    switch (static_cast<int>(ArrSize)) {
    case 1: {
        auto val = m_uniforms_1f.find(name);
        if (val == m_uniforms_1f.end()) {
            return false;
        }
        else {
            const auto& uniformData{ val->second };
            glUniform1fv(uniformData.location, 1, uniformData.value.data());
            return true;
        }
    }
    case 2: {
        auto val = m_uniforms_2f.find(name);
        if (val == m_uniforms_2f.end()) {
            return false;
        }
        else {
            const auto& uniformData{ val->second };
            glUniform2fv(uniformData.location, 1, uniformData.value.data());
            return true;
        }
    }
    case 3: {
        auto val = m_uniforms_3f.find(name);
        if (val == m_uniforms_3f.end()) {
            return false;
        }
        else {
            const auto& uniformData{ val->second };
            glUniform3fv(uniformData.location, 1, uniformData.value.data());
            return true;
        }
    }
    case 4: {
        auto val = m_uniforms_4f.find(name);
        if (val == m_uniforms_4f.end()) {
            return false;
        }
        else {
            const auto& uniformData{ val->second };
            glUniform4fv(uniformData.location, 1, uniformData.value.data());
            return true;
        }
    }
    }
}

// Attribs
void my_gl::ProgramState::setAttrib(const char* name) {
    my_gl::AttribData attribData{
        .location = glGetAttribLocation(m_id, name),
        .name = name,
    };

    m_attribs[name] = std::move(attribData);
}

const my_gl::AttribData* my_gl::ProgramState::getAttrib(const char* name) const {
    auto val{ m_attribs.find(name) };
    return val != m_attribs.end()
            ? &val->second
            : nullptr;
}

// Program util
void my_gl::ProgramState::use() const {
    glUseProgram(m_id);
}

void my_gl::ProgramState::unuse() const {
    glUseProgram(0);
}
