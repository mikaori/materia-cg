#include "material.hpp"
#include <filesystem>

void Material::materialLoadLocation() {
  m_diffuseTexLocation = abcg::glGetUniformLocation(m_program, "diffuseTex");
  m_KaLocation = abcg::glGetUniformLocation(m_program, "Ka");
  m_KdLocation = abcg::glGetUniformLocation(m_program, "Kd");
  m_KsLocation = abcg::glGetUniformLocation(m_program, "Ks");
  m_shininessLocation = abcg::glGetUniformLocation(m_program, "shininess");
  m_mappingModeLocation = abcg::glGetUniformLocation(m_program, "mappingMode");
}

void Material::materialBindLocation() {
  abcg::glUniform1i(m_diffuseTexLocation, 0);
  abcg::glUniform4fv(m_KaLocation, 1, getKa());
  abcg::glUniform4fv(m_KdLocation, 1, getKd());
  abcg::glUniform4fv(m_KdLocation, 1, getKs());
  abcg::glUniform1f(m_shininessLocation, getShininess());
  abcg::glUniform1i(m_mappingModeLocation, getMappingMode());
}

void Material::materialLoadProperties(const std::string basePath,
                                      tinyobj::ObjReader reader) {

  auto const &materials{reader.GetMaterials()};

  if (!materials.empty()) {
    auto const &mat{materials.at(0)}; // First material
    Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2], 1};
    Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
    Ks = {mat.specular[0], mat.specular[1], mat.specular[2], 1};
    shininess = mat.shininess;

    if (!mat.diffuse_texname.empty()) {
      materialLoadDiffuseTexture(basePath + mat.diffuse_texname);
    }
  } else {
    // Default values
    Ka = {0.1f, 0.1f, 0.1f, 1.0f};
    Kd = {0.7f, 0.7f, 0.7f, 1.0f};
    Ks = {1.0f, 1.0f, 1.0f, 1.0f};
    shininess = 25.0f;
  }
}

void Material::materialLoadDiffuseTexture(std::string_view path) {
  if (!std::filesystem::exists(path))
    return;

  abcg::glDeleteTextures(1, &m_diffuseTexture);
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = path});
}

void Material::materialBindTexture() {
  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // Set minification and magnification parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Material::materialLoadModelFromFile(std::string_view path,
                                         bool standardize) {

  tinyobj::ObjReader reader;
  tinyobj::ObjReaderConfig readerConfig;

  auto const basePath{std::filesystem::path{path}.parent_path().string() + "/"};
  readerConfig.mtl_search_path = basePath; // Path to material files

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }
  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  modelLoadModelFromFile(reader, standardize);

  materialLoadProperties(basePath, reader);
}