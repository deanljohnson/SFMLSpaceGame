#pragma once
#include <cereal/cereal.hpp>
#include <cereal/details/util.hpp>
#include <cereal/external/base64.hpp>

namespace cereal
{
	class BasicOutputArchive
		: public cereal::OutputArchive<BasicOutputArchive>,
		public cereal::traits::TextArchive
	{
	private:
		using WriteStream = std::ostream;
		WriteStream& m_writer;
		std::string m_nextName;

	public:
		size_t indentLevel{0};

		BasicOutputArchive(std::ostream& stream)
			: OutputArchive<BasicOutputArchive>(this),
			m_writer(stream)
		{}

		~BasicOutputArchive()
		{}

		void saveBinaryValue(const void* data, size_t size, const char* name = nullptr)
		{
			setNextName(name);
			writeName();

			auto base64string = cereal::base64::encode(reinterpret_cast<const unsigned char *>(data), size);
			saveValue(base64string);
		}

		void setNextName(const std::string& name)
		{
			m_nextName = name;
		}

		void writeName()
		{
			for (size_t i = 0; i < indentLevel; i++) 
			{
				saveValue(std::string("\t"));
			}

			if (!m_nextName.empty())
				m_writer << std::string(m_nextName) << ": ";
		}

		void startNode() 
		{
			indentLevel++;
		}

		void finishNode() 
		{
			indentLevel--;
			m_writer << "\n";
		}

		void saveValue(const std::string& str) { m_writer << str; }
		void saveValue(const char* s) { m_writer << s; }
		void saveValue(bool b) { m_writer << b; }
		void saveValue(int i) { m_writer << i; }
		void saveValue(unsigned u) { m_writer << u; }
		void saveValue(int64_t i64) { m_writer << i64; }
		void saveValue(uint64_t u64) { m_writer << u64; }
		void saveValue(double d) { m_writer << d; }
		void saveValue(std::nullptr_t) { m_writer << "null"; }
		void saveValue(unsigned long lu) { saveLong(lu); };

		//! 32 bit signed long saving to current node
		template <class T, traits::EnableIf<sizeof(T) == sizeof(std::int32_t),
			std::is_signed<T>::value> = traits::sfinae> inline
			void saveLong(T l) { saveValue(static_cast<std::int32_t>(l)); }

		//! non 32 bit signed long saving to current node
		template <class T, traits::EnableIf<sizeof(T) != sizeof(std::int32_t),
			std::is_signed<T>::value> = traits::sfinae> inline
			void saveLong(T l) { saveValue(static_cast<std::int64_t>(l)); }

		//! 32 bit unsigned long saving to current node
		template <class T, traits::EnableIf<sizeof(T) == sizeof(std::int32_t),
			std::is_unsigned<T>::value> = traits::sfinae> inline
			void saveLong(T lu) { saveValue(static_cast<std::uint32_t>(lu)); }

		//! non 32 bit unsigned long saving to current node
		template <class T, traits::EnableIf<sizeof(T) != sizeof(std::int32_t),
			std::is_unsigned<T>::value> = traits::sfinae> inline
			void saveLong(T lu) { saveValue(static_cast<std::uint64_t>(lu)); }
	};

	template<class T>
	inline void prologue(BasicOutputArchive& ar, const cereal::NameValuePair<T>&)
	{}

	template<class T>
	inline void epilogue(BasicOutputArchive& ar, const cereal::NameValuePair<T>&)
	{}

	template<class T>
	inline void prologue(BasicOutputArchive& ar, const cereal::SizeTag<T>&)
	{}

	template<class T>
	inline void epilogue(BasicOutputArchive& ar, const cereal::SizeTag<T>&)
	{}

	template <class T, cereal::traits::EnableIf<!std::is_arithmetic<T>::value,
		!cereal::traits::has_minimal_base_class_serialization<T, cereal::traits::has_minimal_output_serialization, BasicOutputArchive>::value,
		!cereal::traits::has_minimal_output_serialization<T, BasicOutputArchive>::value> = traits::sfinae>
		inline void prologue(BasicOutputArchive& ar, T const &)
	{
		ar.startNode();
	}

	template <class T, cereal::traits::EnableIf<!std::is_arithmetic<T>::value,
		!cereal::traits::has_minimal_base_class_serialization<T, cereal::traits::has_minimal_output_serialization, BasicOutputArchive>::value,
		!cereal::traits::has_minimal_output_serialization<T, BasicOutputArchive>::value> = traits::sfinae>
		inline void epilogue(BasicOutputArchive& ar, T const &)
	{
		ar.finishNode();
	}

	inline void prologue(BasicOutputArchive& ar, const std::nullptr_t&)
	{
		ar.writeName();
	}

	inline void epilogue(BasicOutputArchive& ar, const std::nullptr_t&)
	{}

	template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae>
	inline void prologue(BasicOutputArchive& ar, const T&)
	{
		ar.writeName();
	}

	template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae>
	inline void epilogue(BasicOutputArchive& ar, const T&)
	{}

	template <class CharT, class Traits, class Alloc>
	inline void prologue(BasicOutputArchive& ar, const std::basic_string<CharT, Traits, Alloc>&)
	{
		ar.writeName();
	}

	template <class CharT, class Traits, class Alloc>
	inline void epilogue(BasicOutputArchive& ar, const std::basic_string<CharT, Traits, Alloc>&)
	{}

	template <class T> 
	inline void CEREAL_SAVE_FUNCTION_NAME(BasicOutputArchive & ar, const cereal::NameValuePair<T>& t)
	{
		ar.setNextName(t.name);
		ar(t.value);
	}

	inline void CEREAL_SAVE_FUNCTION_NAME(BasicOutputArchive & ar, const std::nullptr_t& t)
	{
		ar.saveValue(t);
	}

	template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = traits::sfinae>
	inline void CEREAL_SAVE_FUNCTION_NAME(BasicOutputArchive & ar, const T& t)
	{
		ar.saveValue(t);
	}

	template<class CharT, class Traits, class Alloc>
	inline void CEREAL_SAVE_FUNCTION_NAME(BasicOutputArchive & ar, const std::basic_string<CharT, Traits, Alloc>& str)
	{
		ar.saveValue(str);
	}

	template <class T>
	inline void CEREAL_SAVE_FUNCTION_NAME(BasicOutputArchive &, const cereal::SizeTag<T>&)
	{
	}
}

CEREAL_REGISTER_ARCHIVE(BasicOutputArchive)