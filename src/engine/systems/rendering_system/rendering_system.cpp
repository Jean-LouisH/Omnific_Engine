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

#include "rendering_system.hpp"
#include "rendering_sdl/rendering_sdl.hpp"
#include <SDL_image.h>

Lilliputian::RenderingSystem::RenderingSystem(Window& window)
{
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	this->sdlWindow = window.getSDLWindow();
	this->sdlRenderer = SDL_CreateRenderer(
		sdlWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Lilliputian::RenderingSystem::clearBuffers()
{
	SDL_SetRenderDrawColor(this->sdlRenderer, 0, 0, 0, 255);
	SDL_RenderClear(this->sdlRenderer);
}

void Lilliputian::RenderingSystem::render()
{
	for (int i = 0; i < this->sdlRenderables.size(); i++)
	{	
		SDL::Rendering2D::SDLRenderable sdlRenderable = this->sdlRenderables.at(i);
		SDL_RenderCopyEx(
			this->sdlRenderer,
			sdlRenderable.texture,
			&sdlRenderable.textureRect,
			&sdlRenderable.renderingRect,
			sdlRenderable.rotation,
			NULL,
			sdlRenderable.flip
		);
	}
	SDL_RenderPresent(this->sdlRenderer);
}

void Lilliputian::RenderingSystem::process(SceneForest& scene)
{
	Vector<SDL::Rendering2D::Sprite2D> outputsprite2Ds;
	SDL::Rendering2D::Camera2D outputCamera2D;
	outputCamera2D.isStreaming = false;

	Vector<Stack<SceneTree2D>> sceneTree2DStacks = scene.getSceneTree2DStacks();
	int sceneTree2DCount = sceneTree2DStacks.size();

	for (int i = 0; i < sceneTree2DCount; i++)
	{
		SceneTree2D sceneTree2D = sceneTree2DStacks.at(i).top();
		Vector<ComponentVariant> componentVariants = sceneTree2D.getComponentVariants();

		for (int j = 0; j < componentVariants.size(); j++)
		{
			SDL::Rendering2D::Sprite2D outputSprite2D;
			ComponentVariant componentVariant = componentVariants.at(j);
			Camera2D* camera2D;
			Sprite* sprite;
			AnimatedSprite* animatedSprite;
			UITextLabel* uiTextLabel;
			SDL::Rendering2D::Texture outputTexture;
			Image image;

			if (this->isRenderableType(componentVariant.type))
			{
				Transform2D transform2D = sceneTree2D.getEntityTransform(componentVariant.entityID);
				Vector2 position_px = transform2D.position_px;
				Vector2 scale = transform2D.scale;
				SDL_Texture* sdlTexture;

				outputSprite2D.transform.position_px.x = position_px.x;
				outputSprite2D.transform.position_px.y = position_px.y;
				outputSprite2D.transform.scale.x = scale.x;
				outputSprite2D.transform.scale.y = scale.y;
				outputSprite2D.transform.rotation_rad = transform2D.rotation_rad;

				switch (componentVariant.type)
				{
					case ComponentVariant::Type::COMPONENT_TYPE_SPRITE:
						sprite = componentVariant.sprite;
						image = sprite->getImage();
						outputTexture.pixels.width = image.getWidth();
						outputTexture.pixels.height = image.getHeight();

						if (this->sdlTextureCache.count(image.getSDLSurface()) == 0)
						{
							outputTexture.data = SDL_CreateTextureFromSurface(this->sdlRenderer, image.getSDLSurface());
							this->sdlTextureCache.emplace(image.getSDLSurface(), outputTexture.data);
						}
						else
						{
							outputTexture.data = this->sdlTextureCache.at(image.getSDLSurface());
						}

						outputSprite2D.textureFrames.push_back(outputTexture);
						outputSprite2D.alpha = sprite->getAlpha();

						outputsprite2Ds.push_back(outputSprite2D);
						break;
					case ComponentVariant::Type::COMPONENT_TYPE_ANIMATED_SPRITE:
						animatedSprite = componentVariant.animatedSprite;
						image = animatedSprite->getCurrentFrame();
						outputTexture.pixels.width = image.getWidth();
						outputTexture.pixels.height = image.getHeight();

						if (this->sdlTextureCache.count(image.getSDLSurface()) == 0)
						{
							outputTexture.data = SDL_CreateTextureFromSurface(this->sdlRenderer, image.getSDLSurface());
							this->sdlTextureCache.emplace(image.getSDLSurface(), outputTexture.data);
						}
						else
						{
							outputTexture.data = this->sdlTextureCache.at(image.getSDLSurface());
						}

						outputSprite2D.textureFrames.push_back(outputTexture);
						outputSprite2D.alpha = animatedSprite->getAlpha();

						outputsprite2Ds.push_back(outputSprite2D);
						break;
					case ComponentVariant::Type::COMPONENT_TYPE_RECTANGULAR_MESH_2D: break;
					case ComponentVariant::Type::COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_BUTTON: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_RECTANGLE: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_GRAPH_EDIT: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_GRAPH_NODE: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_SCROLLBAR: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_SEPARATOR: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_SLIDER: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_HOVER_CARD: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_ITEM_LIST: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_PANEL: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_PROGRESS_BAR: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_SPIN_BOX: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_TAB: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_TEXT_EDIT: break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_TEXT_LABEL: 
						uiTextLabel = componentVariant.uiTextLabel;
						image = uiTextLabel->getImage();
						outputTexture.pixels.width = image.getWidth();
						outputTexture.pixels.height = image.getHeight();

						if (this->sdlTextureCache.count(image.getSDLSurface()) == 0)
						{
							outputTexture.data = SDL_CreateTextureFromSurface(this->sdlRenderer, image.getSDLSurface());
							this->sdlTextureCache.emplace(image.getSDLSurface(), outputTexture.data);
						}
						else
						{
							outputTexture.data = this->sdlTextureCache.at(image.getSDLSurface());
						}

						outputSprite2D.textureFrames.push_back(outputTexture);
						outputSprite2D.alpha = uiTextLabel->getAlpha();

						outputsprite2Ds.push_back(outputSprite2D);
						break;
					case ComponentVariant::Type::COMPONENT_TYPE_UI_TREE:; break;
				}
			}
			else if (componentVariant.type == ComponentVariant::Type::COMPONENT_TYPE_CAMERA_2D &&
				 j == sceneTree2D.getCurrentCameraIndex())
			{
				Transform2D transform2D = sceneTree2D.getEntityTransform(componentVariant.entityID);
				Vector2 position_px = transform2D.position_px;
				Vector2 scale = transform2D.scale;
				camera2D = componentVariant.camera2D;
				Rectangle viewport_px = camera2D->getViewportDimensions();

				outputCamera2D.isStreaming = true;
				outputCamera2D.transform.position_px.x = position_px.x;
				outputCamera2D.transform.position_px.y = position_px.y;
				outputCamera2D.transform.scale.x = scale.x;
				outputCamera2D.transform.scale.y = scale.y;
				outputCamera2D.transform.rotation_rad = transform2D.rotation_rad;

				outputCamera2D.viewport_px.width = viewport_px.width;
				outputCamera2D.viewport_px.height = viewport_px.height;
			}
		}
	}

	if (outputCamera2D.isStreaming)
	{
		SDL::Rendering2D::buildRenderablesFromSprites(
			&this->sdlRenderables,
			outputsprite2Ds,
			outputCamera2D,
			this->sdlWindow);
	}

	this->clearBuffers();
	this->render();
}

bool Lilliputian::RenderingSystem::isRenderableType(ComponentVariant::Type componentVariantType)
{
	return componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_SPRITE ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_ANIMATED_SPRITE ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_RECTANGULAR_MESH_2D ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_REGULAR_POLYGONAL_MESH_2D ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_BUTTON ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_RECTANGLE ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_GRAPH_EDIT ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_GRAPH_NODE ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_SCROLLBAR ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_SEPARATOR ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_SLIDER ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_HOVER_CARD ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_ITEM_LIST ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_PANEL ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_PROGRESS_BAR ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_SPIN_BOX ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_TAB ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_TEXT_EDIT ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_TEXT_LABEL ||
		componentVariantType == ComponentVariant::Type::COMPONENT_TYPE_UI_TREE;
}