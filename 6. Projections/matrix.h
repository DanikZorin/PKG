#pragma once
#include "matrix_base.h"
template<int N, int M = N, typename T = double>
class matrix : public matrix_base<N, M, T> {
private:
public:

	matrix();
	matrix(const T&);
    matrix(const matrix_base<N, M, T>&);
    matrix(const std::vector <std::vector <T>>&);

};

template<int N, typename T>
class matrix<N, N, T> : public matrix_base<N, N, T> {
private:
public:
	matrix();
	matrix(const T&);
    matrix(const matrix_base<N, N, T>&);
    matrix(const std::vector <std::vector <T>>&);
    matrix transposed() const;
	T det() const;
	matrix inverted() const;
	matrix& invert();
    T trace() const;
};

template<int N, typename T = double>
using SquareMatrix = matrix<N, N, T>;

template <int N, int M, typename T>
matrix<N, M, T> operator -(const matrix<N, M, T>& a);
template <int N, int M, typename T>
matrix<N, M, T> operator + (const matrix<N, M, T>& a, const matrix<N, M, T>& b);
template<int N, int M, int K, typename T>
matrix<N, K, T> operator * (const matrix<N, M, T>& a, const matrix<M, K, T>& b);
template<int N, int M, typename T>
matrix<N, M, T> operator * (const matrix<N, M, T>& a, T b);

template<int N, int M, typename T>
matrix<N, M, T>& operator += (matrix<N, M, T>& a, const matrix<N, M, T>& b);
template<int N, int M, typename T>
matrix<N, M, T>& operator *= (matrix<N, M, T>& a, const matrix<M, M, T>& b);
template<int N, int M, typename T>
matrix<N, M, T>& operator *= (matrix<N, M, T>& a, T b);
template<int N, int M, typename T>
matrix<N, M, T> operator - (const matrix<N, M, T>& a, const matrix<N, M, T>& b);
template<int N, int M, typename T>
matrix<N, M, T>& operator -= (matrix<N, M, T>& a, const matrix<N, M, T>& b);
template<int N, int M, typename T>
matrix<N, M, T> operator / (const matrix<N, M, T>& a, T b);
template<int N, int M, typename T>
matrix<N, M, T> operator /= (matrix<N, M, T>& a, T b);

template<int N, int M, typename T>
bool operator == (const matrix<N, M, T>& a, const matrix<N, M, T>& b);
template<int N, int M, typename T>
bool operator != (const matrix<N, M, T>& a, const matrix<N, M, T>& b);

template<int N, int M, typename T>
std::istream& operator >>(std::istream& stream, matrix<N, M, T>& a);
template<int N, int M, typename T>

std::ostream& operator <<(std::ostream& stream, const matrix<N, M, T>& a);

template<int N, typename T>
matrix<N, N, T> getX__AX_equal_B(const matrix<N, N, T>& B, const matrix<N, N, T>& C);


//----------------------------------------------------------


template<int N,int M, typename T>
matrix<N, M, T>::matrix() {}

template<int N,int M, typename T>
matrix<N, M, T>::matrix(const T& val) {
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			(*this)[i][j] = val;
		}
	}
}
template<int N, int M, typename T>
matrix<N, M, T>::matrix(const matrix_base<N, M, T>& other) : matrix_base<N, M, T>(other) {}

template<int N, int M, typename T>
matrix<N, M, T>::matrix(const std::vector <std::vector <T>>& other) : matrix_base<N, M, T>(other) {}

template<int N,typename T>
matrix<N, N, T>::matrix(const T& val) {
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
			(*this)[i][j] = val;
		}
	}
}
template<int N, typename T>
matrix<N, N, T>::matrix() {
    for (int i = 0;i < N;i++) {
		(*this)[i][i] = 1;
	}
}
template<int N, typename T>
matrix<N, N, T>::matrix(const matrix_base<N, N, T>& other) : matrix_base<N, N, T>(other) {}

template<int N, typename T>
matrix<N, N, T>::matrix(const std::vector <std::vector <T>>& other) : matrix_base<N, N, T>(other) {}

template<int N, typename T>
matrix<N, N, T> matrix<N, N, T>::transposed() const {
	matrix<N, N, T> newMat;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
			newMat[j][i] = (*this)[i][j];
		}
	}
	return newMat;
}
template<int N, typename T>
T matrix<N, N, T>::det() const {

    matrix<N, N, double> temp;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
			temp[i][j] = (*this)[i][j];
		}
	}

    int negative = 1;
    for (int col = 0;col < N-1;col++) {
        if (temp[col][col] == 0) {
			bool succeed = 0;
            for (int line = col + 1;line < N;line++) {
                if (temp[line][col] != 0) {
					succeed = 1;
                    for (int j = 0;j < N;j++) {
						std::swap(temp[col][j], temp[line][j]);
					}
					negative *= -1;
					break;
				}
			}
			if (!succeed) {
				return 0;
			}
		}

        for (int line = col + 1;line < N;line++) {
            double koef = temp[line][col] / temp[col][col];
            temp[line][col] = 0;
            for (int j = col+1;j < N;j++) {
				temp[line][j] -= temp[col][j] * koef;
			}
		}
	}

    double detVal(negative);
    for (int i = 0;i < N;i++) {
		detVal *= temp[i][i];
	}
    return detVal;

}

template<int N, typename T>
matrix<N, N, T> matrix<N, N, T>::inverted() const {
    matrix<N, 2 * N, double> temp(this->extended(matrix<N,N,T>()));
	temp.makeEqualE();
	matrix<N, N, T> res;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
			res[i][j] = temp[i][j + N];
		}
	}
	return res;
}

template<int N, typename T>
matrix<N, N, T>& matrix<N, N, T>::invert() {
	(*this) = inverted();
	return *this;
}

template<int N, typename T>
T matrix<N, N, T>::trace() const {
	T traceVal(0);
    for (int i = 0;i < N;i++) {
		traceVal += (*this)[i][i];
	}
	return traceVal;
}

template<int N, int M, typename T>
matrix<N, M, T> operator -(const matrix<N, M, T>& a) {
	matrix_base<N, M, T> other;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			other[i][j] = -a[i][j];
		}
	}
	return other;
}
template<int N, int M, typename T>
matrix<N, M, T> operator +(const matrix<N, M, T>& a, const matrix<N, M, T>& b) {
	matrix<N, M, T> newmatrix;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			newmatrix[i][j] = a[i][j] + b[i][j];
		}
	}
	return newmatrix;
}
template<int N, int M, int K, typename T>
matrix<N, K, T> operator *(const matrix<N, M, T>& a, const matrix<M, K, T>& b) {
	matrix<N, K, T> newmatrix(0);
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
            for (int l = 0;l < K;l++) {
				newmatrix[i][l] += a[i][j] * b[j][l];
			}
		}
	}
	return newmatrix;
}
template<int N, int M, typename T>
matrix<N, M, T> operator *(const matrix<N, M, T>& a, T b) {
	matrix<N, M, T> newmatrix(a);
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			newmatrix[i][j] *= b;
		}
	}
	return newmatrix;
}
template<int N, int M, typename T>
matrix<N, M, T>& operator += (matrix<N, M, T>& a, const matrix<N, M, T>& b) {
	a = a + b;
	return a;
}
template<int N, int M, typename T>
matrix<N, M, T> operator -(const matrix<N, M, T>& a, const matrix<N, M, T>& b) {
	return a + (-b);
}
template<int N, int M, typename T>
matrix<N, M, T>& operator -=(matrix<N, M, T>& a, const matrix<N, M, T>& b) {
	a += (-b);
	return a;
}
template<int N, int M, typename T>
matrix<N, M, T>& operator *=(matrix<N, M, T>& a, const matrix<M, M, T>& b) {
    a = a * b;
	return a;
}
template<int N, int M, typename T>
matrix<N, M, T>& operator *=(matrix<N, M, T>& a, T b) {
	a = a * b;
	return a;
}
template<int N, int M, typename T>
matrix<N, M, T> operator /(const matrix<N, M, T>& a, T b) {
	return a * (1 / b);
}
template<int N, int M, typename T>
matrix<N, M, T> operator /= (matrix<N, M, T>& a, T b){
    a = a/b;
    return a;
}

template<int N, int M, typename T>
bool operator == (const matrix<N, M, T>& a, const matrix<N, M, T>& b) {
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			if (a[i][j] != b[i][j])
				return 0;
		}
	}
	return 1;
}
template<int N, int M, typename T>
bool operator != (const matrix<N, M, T>& a, const matrix<N, M, T>& b) {
	return !(a == b);
}
template<int N, int M, typename T>
std::istream& operator >>(std::istream& stream, matrix<N, M, T>& other) {
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			stream >> other[i][j];
		}
	}
	return stream;
}
template<int N, int M, typename T>
std::ostream& operator <<(std::ostream& stream, const matrix<N, M, T>& other) {
    for (int i = 0;i < N;i++) {
		/*if (i == M) {
			stream << '\n';
            for (int j = 0;j < 2*M-1;j++) {
				stream << "-";
			}
		}*/
        if (i){
            stream  << '\n';
        }
        for (int j = 0;j < M;j++) {
			//if (j == N) { stream << "| ";}
            stream << other[i][j] << ' ';
		}

	}
	return stream;
}

template<int N, typename T>
matrix<N, N, T>getX__AX_equal_B(const matrix<N, N, T>& B, const matrix<N, N, T>& C) {
    matrix<N, 2 * N, double> temp(B.extended(C));

	temp.makeEqualE();

	matrix<N, N, T> res;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < N;j++) {
			res[i][j] = temp[i][j + N];
		}
	}
	return res;
}
