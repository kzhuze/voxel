#pragma once

#include <stdexcept>
#include <iostream>

class Matrix {
    private:
        float *values;
        int rows;
        int cols;
	
    public:
        Matrix(int _rows, int _cols) {
            rows = _rows;
            cols = _cols;
            values = new float[rows * cols]();
		}
		Matrix(int n, float val) : Matrix(n, n) {
			int c;
			for (int r=0; r<rows; r++) {
				c = r;
				values[r*rows+c] = val;
			}
        }
		Matrix(int m, int n, float _values[]) : Matrix(m, n) {
			values = _values;
		}
        ~Matrix() {
            delete [] values;
        }
		int rows() const {
			return rows;
		}
		int cols() const {
			return cols;
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
                Matrix result(this->rows, this->cols, 0);
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
                Matrix result(this->rows, rhs.cols, 0);
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
        friend std::ostream& operator<<(std::ostream& os, Matrix& rhs) {
            for (int r=1; r<=rhs.rows; r++) {
                for (int c=1; c<=rhs.cols; c++) {
                    os << rhs(r,c) << " ";
                }
                os << "\n";
            }
            return os;
        }
};
