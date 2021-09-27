#pragma once

#include "Xaloc/Renderer/IndexBuffer.h"

#include "DirectXContext.h"

#include <wrl.h>

namespace Xaloc {

	class DirectXIndexBuffer : public IndexBuffer
	{
	public:
		DirectXIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DirectXIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		DirectXContext* m_Context;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
		uint32_t m_Count;
	};

}