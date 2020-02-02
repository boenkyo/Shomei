#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <vector>

namespace sm {

enum class Dimension {
   Zero,
	One,
	Two,
	Three,
	Four,
};

struct Attribute {
	std::size_t index       = 0;
	std::size_t stride      = 0;
	std::size_t offset      = 0;
	sm::Dimension dimension = sm::Dimension::Zero;
};

template <typename T>
struct AttributePtr {
	T* data;
	sm::Dimension dim;
	std::size_t index;
};

template <typename T>
class VertexBuffer {
public:
	VertexBuffer(T vertices[], std::size_t size);

	void set_data(T vertices[], std::size_t size);
	void def_attribute(const sm::Attribute& attribute);

   T* data();
	sm::AttributePtr<T> attribute_ptr(std::size_t index, std::size_t n);
	const std::vector<std::size_t>& vertex_indices() const;

private:
	static const std::size_t MAX_ATTRIBUTES = 16;

	std::vector<T> m_data;
	std::vector<std::size_t> m_vertex_indices;
	std::array<sm::Attribute, MAX_ATTRIBUTES> m_attributes;

	void calc_vertex_indices();
};

template <typename T>
T* VertexBuffer<T>::data() {
   return m_data.data();
}

template <typename T>
VertexBuffer<T>::VertexBuffer(T vertices[], std::size_t size)
    : m_data(&vertices[0], &vertices[size]) {}

template <typename T>
void VertexBuffer<T>::set_data(T vertices[], std::size_t size) {
	std::copy(&vertices[0], &vertices[size], m_data.begin());
}

template <typename T>
void VertexBuffer<T>::def_attribute(const sm::Attribute& attribute) {
	assert(attribute.index < MAX_ATTRIBUTES);
   assert(attribute.dimension != sm::Dimension::Zero);

	m_attributes[attribute.index] = attribute;
	calc_vertex_indices();
}

template <typename T>
sm::AttributePtr<T> VertexBuffer<T>::attribute_ptr(std::size_t index,
                                                   std::size_t n) {
	sm::Attribute& attribute    = m_attributes[index];
	std::size_t attribute_begin = attribute.offset + attribute.stride * n;
	T* ptr                      = &m_data[attribute_begin];

	return {ptr, attribute.dimension, attribute_begin};
}

template <typename T>
const std::vector<std::size_t>& VertexBuffer<T>::vertex_indices() const {
	return m_vertex_indices;
}

template <typename T>
void VertexBuffer<T>::calc_vertex_indices() {
	std::size_t vertex_size =
	   std::accumulate(m_attributes.begin(),
	                   m_attributes.end(),
	                   std::size_t(0),
	                   [](std::size_t sum, sm::Attribute& a) {
		                   return sum + std::size_t(a.dimension);
	                   });

	m_vertex_indices.resize(m_data.size() / vertex_size);
	std::iota(m_vertex_indices.begin(), m_vertex_indices.end(), 0);

	// int n = -vertex_size;
	// std::generate(m_vertex_indices.begin(), m_vertex_indices.end(), [&n] {
	// 	return std::size_t(n += vertex_size);
	// });
}

}  // namespace sm