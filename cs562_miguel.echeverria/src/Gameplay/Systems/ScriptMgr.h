/**
* @file ScriptMgr.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Singleton class that stores all the existing script components
*		 and updates them.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class ScriptComponent;


	class ScriptMgr
	{
	public:

		static ScriptMgr& get_instance();
		~ScriptMgr();

		void initialize();		// Here for convenience, although shouldn't really be used, as each component should be initialized separately when necessary
		void update();

		void add_script(ScriptComponent* scriptComp);
		void remove_script(ScriptComponent* scriptComp);

	private:

		std::vector<ScriptComponent*> m_scripts;

		ScriptMgr();
		ScriptMgr(const ScriptMgr&) = delete;
		ScriptMgr& operator=(const ScriptMgr&) = delete;
	};
}