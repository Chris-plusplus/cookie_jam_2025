#include "LayerMatrix.h"

namespace vs::coll {

consteval bool LayerMatrix::collides(u32 l1, u32 l2) {
	auto matrix = LayerMatrix::get();
	if (l1 > l2) { // layer matrix is sorted
		std::swap(l1, l2);
	}
	if (l1 < matrix.size() && l2 < matrix[l1].size()) {
		// backward's check
		return *(matrix[l1].rbegin() + l2 - l1);
	}
	return false;
}

consteval u32 LayerMatrix::layerCount() {
	return LayerMatrix::get().size();
}

}