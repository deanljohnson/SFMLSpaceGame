#pragma once
#include <Box2DSerialization.h>
#include <iostream>
#include <fstream>
#include <cstdio>

#ifdef RELEASE
#ifndef DATA_PATH
#define DATA_PATH "Data\\"
#endif
#else
#ifndef DATA_PATH
#define DATA_PATH "..\\Data\\"
#endif
#endif

// Change this to change the type of serialization used
#define JSON_SERIALIZATION

#ifdef JSON_SERIALIZATION
#include <cereal\archives\json.hpp>
#define SERIALIZATION_EXTENSION ".json"
#define SERIALIZATION_IN_ARCHIVE cereal::JSONInputArchive
#define SERIALIZATION_OUT_ARCHIVE cereal::JSONOutputArchive
#endif

class Serializer 
{
private:

public:
	template<class T, typename std::enable_if<std::is_default_constructible<T>::value>::type* = nullptr>
	T* Load(std::string name)
	{
		name += "." + T::GetTypeName();
		
		// Open the input file stream
		std::ifstream is(DATA_PATH + name);
		if (is.fail())
		{
			std::cout << "Unable to fine the file " << name << "\n";
		}

		// Create the cereal archive from the input stream
		SERIALIZATION_IN_ARCHIVE ar(is);

		// Load the object
		T* t = new T();
		ar(*t);
		return t;
	}

	template<class T, typename std::enable_if<!std::is_default_constructible<T>::value>::type* = nullptr>
	T* Load(std::string name)
	{
		name += "." + T::GetTypeName();

		// Open the input file stream
		std::ifstream is(DATA_PATH + name);
		if (is.fail())
		{
			std::cout << "Unable to fine the file " << name << "\n";
		}

		// Create the cereal archive from the input stream
		SERIALIZATION_IN_ARCHIVE ar(is);

		// Load the object. Since T is not default
		// constructible, we have to serialize a
		// pointer to it in order to have cereal
		// call the constructor that T does have
		// (see T's load_and_construct method)
		std::unique_ptr<T> ptr;
		ar(ptr);
		return ptr.release();
	}

	template<class T>
	void Save(T* obj, std::string fileName, std::string rootName)
	{
		fileName += "." + T::GetTypeName();

		std::ofstream os(DATA_PATH + fileName);
		SERIALIZATION_OUT_ARCHIVE ar(os);
		ar(cereal::make_nvp(rootName, *obj));
	}

	template<class T>
	void DeleteRecord(T* obj, std::string fileName) 
	{
		fileName += "." + T::GetTypeName();
		std::remove((DATA_PATH + fileName).c_str());
	}
};