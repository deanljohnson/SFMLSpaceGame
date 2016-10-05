#include <UI/UIExpandingBackground.h>

UIExpandingBackground::UIExpandingBackground(ResourceID texID, sf::Vector2i size, UITransform trans)
{
	m_tex = LoadTextureResource(texID);

	m_transform.translate(trans.position);
	m_transform.scale(trans.scale);

	int cornerWidth = m_tex->getSize().x / 3;
	int cornerHeight = m_tex->getSize().y / 3;
	int topWidth = size.x - (cornerWidth * 2);
	int sideHeight = size.y - (cornerHeight * 2);

	m_vertArray = sf::VertexArray(sf::PrimitiveType::Quads, 4 * 9);

	// Top-left
	m_vertArray[0].position = sf::Vector2f(0, 0);
	m_vertArray[1].position = sf::Vector2f(cornerWidth, 0);
	m_vertArray[2].position = sf::Vector2f(cornerWidth, cornerHeight);
	m_vertArray[3].position = sf::Vector2f(0, cornerHeight);

	m_vertArray[0].texCoords = sf::Vector2f(0, 0);
	m_vertArray[1].texCoords = sf::Vector2f(cornerWidth, 0);
	m_vertArray[2].texCoords = sf::Vector2f(cornerWidth, cornerHeight);
	m_vertArray[3].texCoords = sf::Vector2f(0, cornerHeight);

	// Top-middle
	m_vertArray[4].position = sf::Vector2f(cornerWidth, 0);
	m_vertArray[5].position = sf::Vector2f(cornerWidth + topWidth, 0);
	m_vertArray[6].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight);
	m_vertArray[7].position = sf::Vector2f(cornerWidth, cornerHeight);

	m_vertArray[4].texCoords = sf::Vector2f(cornerWidth, 0);
	m_vertArray[5].texCoords = sf::Vector2f(cornerWidth * 2, 0);
	m_vertArray[6].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight);
	m_vertArray[7].texCoords = sf::Vector2f(cornerWidth, cornerHeight);

	// Top-right
	m_vertArray[8].position = sf::Vector2f(cornerWidth + topWidth, 0);
	m_vertArray[9].position = sf::Vector2f(size.x, 0);
	m_vertArray[10].position = sf::Vector2f(size.x, cornerHeight);
	m_vertArray[11].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight);

	m_vertArray[8].texCoords = sf::Vector2f(cornerWidth * 2, 0);
	m_vertArray[9].texCoords = sf::Vector2f(cornerWidth * 3, 0);
	m_vertArray[10].texCoords = sf::Vector2f(cornerWidth * 3, cornerHeight);
	m_vertArray[11].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight);

	// Left
	m_vertArray[12].position = sf::Vector2f(0, cornerHeight);
	m_vertArray[13].position = sf::Vector2f(cornerWidth, cornerHeight);
	m_vertArray[14].position = sf::Vector2f(cornerWidth, cornerHeight + sideHeight);
	m_vertArray[15].position = sf::Vector2f(0, cornerHeight + sideHeight);

	m_vertArray[12].texCoords = sf::Vector2f(0, cornerHeight);
	m_vertArray[13].texCoords = sf::Vector2f(cornerWidth, cornerHeight);
	m_vertArray[14].texCoords = sf::Vector2f(cornerWidth, cornerHeight * 2);
	m_vertArray[15].texCoords = sf::Vector2f(0, cornerHeight * 2);

	// Middle
	m_vertArray[16].position = sf::Vector2f(cornerWidth, cornerHeight);
	m_vertArray[17].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight);
	m_vertArray[18].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight + sideHeight);
	m_vertArray[19].position = sf::Vector2f(cornerWidth, cornerHeight + sideHeight);

	m_vertArray[16].texCoords = sf::Vector2f(cornerWidth, cornerHeight);
	m_vertArray[17].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight);
	m_vertArray[18].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight * 2);
	m_vertArray[19].texCoords = sf::Vector2f(cornerWidth, cornerHeight * 2);

	// Right
	m_vertArray[20].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight);
	m_vertArray[21].position = sf::Vector2f(size.x, cornerHeight);
	m_vertArray[22].position = sf::Vector2f(size.x, cornerHeight + sideHeight);
	m_vertArray[23].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight + sideHeight);

	m_vertArray[20].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight);
	m_vertArray[21].texCoords = sf::Vector2f(cornerWidth * 3, cornerHeight);
	m_vertArray[22].texCoords = sf::Vector2f(cornerWidth * 3, cornerHeight * 2);
	m_vertArray[23].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight * 2);

	// Bottom-left
	m_vertArray[24].position = sf::Vector2f(0, cornerHeight + sideHeight);
	m_vertArray[25].position = sf::Vector2f(cornerWidth, cornerHeight + sideHeight);
	m_vertArray[26].position = sf::Vector2f(cornerWidth, size.y);
	m_vertArray[27].position = sf::Vector2f(0, size.y);

	m_vertArray[24].texCoords = sf::Vector2f(0, cornerHeight * 2);
	m_vertArray[25].texCoords = sf::Vector2f(cornerWidth, cornerHeight * 2);
	m_vertArray[26].texCoords = sf::Vector2f(cornerWidth, cornerHeight * 3);
	m_vertArray[27].texCoords = sf::Vector2f(0, cornerHeight * 3);

	// Bottom-middle
	m_vertArray[28].position = sf::Vector2f(cornerWidth, cornerHeight + sideHeight);
	m_vertArray[29].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight + sideHeight);
	m_vertArray[30].position = sf::Vector2f(cornerWidth + topWidth, size.y);
	m_vertArray[31].position = sf::Vector2f(cornerWidth, size.y);

	m_vertArray[28].texCoords = sf::Vector2f(cornerWidth, cornerHeight * 2);
	m_vertArray[29].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight * 2);
	m_vertArray[30].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight * 3);
	m_vertArray[31].texCoords = sf::Vector2f(cornerWidth, cornerHeight * 3);

	// Bottom-right
	m_vertArray[32].position = sf::Vector2f(cornerWidth + topWidth, cornerHeight + sideHeight);
	m_vertArray[33].position = sf::Vector2f(size.x, cornerHeight + sideHeight);
	m_vertArray[34].position = sf::Vector2f(size.x, size.y);
	m_vertArray[35].position = sf::Vector2f(cornerWidth + topWidth, size.y);

	m_vertArray[32].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight * 2);
	m_vertArray[33].texCoords = sf::Vector2f(cornerWidth * 3, cornerHeight * 2);
	m_vertArray[34].texCoords = sf::Vector2f(cornerWidth * 3, cornerHeight * 3);
	m_vertArray[35].texCoords = sf::Vector2f(cornerWidth * 2, cornerHeight * 3);
}

void UIExpandingBackground::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.texture = m_tex.get();
	states.transform *= m_transform;
	target.draw(m_vertArray, states);
}
