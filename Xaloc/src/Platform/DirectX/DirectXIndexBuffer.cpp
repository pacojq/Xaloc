#include "xapch.h"
#include "DirectXIndexBuffer.h"

#include "Xaloc/Core/Application.h"

#include "glm/glm.hpp"

namespace Xaloc {

	DirectXIndexBuffer::DirectXIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		m_Context = (DirectXContext*)Application::Get().GetWindow().GetContext();

		D3D11_BUFFER_DESC bDesc;
		ZeroMemory(&bDesc, sizeof(bDesc));

		bDesc.ByteWidth = sizeof(uint32_t) * count;
		bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bDesc.CPUAccessFlags = 0;
		bDesc.MiscFlags = 0;
		bDesc.StructureByteStride = 0;
		bDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA mData;
		ZeroMemory(&mData, sizeof(mData));

		mData.pSysMem = indices;

		m_Context->GetDevice()->CreateBuffer(&bDesc, &mData, m_IndexBuffer.GetAddressOf());
	}

	DirectXIndexBuffer::~DirectXIndexBuffer()
	{
	}

	void DirectXIndexBuffer::Bind() const
	{
		m_Context->GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectXIndexBuffer::Unbind() const
	{
		m_Context->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}
	
}