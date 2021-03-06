#ifndef _matrix_hpp
#define _matrix_hpp

#include <vector>
#include <string>

class Matrix;
class Vector {
public:
  Vector();
  Vector(int N, bool has_entry=true);
  Vector(int base, int lvl, bool has_entry=true);
  
  // number of rows
  int rows() const;

  // number of partitions
  int num_partition() const;

  // return offset
  int offset() const;
  
  // 2-norm
  double norm() const;

  // random entries
  void rand(int, int offset);
  void rand(int offset=0);

  // return the random seed
  long rand_seed(int) const;
  
  // form a diagonal matrix
  Matrix to_diag_matrix() const;

  // return the ith entry / reference  
  double  operator[] (int i) const;
  double& operator[] (int i);
  
  // treat as diagonal matrix
  Vector multiply(const Vector&);
  Matrix multiply(const Matrix&);
  friend Vector operator + (const Vector&, const Vector&);
  friend Vector operator - (const Vector&, const Vector&);
  friend bool   operator== (const Vector&, const Vector&);
  friend bool   operator!= (const Vector&, const Vector&);
  friend Vector operator * (const double,  const Vector&);
  
  // for debugging purpose
  void display(const std::string&) const;

  // static methods
  template <int value>
  static Vector constant(int);
  
private:
  int nPart;
  int mRows;
  int mOffset;
  std::vector<long>   seeds;
  std::vector<double> data;

  // for large matrices, we can avoid generating the entries,
  //  but only store the seeds
  bool has_entry;
};

class Matrix {
public:
  Matrix();
  Matrix(int nRow, int nCol, bool has_entry=true);
  Matrix(int base, int level, int nCol, bool has_entry=true);
  //  ~Matrix();

  // consistant with eigen routines
  int rows() const;
  int cols() const;
  int levels() const;
  
  // return the F norm
  double norm() const;
  
  // return the pointer to data
  // used in solve() lapack routine
  double* pointer();
  
  // number of partitions
  int num_partition() const;
  
  // random matrix with a random seed for each partition
  // the partition is horizontal
  void rand(int);
  void rand();

  // return the random seed
  long rand_seed(int) const;
  
  // assignment operator
  //  void operator= (const Matrix&);

  // return the entry / reference
  double operator() (int i, int j) const;
  double& operator() (int i, int j);

  // return the matrix transpose
  Matrix T();
  
  // return matrix block
  Matrix block(int, int, int, int) const;
  Matrix row_block(int, int) const;
  
  // solve A*X=B
  void solve(Matrix &B);
  
  // matrix vector product
  friend Vector operator * (const Matrix&, const Vector&);
  friend Matrix operator * (const Matrix&, const Matrix&);
  friend Matrix operator + (const Matrix&, const Matrix&);
  friend Matrix operator - (const Matrix&, const Matrix&);
  friend bool   operator== (const Matrix&, const Matrix&);
  friend bool   operator!= (const Matrix&, const Matrix&);
  friend Matrix operator * (const double,  const Matrix&);

  // for debugging purpose
  void display(const std::string&) const;

  // static methods
  template <int value>
  static Matrix constant(int m, int n);

  static Matrix identity(int);
  
private:
  int  mLevel;
  int  nPart;
  int  mRows;
  int  mCols;
  std::vector<long>   seeds;
  std::vector<double> data;

  // for large matrices, we can avoid generating the entries,
  //  but only store the seeds
  bool has_entry;
};

template <int value>
Vector Vector::constant(int N) {
  Vector temp(N);
  for (int i=0; i<N; i++)
    temp[i] = value;
  return temp;
}

template <int value>
Matrix Matrix::constant(int m, int n) {
  Matrix temp(m, n);
  for (int i=0; i<m; i++)
    for (int j=0; j<n; j++)
      temp(i, j) = value;
  return temp;
}

#endif
