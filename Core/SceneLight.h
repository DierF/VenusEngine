#pragma once

#include <unordered_map>
#include <memory>

#include "Core/LightSource.h"

namespace VenusEngine
{
	class SceneLight
	{
	public:
		SceneLight()
		{
		}

		~SceneLight()
		{
		}

		SceneLight(SceneLight const&) = delete;

		void operator=(SceneLight const&) = delete;

		bool add(std::string const& name, std::shared_ptr<LightSource> lightSource)
		{
			if (size() >= 8)
			{
				return false;
			}
			m_lightSources[name] = lightSource;
			return true;
		}

		bool remove(std::string const& name)
		{
			m_lightSources.erase(name);
		}

		void clear()
		{
			m_lightSources.clear();
		}

		void draw(ShaderProgram& shaderProgram)
		{
			shaderProgram.enable();

			shaderProgram.setUniformInt("uNumLights", size());
			shaderProgram.setUniformVec3("uAmbientIntensity", Vec3(0.9f, 0.9f, 0.9f));
			shaderProgram.setUniformVec3("uAmbientReflection", Vec3(0.5f, 0.5f, 0.5f));
			shaderProgram.setUniformVec3("uDiffuseReflection", Vec3(0.8f, 0.8f, 0.8f));
			shaderProgram.setUniformVec3("uSpecularReflection", Vec3(1.0f, 1.0f, 1.0f));
			shaderProgram.setUniformFloat("uSpecularPower", 32.0f);
			shaderProgram.setUniformVec3("uEmissiveIntensity", Vec3(0.0f, 0.0f, 0.0f));
			
			int index = 0;
			for (auto& pair : m_lightSources)
			{
				pair.second->setUniforms(shaderProgram, index);
				++index;
			}

			shaderProgram.disable();
		}

		bool hasLightSource(std::string const& name)
		{
			return m_lightSources.count(name);
		}

		std::shared_ptr<LightSource> getLightSource(std::string const& name)
		{
			return m_lightSources[name];
		}

		int size() const
		{
			return static_cast<int>(m_lightSources.size());
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<LightSource>> m_lightSources;
	};
}