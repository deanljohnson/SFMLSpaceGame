#pragma once

struct SpriteKey
{
	SpriteKey();
	explicit SpriteKey(const std::string& tex);
	SpriteKey(const std::string& tex, unsigned short index);

	enum class Type : unsigned char { Invalid, Texture, TexIndex } type;
	unsigned short index;
	std::string texID;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(type, texID, index);
	}
};