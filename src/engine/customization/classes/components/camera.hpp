// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "core/utilities/aliases.hpp"
#include "core/utilities/rectangle.hpp"
#include "core/utilities/aabb_2d.hpp"
#include <glm/glm.hpp>
#include "core/utilities/constants.hpp"
#include "core/component.hpp"
#include "customization/classes/assets/shader.hpp"


namespace Omnia
{
	class OMNIA_ENGINE_API Camera : public Component
	{
	public:
		static constexpr const char* TYPE_STRING = "Camera";
		Camera()
		{
			this->type = TYPE_STRING;
			this->setViewportHeight(480);
		};

		virtual Registerable* instance() override
		{
			Camera* clone = new Camera(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		virtual void deserialize(YAML::Node yamlNode);
		void addShader(std::shared_ptr<Shader> shader);
		void resetAspect();
		void toggleWireframeMode();
		Rectangle getViewportDimensions();
		bool getIsStreaming();
		bool getIsWireframeMode();
		void setViewportWidth(uint32_t width);
		void setViewportHeight(uint32_t height);
		void setLimits(float left, float bottom, float top, float right);
		void setKeepAspect(bool value);
		void setIsStreaming(bool value);
		void setWireframeMode(bool value);
		glm::mat4 getViewToProjectionMatrix();
		std::vector<std::shared_ptr<Shader>> getShaders();
	private:
		const float defaultAspect = 1920.0 / 1080.0;
		float aspect = defaultAspect;
		float fieldOfView = 45.0;
		float nearPlane = 0.1;
		float farPlane = 1000.0;
		AABB2D limits;
		Rectangle viewport;
		bool keepAspect = true;
		bool isStreaming = true;
		bool isWireFrame = false;
		std::vector<std::shared_ptr<Shader>> shaders;
	};
}