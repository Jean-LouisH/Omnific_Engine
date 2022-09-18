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

#include <utilities/aliases.hpp>
#include <glm/glm.hpp>
#include <string>
#include <engine_api.hpp>


namespace Omnia
{
	typedef struct OMNIA_ENGINE_API Collision
	{
		EntityID colliderEntityID = 0;
		std::string colliderName;
		EntityID otherColliderEntityID = 0;
		std::string otherColliderName;
		glm::vec3 attackAngle;
		bool hasPhysicsBody = false;
		bool hasOtherPhysicsBody = false;
		double elasticityRatio = 0.0;
		double mass = 0.0;
		glm::vec3 linearVelocity;
		glm::vec3 rotation;
		double otherElasticityRatio = 0.0;
		double otherMass = 0.0;
		glm::vec3 otherLinearVelocity;
		glm::vec3 otherRotation;
	};
}