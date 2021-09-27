/**
* @file Model.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/25/09
* @brief Model component that can load a gltf file.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include "Components/IComponent.h"


namespace tinygltf
{
	class Model;
}

namespace cs460
{
	class Model : public IComponent
	{
	public:

		void load_gltf();
		void process_model_data(const tinygltf::Model& model);


	private:

		std::string m_fileName = "Empty";
		std::string m_filePath;

		void on_gui() override;
	};
}