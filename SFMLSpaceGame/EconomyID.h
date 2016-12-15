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
	static std::string DEFAULT_ID;

	EconomyID();
	explicit EconomyID(const std::string& id, EconomyAgentType agentType);
public:
	~EconomyID();
	static EconomyID Create(EconomyAgentType agentType = EconomyAgentType::None);
	static EconomyID GetDefault();

	std::string ID;
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
			// Compute individual hash values for two data members and combine them using XOR and bit shifting
			return hash<string>()(k.ID);
		}
	};
}