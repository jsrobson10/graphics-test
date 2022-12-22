
#pragma once

#include <type_traits>
#include <ostream>
#include <array>
#include <cmath>

#define MAT_DO_OP(OP, OT, F) \
	(OT o) const noexcept {\
		matrix<M, N, T> mat;\
\
		for(int i = 0; i < size(); i++)\
		{\
			mat.data[i] = data[i] OP F;\
		}\
\
		return mat;\
	}
#define MAT_MUT_OP(OP, OT, F) \
	(OT o) noexcept {\
		for(int i = 0; i < size(); i++)\
		{\
			data[i] OP F;\
		}\
\
		return *this;\
	}
#define MAT_ALL_OP(OP, OT, F) \
	matrix operator OP MAT_DO_OP(OP, OT, F) \
	matrix& operator OP##= MAT_MUT_OP(OP##=, OT, F)

template <unsigned int M, unsigned int N = M, typename T = double>
class matrix
{
protected:

	std::array<T, M * N> data;

public:
	
	matrix() noexcept
	{

	}

	matrix(const std::array<T, M * N>& data) noexcept
	{
		this->data = data;
	}

	constexpr unsigned int size() const noexcept
	{
		return M * N;
	}

	constexpr unsigned int width() const noexcept
	{
		return N;
	}

	constexpr unsigned int height() const noexcept
	{
		return M;
	}

	void set(int x, int y, T v) noexcept
	{
		data[y * width() + x] = v;
	}

	T get(int x, int y) const noexcept
	{
		return data[y * width() + x];
	}
	
	friend std::ostream& operator<<(std::ostream& o, const matrix& mat)
	{
		if(N == 1)
		{
			o << "vector<" << M << ">(";

			for(int i = 0; i < M - 1; i++)
			{
				o << mat.data[i] << ", ";
			}

			return o << mat.data[M - 1] << ")";
		}
		
		else
		{
			o << "matrix<" << M << "x" << N << ">(";
			
			for(int y = 0; y < M; y++)
			{
				if(y > 0)
				{
					o << ", ";
				}

				o << y << ": [";

				for(int x = 0; x < N - 1; x++)
				{
					o << mat.get(x, y) << ", ";
				}

				o << mat.get(N - 1, y) << "]";
			}

			return o << ")";
		}
	}

	template <unsigned int t_M = M, unsigned int t_N = N>
	constexpr static typename std::enable_if<t_M == t_N, matrix>::type identity() noexcept
	{
		matrix mat;

		for(int y = 0; y < M; y++)
		{
			for(int x = 0; x < N; x++)
			{
				mat.set(x, y, (x == y) ? 1 : 0);
			}
		}

		return mat;
	}

	template <unsigned int t_M = M, unsigned int t_N = N>
	typename std::enable_if<t_N == t_M, matrix&>::type operator *= (const matrix& o) noexcept
	{
		for(int y = 0; y < M; y++)
		{
			for(int x = 0; x < N; x++)
			{
				T sum = 0;

				for(int i = 0; i < N; i++)
				{
					sum += get(i, y) * o.get(x, i);
				}

				set(x, y, sum);
			}
		}

		return *this;
	}

	template <unsigned int t_N, unsigned int t_P, unsigned int c_N = N>
	typename std::enable_if<t_N == c_N, matrix<M, t_P, T>>::type operator * (const matrix<t_N, t_P, T>& o) const noexcept
	{
		matrix<M, t_P, T> mat;

		for(int y = 0; y < M; y++)
		{
			for(int x = 0; x < t_P; x++)
			{
				T sum = 0;

				for(int i = 0; i < N; i++)
				{
					sum += get(i, y) * o.get(x, i);
				}

				mat.set(x, y, sum);
			}
		}

		return mat;
	}

	template <unsigned int t_N = N>
	typename std::enable_if<t_N == 1, matrix&>::type normalize() noexcept
	{
		T sum = 0;

		for(int i = 0; i < M; i++)
		{
			T v = data[i];
			sum += v * v;
		}

		if(sum == 0)
		{
			return *this;
		}

		T m = 1 / std::sqrt(sum);

		for(int i = 0; i < M; i++)
		{
			data[i] *= m;
		}

		return *this;
	}

	template <unsigned int t_N = N>
	typename std::enable_if<t_N == 1, T>::type length2() const noexcept
	{
		T sum = 0;

		for(int i = 0; i < M; i++)
		{
			T v = data[i];
			sum += v * v;
		}

		return sum;
	}

	template <unsigned int t_N = N>
	typename std::enable_if<t_N == 1, T>::type length() const noexcept
	{
		T sum = 0;

		for(int i = 0; i < M; i++)
		{
			T v = data[i];
			sum += v * v;
		}

		return std::sqrt(sum);
	}

	bool operator == (const matrix& o) const noexcept
	{
		for(int i = 0; i < size(); i++)
		{
			if(data[i] != o.data[i])
			{
				return false;
			}
		}

		return true;
	}

	MAT_ALL_OP(+, const matrix&, o.data[i]);
	MAT_ALL_OP(-, const matrix&, o.data[i]);
	MAT_ALL_OP(+, T, o);
	MAT_ALL_OP(-, T, o);
	MAT_ALL_OP(*, T, o);
	MAT_ALL_OP(/, T, o);
};

#undef MAT_DO_OP
#undef MAT_MUT_OP
#undef MAT_ALL_OP

