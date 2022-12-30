
#pragma once

#include "matrix.hpp"
#include "vec.hpp"

namespace matrix_type
{
	template <int N, typename T = double>
	constexpr matrix<N, N, T> identity()
	{
		matrix<N, N, T> mat;

		for(int y = 0; y < N; y++)
		{
			for(int x = 0; x < N; x++)
			{
				mat.set(x, y, (x == y) ? 1 : 0);
			}
		}

		return mat;
	}

	template <int N, typename T = double>
	constexpr matrix<N, N, T> rotation(double angle, int rot_a, int rot_b, double mul)
	{
		matrix<N, N, T> mat = identity<N, T>();

		T v_sin = (T)std::sin(angle);
		T v_cos = (T)std::cos(angle);

		mat.set(rot_a, rot_a, v_cos);
		mat.set(rot_b, rot_b, v_cos);
		mat.set(rot_a, rot_b, v_sin * mul);
		mat.set(rot_b, rot_a, v_sin * -mul);

		return mat;
	}

	template <int N, typename T = double>
	constexpr matrix<N, N, T> rotation(double a, int rot_a, int rot_b)
	{
		return rotation<N, T>(a, rot_a, rot_b, 1);
	}

	template <int N, typename T = double>
	constexpr typename std::enable_if<N >= 2, matrix<N, N, T>>::type rotation2(double a)
	{
		return rotation<N, T>(a, 0, 1);
	}

	template <int N, typename T = double>
	constexpr typename std::enable_if<N >= 3, matrix<N, N, T>>::type rotation3(double a, double b, double g)
	{
		matrix m_a = rotation<N, T>(a, 0, 1);
		matrix m_b = rotation<N, T>(b, 0, 2, -1);
		matrix m_g = rotation<N, T>(g, 1, 2);

		return m_a * m_b * m_g;
	}

	template <int N, typename T = double>
	constexpr matrix<N, N, T> translation(vec<N, T> shift)
	{
		matrix mat = identity<N, T>();

		for(int i = 0; i < N - 1; i++)
		{
			mat.set(N - 1, i, shift[i]);
		}

		return mat;
	}

	template <int N, typename T = double>
	constexpr matrix<N, N, T> scale(vec<N, T> shift)
	{
		matrix mat = identity<N, T>();

		for(int i = 0; i < N; i++)
		{
			mat.set(i, i, shift[i]);
		}

		return mat;
	}
	
	template <int N, typename T = double>
	constexpr typename std::enable_if<N >= 4, matrix<N, N, T>>::type projection(double aspect, double fov, double near, double far)
	{
		matrix mat = identity<N, T>();

		double v_tan = std::tan(fov / 2);
		double diff = far - near;

		mat.set(0, 0, 1 / (aspect * v_tan));
		mat.set(1, 1, 1 / v_tan);
		mat.set(2, 2, -(far + near) / diff);
		mat.set(3, 2, -(2 * far * near) / diff);
		mat.set(2, 3, -1);
		mat.set(3, 3, 0);

		return mat;
	}
};

