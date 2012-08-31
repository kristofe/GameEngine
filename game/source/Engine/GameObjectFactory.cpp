#include "GameObjectFactory.h"
#include "Game.h"
#include "GameObjectFactory.h"
#include "PersistentDataManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "RenderPlane.h"
#include "RendererFPSDisplay.h"
#include "Renderer.h"
#include "MetaballGrid.h"
#include "LetterManager.h"
#include "LetterChooser.h"
#include "WordProgressDisplay.h"
#include "HangedManDisplay.h"
#include "GameLogic.h"
#include "AnimatedTexture.h"
#include "Sequence.h"
#include "LetterDripAnimation.h"
#include "SequenceDelayer.h"
#include "FlipLetter.h"
#include "ScreenFader.h"
#include "LetterSprayAnimation.h"
#include "AnimationPlayer.h"
#include "HangmanStateDisplay.h"
#include "BackgroundChanger.h"
#include "BlowTiles.h"
#include "PoemDisplay.h"
#include "DisableInput.h"
#include "RenderTarget.h"
#include "MetaballManager2D.h"
#include "SprayAnimation.h"
#include "ScratchDisplay.h"
#include "RenderObjectKiller.h"
#include "DropManager.h"
#include "ClearMetaballGrid.h"
#include "Camera.h"
#include "SimpleParticleSystem.h"
#include "DebugText.h"
#include "Cube.h"

GameObject* GameObjectFactory::CreateGameObject(PersistentData& storageData)
{
	std::string objType = storageData.GetName();
	GameObject* go = NULL;
	if(objType == "RenderPlane")
	{
		go = new RenderPlane(storageData);
	}
	else if(objType == "SceneObject")
	{
		go = new SceneObject(storageData);
	}
	else if(objType == "AnimatedTexture")
	{
		go = new AnimatedTexture(storageData);
	}
	else if(objType == "Sequence")
	{
		go = new Sequence(storageData);
	}
	else if(objType == "RendererFPSDisplay")
	{
		go = new RendererFPSDisplay(storageData);
	}
	else if(objType == "MetaballGrid")
	{
		go = new MetaballGrid(storageData);
	}
	else if(objType == "LetterManager")
	{
		go = new LetterManager(storageData);
	}
	else if(objType == "LetterChooser")
	{
		go = new LetterChooser(storageData);
	}
	else if(objType == "WordProgressDisplay")
	{
		go = new WordProgressDisplay(storageData);
	}
	else if(objType == "HangedManDisplay")
	{
		go = new HangedManDisplay(storageData);
	}
	else if(objType == "GameLogic")
	{
		go = new GameLogic(storageData);
	}
	else if(objType == "LetterDripAnimation")
	{
		go = new LetterDripAnimation(storageData);
	}
	else if(objType == "SequenceDelayer")
	{
		go = new SequenceDelayer(storageData);
	}
	else if(objType == "FlipLetter")
	{
		go = new FlipLetter(storageData);
	}
	else if(objType == "ScreenFader")
	{
		go = new ScreenFader(storageData);
	}
	else if(objType == "LetterSprayAnimation")
	{
		go = new LetterSprayAnimation(storageData);
	}
	else if(objType == "AnimationPlayer")
	{
		go = new AnimationPlayer(storageData);
	}
	else if(objType == "HangmanStateDisplay")
	{
		go = new HangmanStateDisplay(storageData);
	}
	else if(objType == "BackgroundChanger")
	{
		go = new BackgroundChanger(storageData);
	}
	else if(objType == "BlowTiles")
	{
		go = new BlowTiles(storageData);
	}
	else if(objType == "PoemDisplay")
	{
		go = new PoemDisplay(storageData);
	}
	else if(objType == "DisableInput")
	{
		go = new DisableInput(storageData);
	}
	else if(objType == "RenderTarget")
	{
		go = new RenderTarget(storageData);
	}
	else if(objType == "MetaballManager2D")
	{
		go = new MetaballManager2D(storageData);
	}
	else if(objType == "SprayAnimation")
	{
		go = new SprayAnimation(storageData);
	}
	else if(objType == "ScratchDisplay")
	{
		go = new ScratchDisplay(storageData);
	}
	else if(objType == "RenderObjectKiller")
	{
		go = new RenderObjectKiller(storageData);
	}
	else if(objType == "DropManager")
	{
		go = new DropManager(storageData);
	}
	else if(objType == "ClearMetaballGrid")
	{
		go = new ClearMetaballGrid(storageData);
	}
	else if(objType == "Camera")
	{
		go = new Camera(storageData);
	}
	else if(objType == "SimpleParticleSystem")
	{
		go = new SimpleParticleSystem(storageData);
	}
	else if(objType == "DebugText")
	{
		go = new DebugText(storageData);
	}
	else if(objType == "Cube")
	{
		go = new Cube(storageData);
	}
	go->PreSetup(storageData);
	go->SetupMeshes(storageData);
	go->SetupMaterialsAndTextures(storageData);
	go->PostSetup(storageData);

	return go;
	
}