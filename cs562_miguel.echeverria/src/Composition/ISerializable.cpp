/**
* @file ISerializable.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief Interface for all the classes that can serialized/deserialized. (Taken
*		 from my gam300 serializer).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "ISerializable.h"


namespace cs460
{
	ISerializable::~ISerializable()
	{
	}


	// Functions to write to a json object, and read from one. Users should call
	// this functions, but they should implement their own fromJson and toJson
	// in internalFromJson and internalToJson.
	void ISerializable::fromJson(json& value)
	{
		//std::cout << "DEBUG : FROMJSON CALLED\n";

		//this->clear();
		this->internalFromJson(value);
	}

	void ISerializable::toJson(json& value)
	{
		//std::cout << "DEBUG : TOJSON CALLED\n";

		value["Rtti Type"] = getTypeName(*this);
		this->internalToJson(value);
	}


	// Load/save a/to .json file.
	void ISerializable::loadFromJsonFile(const char* filePath)
	{
		//std::cout << "DEBUG : LOAD FROM JSON FILE CALLED. FILE : " << filePath << std::endl;

		// Open the file for reading
		std::ifstream inFile(filePath);
		if (inFile.is_open() && inFile.good())
		{
			// Read the json object from a .json file
			json value;
			inFile >> value;

			// Fill this with the json value
			fromJson(value);
		}
	}

	void ISerializable::saveToJsonFile(const char* filePath)
	{
		//std::cout << "DEBUG : SAVE TO JSON FILE CALLED. FILE : " << filePath << std::endl;

		// Open the file for writing
		std::ofstream outFile(filePath);
		if (outFile.is_open() && outFile.good())
		{
			// Save the data to a json object
			json value;
			toJson(value);

			// Write the json object to file
			outFile << std::setw(4) << value;
		}
	}


	// Pure virtual so that objects that inherit from it are forced to implement them.
	// This are the actual functions that the users of this class should implement,
	// and not the ones in public.
	void ISerializable::internalFromJson(json& value)
	{
	}

	void ISerializable::internalToJson(json& value)
	{
	}


	// The following can be implemented by derived classes to delete internal resources
	// (like vectors of spaces, objects, comps etc), as it will be called at the
	// beginning of fromJson.
	//void ISerializable::clear()
	//{
	//}



	// Template specializations for glm types

	template<>
	const json& operator>>(const json& value, std::string& str)
	{
		//std::cout << "DEBUG : OPERATOR>> SPECIALIZATION CALLED. TYPE : std::string\n";

		if (value != nullptr)
		{
			try
			{
				str = value.get_ref<const json::string_t&>();
			}
			catch (const json::type_error&)
			{
				// Json error: the value for key Name is not an string.
				//LogCoreError(error.what());
			}
		}

		return value;
	}


	template<>
	json& operator<<(json& value, const glm::vec2& vec2)
	{
		//std::cout << "DEBUG : OPERATOR<< SPECIALIZATION CALLED. TYPE : " << "glm::vec2\n";

		json val;
		val["x"] = vec2.x;
		val["y"] = vec2.y;
		value = val;
		return value;
	}

	template<>
	const json& operator>>(const json& value, glm::vec2& vec2)
	{
		//std::cout << "DEBUG : OPERATOR>> SPECIALIZATION CALLED. TYPE : " << "glm::vec2\n";

		if (value != nullptr)
		{
			value["x"] >> vec2.x;
			value["y"] >> vec2.y;
		}

		return value;
	}

	template<>
	json& operator<<(json& value, const glm::vec3& vec3)
	{
		//std::cout << "DEBUG : OPERATOR<< SPECIALIZATION CALLED. TYPE : " << "glm::vec3\n";

		json val;
		val["x"] = vec3.x;
		val["y"] = vec3.y;
		val["z"] = vec3.z;
		value = val;
		return value;
	}

	template<>
	const json& operator>>(const json& value, glm::vec3& vec3)
	{
		//std::cout << "DEBUG : OPERATOR>> SPECIALIZATION CALLED. TYPE : " << "glm::vec3\n";

		if (value != nullptr)
		{
			value["x"] >> vec3.x;
			value["y"] >> vec3.y;
			value["z"] >> vec3.z;
		}

		return value;
	}

	template<>
	json& operator<<(json& value, const glm::vec4& vec4)
	{
		//std::cout << "DEBUG : OPERATOR<< SPECIALIZATION CALLED. TYPE : " << "glm::vec4\n";

		json val;
		val["x"] = vec4.x;
		val["y"] = vec4.y;
		val["z"] = vec4.z;
		val["w"] = vec4.w;
		value = val;
		return value;

	}

	template<>
	const json& operator>>(const json& value, glm::vec4& vec4)
	{
		//std::cout << "DEBUG : OPERATOR>> SPECIALIZATION CALLED. TYPE : " << "glm::vec4\n";

		if (value != nullptr)
		{
			value["x"] >> vec4.x;
			value["y"] >> vec4.y;
			value["z"] >> vec4.z;
			value["w"] >> vec4.w;
		}

		return value;
	}
}