/**
* @file RigidbodyMgr.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Contains all the rigidbody components, and updates them.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once


namespace cs460
{
	class Rigidbody;


	class RigidbodyMgr
	{
	public:

		static RigidbodyMgr& get_instance();
		~RigidbodyMgr();

		void add_rigidbody(Rigidbody* rigidbody);
		void remove_rigidbody(Rigidbody* rigidbody);

	private:
		std::vector<Rigidbody*> m_rigidbodies;

		RigidbodyMgr();
		RigidbodyMgr(const RigidbodyMgr&) = delete;
		RigidbodyMgr& operator=(const RigidbodyMgr&) = delete;
	};
}