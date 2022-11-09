#pragma once
#include "graphics/RenderableGameObject.h"
#include <vector>
#include <map>
#include "Graphics/Camera3D.h"
using namespace std;


struct GameObjectParams
{
	std::string objectType;
	std::string fileName;
	Microsoft::WRL::ComPtr<ID3D11Device> device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext = nullptr;
	ConstantBuffer<CB_VS_vertexshader>* vertexshader = nullptr;
	XMFLOAT3 position;


	 GameObjectParams(string type, string file,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>deviceContext,
		ConstantBuffer<CB_VS_vertexshader>& shader,
		XMFLOAT3 pos)
		{
			objectType = type;
			fileName = file;
			this->device = device;
			this->deviceContext = deviceContext;
			this->vertexshader = &shader;
			position = pos;
		}
};

struct RegisteredGameObject
{
	//std::string fileName;
	//Model model;

public:
	//Model* FindModelByPath(std::string filePath);
};

class ObjectManager
{
	std::vector<RenderableGameObject> mRegisteredObjects;
	std::map<std::string, Model> mSavedModels;

public:

	Camera3D* mCamera;
	//it should give move all update's logic to this manager
	bool RegisterObject(GameObjectParams& params);
	bool UnregisterObject();

	Model* FindModelByPath(const std::string& filePath);

	// not sure, probably it should do on enitialization
	void UpdateRegisteredObjects();

	//here I would like to show all registered objects and their settings. 
	//probably I should add more info about object to itself to tell about them 
	//  and some way to hide object on the scene (forst of all hide and show) -
	// need to be sure that object are not processing if it's hidden
	void UpdateGUI();
};

