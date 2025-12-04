#pragma once
#include <cmath>
#include <vector>
#include <iostream>
template <int N, int M, typename T = double>
class matrix_base {
protected:
	T** data;

	void Erase();
	void Copy(const matrix_base<N,M,T>&);
public:
	matrix_base(T = 0);
    matrix_base(const std::vector <std::vector <T>>&);
    matrix_base(const matrix_base<N,M,T>&);
    ~matrix_base();
    int rank() const;

	matrix_base<N,M,T>& operator =(const matrix_base<N,M,T>&);
    matrix_base& makeEqualE();

	matrix_base<M,N,T> transposed() const;

    std::vector <T> getRow(int i) const;
    std::vector <T> getColumn(int j) const;

    template<int K>
    matrix_base<N, M + K, T> extended(const matrix_base<N, K, T>&) const;

    matrix_base& setRow(int i, std::vector<T> row);
    matrix_base& setColumn(int j, std::vector<T> col);


    operator matrix_base<N-1,M-1, T>() const;
    operator matrix_base<N+1,M+1, T>() const;

    const T operator() (int i, int j) const;
    T operator() (int i, int j);
    const T* operator[] (int i) const;
    T* operator[] (int i);

	class OutOfBounds {};
	class ZeroDivision {};
};

// -------------------------------------------------
template<int N,int M, typename T>
matrix_base<N, M, T>& matrix_base<N, M, T>::makeEqualE() {
    if (N > M) {
        throw matrix_base::OutOfBounds();
    }
    for (int col = 0;col < N;col++) {
        if ((*this)[col][col] == 0) {
            bool succeed = 0;
            for (int line = col;line < N;line++) {
                if ((*this)[line][col] != 0) {
                    succeed = 1;
                    for (int k = 0;k < M;k++) {
                        std::swap((*this)[col][k], (*this)[line][k]);
                    }
                    break;
                }
            }
            if (!succeed) {
                throw matrix_base::ZeroDivision();
            }
        }
        T koef = (*this)[col][col];
        for (int j = col;j < M;j++) {
            (*this)[col][j] /= koef;
        }
        for (int line = 0;line < N;line++) {
            if (line == col)
                continue;
            koef = (*this)[line][col];
            for (int j = 0;j < M;j++) {
                (*this)[line][j] -= (*this)[col][j] * koef;
            }
        }
    }
    return *this;
}


template<int N, int M, typename T>
int matrix_base<N, M, T>::rank() const {
	if (N > M) {
		return this->transposed().rank();
	}
    matrix_base<N, M, double> temp;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			temp[i][j] = (*this)[i][j];
		}
	}
    for (int col = 0;col < N - 1;col++) {
		if (temp[col][col] == 0) {
			bool succeed = 0;
            for (int i = col;i < N;i++) {
                for (int j = col;j < M;j++) {
					if (temp[i][j] != 0) {
						succeed = 1;
                        for (int k = 0;k < M;k++) {
							std::swap(temp[col][k], temp[i][k]);
						}
                        for (int k = 0;k < N;k++) {
							std::swap(temp[k][col], temp[k][j]);
						}
						break;
					}
				}
				if (succeed) {
					break;
				}
			}
			if (!succeed) {
				return 0;
			}
		}
        for (int line = col + 1;line < N;line++) {
            double koef = temp[line][col] / temp[col][col];
            for (int j = col;j < M;j++) {
				temp[line][j] -= temp[col][j] * koef;
			}
		}
	}
    for (int i = N - 1;i >= 0;i--) {
		if (temp[i][i] != 0) {
			return i + 1;
		}
	}
}
template<int N, int M, typename T>
template<int K>
matrix_base<N, M+K, T> matrix_base<N,M,T>::extended(const matrix_base<N,K,T>& other) const {
    matrix_base<N, M+K, T> res;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
            res[i][j] = (*this)[i][j];
        }
        for (int j = 0;j < K;j++) {
            res[i][j + M] = other[i][j];
        }
    }
    return res;
}
template<int N, int M, typename T>
matrix_base<N,M,T>& matrix_base<N,M,T>::setRow(int i, std::vector<T> row){
    for (int j=0;j<M;j++){
        data[i][j]=0;
    }
    for (int j=0;j<row.size() && j<M;j++){
        data[i][j]=row[j];
    }
    return *this;
}
template<int N, int M, typename T>
matrix_base<N,M,T>& matrix_base<N,M,T>::setColumn(int j, std::vector<T> col){
    for (int i=0;i<N;i++){
        data[i][j]=0;
    }
    for (int i=0;i<col.size() && i < N;i++){
        data[i][j]=col[i];
    }
    return *this;
}
template<int N, int M, typename T>
matrix_base<N, M, T>::operator matrix_base<N-1,M-1, T>() const{
    matrix_base<N-1, M-1, T> res;
    for (int i=0;i<N-1;i++){
        for (int j=0;j<M-1;j++){
            res[i][j] = data[i][j];
        }
    }
    return res;
}
template<int N, int M, typename T>
matrix_base<N, M, T>::operator matrix_base<N+1,M+1, T>() const{
    matrix_base<N+1, M+1, T> res;
    for (int i=0;i<N;i++){
        for (int j=0;j<M;j++){
            res[i][j] = data[i][j];
        }
    }
    res[N][M] = 1;
    return res;
}

template<int N, int M, typename T>
const T* matrix_base<N, M, T>::operator[](int i) const {
	if (i >= N || i < 0) {
		throw OutOfBounds();
	}
	return data[i];
}
template<int N, int M, typename T>
T* matrix_base<N, M, T>::operator[](int i) {
	if (i >= N || i < 0) {
		throw OutOfBounds();
	}
	return data[i];
}
template<int N, int M, typename T>
const T matrix_base<N, M, T>::operator()(int i, int j) const {
	if (i >= N || i < 0 || j < 0 || j >= M) {
		throw OutOfBounds();
	}
	return (*this)[i][j];
}
template<int N, int M, typename T>
T matrix_base<N, M, T>::operator()(int i, int j) {
	if (i >= N || i < 0 || j < 0 || j >= M) {
		throw OutOfBounds();
	}
	return (*this)[i][j];
}
template<int N, int M, typename T>
void matrix_base<N, M, T>::Erase() {
    for (int i = 0;i < N;i++) {
		delete[] data[i];
	}
	delete[] data;
}
template<int N, int M, typename T>
void matrix_base<N, M, T>::Copy(const matrix_base<N, M, T>& other) {
	data = new T * [N];
    for (int i = 0;i < N;i++) {
		data[i] = new T[M];
        for (int j = 0;j < M;j++) {
			data[i][j] = other.data[i][j];
		}
	}
}




template<int N, int M, typename T>
matrix_base<N, M, T>::matrix_base(T newData) {
	data = new T * [N];
    for (int i = 0;i < N;i++) {
		data[i] = new T[M];
        for (int j = 0;j < M;j++) {
			data[i][j] = newData;
		}
	}
}

template<int N, int M, typename T>
matrix_base<N, M, T>::matrix_base(const std::vector <std::vector <T>>& other) :matrix_base() {
    int N_ = other.size();
    for (int i = 0;i < std::min(N, N_);i++) {
        int M_ = other[i].size();
        for (int j = 0;j < std::min(M, M_);j++) {
			data[i][j] = other[i][j];
		}
	}
}
template<int N, int M, typename T>
matrix_base<N, M, T>::matrix_base(const matrix_base<N, M, T>& other) {
	Copy(other);
}
template<int N, int M, typename T>
matrix_base<N, M, T>::~matrix_base(){
    for (int i=0;i<N;i++){
        delete[] data[i];
    }
    delete[] data;
}

template<int N, int M, typename T>
matrix_base<M, N, T> matrix_base<N, M, T>::transposed() const {
	matrix_base<M, N, T> newMat;
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
			newMat[j][i] = data[i][j];
		}
	}
	return newMat;
}

template<int N, int M, typename T>
std::vector <T> matrix_base<N, M, T>::getRow(int i) const {
	if (i < 0 || i >= N) {
		throw OutOfBounds();
	}
	std::vector <T> row(N);
    for (int j = 0;j < M;j++) {
		row[j] = data[i][j];
	}
	return row;
}
template<int N, int M, typename T>
std::vector <T> matrix_base<N, M, T>::getColumn(int j) const {
	if (j < 0 || j >= M) {
		throw OutOfBounds();
	}
	std::vector <T> column(M);
    for (int i = 0;i < N;i++) {
		column[i] = data[i][j];
	}
	return column;
}

template<int N, int M, typename T>
matrix_base<N, M, T>& matrix_base<N, M, T>::operator =(const matrix_base<N, M, T>& other) {
	if (&other != this) {
		Erase();
		Copy(other);
	}
	return *this;
}

