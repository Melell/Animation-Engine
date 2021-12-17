/**
* @file ClothMgr.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/12
* @brief Updates all the cloth components currently active.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Cloth;


	class ClothMgr
	{
	public:

		static ClothMgr& get_instance();
		~ClothMgr();

		void update();			// Updates all the cloth components in order
		void debug_draw();		// Uses debug drawing to render each cloth

		void add_cloth(Cloth* cloth);
		void remove_cloth(Cloth* cloth);

	private:
		std::vector<Cloth*> m_cloths;

		ClothMgr();
		ClothMgr(const ClothMgr&) = delete;
		ClothMgr& operator=(const ClothMgr&) = delete;
	};
}