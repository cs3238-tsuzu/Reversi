#pragma once
#include<vector>
#include<memory>

class SceneManager;

class Scene{
private:

public:
	std::unique_ptr<SceneManager> Parent;

	virtual ~Scene() = default;

	virtual void Update();

	virtual void Render();

	Scene() = default;
	Scene(const Scene& sc){
		if (sc.Parent != nullptr)
			this->Parent = std::make_unique<SceneManager>(*(sc.Parent.get()));
	}
};

class SceneManager{
private:
	std::vector<std::unique_ptr<Scene>> StackScene;
	int now = 0;
public:
	SceneManager() = default;
	SceneManager(const SceneManager& sm){
		this->StackScene.reserve(sm.StackScene.size());
		for (const auto& ss : sm.StackScene){
			this->StackScene.emplace_back(std::make_unique<Scene>(*ss));
		}
	}
	SceneManager(const std::unique_ptr<Scene>& FirstScene){
		StackScene.emplace_back(std::make_unique<Scene>(*FirstScene));
		StackScene[now]->Parent = std::move(std::make_unique<SceneManager>(*this));
	}

	void NextAddScene(std::unique_ptr<Scene>& AddScene){
		if (now < StackScene.size()){
			StackScene.emplace_back(std::move(AddScene));
			now++;
			StackScene[now]->Parent = std::move(std::make_unique<SceneManager>(*this));
		}
	}

	void NextExistingScene(){
		if (now<StackScene.size())
			now++;
	}

	void BackScene(bool Alive){
		if (!Alive)
			StackScene.pop_back();
		if (now > 0)
			now--;
	}

	~SceneManager(){}

	void Update(){

		StackScene[now]->Update();
	}

	void Render(){
		StackScene[now]->Render();
	}

};
