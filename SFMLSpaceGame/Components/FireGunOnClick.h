#pragma once
#include <Components/Component.h>
#include <Components/GameWorldClickListener.h>
#include <Interfaces/MouseListener.h>
#include <Interfaces/Gun.h>
#include <vector>

template<class... TGunTypes>
class FireGunOnClick : public Component, MouseListener
{
private:
	std::vector<Gun*> m_guns;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), button);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<FireGunOnClick<TGunTypes...>>& construct)
	{
		EntityID selfID;
		sf::Mouse::Button button;
		ar(selfID, button);
		construct(selfID, button);
	}

	template<class T>
	void AddGunsOfTypes()
	{
		static_assert(std::is_base_of<Gun, T>::value,
			"T must inherit from Gun");

		m_guns.push_back(static_cast<Gun*>(&entity->GetComponent<T>()));
	}

	template<class TGunA, class TGunB, class... TGuns>
	void AddGunsOfTypes()
	{
		AddGunsOfTypes<TGunA>();
		AddGunsOfTypes<TGunB, TGuns...>();
	}
public:
	explicit FireGunOnClick(EntityID ent, sf::Mouse::Button btn = sf::Mouse::Button::Left)
		: Component(ent)
	{
		button = btn;

		AddGunsOfTypes<TGunTypes...>();

		if (entity->HasComponent<GameWorldClickListener>())
		{
			auto& gwcl = entity->GetComponent<GameWorldClickListener>();
			gwcl.AddClickListener(this);
			gwcl.AddHeldListener(this);
		}
	}

	virtual void OnClick(b2Vec2 pos) override{};
	virtual void OnHeld(b2Vec2 pos) override
	{
		for (auto g : m_guns)
		{
			g->Shoot();
		}
	};
};