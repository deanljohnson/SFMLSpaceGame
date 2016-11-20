#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class HardPointWidget : public sf::Drawable
{
public:
	enum class Type { Gun, MissileLauncher };
private:
	const float ARM_LENGTH = 20.f;
	std::unique_ptr<sf::Shape> m_posControl;
	std::unique_ptr<sf::Shape> m_angleControl;
	sf::VertexArray m_arm;

	Type m_type;

public:
	explicit HardPointWidget(const sf::Vector2f& position, Type type);
	HardPointWidget(HardPointWidget&& other)
		: m_posControl{std::move(other.m_posControl)},
		  m_angleControl{std::move(other.m_angleControl)},
		  m_arm{std::move(other.m_arm)},
		  m_type{std::move(other.m_type)}
	{
	}

	HardPointWidget& operator=(HardPointWidget&& other)
	{
		if (this == &other)
			return *this;
		m_posControl = std::move(other.m_posControl);
		m_angleControl = std::move(other.m_angleControl);
		m_arm = std::move(other.m_arm);
		m_type = std::move(other.m_type);
		return *this;
	}

	void Select();
	void UnSelect();
	bool PositionControlContains(float x, float y);
	bool AngleControlContains(float x, float y);

	void SetPositionControlLocation(float x, float y);
	void SetAngleControlLocation(float x, float y);

	sf::Vector2f GetHardPointLocation() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	float GetAngle() const;
	void SetAngle(float angle);

	Type GetType() const;
};