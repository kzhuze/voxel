#pragma once

#include <cmath>
#include "Matrix.hpp"

namespace matrix {
	void translate(Matrix& mat, float vec[]) {
		for (int r=1; r<=mat.rows(); r++) {
			mat(r,mat.cols()) = vec[r-1];
		}
		return;
	}
	void scale(Matrix& mat, float vec[]) {
		if (mat.rows() == mat.cols()) {
			int c;
			for (int r=1; r<=mat.rows(); r++) {
				c = r;
				mat(r,c) *= vec[r-1];
			}
		}
		return;
	}
	Matrix rotation(float theta, float vec[]) {
	}
	Matrix projection(float fov, float ratio, float near, float far) {
	}
}
