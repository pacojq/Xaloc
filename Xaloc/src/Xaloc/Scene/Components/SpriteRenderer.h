#pragma once

#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Scene/Component.h"

namespace Xaloc {

	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer(const std::string& filename);
		SpriteRenderer(Ref<Texture2D>& texture);
		
		void OnUpdate(Timestep ts) override;


	private:
		Ref<Texture2D> m_Texture;

	};

}