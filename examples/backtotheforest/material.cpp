#include "material.hpp"
#include <filesystem>

// recupera a localização das variáveis uniformes no shader relacionadas ao material
void Material::materialLoadLocation() {
  m_diffuseTexLocation = abcg::glGetUniformLocation(m_program, "diffuseTex");
  m_KaLocation = abcg::glGetUniformLocation(m_program, "Ka");
  m_KdLocation = abcg::glGetUniformLocation(m_program, "Kd");
  m_KsLocation = abcg::glGetUniformLocation(m_program, "Ks");
  m_shininessLocation = abcg::glGetUniformLocation(m_program, "shininess");
  m_mappingModeLocation = abcg::glGetUniformLocation(m_program, "mappingMode");
}

// define os valores das variáveis uniformes no shader referente ao material
void Material::materialBindLocation() {
  abcg::glUniform1i(m_diffuseTexLocation, 0);
  abcg::glUniform4fv(m_KaLocation, 1, getKa());
  abcg::glUniform4fv(m_KdLocation, 1, getKd());
  abcg::glUniform4fv(m_KdLocation, 1, getKs());
  abcg::glUniform1f(m_shininessLocation, getShininess());
  abcg::glUniform1i(m_mappingModeLocation, getMappingMode());
}

// carrega as propriedades do material .mtl
void Material::materialLoadProperties(const std::string basePath,
                                      tinyobj::ObjReader reader) {

  auto const &materials{reader.GetMaterials()};

  // verifica se há algum material. Se não houver nenhum material, utiliza os valores padrão
  if (!materials.empty()) {
    auto const &mat{materials.at(0)}; // realiza a leitura do primeiro material
    Ka = {mat.ambient[0], mat.ambient[1], mat.ambient[2], 1};
    Kd = {mat.diffuse[0], mat.diffuse[1], mat.diffuse[2], 1};
    Ks = {mat.specular[0], mat.specular[1], mat.specular[2], 1};
    shininess = mat.shininess;

    // verifica se existe uma textura difusa
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

// carrega a textura difusa
void Material::materialLoadDiffuseTexture(std::string_view path) {
  
  // verifica se o caminho do arquivo existe
  if (!std::filesystem::exists(path))
    return;

  // exclui texturas carregadas anteriormente
  abcg::glDeleteTextures(1, &m_diffuseTexture);

  // carrega a nova textura
  m_diffuseTexture = abcg::loadOpenGLTexture({.path = path});
}

// realiza o bind e o mapeamento da textura
void Material::materialBindTexture() {
  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_2D, m_diffuseTexture);

  // configurar os filtros de textura
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // define os parametros de empacotamento da textura texture wrapping parameters
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  abcg::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


// realiza o carregamento do material através do arquivo .obj
void Material::materialLoadModelFromFile(std::string_view path,
                                         bool standardize) {

  tinyobj::ObjReader reader;
  tinyobj::ObjReaderConfig readerConfig;

  auto const basePath{std::filesystem::path{path}.parent_path().string() + "/"};
  readerConfig.mtl_search_path = basePath; // path para o arquivo do material

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