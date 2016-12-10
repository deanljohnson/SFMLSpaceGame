#pragma once

struct SpriteKey
{
	SpriteKey();
	explicit SpriteKey(const std::string& tex);
	SpriteKey(const std::string& tex, unsigned short index);
	SpriteKey(const std::string& tex, const std::string& key);

	enum class Type : unsigned char { Invalid, Texture, TexIndex, TexKey } type;
	unsigned short index;
	std::string texKey;
	std::string texID;

	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(type), CEREAL_NVP(texID));
		switch(type)
		{
		case Type::TexIndex:
			ar(CEREAL_NVP(index));
			break;
		case Type::TexKey:
			ar(CEREAL_NVP(texKey));
			break;
		}
	}
};