#pragma once

#include <string>

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

				if (meta.Attribute() == "int")
				{
					DrawInt(ptrClass, meta, name);
				}
				else if (meta.Attribute() == "float")
				{
					DrawFloat(ptrClass, meta, name);
				}
				else if (meta.Attribute() == "string")
				{
					DrawString(ptrClass, meta, name);
				}
				else if (meta.Attribute() == "vec4")
				{
					DrawVec4(ptrClass, meta, name);
				}
			}
		}

	public:
		template <typename T>
		static void DrawInt(T* ptrClass, const FieldMeta& meta, const std::string& name)
		{
			DrawField(name, [&]()
				{
					int t = meta.Get<int>(*ptrClass);
					ImGui::DragInt((Mark + name).c_str(), &t);
					meta.Set(*ptrClass, t);
				});
		}

		template <typename T>
		static void DrawFloat(T* ptrClass, const FieldMeta& meta, const std::string& name)
		{
			DrawField(name, [&]()
				{
					float t = meta.Get<float>(*ptrClass);
					ImGui::DragFloat((Mark + name).c_str(), &t, 0.01f, 0, 0, "%.2f");
					meta.Set(*ptrClass, t);
				});
		}

		template <typename T>
		static void DrawString(T* ptrClass, const FieldMeta& meta, const std::string& name)
		{
			DrawField(name, [&]()
				{
					std::string t = meta.Get<std::string>(*ptrClass);
					ImGui::InputText((Mark + name).c_str(), t.data(), 100);
					meta.Set(*ptrClass, t);
				});
		}

		template <typename T>
		static void DrawVec4(T* ptrClass, const FieldMeta& meta, const std::string& name)
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

			func();

			ImGui::Columns(1);

			ImGui::PopID();
		}

	private:
		static std::string TransName(const std::string& fieldName);

	private:
		static const std::string Mark;
	};
}
