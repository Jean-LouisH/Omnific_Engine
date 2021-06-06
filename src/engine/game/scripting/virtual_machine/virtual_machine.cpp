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

#include "virtual_machine.hpp"
#include "embedded_module.hpp"
#include <iostream>
#include <utilities/collections/set.hpp>

Lilliputian::VirtualMachine::VirtualMachine()
{

}

Lilliputian::VirtualMachine::~VirtualMachine()
{

}

void Lilliputian::VirtualMachine::loadModules(Scene scene)
{
	this->modules.clear();

	pybind11::module_ sys = pybind11::module_::import("sys");
	pybind11::object path = sys.attr("path");
	Set<String> addedPaths;

	Vector<SceneTree2D> sceneTrees = scene.getSceneTree2Ds();
	Set<String> scripts;

	for (int i = 0; i < sceneTrees.size(); i++)
	{
		Map<EntityID, Entity2D> entities = sceneTrees.at(i).getEntity2Ds();
		
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			Vector<String> entityScripts = entities.at(it->first).scripts;

			for (int j = 0; j < entityScripts.size(); j++)
				scripts.emplace(entityScripts.at(j));
		}
	}

	for (auto it = scripts.begin(); it != scripts.end(); it++)
	{
		try
		{
			String scriptFilepath = *it;

			if (addedPaths.count(scriptFilepath) == 0)
			{
				String newPath = OS::getFileAccess().getExecutableDirectory() + 
					"//data//" + OS::getFileAccess().getPathBeforeFile(scriptFilepath);
#ifdef _DEBUG
				newPath = OS::getFileAccess().getExecutableDirectory();
				newPath = newPath.substr(0, newPath.find("out\\build\\x64-Debug\\src\\main"));
				String dataFolder = "data//editor//";
#if (DEBUG_DEMO_MODE)
				dataFolder = "data//demos//";
#endif
				newPath += dataFolder + OS::getFileAccess().getPathBeforeFile(scriptFilepath);
#endif

				pybind11::str newPathObj = pybind11::str(newPath);
				newPathObj = newPathObj.attr("replace")("//", "/");
				newPathObj = newPathObj.attr("replace")("/", "\\");
				newPath = newPathObj.cast<std::string>();

				path.attr("insert")(0, newPath);
				addedPaths.emplace(newPath);
			}

			Module newModule;
			Vector<String> methodNames = { "on_start", "on_input", "on_frame", "on_compute", "on_late", "on_final" };
			String moduleName = OS::getFileAccess().getFileNameWithoutExtension(scriptFilepath);
			pybind11::module_ newPybind11Module = pybind11::module_::import(moduleName.c_str());

			newModule.setData(newPybind11Module);

			for (int i = 0; i < methodNames.size(); i++)
			{
				try
				{
					String methodName = methodNames.at(i);
					pybind11::object test = newPybind11Module.attr(methodName.c_str());
					newModule.setCallable(methodName);
				}
				catch (const pybind11::error_already_set& e) //using the exception catch to detect if method is not callable
				{

				}
			}

			this->modules.emplace(moduleName, newModule);
		}
		catch (const pybind11::error_already_set& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
}

void Lilliputian::VirtualMachine::executeOnStartMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_start");
}

void Lilliputian::VirtualMachine::executeOnInputMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_input");
}

void Lilliputian::VirtualMachine::executeOnFrameMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_frame");
}

void Lilliputian::VirtualMachine::executeOnComputeMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_compute");
}

void Lilliputian::VirtualMachine::executeOnLateMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_late");
}

void Lilliputian::VirtualMachine::executeOnFinalMethods(Vector<ScriptCallBatch> scriptCallBatches)
{
	this->executeMethods(scriptCallBatches, "on_final");
}

void Lilliputian::VirtualMachine::executeMethods(Vector<ScriptCallBatch> scriptCallBatches, const char* methodName)
{
	for (int i = 0; i < scriptCallBatches.size(); i++)
	{
		ScriptCallBatch scriptCallBatch = scriptCallBatches.at(i);

		for (int j = 0; j < scriptCallBatch.scripts.size(); j++)
		{
			String scriptPath = scriptCallBatch.scripts.at(j);
			String scriptName = OS::getFileAccess().getFileNameWithoutExtension(scriptPath);

			if (this->modules.count(scriptName))
			{
				if (this->modules.at(scriptName).hasCallable(methodName))
				{
					ScriptingAPIs::bindEntity(
						scriptCallBatch.sceneTreeID,
						scriptCallBatch.entityID);

					try
					{
						this->modules.at(scriptName).call(methodName);
					}
					catch (const pybind11::error_already_set& e)
					{
						std::cout << e.what() << std::endl;
					}
				}
			}
		}
	}
}