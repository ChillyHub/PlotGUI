#include "ProjectsManager.h"

#include "Component/Console.h"

#include "Reflection/Include/Reflect.h"

namespace PlotGUI
{
	ProjectsManager::ProjectsManager()
	{
		int i = 0;
		const auto& map = Registry::Instance().GetClassesMap();
		for (const auto& [str, ptr] : map)
		{
			if (ptr->GetClassAttribute() == "Project")
			{
				ProjectState state;
				state.name = ptr->GetClassNameRefl();
				state.className = state.name;
				state.isActive = false;// (i == 0) ? true : false;

				m_ProjectStates[state.name] = state;
			}

			i++;
		}
	}

	ProjectsManager::~ProjectsManager()
	{
		m_ProjectStates.clear();
		m_Projects.clear();
	}

	void ProjectsManager::CreateProject(const std::string& projectName)
	{
		if (m_ProjectStates.find(projectName) != m_ProjectStates.end() && m_Projects.find(projectName) == m_Projects.end())
		{
			m_Projects[projectName] = std::shared_ptr<Script>(REFLECT_CREATE(Script*, m_ProjectStates[projectName].className));
			m_Projects[projectName]->Start();
		}
	}

	void ProjectsManager::ActiveProject(const std::string& projectName)
	{
		if (m_Projects.find(projectName) == m_Projects.end())
		{
			CreateProject(projectName);
		}
		if (m_ProjectStates.find(projectName) != m_ProjectStates.end())
		{
			m_ProjectStates[projectName].isActive = true;
		}
	}

	void ProjectsManager::DisableProject(const std::string& projectName)
	{
		if (m_ProjectStates.find(projectName) != m_ProjectStates.end())
		{
			m_ProjectStates[projectName].isActive = false;
		}
	}

	void ProjectsManager::DestroyProject(const std::string& projectName)
	{
		if (m_ProjectStates.find(projectName) != m_ProjectStates.end())
		{
			m_ProjectStates[projectName].isActive = false;
		}
		if (m_Projects.find(projectName) != m_Projects.end())
		{
			m_Projects.erase(projectName);
		}
	}

	void ProjectsManager::UpdateProjects()
	{
		for (auto& [_, state] : GetProjectStates())
		{
			if (state.isActive)
			{
				Script* s = GetProject(state.name).get();

				try
				{
					s->Update();
				}
				catch (const std::exception& e)
				{
					DisableProject(state.name);
					Console::LogError(e.what());
				}
			}
		}
	}

	void ProjectsManager::OnPlotProjects()
	{
		for (auto& [_, state] : GetProjectStates())
		{
			if (state.isActive)
			{
				Script* s = GetProject(state.name).get();

				try
				{
					s->OnPlot();
				}
				catch (const std::exception& e)
				{
					DisableProject(state.name);
					Console::LogError(e.what());
				}
			}
		}
	}

	void ProjectsManager::OnChangeProject(const std::string& projectName)
	{
		if (m_ProjectStates.find(projectName) != m_ProjectStates.end())
		{
			ActiveProject(projectName);

			Script* s = GetProject(projectName).get();

			try
			{
				s->Update();
			}
			catch (const std::exception& e)
			{
				DisableProject(projectName);
				Console::LogError(e.what());
			}
		}
	}

	const std::map<std::string, ProjectState>& ProjectsManager::GetProjectStates() const
	{
		return m_ProjectStates;
	}

	std::shared_ptr<Script> ProjectsManager::GetProject(const std::string& projectName)
	{
		CreateProject(projectName);
		return m_Projects[projectName];
	}
}
