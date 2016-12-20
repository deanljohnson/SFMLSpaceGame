#pragma once
#include <Box2DSerialization.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <StringHelper.h>
#include <cereal\archives\json.hpp>
#include <cereal\archives\binary.hpp>

#include <cereal\types\utility.hpp>

#include <BasicArchive.h>

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
#define SERIALIZATION_IN_ARCHIVE cereal::JSONInputArchive
#define SERIALIZATION_OUT_ARCHIVE cereal::JSONOutputArchive
#endif

#ifdef BINARY_SERIALIZATION
#define SERIALIZATION_IN_ARCHIVE cereal::BinaryInputArchive
#define SERIALIZATION_OUT_ARCHIVE cereal::BinaryOutputArchive
#endif

#ifndef SERIALIZED_WITH_NAME
#define SERIALIZED_WITH_NAME(s) public: static std::string GetTypeName() { static const std::string name = s; return name; }
#endif

template<typename IN_ARCHIVE = SERIALIZATION_IN_ARCHIVE, typename OUT_ARCHIVE = SERIALIZATION_OUT_ARCHIVE>
class Serializer 
{
private:
public:
	template<class T>
	typename std::enable_if<std::is_default_constructible<T>::value, T>::type* Load(std::string name)
	{
		auto extension = "." + T::GetTypeName();
		if (!StringHelper::StrEndsWith(name, extension))
			name += extension;
		
		// Open the input file stream
		auto mode = std::is_same<IN_ARCHIVE, cereal::BinaryInputArchive>::value
					? std::ios::in | std::ios::binary
					: std::ios::in;
		std::ifstream is(DATA_PATH + name, mode);
		if (is.fail())
		{
			std::cout << "Unable to fine the file " << name << "\n";
		}

		// Create the cereal archive from the input stream
		IN_ARCHIVE ar(is);

		// Load the object
		T* t = new T();
		ar(*t);
		return t;
	}

	template<class T>
	typename std::enable_if<!std::is_default_constructible<T>::value, T>::type* Load(std::string name)
	{
		auto extension = "." + T::GetTypeName();
		if (!StringHelper::StrEndsWith(name, extension))
			name += extension;

		// Open the input file stream
		auto mode = std::is_same<IN_ARCHIVE, cereal::BinaryInputArchive>::value
			? std::ios::in | std::ios::binary
			: std::ios::in;
		std::ifstream is(DATA_PATH + name, mode);
		if (is.fail())
		{
			std::cout << "Unable to find the file " << name << "\n";
		}

		// Create the cereal archive from the input stream
		IN_ARCHIVE ar(is);

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
		auto extension = "." + T::GetTypeName();
		if (!StringHelper::StrEndsWith(fileName, extension))
			fileName += extension;

		// open the output file stream
		auto mode = std::is_same<OUT_ARCHIVE, cereal::BinaryOutputArchive>::value
			? std::ios::out | std::ios::binary
			: std::ios::out;
		std::ofstream os(DATA_PATH + fileName, mode);

		//cereal::BasicOutputArchive arB(os);
		//arB(std::string("hello"));
		//arB(std::make_pair(std::string("hello"), 7));
		//arB(cereal::make_nvp(rootName, *obj));

		OUT_ARCHIVE ar(os);
		ar(cereal::make_nvp(rootName, *obj));
	}

	template<class T>
	void DeleteRecord(T* obj, std::string fileName) 
	{
		auto extension = "." + T::GetTypeName();
		if (!StringHelper::StrEndsWith(fileName, extension))
			fileName += extension;

		std::remove((DATA_PATH + fileName).c_str());
	}
};

typedef Serializer<cereal::BinaryInputArchive, cereal::BinaryOutputArchive> BinarySerializer;