#pragma once


namespace cs460
{
	// Don't use this function. Meant for internal use in getTypeName overloads.
	std::string extractClassOrStruct(const char* name);


	/**
	* @brief Returns the name of the type of T (without 'class' or 'struct' prefix).
	* @param T				The type of which we want to obtain the name.
	* @return std::string	The name of the type T.
	*/
	template<typename T>
	std::string getTypeName()
	{
		return extractClassOrStruct(typeid(T).name());
	}


	/**
	* @brief Returns the name of the type of instance.
	* @param T				The type of the instance (without 'class' or 'struct' prefix) of
	*						which we want to obtain the type (can also be a pointer).
	* @param instance		A const reference to the instance of which we want to know its type.
	* @return std::string	The name of the type T.
	*/
	template<typename T>
	std::string getTypeName(const T& instance)
	{
		return extractClassOrStruct(typeid(instance).name());
	}
}