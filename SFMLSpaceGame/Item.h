#pragma once
#include <string>
#include <cereal\access.hpp>

#ifndef NAMED_ITEM
#define NAMED_ITEM(s) public: static std::string GetTypeName() { static const std::string name = s; return name; }
#endif

enum class ItemType
{
	Credits, FuelCells, Ore, Food, Narcotics, LaserRig, MissileRig
};

extern std::unordered_map<ItemType, std::string> ItemTypeToString;
extern std::unordered_map<std::string, ItemType> StringToItemType;

class Item
{
private:
	friend class cereal::access;

	template<class Archive>
	void load(Archive& ar) 
	{
		std::string itemString;
		ar(cereal::make_nvp("type", itemString),
			CEREAL_NVP(amount));

		type = StringToItemType[itemString];
	}

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::make_nvp("type", ItemTypeToString[type]),
			CEREAL_NVP(amount));
	}

public:
	virtual ~Item()
	{
	}

	const static std::string NO_DETAIL;

	ItemType type;
	unsigned int amount;

	Item();
	explicit Item(ItemType _type, unsigned int _amt = 0);

	void Stack(const Item& other);

	bool AreStackable(const Item& other);
	bool IsDetailed() const;
	bool IsEquippable() const;

	std::string GetTypeName() const;
	virtual std::string GetDisplayString() const;
	virtual const std::string& GetDetail() const;
};

class CreditsItem : public Item
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this));
	}

public:
	NAMED_ITEM("Credits");

	explicit CreditsItem(unsigned int amt = 0) : Item(ItemType::Credits, amt) { }
};

class FuelCellsItem : public Item
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this));
	}

public:
	NAMED_ITEM("FuelCells");

	explicit FuelCellsItem(unsigned int amt = 0) : Item(ItemType::FuelCells, amt) { }
};

class OreItem : public Item
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this));
	}

public:
	NAMED_ITEM("Ore");

	explicit OreItem(unsigned int amt = 0) : Item(ItemType::Ore, amt) { }
};

class FoodItem : public Item
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this));
	}

public:
	NAMED_ITEM("Food");

	explicit FoodItem(unsigned int amt = 0) : Item(ItemType::Food, amt) { }
};

class NarcoticsItem : public Item
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this));
	}

public:
	NAMED_ITEM("Narcotics");

	explicit NarcoticsItem(unsigned int amt = 0) : Item(ItemType::Narcotics, amt) { }
};

class LaserRigItem : public Item
{
protected:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this), 
			cereal::make_nvp("rigName", rigName));
	}

public:
	NAMED_ITEM("LaserRig");

	std::string rigName;

	explicit LaserRigItem(unsigned int amt = 0, const std::string& _rigName = "") 
		: Item(ItemType::LaserRig, amt),
		  rigName(_rigName)
	{ }

	virtual std::string GetDisplayString() const override;
	virtual const std::string& GetDetail() const override;
};

class MissileRigItem : public Item
{
protected:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Item>(this),
			cereal::make_nvp("rigName", rigName));
	}

public:
	NAMED_ITEM("MissileRig");

	std::string rigName;

	explicit MissileRigItem(unsigned int amt = 0, const std::string& _rigName = "")
		: Item(ItemType::MissileRig, amt),
		rigName(_rigName)
	{ }

	virtual std::string GetDisplayString() const override;
	virtual const std::string& GetDetail() const override;
};

// Disambiguates serialization functions
namespace cereal
{
	template <class Archive>
	struct specialize<Archive, Item, cereal::specialization::member_load_save> {};
	template <class Archive>
	struct specialize<Archive, CreditsItem, cereal::specialization::member_serialize> {};
	template <class Archive>
	struct specialize<Archive, FoodItem, cereal::specialization::member_serialize> {};
	template <class Archive>
	struct specialize<Archive, OreItem, cereal::specialization::member_serialize> {};
	template <class Archive>
	struct specialize<Archive, FuelCellsItem, cereal::specialization::member_serialize> {};
	template <class Archive>
	struct specialize<Archive, NarcoticsItem, cereal::specialization::member_serialize> {};
	template <class Archive>
	struct specialize<Archive, LaserRigItem, cereal::specialization::member_serialize> {};
	template <class Archive>
	struct specialize<Archive, MissileRigItem, cereal::specialization::member_serialize> {};
}