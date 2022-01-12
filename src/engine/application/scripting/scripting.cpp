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

#include "scripting.hpp"
#include "scripting_apis/scripting_apis.hpp"

Esi::Scripting::Scripting()
{
	this->vm = std::unique_ptr<VirtualMachine>(new VirtualMachine());
	this->native = std::unique_ptr<Native>(new Native());
}

void Esi::Scripting::loadModules(Scene scene)
{
	this->vm->loadModules(scene);
	this->native->loadModules();
}

void Esi::Scripting::executeOnStartMethods(Scene& scene)
{
	this->vm->executeOnStartMethods(scene.generateCallBatches(CallType::START));
}

void Esi::Scripting::executeOnInputMethods(Scene& scene)
{
	this->vm->executeOnInputMethods(scene.generateCallBatches(CallType::UPDATE));
}

void Esi::Scripting::executeOnFrameMethods(Scene& scene)
{
	this->vm->executeOnFrameMethods(scene.generateCallBatches(CallType::UPDATE));
	this->native->executeOnFrameMethods();
}

void Esi::Scripting::executeOnComputeMethods(Scene& scene)
{
	this->vm->executeOnComputeMethods(scene.generateCallBatches(CallType::UPDATE));
}

void Esi::Scripting::executeOnOutputMethods(Scene& scene)
{
	this->vm->executeOnOutputMethods(scene.generateCallBatches(CallType::UPDATE));
}

void Esi::Scripting::executeOnFinishMethods(Scene& scene)
{
	this->vm->executeOnFinishMethods(scene.generateCallBatches(CallType::FINISH));
}

void Esi::Scripting::setSceneStorage(SceneStorage* sceneStorage)
{
	ScriptingAPIs::setSceneStorage(sceneStorage);
}