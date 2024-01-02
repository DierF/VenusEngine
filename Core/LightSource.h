#pragma once

#include "Math/Vector3.h"
#include "Render/ShaderProgram.h"

namespace VenusEngine
{
	enum LightType
	{
		DIRECTIONAL = 0,
		POINT = 1,
		SPOT = 2
	};

	class LightSource
	{
	public:
		LightSource(Vec3 const& diffuseIntensity, Vec3 const& specularIntensity)
			: m_diffuseIntensity(diffuseIntensity), m_specularIntensity(specularIntensity)
		{
		}

		virtual ~LightSource()
		{
		}

		virtual void setUniforms(ShaderProgram& program, int lightNum)
		{
			std::string name("uLights[" + std::to_string(lightNum) + "].");
			program.setUniformVec3(name + "diffuseIntensity" , m_diffuseIntensity);
			program.setUniformVec3(name + "specularIntensity", m_specularIntensity);
		}

	private:
		Vec3 m_diffuseIntensity;
		Vec3 m_specularIntensity;
	};

	class DirectionalLightSource : public LightSource
	{
	public:
		DirectionalLightSource(Vec3 const& diffuseIntensity, Vec3 const& specularIntensity, Vec3 const& direction)
			: LightSource(diffuseIntensity, specularIntensity), m_direction(direction)
		{
		}

		virtual ~DirectionalLightSource()
		{
		}

		virtual void setUniforms(ShaderProgram& program, int lightNum)
		{
			LightSource::setUniforms(program, lightNum);
			std::string name("uLights[" + std::to_string(lightNum) + "].");
			program.setUniformInt( name + "type"     , LightType::DIRECTIONAL);
			program.setUniformVec3(name + "direction", m_direction);
		}

	private:
		Vec3 m_direction;
	};

	class LocationLightSource : public LightSource
	{
	public:
		LocationLightSource(Vec3 const& diffuseIntensity, Vec3 const& specularIntensity, Vec3 const& position, Vec3 const& attenuationCoefficients)
			: LightSource(diffuseIntensity, specularIntensity), m_position(position), m_attenuationCoefficients(attenuationCoefficients)
		{
		}

		virtual ~LocationLightSource()
		{
		}

		virtual void setUniforms(ShaderProgram& program, int lightNum)
		{
			LightSource::setUniforms(program, lightNum);
			std::string name("uLights[" + std::to_string(lightNum) + "].");
			program.setUniformVec3(name + "position"               , m_position);
			program.setUniformVec3(name + "attenuationCoefficients", m_attenuationCoefficients);
		}

	private:
		Vec3 m_position;
		Vec3 m_attenuationCoefficients;
	};

	class PointLightSource : public LocationLightSource
	{
	public:
		PointLightSource(Vec3 const& diffuseIntensity, Vec3 const& specularIntensity, Vec3 const& position, Vec3 const& attenuationCoefficients)
			: LocationLightSource(diffuseIntensity, specularIntensity, position, attenuationCoefficients)
		{
		}

		virtual ~PointLightSource()
		{
		}

		virtual void setUniforms(ShaderProgram& program, int lightNum)
		{
			LocationLightSource::setUniforms(program, lightNum);
			std::string name("uLights[" + std::to_string(lightNum) + "].");
			program.setUniformInt(name + "type", LightType::POINT);
		}
	};

	class SpotLightSource : public LocationLightSource
	{
	public:
		SpotLightSource(Vec3 const& diffuseIntensity, Vec3 const& specularIntensity, Vec3 const& position, Vec3 const& attenuationCoefficients, Vec3 const& direction, float cutoffCosAngle, float falloff)
			: LocationLightSource(diffuseIntensity, specularIntensity, position, attenuationCoefficients), m_direction(direction), m_cutoffCosAngle(cutoffCosAngle), m_falloff(falloff)
		{
		}

		virtual ~SpotLightSource()
		{
		}

		virtual void setUniforms(ShaderProgram& program, int lightNum)
		{
			LocationLightSource::setUniforms(program, lightNum);
			std::string name("uLights[" + std::to_string(lightNum) + "].");
			program.setUniformInt(  name + "type"          , LightType::SPOT);
			program.setUniformVec3( name + "direction"     , m_direction);
			program.setUniformFloat(name + "cutoffCosAngle", m_cutoffCosAngle);
			program.setUniformFloat(name + "falloff"       , m_falloff);
		}

	private:
		Vec3 m_direction;
		float m_cutoffCosAngle;
		float m_falloff;
	};
}