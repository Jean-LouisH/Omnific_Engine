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
#include <vector>
#include "core/utilities/constants.hpp"
#include <stdint.h>
#include "core/component.hpp"

namespace Omnia
{
	class OMNIA_ENGINE_API PropertyAnimation : public Component
	{
	public:
		float value = 0.0;
		std::vector<float> keyFrames;
		float duration = 0.0;
		float playbackSpeedPercentage = 1.0;
		float delay = 0.0;
		float progress = 0.0;
		float maximumValue = 0.0;
		float minimumValue = 0.0;
		uint8_t repeats = 0;
		uint8_t repeatCount = 0;
		bool isPlaying = false;

		PropertyAnimation()
		{
			this->type = TYPE_STRING;
		};
		static constexpr const char* TYPE_STRING = "PropertyAnimation";

		virtual Registerable* instance() override
		{
			PropertyAnimation* clone = new PropertyAnimation(*this);
			clone->id = UIDGenerator::getNewUID();
			return clone;
		}
		virtual void deserialize(YAML::Node yamlNode);
	private:
	};
}