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

#include "stb_image.h"
#include "stb_image_write.h"

#include "foundations/aliases.hpp"
#include <SDL_surface.h>
#include <SDL_render.h>
#include <stdint.h>
#include "foundations/resources/resource.hpp"
#include "font.hpp"
#include <string>
#include "colour.hpp"
#include "font.hpp"
#include <string>
#include <memory>
#include <glm/glm.hpp>

namespace Omnific
{
	class OMNIFIC_ENGINE_API Image : public Resource
	{
	public:
		enum class Alignment
		{
			TOP_LEFT,
			TOP_RIGHT,
			CENTRE
		};

		static constexpr const char* TYPE_STRING = "Image";
		Image() 
		{ 
			this->type = TYPE_STRING;
		};
		Image(std::string text, std::shared_ptr<Font> font, std::shared_ptr<Colour> colour, uint16_t wrap_length);
		Image(std::shared_ptr<Colour> colour);
		Image(std::shared_ptr<Colour> colour, int width, int height);
		Image(uint8_t* data, int width, int height, int colour_channels);
		Image(std::string filepath);

		virtual Registerable* instance() override
		{
			Image* clone = new Image(*this);
			clone->id = UIDGenerator::get_new_uid();
			return clone;
		}
		uint8_t* get_data();
		uint32_t get_width();
		uint32_t get_height();
		uint32_t get_depth();
		uint32_t get_pitch();
		glm::vec2 get_dimensions();
		uint8_t get_bytes_per_pixel();
		Alignment get_alignment();

		static void normal_blend(
			uint8_t* lower_image_data,
			glm::vec2 lower_position,
			glm::vec2 lower_dimensions,
			uint8_t* upper_image_data,
			glm::vec2 upper_position,
			glm::vec2 upper_dimensions
		);
	private:
		std::shared_ptr<uint8_t> data = {nullptr, stbi_image_free };
		int width = 0;
		int height = 0;
		int colour_channels = 0;
		Alignment alignment;

		void colour_pixel(uint32_t fill_colour, int x, int y);
		void set_to_default();
		void set_to_colour(std::shared_ptr<Colour> colour, int width, int height);
		void set_to_parameters(int colour_channels, int width, int height, uint8_t* data);
	};
}