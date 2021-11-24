/**
* @file ISerializable.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Interface for all the classes that can serialized/deserialized. (Taken
*		 from my gam300 serializer).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once

#include <json/json.hpp>

using json = nlohmann::json;


namespace cs460
{
	class ISerializable
	{
	public:

		// Empty for now, but virtual so that appropriate destructor is called.
		virtual ~ISerializable();

		// Functions to write to a json object, and read from one. Users should call
		// this functions, but they should implement their own fromJson and toJson
		// in internalFromJson and internalToJson.
		void fromJson(json& value);
		void toJson(json& value);

		// Load/save the data of this object with/to the .json file passed as parameter.
		void loadFromJsonFile(const char* filePath);
		void saveToJsonFile(const char* filePath);

	private:

		// Pure virtual so that objects that inherit from it are forced to implement them.
		// This are the actual functions that the users of this class should implement,
		// and not the ones in public.
		virtual void internalFromJson(json& value) = 0;
		virtual void internalToJson(json& value) = 0;

		// The following can be implemented by derived classes to delete internal resources
		// (like vectors of spaces, objects, comps etc), as it will be called at the
		// beginning of fromJson.
		//virtual void clear();
	};


	// IMPORTANT: USE THIS OPERATORS TO SERIALIZE/DESERIALIZE SIMPLE TYPES

	// Operator overloads for built-in types.
	template<typename T>
	json& operator<<(json& value, const T& type)
	{
		//std::cout << "DEBUG : TEMPLATE OPERATOR>> CALLED. TYPE : " << typeid(type).name() << "\n";

		value = type;
		return value;
	}

	template<typename T>
	const json& operator>>(const json& value, T& type)
	{
		//std::cout << "DEBUG : TEMPLATE OPERATOR>> CALLED. TYPE : "<<typeid(type).name()<<"\n";

		if (value != nullptr)
			type = value.get<T>();

		return value;
	}


	// Template specializations for glm types and others

	template<>
	const json& operator>>(const json& value, std::string& str);

	template<>
	json& operator<<(json& value, const glm::vec2& vec2);
	template<>
	const json& operator>>(const json& value, glm::vec2& vec2);

	template<>
	json& operator<<(json& value, const glm::vec3& vec3);
	template<>
	const json& operator>>(const json& value, glm::vec3& vec3);

	template<>
	json& operator<<(json& value, const glm::vec4& vec4);
	template<>
	const json& operator>>(const json& value, glm::vec4& vec4);
}