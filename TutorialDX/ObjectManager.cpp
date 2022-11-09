#include "ObjectManager.h"

bool ObjectManager::RegisterObject(GameObjectParams& params)
{
	if (params.objectType == "RenderableGameObject")
	{
		RenderableGameObject object;
		Model* model = FindModelByPath(params.fileName);
		if (model != nullptr)
		{
			//if (object.Initialize(*model))
			{
			//	object.SetPosition(params.position);
			//	mRegisteredObjects.push_back(object);
			}
		}
		else
		{
			if (object.Initialize(params.fileName, params.device.Get(), params.deviceContext.Get(), *params.vertexshader))
			{
				object.SetPosition(params.position);
				mRegisteredObjects.push_back(object);
				//mSavedModels[params.fileName] = *model;
				return true;
			}
		}
	}

	return false;
}

void ObjectManager::UpdateRegisteredObjects()
{
	for (RenderableGameObject object : mRegisteredObjects)
	{
		object.Draw(mCamera->GetViewMatrix() * mCamera->GetProjectionMatrix());
	}
}

Model* ObjectManager::FindModelByPath(const std::string& filePath)
{
	std::map<std::string, Model>::iterator modelIterator = mSavedModels.find(filePath);
	if (modelIterator != mSavedModels.end())
	{
		return &modelIterator->second;
	}

	return nullptr;
}
