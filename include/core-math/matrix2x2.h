#pragma once 

#ifndef _MATRIX2X2_H
#define _MATRIX2X2_H


//! This class provides functions to handle 3-dimensional matrices
/*! The arrangement of the matrix is row-like.
    The index of a specific position is:
    <pre>
       0  1
       2  3
    </pre>
*/
template <class FloatType> class Matrix2x2
{
public:
	//! An uninitialized matrix
	Matrix2x2() {}

	//! Initializes a matrix from file
	Matrix2x2(const std::string& filename) {
		loadMatrixFromFile(filename);
	}

	//! Initialize with values stored in an array
	Matrix2x2(const FloatType* values) {
		for (unsigned int i = 0; i < 4; i++) {
			matrix[i] = values[i];
		}
	}

	//! Initialize from 3 row vectors
	Matrix2x2(const point2d<FloatType>& v0, const point2d<FloatType>& v1) {
		matrix[0] = v0.x;	matrix[1] = v0.y;
		matrix[2] = v1.x;	matrix[3] = v1.y;
	}

	//! Initializes the matrix row wise
	Matrix2x2(	const FloatType &m00, const FloatType &m01,
				const FloatType &m10, const FloatType &m11) 
	{
		_m00 = m00;	_m01 = m01;
		_m10 = m10;	_m11 = m11;
	}

	//! Initialize with a matrix from another type
	template<class U>
	Matrix2x2(const Matrix2x2<U>& other) {
		for (unsigned int i = 0; i < 4; i++) {
			matrix[i] = (FloatType)other.getRawData()[i];
		}
	}

	//! destructor
	~Matrix2x2() {}

	//! Access element of Matrix at row x and column y for constant access
	inline FloatType at(unsigned char x, unsigned char y) const {
		assert((x<2)&&(y<2)); // no test if x<0 or y<0; they are unsigned char
		return matrix2[x][y]; 
	}
	//! Access element of Matrix at row x and column y
	inline  FloatType& at(unsigned char x, unsigned char y) {
		assert((x<2)&&(y<2)); // no test if x<0 or y<0; they are unsigned char
		return matrix2[x][y]; 
	}

	//! Access i,j-th row of Matrix for constant access
	inline  FloatType operator() (unsigned int i, unsigned int j) const {
		assert(i<2 && j<2);
		return matrix2[i][j];
	}
	//! Access i,j-th element of Matrix
	inline  FloatType& operator() (unsigned int i, unsigned int j) {
		assert(i<2 && j<2);
		return matrix2[i][j]; 
	}

	//! Access i-th element of the Matrix for constant access
	inline  FloatType operator[] (unsigned int i) const {
		assert(i<4);
		return matrix[i];
	}
	//! Access i-th element of the Matrix
	inline  FloatType& operator[] (unsigned int i) {
		assert(i<4);
		return matrix[i];
	}


	//! overwrite the matrix with an identity-matrix
	void setIdentity() {
		setScale(1.0, 1.0);
	}
	static Matrix2x2 identity() {
		Matrix2x2 res;	res.setIdentity();
		return res;
	}

	//! sets the matrix zero (or a specified value)
	void setZero(FloatType v = (FloatType)0) {
		matrix[0] = matrix[1] = v;
		matrix[2] = matrix[3] = v;
	}
	static Matrix2x2 zero() {
		Matrix2x2 res; res.zero();
		return res;
	}

	//! overwrite the matrix with a scale-matrix
	void setScale(FloatType x, FloatType y) {
		matrix[0] =   x; matrix[1] = 0.0; 
		matrix[2] = 0.0; matrix[3] =   y;
	}
	static Matrix2x2 scale(FloatType x, FloatType y) {
		Matrix2x2 res;	res.setScale(x,y);
		return res;
	}

	//! overwrite the matrix with a scale-matrix
	void setScale(FloatType s) {
		setScale(s,s);
	}
	static Matrix2x2 scale(FloatType s) {
		Matrix2x2 res;	res.setScale(s);
		return res;
	}
	
	//! overwrite the matrix with a scale-matrix
	void setScale(const point2d<FloatType>& v) {
		matrix[0] = v.x; matrix[1] = 0.0;
		matrix[2] = 0.0; matrix[3] = v.y;
	}
	static Matrix2x2 scale(const point2d<FloatType>& v) {
		Matrix2x2 res;	res.setScale(v);
		return res;
	}
	
	
	//! overwrite the matrix with a diagonal matrix
	void setDiag(FloatType x, FloatType y) {
		setScale(x,y);
	}
	static Matrix2x2 diag(FloatType x, FloatType y) {
		Matrix2x2 res;	res.setDiag(x,y);
		return res;
	}




	//! return the product of the operand with matrix
	Matrix2x2 operator* (const Matrix2x2& other) const {
		Matrix2x2<FloatType> result;
		//TODO unroll the loop
		for (unsigned char i = 0; i < 2; i++) {
			for (unsigned char j = 0; j < 2; j++) {
				result.at(i,j) = 
					this->at(i,0) * other.at(0,j) + 
					this->at(i,1) * other.at(1,j) + 
			}
		}
		return result;
	}
	//! multiply operand with matrix b
	Matrix2x2& operator*= (const Matrix2x2& other) {
		Matrix2x2<FloatType> prod = (*this)*other;
		*this = prod;
		return *this;
	}
	//! multiply each element in the matrix with a scalar factor
	Matrix2x2 operator* (FloatType r) const {
		Matrix2x2<FloatType> result;
		for (unsigned int i = 0; i < 4; i++) {
			result.matrix[i] = matrix[i] * r;
		}
		return result;
	}
	//! multiply each element in the matrix with a scalar factor
	Matrix2x2& operator*= (FloatType r) {
		for (unsigned int i = 0; i < 4; i++) {
			matrix[i] *= r;
		}
		return *this;
	}
	//! divide the matrix by a scalar factor
	Matrix2x2 operator/ (FloatType r) const {
		Matrix2x2<FloatType> result;
		for (unsigned int i = 0; i < 4; i++) {
			result.matrix[i] = matrix[i] / r;
		}
		return result;
	}
	//! divide each element in the matrix with a scalar factor
	Matrix2x2& operator/= (FloatType r) {
		for (unsigned int i = 0; i < 4; i++) {
			matrix[i] /= r;
		}
		return *this;
	}
	//! transform a 3D-vector with the matrix
	point3d<FloatType> operator* (const point3d<FloatType>& v) const {
		return point3d<FloatType>(
			matrix[0]*v[0] + matrix[1]*v[1] + matrix[2]*v[2],
			matrix[3]*v[0] + matrix[4]*v[1] + matrix[5]*v[2],
			matrix[6]*v[0] + matrix[7]*v[1] + matrix[8]*v[2]
		);
	}
	//! return the sum of the operand with matrix b
	Matrix2x2 operator+ (const Matrix2x2& other) const {
		Matrix2x2<FloatType> result;
		for (unsigned int i = 0; i < 4; i++) {
			result.matrix[i] = matrix[i] + other.matrix[i];
		}
	}

	//! add matrix other to the operand
	Matrix2x2& operator+= (const Matrix2x2& other) {
		for (unsigned int i = 0; i < 4; i++) {
			matrix[i] += other.matrix[i];
		}
		return *this;
	}

	//! return the difference of the operand with matrix b
	Matrix2x2 operator- (const Matrix2x2& other) const {
		Matrix2x2<FloatType> result;
		for (unsigned int i = 0; i < 4; i++) {
			result.matrix[i] = matrix[i] - other.matrix[i];
		}
	}
	//! subtract matrix other from the operand
	Matrix2x2 operator-= (const Matrix2x2& other) {
		for (unsigned int i = 0; i < 4; i++) {
			matrix[i] -= other.matrix[i];
		}
		return *this;
	}

	//! return the determinant of the matrix
	FloatType det() const {
		return matrix[0]*matrix[3] - matrix[1]*matrix[2];
	}


	//! get the x column out of the matrix
	point2d<FloatType> xcol() const {
		return point2d<FloatType>(matrix[0],matrix[2]);
	}
	//! get the y column out of the matrix
	point3d<FloatType> ycol() const {
		return point3d<FloatType>(matrix[1],matrix[3]);
	}
	//! get the x row out of the matrix
	point2d<FloatType> xrow() const {
		point3d<FloatType>(matrix[0],matrix[1]);
	}
	//! get the y row out of the matrix
	point2d<FloatType> yrow() const {
		return point2d<FloatType>(matrix[2],matrix[3]);
	}

	//! return the inverse matrix; but does not change the current matrix
	Matrix2x2 getInverse() const {
		FloatType inv[9];

		inv[0] = matrix[4]*matrix[8] - matrix[5]*matrix[7];
		inv[1] = -matrix[1]*matrix[8] + matrix[2]*matrix[7];
		inv[2] = matrix[1]*matrix[5] - matrix[2]*matrix[4];

		inv[3] = -matrix[3]*matrix[8] + matrix[5]*matrix[6];
		inv[4] = matrix[0]*matrix[8] - matrix[2]*matrix[6];
		inv[5] = -matrix[0]*matrix[5] + matrix[2]*matrix[3];

		inv[6] = matrix[3]*matrix[7] - matrix[4]*matrix[6];
		inv[7] = -matrix[0]*matrix[7] + matrix[1]*matrix[6];
		inv[8] = matrix[0]*matrix[4] - matrix[1]*matrix[3];

		FloatType matrixDet = det();
		
		FloatType matrixDetr= (FloatType)1.0 / matrixDet;

		Matrix2x2<FloatType> res;
		for (unsigned int i = 0; i < 9; i++) {
			res.matrix[i] = inv[i] * matrixDetr;
		}
		return res;

	}

	//! overwrite the current matrix with its inverse
	void invert() {
		*this = getInverse();
	}
	//! return the transposed matrix
	Matrix2x2 getTranspose() const {
		Matrix2x2<FloatType> result;
		for(unsigned char x = 0; x < 3; x++) {
			result.at(x,0) = at(0,x);
			result.at(x,1) = at(1,x);
			result.at(x,2) = at(2,x);
		}
		return result;
	}
	//! transpose the matrix in place
	void transpose() {
		*this = getTranspose();
	}

	FloatType* getRawData()
	{
		return matrix;
	}

	//! computes the tensor product between two vectors
	static Matrix2x2 tensorProduct(const point3d<FloatType> &v0, const point3d<FloatType> &v1) {
		Matrix2x2 ret;
		ret._m00 = v0.x * v1.x;		ret._m01 = v0.x * v1.y;		ret._m02 = v0.x * v1.z;
		ret._m10 = v0.y * v1.x;		ret._m11 = v0.y * v1.y;		ret._m12 = v0.y * v1.z;
		ret._m20 = v0.z * v1.x;		ret._m21 = v0.z * v1.y;		ret._m22 = v0.z * v1.z;
		return ret;
	}

	//! save matrix to .mat file
	void saveMatrixToFile(const std::string &name) const {
		std::ofstream out(name.c_str());
		out << 3 << " " << 3 << std::endl;
		for (unsigned int i = 0; i < 3; i++) {
			for (unsigned int j = 0; j < 3; j++) {
				out << matrix2[i][j] << " ";
			}
			out << std::endl;
		}
	}
	//! load matrix from .mat file
	void loadMatrixFromFile(const std::string &name) {
		std::ifstream in(name.c_str());
		if (!in.is_open()) throw std::ifstream::failure(std::string("Could not open matrix file").append(name));
		unsigned int height, width;
		in >> height >> width;
		assert(height == width && height == 3);
		for(unsigned int i = 0; i < 3; i++) {
			for(unsigned int j = 0; j < 3; j++) {
				in >> matrix2[i][j];
			}
		}
	}

	//! prints the matrix
	void print() {
		for (unsigned int i = 0; i < 3; i++) {
			for (unsigned int j = 0; j < 3; j++) {
				std::cout << at(i,j) << " ";
			}
			std::cout << std::endl;
		}
	}

	/*
	//! computes eigenvalues and eigenvectors numerically (numerical recipies); Eigenvalues/vectors are left to the matrix
	bool computeEigenvaluesAndEigenvectorsNR(FloatType &lambda_0, FloatType &lambda_1, FloatType &lambda_2, point3d<FloatType>& ev0, point3d<FloatType>& ev1, point3d<FloatType>& ev2) {

		// Use jacobi's method:
		// Build 3x3 matrix NR-style:
		FloatType** CV = new FloatType*[4];
		for (int i1 = 0; i1 < 4; i1++)	CV[i1] = new FloatType[4];

		FloatType lambda[4];
		
		FloatType** v = new FloatType*[4];
		for(int i1 = 0; i1 < 4; i1++)	v[i1] = new FloatType[4];


		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				CV[i+1][j+1] = (FloatType)matrix[i+3*j];

		int num_of_required_jabobi_rotations;

		if (!jacobi(CV, 3, lambda, v, &num_of_required_jabobi_rotations)) {
			for(int i1 = 0; i1 < 4; i1++) {
				delete[] v[i1];
				delete[] CV[i1];
			}
			delete[] v;
			delete[] CV;

			return false;
		}

		point3d<FloatType> vec1(v[1][1], v[2][1], v[3][1]);
		point3d<FloatType> vec2(v[1][2], v[2][2], v[3][2]);
		point3d<FloatType> vec3(v[1][3], v[2][3], v[3][3]);

		// Sort eigenvectors such the ev[0] is the smallest...
		if (fabs(lambda[1]) < fabs(lambda[2]) && fabs(lambda[1]) < fabs(lambda[3])) {
			ev2 = vec1;
			lambda_2 = lambda[1];
			if (fabs(lambda[2]) < fabs(lambda[3])) {
				ev1 = vec2;
				ev0 = vec3;
				lambda_1 = lambda[2];
				lambda_0 = lambda[3];
			} else {
				ev0 = vec2;
				ev1 = vec3;
				lambda_1 = lambda[3];
				lambda_0 = lambda[2];
			}
		} else if (fabs(lambda[2]) < fabs(lambda[1]) && fabs(lambda[2]) < fabs(lambda[3])) {
			ev2 = vec2;
			lambda_2 = lambda[2];
			if (fabs(lambda[1]) < fabs(lambda[3])) {
				ev1 = vec1;
				ev0 = vec3;
				lambda_1 = lambda[1];
				lambda_0 = lambda[3];
			} else {
				ev0 = vec1;
				ev1 = vec3;
				lambda_1 = lambda[3];
				lambda_0 = lambda[1];
			}
		} else { // lambda[3] smallest!
			ev2 = vec3;
			lambda_2 = lambda[3];
			if (fabs(lambda[1]) < fabs(lambda[2])) {
				ev1 = vec1;
				ev0 = vec2;
				lambda_1 = lambda[1];
				lambda_0 = lambda[2];
			} else {
				ev0 = vec1;
				ev1 = vec2;
				lambda_1 = lambda[2];
				lambda_0 = lambda[1];
			}
		}


		for(int i1 = 0; i1 < 4; i1++) {
			delete[] v[i1];
			delete[] CV[i1];
		}
		delete[] v;
		delete[] CV;

		return true;
	}
	*/


protected:
	//! calculate determinant of a 3x3 sub-matrix given by the indices of the rows and columns
	FloatType det3x3(unsigned int i0 = 0, unsigned int i1 = 1, unsigned int i2 = 2, unsigned int j0 = 0, unsigned int j1 = 1, unsigned int j2 = 2) const {
		return
			  (matrix2[i0][j0]*matrix2[i1][j1]*matrix2[i2][j2])
			+ (matrix2[i0][j1]*matrix2[i1][j2]*matrix2[i2][j0])
			+ (matrix2[i0][j2]*matrix2[i1][j0]*matrix2[i2][j1])
			- (matrix2[i2][j0]*matrix2[i1][j1]*matrix2[i0][j2])
			- (matrix2[i2][j1]*matrix2[i1][j2]*matrix2[i0][j0])
			- (matrix2[i2][j2]*matrix2[i1][j0]*matrix2[i0][j1]);
	}

private:
	union {
		//! access matrix using a single array
		FloatType matrix[9];
		//! access matrix using a two-dimensional array
		FloatType matrix2[3][3];
		//! access matrix using single elements
		struct { 
			FloatType
				_m00, _m01, _m02,
				_m10, _m11, _m12,
				_m20, _m21, _m22;
		};
	};
};

//! writes to a stream
template <class FloatType> 
__forceinline std::ostream& operator<<(std::ostream& s, const Matrix2x2<FloatType>& m)
{ 
	return (
		s << 
		m(0,0) << " " << m(0,1) << " " << m(0,2) << std::endl <<
		m(1,0) << " " << m(1,1) << " " << m(1,2) << std::endl <<
		m(2,0) << " " << m(2,1) << " " << m(2,2) << std::endl
		);
}

//! reads from a stream
template <class FloatType> 
__forceinline std::istream& operator>>(std::istream& s, const Matrix2x2<FloatType>& m)
{ 
	return (
		s >> 
		m(0,0) >> m(0,1) >> m(0,2) >>
		m(1,0) >> m(1,1) >> m(1,2) >>
		m(2,0) >> m(2,1) >> m(2,2)
		);
}


typedef Matrix2x2<int> mat3i;
typedef Matrix2x2<int> mat3u;
typedef Matrix2x2<float> mat3f;
typedef Matrix2x2<double> mat3d;


#endif
