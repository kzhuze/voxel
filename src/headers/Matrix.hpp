#pragma once

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace mtx {
    
    enum format { COL, ROW };
    format fmt;

    // vector
    template<typename T, int N>
    class vector {
        private:
            T m_values[N];
        public:
            vector();
            vector(double, ...);
            T& operator[](unsigned);
            vector unit();
    };

    template<typename T, int N>
    vector<T, N>::vector() : m_values() {}

    template<typename T, int N>
    vector<T, N>::vector(double val, ...) {
        va_list args;
        va_start(args, val);
        int n_arg = N;
        int idx = 0;
        m_values[idx] = val;
        while (--n_arg) {   
            m_values[++idx] = va_arg(args, double);
        }
        va_end(args);
    }
    template<typename T, int N>
    T& vector<T, N>::operator[](unsigned idx) {
        if (0 <= idx && idx < N) {
            return m_values[idx];
        }
        throw std::out_of_range("Vector index out of range");
    }
    template<typename T, int N>
    vector<T, N> vector<T, N>::unit() {
        T mag = 0.0;
        for (T &val : m_values) {
            mag += val*val;
        }
        mag=sqrt(mag);
        vector<T, N> result;
        for (int i=0; i<N; i++) {
            result.m_values[i]=m_values[i]/mag;
        }
        return result;
    }

    // matrix
    template<typename T, int N>
    class matrix {
        private:
            vector<T, N> m_values[N];
        public:
            matrix();
            matrix(double, ...);
            T* value_ptr();
            T& operator()(unsigned, unsigned);
            matrix operator+(matrix&);
            matrix operator*(matrix&);
    };
    
    template<typename T, int N>
    matrix<T, N>::matrix() : m_values() {}
    
    template<typename T, int N>
    matrix<T, N>::matrix(double val, ...) : m_values() {
        va_list args;
        va_start(args, val);
        m_values[0][0] = val;
        int *i,*j,r,c;
        if (fmt == COL) {
            i=&c,j=&r;
        }
        else
        if (fmt == ROW) {
            i=&r,j=&c;
        }
        for (r=0; r<N; r++) {
            for (c=0; c<N; c++) {
                if (!(r==0 && c==0)) {
                    m_values[*i][*j]=va_arg(args, double);
                }
            }
        }
        va_end(args);
    }
    template<typename T, int N>
    T* matrix<T, N>::value_ptr() {
        return (T*)((void*)m_values);
    }
    template<typename T, int N>
    T& matrix<T, N>::operator()(unsigned r, unsigned c) {
        if (0 <= r && r < N) {
            if (0 <= c && c < N) {
                int i, k;
                if (fmt == COL) {
                    i=c, k=r;
                }
                else
                if (fmt == ROW) {
                    i=r, k=c;
                }
                return m_values[i][k];
            }
        }
        throw std::out_of_range("Matrix index out of range");
    }
    template<typename T, int N>
    matrix<T, N> matrix<T, N>::operator+(matrix& rhs) {
        matrix<T, N> result;
        for (int r=0; r<N; r++) {
            for (int c=0; c<N; c++) {
                result(r,c)=(*this)(r,c)+rhs(r,c);
            }
        }
        return result;
    }
    template<typename T, int N>
    matrix<T, N> matrix<T, N>::operator*(matrix& rhs) {
        matrix<T, N> result;
        for (int r=0; r<N; r++) {
            for (int c=0; c<N; c++) {
                for (int k=0; k<N; k++) {
                    result(r,c) += (*this)(r,k)*rhs(k,c);
                }
            }
        }
        return result;
    }
    template<typename T, int N>
    matrix<T, N> operator*(T lhs, matrix<T, N>& rhs) {
        matrix<T, N> result;
        for (int r=0; r<N; r++) {
            for (int c=0; c<N; c++) {
                result(r,c)=lhs*rhs(r,c);
            }
        }
        return result;
    }

    // transformations
    typedef vector<float, 3> vec3;
    typedef matrix<float, 4> mat4;
    
    template<typename T>
    T radians(T deg) {
        return (deg/180.0)*3.1415926;
    }

    template<typename T, int N>
    void identity(matrix<T, N>& mat) {
        for (int i=0; i<N; i++) {
            mat(i,i)=1.0;
        }
    }
    void translate(mat4& mat, vec3 vec) {
        for (int r=0; r<3; r++) {
            mat(r,3) += vec[r];
        }
    }

    void scale(mat4& mat, vec3 vec) {
        for (int i=0; i<3; i++) {
            mat(i,i) *= vec[i];
        }
    }
    void rotate(mat4& mat, float t, vec3 u) {
        mat4 I;
        identity(I);
        scale(I, vec3(cos(t),cos(t),cos(t)));
        mat4 cross(0.0, -u[2]+0.0,      u[1], 0.0,
                  u[2],       0.0, -u[0]+0.0, 0.0, 
             -u[1]+0.0,      u[0],       0.0, 0.0,
                   0.0,       0.0,       0.0, 0.0);
        cross=sin(t)*cross;
        mat4 outer(u[0]*u[0], u[0]*u[1], u[0]*u[2], 0.0,
                   u[1]*u[0], u[1]*u[1], u[1]*u[2], 0.0,
                   u[2]*u[0], u[2]*u[1], u[2]*u[2], 0.0,
                         0.0,       0.0,       0.0, 0.0);
        outer=(1-cos(t))*outer;
        mat4 rotate = I+cross+outer;
        mat=rotate*mat;
    }
    mat4 perspective(float fov, float ratio, float n, float f) {
        float r = n*tan(fov/2);
        float t = ratio * r;
        return mat4(  n/r,   0.0,            0.0,             0.0,
                      0.0,   n/t,            0.0,             0.0,
                      0.0,   0.0,   -(f+n)/(f-n),  -2*(f*n)/(f-n),
                      0.0,   0.0,           -1.0,             0.0);
    }
}

// printing
template<typename T, int N>
std::ostream& operator<<(std::ostream& os, mtx::vector<T, N>& rhs) {
    unsigned decimal = 3;
    for (int i=0; i<N; i++) {
        os << " ";
        os << std::fixed;
        os << std::setprecision(decimal);
        os << rhs[i]; 
    }   
    os << std::endl;
    return os;
}


template<typename T, int N>
std::ostream& operator<<(std::ostream& os, mtx::matrix<T, N>& rhs) {
    T* ptr = rhs.value_ptr();
    T max_value = *std::max_element(ptr, ptr+N*N);
    unsigned decimal = 3;
    unsigned spacing = ((unsigned)abs(log10(abs(max_value)))+1)+(decimal+2)+1;
    for (int r=0; r<N; r++) {
        for (int c=0; c<N; c++) {
            os << std::setw(spacing);
            os << std::fixed;
            os << std::setprecision(decimal);
            os << rhs(r,c);
        }
        os << std::endl;
    }
    return os;
}
