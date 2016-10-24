#include <Components/ItemPickup.h>
#include <Entity.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>

void ItemPickup::Init()
{
	m_physics = &entity->GetComponent<Physics>();
}

void ItemPickup::Update()
{
	auto body = m_physics->GetBody();
	if (body->GetContactList() == nullptr
		|| body->GetContactList()->other == nullptr)
		return;

	auto contact = body->GetContactList();
	while (true)
	{
		if (contact == nullptr)
			return;

		// If one of the fixtures is a nonsensor
		if (contact->contact->IsTouching()
			&& (!contact->contact->GetFixtureA()->IsSensor()
				|| !contact->contact->GetFixtureB()->IsSensor()))
		{
			auto userData = contact->other->GetUserData();
			if (userData != nullptr) // If contact is not with a body with defined entity data
			{
				b2WorldManifold manifold;
				contact->contact->GetWorldManifold(&manifold);

				auto otherEnt = static_cast<Entity*>(userData);

				for (auto& i : m_items)
				{
					Event itemEvent;
					itemEvent.itemPickup = Event::ItemPickupEvent();
					itemEvent.itemPickup.item = i;
					itemEvent.type = EventType::ItemPickup;
					otherEnt->events.Push(itemEvent);
				}

				entity->Destroy();
				return;
			}

			break;
		}

		contact = contact->next;
	}
}

void ItemPickup::AddItem(const Item& item)
{
	m_items.push_back(item);
}