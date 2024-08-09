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
			if (size() >= 16)
			{
				return false;
			}
			m_lightSources[name] = lightSource;
			return true;
		}

		void remove(std::string const& name)
		{
			m_lightSources.erase(name);
			if (name == m_activeLightSourceName)
			{
				m_activeLightSourceName.clear();
			}
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

		void setActiveLightSource(std::string const& activeLightSourceName)
		{
			m_activeLightSourceName = activeLightSourceName;
		}

		bool hasActiveLightSource() const
		{
			return !m_activeLightSourceName.empty();
		}

		std::shared_ptr<LightSource> getActiveLightSource()
		{
			return m_lightSources[m_activeLightSourceName];
		}

		void changeActiveLightSourceName(std::string const& newActiveLightSourceName)
		{
			if (!hasActiveLightSource())
			{
				return;
			}
			auto activeLightSource = getActiveLightSource();
			remove(m_activeLightSourceName);
			add(newActiveLightSourceName, activeLightSource);
			setActiveLightSource(newActiveLightSourceName);
		}

		std::string const& activeLightSourceName() const
		{
			return m_activeLightSourceName;
		}

		std::vector<std::string> allLightSourceNames() const
		{
			std::vector<std::string> names;
			names.reserve(m_lightSources.size());
			for (auto iter = m_lightSources.begin(); iter != m_lightSources.end(); ++iter)
			{
				names.push_back(iter->first);
			}
			return names;
		}

		int size() const
		{
			return static_cast<int>(m_lightSources.size());
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<LightSource>> m_lightSources;
		std::string                                                   m_activeLightSourceName;
	};
}