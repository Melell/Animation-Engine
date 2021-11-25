/**
* @file Factory.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/20/11
* @brief System in charge of creating objects from names (names that match the class name).
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once



namespace cs460
{
	class ICreator
	{
	public:

		ICreator();
		virtual ~ICreator();

		virtual IBase* create() = 0;
	};

	template <typename T>
	class TCreator : public ICreator
	{
	public:

		TCreator()
		{
		}

		virtual ~TCreator()
		{
		}

		IBase* create() override
		{
			return dynamic_cast<IBase*>(new T);
		}
	};


	
	class Factory
	{
	public:

		static Factory& get_instance();
		~Factory();

		// System management functions, doesn't need an update
		bool initialize();
		void close();

		// Create from a type or from the name of a class
		template <typename T>
		IBase* create();
		IBase* create(const std::string& className);

		// Add a creator from a type or from a specific name (associated to a type)
		template <typename T>
		void add_creator();
		template <typename T>
		void add_creator(const std::string& className);

	private:
		std::unordered_map<std::string, ICreator*> m_creators;

		Factory();
		Factory(const Factory&) = delete;
		Factory& operator=(const Factory&) = delete;
	};


	template <typename T>
	IBase* Factory::create()
	{
		const std::string& typeName = getTypeName<T>();
		auto found = m_creators.find(typeName);

		if (found == m_creators.end())
			return nullptr;

		return found->second->create();
	}


	template <typename T>
	void Factory::add_creator()
	{
		const std::string& typeName = getTypeName<T>();
		add_creator<T>(typeName);
	}

	template <typename T>
	void Factory::add_creator(const std::string& className)
	{
		auto found = m_creators.find(className);
		if (found != m_creators.end())
		{
			std::cout << "WARNING: Couldn't add creator with name " << className << ", because there is already one with that name\n";
			return;
		}

		m_creators[className] = new TCreator<T>;
	}
}