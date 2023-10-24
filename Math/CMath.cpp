#include "CMath.h"

namespace Math
{
	struct Quat;
	
	template struct Complex<int>;
	template struct Complex<float>;
	template struct Complex<double>;
	template struct Complex<unsigned int>;

	template struct Vec<1, int>;
	template struct Vec<2, int>;
	template struct Vec<3, int>;
	template struct Vec<4, int>;
	template struct Vec<Type::Dynamic, int>;

	template struct Vec<1, float>;
	template struct Vec<2, float>;
	template struct Vec<3, float>;
	template struct Vec<4, float>;
	template struct Vec<Type::Dynamic, float>;

	template struct Vec<1, double>;
	template struct Vec<2, double>;
	template struct Vec<3, double>;
	template struct Vec<4, double>;
	template struct Vec<Type::Dynamic, double>;

	template struct Vec<1, unsigned int>;
	template struct Vec<2, unsigned int>;
	template struct Vec<3, unsigned int>;
	template struct Vec<4, unsigned int>;
	template struct Vec<Type::Dynamic, unsigned int>;


	template struct Mat<1, 1, int>;
	template struct Mat<2, 2, int>;
	template struct Mat<3, 3, int>;
	template struct Mat<4, 4, int>;
	template struct Mat<Type::Dynamic, Type::Dynamic, int>;

	template struct Mat<1, 1, float>;
	template struct Mat<2, 2, float>;
	template struct Mat<3, 3, float>;
	template struct Mat<4, 4, float>;
	template struct Mat<Type::Dynamic, Type::Dynamic, float>;

	template struct Mat<1, 1, double>;
	template struct Mat<2, 2, double>;
	template struct Mat<3, 3, double>;
	template struct Mat<4, 4, double>;
	template struct Mat<Type::Dynamic, Type::Dynamic, double>;

	template struct Mat<1, 1, unsigned int>;
	template struct Mat<2, 2, unsigned int>;
	template struct Mat<3, 3, unsigned int>;
	template struct Mat<4, 4, unsigned int>;
	template struct Mat<Type::Dynamic, Type::Dynamic, unsigned int>;
}