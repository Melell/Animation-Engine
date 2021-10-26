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
	struct Model;


	class ModelInstance : public IComponent
	{
	public:

		ModelInstance();
		virtual ~ModelInstance();

		// Create all the nodes/components from the internal Model resources
		void generate_nodes();
		void generate_components();

		unsigned get_instance_id() const;

	private:

		Model* m_model = nullptr;
		std::string m_previewName = "Empty";
		unsigned m_instanceId = 0;

		void on_gui() override;
	};
}