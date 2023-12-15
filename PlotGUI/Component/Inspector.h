#pragma once

#include <string>
#include <sstream>

#include "imgui.h"
#include "imgui_internal.h"

#include "Reflection/Include/Reflect.h"
#include "Math/CMath.h"

namespace PlotGUI
{
	class Inspector
	{
	public:
		template <typename T>
		static void ShowRegisteredFields(T* ptrClass, const std::string& className)
		{
			const auto& metas = Registry::Instance().Find(className)->GetFieldMetas();
			for (auto& meta : metas)
			{
				std::string name = TransName(meta.Name());

				if (meta.TypeName() == "int")
				{
					DrawInt(ptrClass, meta, name, meta.Attribute());
				}
				else if (meta.TypeName() == "unsigned int")
				{
					DrawInt(ptrClass, meta, name, meta.Attribute());
				}
				else if (meta.TypeName() == "float")
				{
					DrawFloat(ptrClass, meta, name, meta.Attribute());
				}
				else if (meta.TypeName() == "double")
				{
					DrawDouble(ptrClass, meta, name, meta.Attribute());
				}
				else if (meta.TypeName() == "string")
				{
					DrawString(ptrClass, meta, name, meta.Attribute());
				}
				else if (meta.TypeName() == "vec4")
				{
					DrawVec4(ptrClass, meta, name, meta.Attribute());
				}
			}
		}

	public:
		template <typename T>
		static void DrawInt(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, attr, [&](std::unordered_map<std::string, std::pair<double, double>>& attrMap)
			{
				int t = meta.Get<int>(*ptrClass);

				bool set = true;

				if (attrMap.find("ReadOnly") != attrMap.end())
				{
					set = false;
					ImGui::DragInt((Mark + name).c_str(), &t, 1, 0, 0, "%d", ImGuiSliderFlags_ReadOnly);
				}
				else if (attrMap.find("Range") != attrMap.end())
				{
					const int min = static_cast<int>(attrMap["Range"].first);
					const int max = static_cast<int>(attrMap["Range"].second);

					ImGui::SliderInt((Mark + name).c_str(), &t, min, max);
				}
				else
				{
					ImGui::DragInt((Mark + name).c_str(), &t);
				}

				if (set)
				{
					meta.Set(*ptrClass, t);
				}
			});
		}

		template <typename T>
		static void DrawUnsignedInt(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, attr, [&](std::unordered_map<std::string, std::pair<double, double>>& attrMap)
			{
				int t = static_cast<int>(meta.Get<unsigned int>(*ptrClass));

				bool set = true;

				if (attrMap.find("ReadOnly") != attrMap.end())
				{
					set = false;
					ImGui::DragInt((Mark + name).c_str(), &t, 1, 0, 0, "%d", ImGuiSliderFlags_ReadOnly);
				}
				else if (attrMap.find("Range") != attrMap.end())
				{
					const int min = static_cast<int>(attrMap["Range"].first);
					const int max = static_cast<int>(attrMap["Range"].second);

					ImGui::SliderInt((Mark + name).c_str(), &t, min, max);
				}
				else
				{
					ImGui::DragInt((Mark + name).c_str(), &t);
				}

				if (set)
				{
					unsigned int ut = static_cast<unsigned int>(t);
					meta.Set(*ptrClass, ut);
				}
			});
		}

		template <typename T>
		static void DrawFloat(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, attr, [&](std::unordered_map<std::string, std::pair<double, double>>& attrMap)
			{
				float t = meta.Get<float>(*ptrClass);

				bool set = true;

				if (attrMap.find("ReadOnly") != attrMap.end())
				{
					set = false;
					ImGui::DragFloat((Mark + name).c_str(), &t, 0.01f, 0, 0, "%.2f", ImGuiSliderFlags_ReadOnly);
				}
				else if (attrMap.find("Range") != attrMap.end())
				{
					const float min = static_cast<float>(attrMap["Range"].first);
					const float max = static_cast<float>(attrMap["Range"].second);

					ImGui::SliderFloat((Mark + name).c_str(), &t, min, max, "%.2f", ImGuiSliderFlags_AlwaysClamp);
				}
				else
				{
					ImGui::DragFloat((Mark + name).c_str(), &t, 0.01f, 0, 0, "%.2f");
				}
				
				if (set)
				{
					meta.Set(*ptrClass, t);
				}
			});
		}

		template <typename T>
		static void DrawDouble(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, attr, [&](std::unordered_map<std::string, std::pair<double, double>>& attrMap)
			{
				float t = static_cast<float>(meta.Get<double>(*ptrClass));

				bool set = true;

				if (attrMap.find("ReadOnly") != attrMap.end())
				{
					set = false;
					ImGui::DragFloat((Mark + name).c_str(), &t, 0.001f, 0, 0, "%.4f", ImGuiSliderFlags_ReadOnly);
				}
				else if (attrMap.find("Range") != attrMap.end())
				{
					const float min = static_cast<float>(attrMap["Range"].first);
					const float max = static_cast<float>(attrMap["Range"].second);

					ImGui::SliderFloat((Mark + name).c_str(), &t, min, max, "%.4f", ImGuiSliderFlags_AlwaysClamp);
				}
				else
				{
					ImGui::DragFloat((Mark + name).c_str(), &t, 0.0001f, 0, 0, "%.4f");
				}

				if (set)
				{
					double dt = static_cast<double>(t);
					meta.Set(*ptrClass, dt);
				}
			});
		}

		template <typename T>
		static void DrawString(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, attr, [&](std::unordered_map<std::string, std::pair<double, double>>& attrMap)
			{
				std::string t = meta.Get<std::string>(*ptrClass);

				bool set = true;

				if (attrMap.find("ReadOnly") != attrMap.end())
				{
					set = false;
					ImGui::InputText((Mark + name).c_str(), t.data(), 100, ImGuiInputTextFlags_ReadOnly);
				}
				else
				{
					ImGui::InputText((Mark + name).c_str(), t.data(), 100);
				}

				if (set)
				{
					meta.Set(*ptrClass, t);
				}
			});
		}

		template <typename T>
		static void DrawVec4(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, attr, [&](std::unordered_map<std::string, std::pair<double, double>>& attrMap)
			{
				Math::Vec4 t = meta.Get<Math::Vec4>(*ptrClass);

				bool set = true;

				if (attrMap.find("ReadOnly") != attrMap.end())
				{
					set = false;
					ImGui::DragFloat4((Mark + name).c_str(), t.data(), 0.01f, 0, 0, "%.2f", ImGuiSliderFlags_ReadOnly);
				}
				else
				{
					ImGui::DragFloat4((Mark + name).c_str(), t.data(), 0.01f, 0, 0, "%.2f");
				}

				if (set)
				{
					meta.Set(*ptrClass, t);
				}
			});
		}

	private:
		template <typename Func>
		static void DrawField(const std::string& fieldName, const std::string& attr, Func func)
		{
			std::unordered_map<std::string, std::pair<double, double>> attrMap;

			std::istringstream is(attr);
			std::string token;

			while (is >> token)
			{
				auto offset = token.find("Space");
				if (offset != std::string::npos)
				{
					std::string s = token.substr(offset + 6, token.size() - offset - 7);
					int count = std::stoi(s);

					attrMap["Space"] = std::pair<double, double>(static_cast<double>(count), 0.0);

					continue;
				}

				offset = token.find("Range");
				if (offset != std::string::npos)
				{
					std::string s = token.substr(offset + 6, token.size() - offset - 7);

					const auto of = s.find(',');
					float min = std::stof(s.substr(0, of));
					float max = std::stof(s.substr(of + 1, s.size() - of - 1));

					attrMap["Range"] = std::pair<double, double>(static_cast<double>(min), static_cast<double>(max));

					continue;
				}

				offset = token.find("ReadOnly");
				if (offset != std::string::npos)
				{
					attrMap["ReadOnly"] = std::pair<double, double>(0.0, 0.0);

					continue;
				}
			}

			ImGui::PushID(fieldName.c_str());

			if (attrMap.find("Space") != attrMap.end())
			{
				const int count = static_cast<int>(attrMap["Space"].first);
				for (int i = 0; i < count; ++i)
				{
					ImGui::Spacing();
				}
			}

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text(fieldName.c_str());
			ImGui::NextColumn();

			ImGui::PushItemWidth(-1.0f);
			func(attrMap);
			ImGui::PopItemWidth();

			ImGui::Columns(1);

			ImGui::PopID();
		}

	private:
		static std::string TransName(const std::string& fieldName);

	private:
		static const std::string Mark;
	};
}
