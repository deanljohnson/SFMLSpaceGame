#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>

class AnimationDefinition
{
private:
	std::string m_texId;
	std::shared_ptr<sf::Texture> m_texture;
	std::vector<sf::IntRect> m_frames;
	// stores an additional rect per animation
	// generally used for colliders
	std::vector<sf::IntRect> m_contentRects; 
	float m_length;

	friend class cereal::access;

	template <class Archive>
	void save(Archive& ar) const
	{
		ar(cereal::make_nvp("TexLoc", m_texId));

		// Find out if all the frames 
		// have the same width/height
		// Technically this does not guarentee
		// that all frames are consistent, but 
		// it should be adequate for our use-case
		bool isConsistent = true;
		if (!m_frames.empty())
		{
			for (size_t i = 0; i < m_frames.size(); i++)
			{
				if (m_frames[i].width != m_frames[0].width
					|| m_frames[i].height != m_frames[0].height
					|| m_frames[i].width != m_contentRects[i].width
					|| m_frames[i].height != m_contentRects[i].height)
				{
					isConsistent = false;
					break;
				}
			}
		}

		ar(cereal::make_nvp("IsConsistent", isConsistent));

		if (isConsistent)
		{
			auto frameToSave = m_frames.empty()
							? sf::IntRect{ 0, 0, 0, 0 }
							: m_frames[0];

			ar(cereal::make_nvp("ConsistentFrame", frameToSave),
				cereal::make_nvp("Count", m_frames.size()));
		}
		else
		{
			ar(cereal::make_nvp("Frames", m_frames),
				cereal::make_nvp("ContentRects", m_contentRects));
		}

		ar(cereal::make_nvp("Length", m_length));
	}

	template <class Archive>
	void load(Archive& ar)
	{
		ar(cereal::make_nvp("TexLoc", m_texId));
		m_texture = LoadTexture(m_texId);

		bool isConsistent = false;
		ar(cereal::make_nvp("IsConsistent", isConsistent));

		if (isConsistent)
		{
			sf::IntRect consistentFrame;
			size_t count;
			ar(cereal::make_nvp("ConsistentFrame", consistentFrame), 
				cereal::make_nvp("Count", count));

			m_frames.resize(count);
			m_contentRects.resize(count);

			int numAcross = m_texture->getSize().x / consistentFrame.width;
			// Replicate the consistent frame
			for (int i = 0; i < count; i++)
			{
				sf::IntRect frame = { consistentFrame.width * (i % numAcross),
								consistentFrame.height * (i / numAcross),
								consistentFrame.width, consistentFrame.height };
				m_frames[i] = frame;
				m_contentRects[i] = frame;
			}
		}
		else
		{
			ar(cereal::make_nvp("Frames", m_frames),
				cereal::make_nvp("ContentRects", m_contentRects));
		}

		ar(cereal::make_nvp("Length", m_length));
	}

public:
	AnimationDefinition();
	AnimationDefinition(const std::string& id, const sf::Vector2f& frameSize, float length, int numFrames = 0);

	int FrameCount() const;

	std::shared_ptr<sf::Texture> GetTexture() const;
	float GetLength() const;
	sf::IntRect GetFrame(size_t index) const;

	sf::IntRect GetContentRect(size_t index) const;
	void SetContentRect(size_t index, const sf::IntRect& rect);

	static std::string GetTypeName();
};