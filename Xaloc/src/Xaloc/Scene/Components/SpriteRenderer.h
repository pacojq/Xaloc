#pragma once

#include "Xaloc/Renderer/SubTexture2D.h"
#include "Xaloc/Scene/Component.h"

namespace Xaloc {

	class SpriteRenderer : public Component
	{
	public:
		//SpriteRenderer(const std::string& filename);
		SpriteRenderer(const Ref<Texture2D>& texture);
		SpriteRenderer(const Ref<SubTexture2D>& subTexture);
		
		void OnUpdate(Timestep ts) override;

		void SetDepth(float depth) { m_Depth = depth; }
		float GetDepth() { return m_Depth; }

	private:
		Ref<SubTexture2D> m_SubTexture;
		float m_Depth = 0;

	};

}