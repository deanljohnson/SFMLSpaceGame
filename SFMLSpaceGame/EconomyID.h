#pragma once
#include <EconomyAgentType.h>

class EconomyID
{
public:
	EconomyID(const EconomyID& other)
		: ID{other.ID }
	{
	}

	EconomyID& operator=(const EconomyID& other)
	{
		if (this == &other)
			return *this;
		ID = other.ID;
		return *this;
	}
	
private:
	static long long DEFAULT_ID;

	EconomyID();
	explicit EconomyID(long long id, EconomyAgentType agentType);
public:
	static EconomyID Create(EconomyAgentType agentType = EconomyAgentType::None);
	static EconomyID GetDefault();

	long long ID;
	EconomyAgentType agentType;

	bool IsDefault() const;
	bool operator ==(const EconomyID& other) const;
	bool operator !=(const EconomyID& other) const;
	

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::make_nvp("ID", ID),
			cereal::make_nvp("type", EconomyAgentTypeToString[agentType]));
	}

	template<class Archive>
	void load(Archive& ar)
	{
		std::string agentTypeString;
		ar(cereal::make_nvp("ID", ID),
			cereal::make_nvp("type", agentTypeString));

		agentType = StringToEconomyAgentType[agentTypeString];
	}
};

namespace std
{
	template <>
	struct hash<EconomyID>
	{
		size_t operator()(const EconomyID& k) const
		{
			return hash<long long>()(k.ID);
		}
	};
}