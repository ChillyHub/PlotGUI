#pragma once

#include "Utils/Singleton.h"

#include "Component/Script.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <map>

namespace PlotGUI
{
	struct ProjectState
	{
		std::string name = "Start";
		std::string className = "StartScript";
		bool isActive = false;
	};

	class ProjectsManager : public Singleton<ProjectsManager>
	{
	public:
		ProjectsManager();
		~ProjectsManager() override;

	public:
		void CreateProject(const std::string& projectName);
		void ActiveProject(const std::string& projectName);
		void DisableProject(const std::string& projectName);
		void DestroyProject(const std::string& projectName);

		void UpdateProjects();
		void OnPlotProjects();
		void OnChangeProject(const std::string& projectName);

		[[nodiscard]] const std::map<std::string, ProjectState>& GetProjectStates() const;
		std::shared_ptr<Script> GetProject(const std::string& projectName);

	private:
		std::map<std::string, ProjectState> m_ProjectStates;
		std::unordered_map<std::string, std::shared_ptr<Script>> m_Projects;
	};
}
