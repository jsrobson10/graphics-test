
#pragma once

#include <type_traits>
#include <ostream>
#include <array>
#include <cmath>

#define CN ,

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
#define MAT_CONVERT(LIMIT, SIZE, NAME, JUMP) \
	template <class ST, unsigned int t_M = M, unsigned int t_N = N>\
		constexpr typename std::enable_if<t_N == 1 && t_M >= LIMIT, matrix_sub_t<SIZE, 1>>::type NAME() { return matrix_sub_t<SIZE, 1>(matrix_lookup<T, t_M, STORE_T>(JUMP, data)); }
#define MAT_MNTST matrix<M, N, T, ST>

template <typename T, size_t N, class STORE_T>
struct matrix_lookup
{
	std::array<int, N> lookup;
	STORE_T* data;

	matrix_lookup() : data(nullptr) { }
	matrix_lookup(const std::array<int, N>& lookup, STORE_T& data) : lookup(lookup), data(&data) { }

	const T& operator[](size_t id) const { return (*data)[lookup[id]]; }
	      T& operator[](size_t id)       { return (*data)[lookup[id]]; }
};

template <unsigned int M, unsigned int N = M, typename T = double, class STORE_T = std::array<T, M * N>>
class matrix
{
private:

	STORE_T data;

	template <unsigned int t_M, unsigned int t_N>
	using matrix_sub_t = matrix<t_M, t_N, T, matrix_lookup<T, t_M * t_N, STORE_T>>;

public:
	
	matrix() : data() { }
	matrix(const STORE_T& data) : data(data) { }

	constexpr unsigned int size()   const noexcept { return M * N; }
	constexpr unsigned int width()  const noexcept { return N; }
	constexpr unsigned int height() const noexcept { return M; }

	void set(int x, int y, T v) noexcept
	{
		data[y * width() + x] = v;
	}

	const T& get(int x, int y) const noexcept
	{
		return data[y * width() + x];
	}
	
	T& get(int x, int y) noexcept
	{
		return data[y * width() + x];
	}

	operator matrix<M, N, T, std::array<T, M * N>>()
	{
		matrix<M, N, T, std::array<T, M * N>> m;

		for(int i = 0; i < size(); i++)
		{
			m.data[i] = data[i];
		}

		return m;
	}

	friend std::ostream& operator<<(std::ostream& o, const matrix& mat)
	{
		if(N == 1)
		{
			o << "vec<" << M << ">(";

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

	template <class ST, unsigned int t_M = M, unsigned int t_N = N>
	typename std::enable_if<t_N == t_M, matrix&>::type operator *= (const matrix<M, N, T, ST>& o) noexcept
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

	template <class ST, unsigned int t_N, unsigned int t_P>
	typename std::enable_if<t_N == N, matrix<M, t_P, T>>::type operator * (const matrix<t_N, t_P, T, ST>& o) const noexcept
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

	template <class ST>
	bool operator == (const matrix<M, N, T, ST>& o) const noexcept
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

	template <unsigned int t_M, unsigned int t_N>
	typename std::enable_if<t_M >= 1 && t_N >= 1 && t_M * t_N <= M * N, matrix_sub_t<t_M, t_N>>::type sub_matrix(const std::array<int, t_M * t_N>& lookup)
	{
		return matrix_sub_t<t_M, t_N>(matrix_lookup<T, t_M * t_N, STORE_T>(lookup, data));
	}

	template <unsigned int t_M>
	typename std::enable_if<t_M >= 1 && t_M <= M * N, matrix_sub_t<t_M, 1>>::type sub_matrix(const std::array<int, t_M>& lookup)
	{
		return matrix_sub_t<t_M, 1>(matrix_lookup<T, t_M, STORE_T>(lookup, data));
	}

	constexpr matrix_sub_t<M, 1> col(int x)
	{
		std::array<int, M> lookup;

		for(int y = 0; y < M; y++)
		{
			lookup[y] = y * N + x;
		}

		return matrix_sub_t<M, 1>(matrix_lookup<T, M, STORE_T>(lookup, data));
	}

	constexpr matrix_sub_t<N, 1> row(int y)
	{
		std::array<int, N> lookup;
		int off = y * N;

		for(int x = 0; x < N; x++)
		{
			lookup[x] = off + x;
		}

		return matrix_sub_t<N, 1>(matrix_lookup<T, N, STORE_T>(lookup, data));
	}
	
	constexpr std::array<matrix_sub_t<M, 1>, N> cols()
	{
		std::array<matrix_sub_t<M, 1>, N> arr;

		for(int i = 0; i < N; i++)
		{
			arr[i] = col(i);
		}

		return arr;
	}

	constexpr std::array<matrix_sub_t<N, 1>, M> rows()
	{
		std::array<matrix_sub_t<N, 1>, M> arr;

		for(int i = 0; i < M; i++)
		{
			arr[i] = row(i);
		}

		return arr;
	}

	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 1, T&>::type x() { return data[0]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 2, T&>::type y() { return data[1]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 3, T&>::type z() { return data[2]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 4, T&>::type w() { return data[3]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 3, T&>::type r() { return data[0]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 3, T&>::type g() { return data[1]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 3, T&>::type b() { return data[2]; }
	template <unsigned int t_M = M, unsigned int t_N = N> constexpr typename std::enable_if<t_N == 1 && t_M >= 4, T&>::type a() { return data[3]; }

	MAT_CONVERT(2, 2, xy, {0 CN 1}); MAT_CONVERT(2, 2, yx, {1 CN 0}); MAT_CONVERT(3, 2, xz, {0 CN 2});
	MAT_CONVERT(3, 2, yz, {1 CN 2}); MAT_CONVERT(3, 2, zx, {2 CN 0}); MAT_CONVERT(3, 2, zy, {2 CN 1});
	MAT_CONVERT(3, 2, rg, {0 CN 1}); MAT_CONVERT(3, 2, gr, {1 CN 0}); MAT_CONVERT(3, 2, rb, {0 CN 2});
	MAT_CONVERT(3, 2, gb, {1 CN 2}); MAT_CONVERT(3, 2, br, {2 CN 0}); MAT_CONVERT(3, 2, bg, {2 CN 1});

	MAT_CONVERT(3, 3, xyz, {0 CN 1 CN 2}); MAT_CONVERT(3, 3, xzy, {0 CN 2 CN 1}); MAT_CONVERT(3, 3, yzx, {1 CN 2 CN 0});
	MAT_CONVERT(3, 3, yxz, {1 CN 0 CN 2}); MAT_CONVERT(3, 3, zxy, {2 CN 0 CN 1}); MAT_CONVERT(3, 3, zyx, {2 CN 1 CN 0});
	MAT_CONVERT(3, 3, rgb, {0 CN 1 CN 2}); MAT_CONVERT(3, 3, rbg, {0 CN 2 CN 1}); MAT_CONVERT(3, 3, gbr, {1 CN 2 CN 0});
	MAT_CONVERT(3, 3, grb, {1 CN 0 CN 2}); MAT_CONVERT(3, 3, brg, {2 CN 0 CN 1}); MAT_CONVERT(3, 3, bgr, {2 CN 1 CN 0});

	MAT_CONVERT(4, 4, xyzw, {0 CN 1 CN 2 CN 3}); MAT_CONVERT(4, 4, xzyw, {0 CN 2 CN 1 CN 3}); MAT_CONVERT(4, 4, yzxw, {1 CN 2 CN 0 CN 3});
	MAT_CONVERT(4, 4, yxzw, {1 CN 0 CN 2 CN 3}); MAT_CONVERT(4, 4, zxyw, {2 CN 0 CN 1 CN 3}); MAT_CONVERT(4, 4, zyxw, {2 CN 1 CN 0 CN 3});
	MAT_CONVERT(4, 4, xywz, {0 CN 1 CN 3 CN 2}); MAT_CONVERT(4, 4, xzwy, {0 CN 2 CN 3 CN 1}); MAT_CONVERT(4, 4, yzwx, {1 CN 2 CN 3 CN 0});
	MAT_CONVERT(4, 4, yxwz, {1 CN 0 CN 3 CN 2}); MAT_CONVERT(4, 4, zxwy, {2 CN 0 CN 3 CN 1}); MAT_CONVERT(4, 4, zywx, {2 CN 1 CN 3 CN 0});
	MAT_CONVERT(4, 4, xwyz, {0 CN 3 CN 1 CN 2}); MAT_CONVERT(4, 4, xwzy, {0 CN 3 CN 2 CN 1}); MAT_CONVERT(4, 4, ywzx, {1 CN 3 CN 2 CN 0});
	MAT_CONVERT(4, 4, ywxz, {1 CN 3 CN 0 CN 2}); MAT_CONVERT(4, 4, zwxy, {2 CN 3 CN 0 CN 1}); MAT_CONVERT(4, 4, zwyx, {2 CN 3 CN 1 CN 0});
	MAT_CONVERT(4, 4, wxyz, {3 CN 0 CN 1 CN 2}); MAT_CONVERT(4, 4, wxzy, {3 CN 0 CN 2 CN 1}); MAT_CONVERT(4, 4, wyzx, {3 CN 1 CN 2 CN 0});
	MAT_CONVERT(4, 4, wyxz, {3 CN 1 CN 0 CN 2}); MAT_CONVERT(4, 4, wzxy, {3 CN 2 CN 0 CN 1}); MAT_CONVERT(4, 4, wzyx, {3 CN 2 CN 1 CN 0});
	MAT_CONVERT(4, 4, rgba, {0 CN 1 CN 2 CN 3}); MAT_CONVERT(4, 4, rbga, {0 CN 2 CN 1 CN 3}); MAT_CONVERT(4, 4, gbra, {1 CN 2 CN 0 CN 3});
	MAT_CONVERT(4, 4, grba, {1 CN 0 CN 2 CN 3}); MAT_CONVERT(4, 4, brga, {2 CN 0 CN 1 CN 3}); MAT_CONVERT(4, 4, bgra, {2 CN 1 CN 0 CN 3});
	MAT_CONVERT(4, 4, rgab, {0 CN 1 CN 3 CN 2}); MAT_CONVERT(4, 4, rbag, {0 CN 2 CN 3 CN 1}); MAT_CONVERT(4, 4, gbar, {1 CN 2 CN 3 CN 0});
	MAT_CONVERT(4, 4, grab, {1 CN 0 CN 3 CN 2}); MAT_CONVERT(4, 4, brag, {2 CN 0 CN 3 CN 1}); MAT_CONVERT(4, 4, bgar, {2 CN 1 CN 3 CN 0});
	MAT_CONVERT(4, 4, ragb, {0 CN 3 CN 1 CN 2}); MAT_CONVERT(4, 4, rabg, {0 CN 3 CN 2 CN 1}); MAT_CONVERT(4, 4, gabr, {1 CN 3 CN 2 CN 0});
	MAT_CONVERT(4, 4, garb, {1 CN 3 CN 0 CN 2}); MAT_CONVERT(4, 4, barg, {2 CN 3 CN 0 CN 1}); MAT_CONVERT(4, 4, bagr, {2 CN 3 CN 1 CN 0});
	MAT_CONVERT(4, 4, argb, {3 CN 0 CN 1 CN 2}); MAT_CONVERT(4, 4, arbg, {3 CN 0 CN 2 CN 1}); MAT_CONVERT(4, 4, agbr, {3 CN 1 CN 2 CN 0});
	MAT_CONVERT(4, 4, agrb, {3 CN 1 CN 0 CN 2}); MAT_CONVERT(4, 4, abrg, {3 CN 2 CN 0 CN 1}); MAT_CONVERT(4, 4, abgr, {3 CN 2 CN 1 CN 0});

	template <class ST, unsigned int t_N = N> typename
		std::enable_if<t_N == 1, matrix>::type operator * MAT_DO_OP(*, const MAT_MNTST&, o.data[i]);
	template <class ST, unsigned int t_N = N> typename
		std::enable_if<t_N == 1, matrix&>::type operator *= MAT_MUT_OP(*, const MAT_MNTST&, o.data[i]);
	template <class ST, unsigned int t_N = N> typename
		std::enable_if<t_N == 1, matrix>::type operator / MAT_DO_OP(/, const MAT_MNTST&, o.data[i]);
	template <class ST, unsigned int t_N = N> typename
		std::enable_if<t_N == 1, matrix&>::type operator /= MAT_MUT_OP(/, const MAT_MNTST&, o.data[i]);
	template <class ST>
		matrix<M, N, T, STORE_T> operator + MAT_DO_OP(+, const MAT_MNTST&, o.data[i]);
	template <class ST>
		matrix<M, N, T, STORE_T>& operator += MAT_MUT_OP(+, const MAT_MNTST&, o.data[i]);
	template <class ST>
		matrix<M, N, T, STORE_T> operator - MAT_DO_OP(-, const MAT_MNTST&, o.data[i]);
	template <class ST>
		matrix<M, N, T, STORE_T>& operator -= MAT_MUT_OP(-, const MAT_MNTST&, o.data[i]);

	MAT_ALL_OP(+, T, o);
	MAT_ALL_OP(-, T, o);
	MAT_ALL_OP(*, T, o);
	MAT_ALL_OP(/, T, o);
};

#undef MAT_DO_OP
#undef MAT_MUT_OP
#undef MAT_ALL_OP

