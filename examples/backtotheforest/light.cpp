#include "light.hpp"


void Light::lightBind(){
  abcg::glUniform4fv(m_lightDirLocation, 1, getLightDirection());
  abcg::glUniform4fv(m_IaLocation, 1, getIa());
  abcg::glUniform4fv(m_IdLocation, 1, getId());
  abcg::glUniform4fv(m_IdLocation, 1, getIs());
  abcg::glUniform1f(m_lightDiameterLocation, getLightDiameter());
}
