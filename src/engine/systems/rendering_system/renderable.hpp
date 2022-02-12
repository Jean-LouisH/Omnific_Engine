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

#include <vector>
#include <application/scene/components/transform.hpp>
#include <application/scene/components/camera.hpp>
#include <application/scene/assets/image.hpp>
#include <application/scene/assets/material.hpp>
#include <application/scene/assets/mesh.hpp>
#include <application/scene/assets/shader.hpp>
#include <glm/glm.hpp>

#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "texture.hpp"
#include "shader_program.hpp"
#include <memory>

namespace Omnific
{
	/* Caches memory locations for values 
	   relevant to rendering a single Entity. */
	class Renderable
	{
	public:
		EntityID id = 0;
		std::shared_ptr<Transform> entityTransform;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Material> material;
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::vector<std::shared_ptr<ShaderProgram>> shaderPrograms;
	private:
	};
}

