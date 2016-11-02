#pragma once
#include <string>

class EconomyID
{
public:
	EconomyID(const EconomyID& other)
		: m_id{other.m_id}
	{
	}

	EconomyID& operator=(const EconomyID& other)
	{
		if (this == &other)
			return *this;
		m_id = other.m_id;
		return *this;
	}
	
private:
	static std::string DEFAULT_ID;
	std::string m_id;

	EconomyID();
	explicit EconomyID(const std::string& id);
public:
	~EconomyID();
	static EconomyID Create();
	static EconomyID GetDefault();

	bool IsDefault() const;
	bool operator ==(const EconomyID& other) const;
	bool operator !=(const EconomyID& other) const;
	std::string GetStringID() const;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(m_id);
	}
};

namespace std
{
	template <>
	struct hash<EconomyID>
	{
		size_t operator()(const EconomyID& k) const
		{
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return hash<string>()(k.GetStringID());
		}
	};
}