#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <algorithm>

template <typename T>
class MatrixBase {
public:
	MatrixBase()
	    : x(&m_data[0])
	    , y(&m_data[1])
	    , z(&m_data[2]) {
          std::iota(m_data.begin(), m_data.end(), 0);
       }

	T* x;
   T* y;
   T* z;

private:
	std::array<T, 3> m_data;
};