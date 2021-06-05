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

#include "game.hpp"
#include "boot_loader.hpp"
#include "utilities/constants.hpp"
#include <iostream>

Lilliputian::Game::Game()
{
	this->scripting = new Scripting();
	this->sceneStorage = new SceneStorage();
	this->commandLine = new CommandLine(
		this->sceneSerializer,
		this->sceneStorage);

	ScriptingAPIs::initialize();
}

void Lilliputian::Game::initialize()
{
	BootLoader bootLoader;
	Scene entryScene;
	String dataDirectory = "data/";
#ifdef _DEBUG
	dataDirectory = DEBUG_DATA_FILEPATH;
#endif
	String bootFilename = "boot.yml";
	String bootFilepath = dataDirectory + bootFilename;

	if (OS::getFileAccess().exists(bootFilepath))
	{
		this->configuration = bootLoader.loadFromFile(bootFilepath);
#ifdef _DEBUG
		String debugDataFilepath = DEBUG_DATA_FILEPATH;
		String debugEditorDataFilepath = DEBUG_EDITOR_DATA_FILEPATH;
		if (debugDataFilepath == debugEditorDataFilepath)
			this->configuration->entrySceneFilepath = "assets/scenes/debug.yml";
#endif
		OS::getFileAccess().setDataDirectory(dataDirectory);
		this->sceneSerializer = new SceneSerializer(dataDirectory);
		ScriptingAPIs::getSceneAPI().setSceneSerializer(this->sceneSerializer);

		if (this->sceneSerializer->doesSceneExist(this->configuration->entrySceneFilepath))
		{
			entryScene = this->sceneSerializer->loadFromFile(this->configuration->entrySceneFilepath);
			this->sceneStorage->addScene(this->configuration->entrySceneFilepath, entryScene);
			this->scripting->setSceneStorage(this->sceneStorage);
		}
	}
	else
	{
		this->configuration = new BootConfiguration();
		this->configuration->isLoaded = false;
	}

#ifdef DEBUG_CONSOLE_ENABLED
	std::cout << "\tLilliputian Engine Debug Console Enabled";
	std::cout << "\n\nPress '`' in-game to write to command line via console.";
	std::cout << "\n\nIf user priviledges are enabled through the command line API, the ";
	std::cout << "\ncommand line will be accessed in the game window instead.";
	std::cout << "\n\nTo see the list of commands, enter 'commands'.";
	std::cout << "\n\n";
#endif
}

void Lilliputian::Game::executeOnStartMethods()
{
	if (this->sceneStorage->hasActiveSceneChanged())
		this->scripting->loadCurrentSceneScriptModules(this->sceneStorage->getActiveScene());

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnStartMethods(this->getActiveScene());
}

void Lilliputian::Game::executeOnInputMethods()
{
#ifdef DEBUG_CONSOLE_ENABLED
	if (OS::getHid().hasRequestedCommandLine())
	{
		String command;

		OS::getWindow().hide();
		std::cout << ">";
		std::getline(std::cin, command);
		this->commandLine->execute(command);
		OS::getWindow().show();
	}
#endif

	if (!this->sceneStorage->isEmpty() && OS::getHid().getHasDetectedInputChanges())
		this->scripting->executeOnInputMethods(this->getActiveScene());
}

void Lilliputian::Game::executeOnFrameMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFrameMethods(this->getActiveScene());
}

void Lilliputian::Game::executeOnComputeMethods()
{
	uint32_t msPerComputeUpdate = this->configuration->msPerComputeUpdate;

	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnComputeMethods(this->getActiveScene());
}

void Lilliputian::Game::executeOnLateMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnLateMethods(this->getActiveScene());
}

void Lilliputian::Game::executeOnFinalMethods()
{
	if (!this->sceneStorage->isEmpty())
		this->scripting->executeOnFinalMethods(this->getActiveScene());

	Vector<SceneTree2D>& sceneTree2Ds = this->sceneStorage->getActiveScene().getSceneTree2Ds();

	for (int i = 0; i < sceneTree2Ds.size(); i++)
		sceneTree2Ds.at(i).getEventBus().clear();
}

void Lilliputian::Game::deinitialize()
{
	delete this->configuration;
	delete this->sceneSerializer;
	delete this->scripting;
}

Lilliputian::Scene& Lilliputian::Game::getActiveScene()
{
	return this->sceneStorage->getActiveScene();
}

Lilliputian::BootConfiguration& Lilliputian::Game::getConfiguration()
{
	return *this->configuration;
}