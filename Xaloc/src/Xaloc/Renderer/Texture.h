#pragma once

#include <string>
#include "Xaloc/Core/Core.h"

namespace Xaloc {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		
		virtual uint32_t GetRendererID() const = 0;

		// Give a pointer to a memory address and upload
		// stuff to the GPU.
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};



	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
		static Ref<Texture2D> Create(const std::string& path);

		virtual const std::string& AssetID() const { return m_AssetID; }

	private:
		std::string m_AssetID = "";

		friend class AssetManager;
	};
	
}