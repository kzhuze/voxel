#include <stdexcept>
#include <iostream>

class Matrix {
	private:
		float *values;
		int rows;
		int cols;
	
	public:
		Matrix(int R, int C, int val) {
			values = new float[R * C];
			rows = R;
			cols = C;
			for (int r=0; r<rows; r++) {
				for (int c=0; c<cols; c++) {
					if (r == c) {
						values[r*rows+c] = 1;
					} else {
						values[r*rows+c] = 0;
					}
				}
			}
		}
		~Matrix() {
			delete [] values;
		}
		float& operator()(int r, int c) {
			if (1 <= r && r <= rows) {
				if (1 <= c && c <= cols) {
					return values[(r-1)*rows+(c-1)];
				}
			}
			throw std::out_of_range("Matrix indices out of range");
		}
		Matrix operator+(Matrix& rhs) {
			if (this->rows==rhs.rows && this->cols==rhs.cols) {	
				Matrix result(this->rows, this->cols);
				for (int r=1; r<=this->rows; r++) {
					for (int c=1; c<=this->cols; c++) {
						result(r,c)=(*this)(r,c)+rhs(r,c);
					}
				}
				return result;
			}
			throw std::domain_error("Matrix dimensions incompatible");
		}
		Matrix operator*(Matrix& rhs) {
			if (this->cols==rhs.rows) {
				Matrix result(this->rows, rhs.cols);
				for (int r=1; r<=this->rows; r++) {
					for (int c=1; c<=rhs.cols; c++) {
						for (int k=1; k<=this->cols; k++) {
							result(r,c) += (*this)(r,k)*rhs(k,c);
						}
					}
				}
				return result;
			}
			throw std::domain_error("Matrix dimensions incompatible");
		}
		friend std::ostream& operator<<(std::ostream& os, Matrix& mat) {
			for (int r=0; r<mat.rows; r++) {
				for (int c=0; c<mat.cols; c++) {
					os << mat.values[r*mat.rows+c] << " ";
				}
				os << "\n";
			}
			return os;
		}
};

int main() {
	Matrix mat1(2,2);
	Matrix mat2(2,2);
	Matrix mat3 = mat1 + mat2;
	Matrix mat4 = mat3 * mat3;
	std::cout << mat3 << mat4;
	return 0;
}
