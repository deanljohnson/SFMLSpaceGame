#include <Components/SoundSource.h>
#include <Entity.h>

void SoundSource::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_soundBuffer = LoadSoundBuffer(m_soundID);

	m_sound.setBuffer(*m_soundBuffer.get());
	m_sound.setMinDistance(.2f);
	m_sound.setAttenuation(.1f);
}

void SoundSource::Play()
{
	// Sound is spatialized in 3D by SFML
	// The y axis corresponds to the third dimension in
	// SFML's coordinates
	m_sound.setPosition(m_position->X(), 0.f, m_position->Y());
	m_sound.play();
}