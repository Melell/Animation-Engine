#include "pch.h"
#include "Rigidbody.h"
#include "Physics/RigidbodyMgr.h"


namespace cs460
{
	Rigidbody::Rigidbody()
	{
		RigidbodyMgr::get_instance().add_rigidbody(this);
	}

	Rigidbody::~Rigidbody()
	{
		RigidbodyMgr::get_instance().remove_rigidbody(this);
	}


	void Rigidbody::on_gui()
	{
	}
}