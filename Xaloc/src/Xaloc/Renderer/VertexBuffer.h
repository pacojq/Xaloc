#pragma once

/*
	A vertex buffer is used to store the vertices of a mesh.
	They can be ordered using a given VertexBufferLayout.

	For example, a "hello-world" triangle would be like:


			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f,		0.8f, 0.0f, 0.2f, 1.0f,     // First vertex
				0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,     // Second vertex
				0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,     // Third vertex
			};

			Ref<Xaloc::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Xaloc::VertexBuffer::Create(vertices, sizeof(vertices)));

			Xaloc::VertexBufferLayout layout = {
				{ Xaloc::ShaderDataType::Float3, "a_Position" },
				{ Xaloc::ShaderDataType::Float4, "a_Color" }
			};
			vertexBuffer->SetLayout(layout);


	We define a layout in which, for each vertex, we have a Float3 indicating
	its position and a Float4 storing its color.

	After having the vertices ready, we can go and set up the indices in the IndexBuffer:


			uint32_t indices[3] = { 0, 1, 2 };
			Ref<Xaloc::IndexBuffer> indexBuffer;
			indexBuffer.reset(Xaloc::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));


	And finally we can put them both together inside the VertexArray:

			m_VertexArray.reset(Xaloc::VertexArray::Create());
			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);
*/
namespace Xaloc {




	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;


			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;

			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;

			case ShaderDataType::Bool:		return 1;
		}

		XA_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	struct VertexBufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		VertexBufferElement() {}

		VertexBufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{			
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;


			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;

			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;

			case ShaderDataType::Bool:		return 1;
			}

			XA_CORE_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
	};




	/*
		Determines how the data inside the VertexBuffer is structured.
		For each vertex, we will have one or many BufferElements.
	*/
	class VertexBufferLayout
	{
	public:

		VertexBufferLayout() {}

		VertexBufferLayout(const std::initializer_list<VertexBufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }

		std::vector<VertexBufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<VertexBufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<VertexBufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<VertexBufferElement> m_Elements;
		uint32_t m_Stride;
	};





	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual const VertexBufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};



}