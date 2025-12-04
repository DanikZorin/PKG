#ifndef MATRIX_LIGHT_H
#define MATRIX_LIGHT_H

#include <iostream>
#include <vector>
template <typename T>
class matrix_light{
    T** data;
    int N, M;

    void erase();
    void copy(const matrix_light<T>&);
public:
    matrix_light(int n);
    matrix_light(int n, int m, const T& val = T());
    matrix_light(const matrix_light<T>&);
    matrix_light(const std::vector <std::vector<T>>&);
    ~matrix_light();

    matrix_light& operator=(const matrix_light<T>& obj);

    int getN() const;
    int getM() const;

    int rank() const;

    matrix_light& makeEqualE();

    matrix_light transposed() const;
    matrix_light& transpose();

    std::vector <T> getRow(int i) const;
    std::vector <T> getColumn(int j) const;
    std::vector <T*> getRowToChange(int i);
    std::vector <T*> getColumnToChange(int j);

    matrix_light extended(const matrix_light<T>&) const;

    matrix_light& setRow(int i, std::vector<T> row);
    matrix_light& setColumn(int j, std::vector<T> col);

    T det() const;
    matrix_light inverted() const;
    matrix_light& invert();
    T trace() const;

    const T* operator[] (int i) const;
    T* operator[] (int i);


};

template <typename T>
matrix_light<T> operator -(const matrix_light<T>& a);
template <typename T>
matrix_light<T> operator + (const matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
matrix_light<T> operator * (const matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
matrix_light<T> operator * (const matrix_light<T>& a, T b);

template<typename T>
matrix_light<T>& operator += (matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
matrix_light<T>& operator *= (matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
matrix_light<T>& operator *= (matrix_light<T>& a, T b);
template<typename T>
matrix_light<T> operator - (const matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
matrix_light<T>& operator -= (matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
matrix_light<T> operator / (const matrix_light<T>& a, T b);

template<typename T>
bool operator == (const matrix_light<T>& a, const matrix_light<T>& b);
template<typename T>
bool operator != (const matrix_light<T>& a, const matrix_light<T>& b);


template<typename T1, typename T2, typename T3>
matrix_light<T3> operator * (const matrix_light<T1>& a, const matrix_light<T2>& b);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
template<typename T>
void matrix_light<T>::erase() {
    for (int i = 0;i < N;i++) {
        delete[] data[i];
    }
    delete[] data;
    N = 0;
    M = 0;
}
template<typename T>
void matrix_light<T>::copy(const matrix_light<T>& other) {
    N = other.getN();
    M = other.getM();
    data = new T * [N];
    for (int i = 0;i < N;i++) {
        data[i] = new T[M];
        for (int j = 0;j < M;j++) {
            data[i][j] = other[i][j];
        }
    }
}
template<typename T>
matrix_light<T>::matrix_light(int n):matrix_light(n,n,0){
    for (int i=0;i<N;i++){
        data[i][i] = 1;
    }
}
template<typename T>
matrix_light<T>::matrix_light(int n, int m, const T& val):N(n),M(m){
    if (n<=0 ||m<=0){
        throw std::invalid_argument("Bad size matrix");
    }
    data = new T*[N];
    for (int i=0;i<N;i++){
        data[i] = new T[M];
        for (int j=0;j<M;j++){
            data[i][j] = val;
        }
    }
}
template<typename T>
matrix_light<T>::matrix_light(const matrix_light<T>& obj){
    copy(obj);
}
template<typename T>
matrix_light<T>::matrix_light(const std::vector <std::vector<T>>& data_): matrix_light(data_.size(), data_[0].size()){
    for (int i=0;i<N;i++){
        for (int j=0;j<M;j++){
            data[i][j] = data_[i][j];
        }
    }
}
template<typename T>
matrix_light<T>::~matrix_light(){
    erase();
}
template<typename T>
matrix_light<T>& matrix_light<T>::operator=(const matrix_light<T>& obj){
    if (this != &obj){
        erase();
        copy(obj);
    }
    return *this;
}

template<typename T>
int matrix_light<T>::getN() const{
    return N;
}
template<typename T>
int matrix_light<T>::getM() const{
    return M;
}

template<typename T>
int matrix_light<T>::rank() const{
    if (N > M) {
        return this->transposed().rank();
    }
    matrix_light<double> temp(N, M);
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
            temp[i][j] = data[i][j];
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

template<typename T>
matrix_light<T>& matrix_light<T>::makeEqualE(){
    if (N > M) {
        throw std::invalid_argument("Bad size matrix");
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
                throw std::invalid_argument("Zero devision matrix");;
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

template<typename T>
matrix_light<T> matrix_light<T>::transposed() const{
    matrix_light<T> newMat(M, N);
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
            newMat[j][i] = data[i][j];
        }
    }
    return newMat;
}
template<typename T>
matrix_light<T>& matrix_light<T>::transpose(){
    (*this) = transposed();
    return *this;
}

template<typename T>
std::vector<T> matrix_light<T>::getRow(int i) const{
    if (i < 0 || i >= N) {
        throw std::invalid_argument("Out of bounds matrix");
    }
    std::vector <T> row(N);
    for (int j = 0;j < M;j++) {
        row[j] = data[i][j];
    }
    return row;
}
template<typename T>
std::vector<T> matrix_light<T>::getColumn(int j) const{
    if (j < 0 || j >= M) {
        throw std::invalid_argument("Out of bounds matrix");
    }
    std::vector <T> column(M);
    for (int i = 0;i < N;i++) {
        column[i] = data[i][j];
    }
    return column;
}
template<typename T>
std::vector<T*> matrix_light<T>::getRowToChange(int i){
    if (i < 0 || i >= N) {
        throw std::invalid_argument("Out of bounds matrix");
    }
    std::vector <T*> row(N);
    for (int j = 0;j < M;j++) {
        row[j] = &data[i][j];
    }
    return row;
}
template<typename T>
std::vector<T*> matrix_light<T>::getColumnToChange(int j){
    if (j < 0 || j >= M) {
        throw std::invalid_argument("Out of bounds matrix");
    }
    std::vector <T*> column(M);
    for (int i = 0;i < N;i++) {
        column[i] = &data[i][j];
    }
    return column;
}

template<typename T>
matrix_light<T> matrix_light<T>::extended(const matrix_light<T>& obj) const{
    if (obj.getN() != N){
        throw std::invalid_argument("Bad size matrix");
    }
    matrix_light<T> res(N, M+obj.getM());
    for (int i = 0;i < N;i++) {
        for (int j = 0;j < M;j++) {
            res[i][j] = data[i][j];
        }
        for (int j = 0;j < obj.getM();j++) {
            res[i][j + M] = obj[i][j];
        }
    }
    return res;
}

template<typename T>
matrix_light<T>& matrix_light<T>::setRow(int i, std::vector<T> row){
    for (int j=0;j<M;j++){
        data[i][j]=0;
    }
    for (int j=0;j<row.size() && j<M;j++){
        data[i][j]=row[j];
    }
    return *this;
}
template<typename T>
matrix_light<T>& matrix_light<T>::setColumn(int j, std::vector<T> col){
    for (int i=0;i<N;i++){
        data[i][j]=0;
    }
    for (int i=0;i<col.size() && i < N;i++){
        data[i][j]=col[i];
    }
    return *this;
}

template<typename T>
T matrix_light<T>::det() const{
    if (N != M){
        throw std::invalid_argument("Bad size matrix");
    }
    matrix_light<double> temp(N, N);
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

template<typename T>
matrix_light<T> matrix_light<T>::inverted() const{
    if (N != M){
        throw std::invalid_argument("Bad Size matrix");
    }
    if (rank() != N){
        throw std::invalid_argument("Zero division matrix");;
    }
    matrix_light<double> temp(this->extended(matrix_light<T>(N)));
    temp.makeEqualE();
    matrix_light<T> res(N);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i][j] = temp[i][j + N];
        }
    }
    return res;
}
template<typename T>
matrix_light<T>& matrix_light<T>::invert(){
    (*this) = inverted();
    return *this;
}
template<typename T>
T matrix_light<T>::trace() const{
    T traceVal(0);
    for (int i = 0; i < N; i++) {
        traceVal += (*this)[i][i];
    }
    return traceVal;
}
template<typename T>
const T* matrix_light<T>::operator[] (int i) const{
    if (i >= N || i < 0) {
        throw std::invalid_argument("Out of bounds matrix");
    }
    return data[i];
}
template<typename T>
T* matrix_light<T>::operator[] (int i){
    if (i >= N || i < 0) {
        throw std::invalid_argument("Out of bounds matrix");
    }
    return data[i];
}
template<typename T>
matrix_light<T> operator -(const matrix_light<T>& obj) {
    matrix_light<T> res(obj.N, obj.M);
    for (int i = 0;i < obj.N;i++) {
        for (int j = 0;j < obj.M;j++) {
            res[i][j] = -obj[i][j];
        }
    }
    return res;
}
template<typename T>
matrix_light<T> operator +(const matrix_light<T>& a, const matrix_light<T>& b) {
    if (a.getN() != b.getN() || a.getM() != b.getM()){
        throw std::invalid_argument("Bad size matrix");;
    }
    matrix_light<T> res(a.N, a.M);
    for (int i = 0;i < a.N;i++) {
        for (int j = 0;j < a.M;j++) {
            res[i][j] = a[i][j] + b[i][j];
        }
    }
    return res;
}
template<typename T>
matrix_light<T> operator *(const matrix_light<T>& a, const matrix_light<T>& b) {
    if (a.getM() != b.getN()){
        throw std::invalid_argument("Bad size matrix");;
    }
    matrix_light<T> res(a.getN(), b.getM());
    for (int i = 0;i < a.getN();i++) {
        for (int j = 0;j < a.getM();j++) {
            for (int l = 0;l < b.getM();l++) {
                res[i][l] += a[i][j] * b[j][l];
            }
        }
    }
    return res;
}
template<typename T>
matrix_light<T> operator *(const matrix_light<T>& a, T b) {
    matrix_light<T> res(a);
    for (int i = 0;i < a.N;i++) {
        for (int j = 0;j < a.M;j++) {
            res[i][j] *= b;
        }
    }
    return res;
}
template<typename T>
matrix_light<T>& operator += (matrix_light<T>& a, const matrix_light<T>& b) {
    a = a + b;
    return a;
}
template<typename T>
matrix_light<T> operator -(const matrix_light<T>& a, const matrix_light<T>& b) {
    return a + (-b);
}
template<typename T>
matrix_light<T>& operator -=(matrix_light<T>& a, const matrix_light<T>& b) {
    a += (-b);
    return a;
}
template<typename T>
matrix_light<T>& operator *=(matrix_light<T>& a, const matrix_light<T>& b) {
    a = a * b;
    return a;
}
template<typename T>
matrix_light<T>& operator *=(matrix_light<T>& a, T b) {
    a = a * b;
    return a;
}
template<typename T>
matrix_light<T> operator /(const matrix_light<T>& a, T b) {
    return a * (1 / b);
}
template<typename T>
bool operator == (const matrix_light<T>& a, const matrix_light<T>& b) {
    if (a.getN() != b.getN() || a.getM() != b.getM()){
        throw matrix_light<T>::BadSize();
    }
    for (int i = 0;i < a.getN();i++) {
        for (int j = 0;j < a.getM();j++) {
            if (a[i][j] != b[i][j])
                return 0;
        }
    }
    return 1;
}
template<typename T>
bool operator != (const matrix_light<T>& a, const matrix_light<T>& b) {
    return !(a == b);
}
template<typename T1, typename T2, typename T3>
matrix_light<T3> operator * (const matrix_light<T1>& a, const matrix_light<T2>& b){
    if (a.getM() != b.getN()){
        throw std::invalid_argument("Bad size matrix");;
    }
    matrix_light<T3> res(a.getN(), b.getM());
    for (int i = 0;i < a.getN();i++) {
        for (int j = 0;j < a.getM();j++) {
            for (int l = 0;l < b.getM();l++) {
                res[i][l] += a[i][j] * b[j][l];
            }
        }
    }
    return res;
}

template<typename T>
std::istream& operator >>(std::istream& stream, matrix_light<T>& obj) {
    for (int i = 0;i < obj.getN();i++) {
        for (int j = 0;j < obj.getM();j++) {
            stream >> obj[i][j];
        }
    }
    return stream;
}
template<typename T>
std::ostream& operator <<(std::ostream& stream, const matrix_light<T>& obj) {
    for (int i = 0;i < obj.getN();i++) {
        /*if (i == M) {
            stream << '\n';
            for (int j = 0;j < 2*M-1;j++) {
                stream << "-";
            }
        }*/
        if (i){
            stream  << '\n';
        }
        for (int j = 0;j < obj.getM();j++) {
            //if (j == N) { stream << "| ";}
            stream << obj[i][j] << ' ';
        }

    }
    return stream;
}

template<typename T>
matrix_light<T> getX__AX_equal_B(const matrix_light<T>& A, const matrix_light<T>& B) {
    if (A.getN() != A.getM() || A.getN() != B.getN() || A.getN() != B.getM()){
        throw matrix_light<T>::BadSize();
    }
    matrix_light<double> temp(A.extended(B));

    temp.makeEqualE();

    matrix_light<T> res(A.getN(),A.getN());
    for (int i = 0;i < A.getN();i++) {
        for (int j = 0;j < A.getN();j++) {
            res[i][j] = temp[i][j + A.getN()];
        }
    }
    return res;
}

#endif // MATRIX_LIGHT_H
