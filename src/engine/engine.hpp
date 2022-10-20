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
#include <string>
#include "core/singletons/configuration.hpp"
#include "core/singletons/scene_storage.hpp"
#include "core/system.hpp"
#include "core/singletons/os/os.hpp"
#include "core/singletons/os/profiler.hpp"
#include <memory>

#include <stdint.h>
#include "core/scene/scene.hpp"
#include "core/singletons/os/os.hpp"
#include "core/singletons/os/file_access.hpp"
#include "core/singletons/os/profiler.hpp"
#include <vector>
#include <stack>
#include <string>
#include <memory>

#include "engine_api.hpp"

namespace Omnia
{
	/* The main class responsible for engine runtime 
	   that supports the application built on it. */
	class OMNIA_ENGINE_API Engine
	{
	public:
		Engine(
			int argc,
			char* argv[]);

		void run();
	private:
		enum class State
		{
			INITIALIZING,
			RUNNING_APPLICATION_WINDOWED,
			RUNNING_APPLICATION_FULLSCREEN,
			RUNNING_APPLICATION_FULLSCREEN_DESKTOP,
			RESTARTING,
			FINALIZING
		};

		State state;
		std::unordered_map<std::string, std::shared_ptr<System>> updateSystems;
		std::unordered_map<std::string, std::shared_ptr<System>> outputSystems;

		int argc = 0;
		char** argv = nullptr;

		bool isRunning();
		/* Returns "true" if initialization is successful */
		void initialize();
		void queryInput();
		/* Runs on a dedicated update thread. */
		void runUpdateLoop(std::shared_ptr<HiResTimer> updateProcessTimer);
		/* Runs on a dedicated output thread. */
		void runOutputLoop(std::shared_ptr<HiResTimer> outputProcessTimer);
		/* For the current single-threaded engine 
		   loop implementation*/
		void sleepThisThreadForRemainingTime(uint32_t targetFPS, std::shared_ptr<HiResTimer> runTimer);
		void finalize();

		template<class T>
		std::shared_ptr<T> getSystem()
		{
			std::shared_ptr<T> system;

			if (this->updateSystems.count(T::TYPE_STRING))
				system = std::dynamic_pointer_cast<T>(this->updateSystems.at(T::TYPE_STRING));
			else if (this->outputSystems.count(T::TYPE_STRING))
				system = std::dynamic_pointer_cast<T>(this->outputSystems.at(T::TYPE_STRING));

			return system;
		}
	};
}

