#include "Editor.h"

namespace Glide3D
{
	void Editor::InitializeEditor()
	{

	}

	Editor::Editor(std::shared_ptr<Renderer> renderer, GLFWwindow* window) :
		m_Renderer(renderer), m_Window(window)
	{

	}

	void Editor::OnUpdate()
	{
		auto& entities = m_Renderer->m_Entities;

		if (ImGui::Begin("Loader"))
		{
			static std::string model_path;

			// Texture paths
			static std::string albedo_path;
			static std::string metallic_path;
			static std::string roughness_path;
			static std::string normal_path;
			static std::string ao_path;

			OPENFILENAME ofn;
			TCHAR szFile[1024] = { 0 };

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = glfwGetWin32Window(m_Window);
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = L"\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			ImGui::Text("Model File : "); ImGui::SameLine();
			ImGui::Text(model_path.c_str()); ImGui::SameLine();

			if (ImGui::Button("Select Model File..."))
			{
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::wstring str;
					str = ofn.lpstrFile;
					model_path = std::string(str.begin(), str.end());
				}
			}

			ImGui::Text("Albedo File : "); ImGui::SameLine();
			ImGui::Text(albedo_path.c_str()); ImGui::SameLine();

			if (ImGui::Button("Select Albedo Texture..."))
			{
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::wstring str;
					str = ofn.lpstrFile;
					albedo_path = std::string(str.begin(), str.end());
				}
			}

			ImGui::Text("Normal File : "); ImGui::SameLine();
			ImGui::Text(normal_path.c_str()); ImGui::SameLine();

			if (ImGui::Button("Select Normal Texture"))
			{
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::wstring str;
					str = ofn.lpstrFile;
					normal_path = std::string(str.begin(), str.end());
				}
			}

			ImGui::Text("Metallic File : "); ImGui::SameLine();
			ImGui::Text(metallic_path.c_str()); ImGui::SameLine();

			if (ImGui::Button("Select Metallic Texture"))
			{
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::wstring str;
					str = ofn.lpstrFile;
					metallic_path = std::string(str.begin(), str.end());
				}
			}

			ImGui::Text("Roughness File : "); ImGui::SameLine();
			ImGui::Text(roughness_path.c_str()); ImGui::SameLine();

			if (ImGui::Button("Select Roughness Texture"))
			{
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::wstring str;
					str = ofn.lpstrFile;
					roughness_path = std::string(str.begin(), str.end());
				}
			}

			ImGui::Text("AO File : "); ImGui::SameLine();
			ImGui::Text(ao_path.c_str()); ImGui::SameLine();

			if (ImGui::Button("Select AO Texture..."))
			{
				if (GetOpenFileName(&ofn) == TRUE)
				{
					std::wstring str;
					str = ofn.lpstrFile;
					ao_path = std::string(str.begin(), str.end());
				}
			}

			if (ImGui::Button("Create"))
			{
				if (model_path.size() > 0)
				{
					std::unique_ptr<Object> object(new Object);
					Entity* entity = new Entity(object.get());
					entity->GetTransform().Translate(glm::vec3(0.0f));

					m_Renderer->AddEntity(entity);
					FileLoader::LoadOBJFile(object.get(), model_path);
					
					object->AddTextureMapToMesh(albedo_path, TextureType::Albedo);
					object->AddTextureMapToMesh(normal_path, TextureType::Normal);
					object->AddTextureMapToMesh(metallic_path, TextureType::Metalness);
					object->AddTextureMapToMesh(roughness_path, TextureType::Roughness);
					object->AddTextureMapToMesh(ao_path, TextureType::AO);

					m_Objects.emplace_back(std::move(object));

					model_path = "";
					albedo_path = "";
					normal_path = "";
					metallic_path = "";
					roughness_path = "";
					ao_path = "";

				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Entity Control"))
		{
			for (auto& e : entities)
			{
				const Object* object = e.second.at(0)->p_Object;
				const std::string& object_name = object->m_ObjectName;

				if (ImGui::CollapsingHeader(object->m_ObjectName.c_str()))
				{
					int j = 0; 

					for (auto& i : e.second)
					{
						std::string entity_name = "Entity - " + std::to_string(j);

						ImGui::BeginChild(object_name.c_str());
						ImGui::CollapsingHeader(entity_name.c_str());
						ImGui::Text("Translation");
						ImGui::EndChild();

						j++;
					}
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Insert Entities"))
		{
			for (auto& e : m_Objects)
			{
				if (ImGui::Button(e->m_ObjectName.c_str()))
				{
					std::unique_ptr<Entity> entity(new Entity(e.get()));
					m_Entities.push_back(std::move(entity));

					m_Renderer->AddEntity(entity.get());
				}
			}
		}
	}
}