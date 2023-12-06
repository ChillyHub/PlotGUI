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
				else if (meta.TypeName() == "float")
				{
					DrawFloat(ptrClass, meta, name, meta.Attribute());
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
			DrawField(name, [&]()
			{
				int t = meta.Get<int>(*ptrClass);

				std::istringstream is(attr);
				std::string token;

				bool draw = false;

				while (is >> token)
				{
					auto offset = token.find("Range");
					if (offset != std::string::npos)
					{
						std::string s = token.substr(offset + 5, token.size() - offset - 6);

						const auto of = s.find(',');
						int min = std::stoi(s.substr(0, of));
						int max = std::stoi(s.substr(of + 1, s.size() - of - 1));

						draw = true;
						ImGui::SliderInt((Mark + name).c_str(), &t, min, max);

						break;
					}

					offset = token.find("ReadOnly");
					if (offset != std::string::npos)
					{
						draw = true;
						ImGui::DragInt((Mark + name).c_str(), &t, 1, 0, 0, "%d", ImGuiSliderFlags_ReadOnly);

						break;
					}
				}

				if (!draw)
				{
					ImGui::DragInt((Mark + name).c_str(), &t);
				}

				meta.Set(*ptrClass, t);
			});
		}

		template <typename T>
		static void DrawFloat(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, [&]()
			{
				float t = meta.Get<float>(*ptrClass);

				std::istringstream is(attr);
				std::string token;

				bool draw = false;

				while (is >> token)
				{
					auto offset = token.find("Range");
					if (offset != std::string::npos)
					{
						std::string s = token.substr(offset + 6, token.size() - offset - 7);

						const auto of = s.find(',');
						float min = std::stof(s.substr(0, of));
						float max = std::stof(s.substr(of + 1, s.size() - of - 1));

						draw = true;
						ImGui::SliderFloat((Mark + name).c_str(), &t, min, max, "%.2f", ImGuiSliderFlags_AlwaysClamp);

						break;
					}

					offset = token.find("ReadOnly");
					if (offset != std::string::npos)
					{
						ImGui::DragFloat((Mark + name).c_str(), &t, 0.01f, 0, 0, "%.2f", ImGuiSliderFlags_ReadOnly);

						break;
					}
				}

				if (!draw)
				{
					ImGui::DragFloat((Mark + name).c_str(), &t, 0.01f, 0, 0, "%.2f");
				}
				
				meta.Set(*ptrClass, t);
			});
		}

		template <typename T>
		static void DrawString(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, [&]()
			{
				std::string t = meta.Get<std::string>(*ptrClass);
				ImGui::InputText((Mark + name).c_str(), t.data(), 100);
				meta.Set(*ptrClass, t);
			});
		}

		template <typename T>
		static void DrawVec4(T* ptrClass, const FieldMeta& meta, const std::string& name, const std::string& attr = std::string())
		{
			DrawField(name, [&]()
			{
				Math::Vec4 t = meta.Get<Math::Vec4>(*ptrClass);
				ImGui::DragFloat4((Mark + name).c_str(), t.data(), 0.01f, 0, 0, "%.2f");
				meta.Set(*ptrClass, t);
			});
		}

	private:
		template <typename Func>
		static void DrawField(const std::string& fieldName, Func func)
		{
			ImGui::PushID(fieldName.c_str());

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 150.0f);
			ImGui::Text(fieldName.c_str());
			ImGui::NextColumn();

			ImGui::PushItemWidth(-1.0f);
			func();
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
