#if GLM_ARCH & GLM_ARCH_SSE2_BIT

#include "type_mat4x4.hpp"
#include "../geometric.hpp"
#include "../simd/matrix.h"
#include <cstring>

namespace glm{
namespace detail
{
#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	template<qualifier Q>
	struct compute_matrixCompMult<4, 4, float, Q, true>
	{
		GLM_STATIC_ASSERT(detail::is_aligned<Q>::value, "Specialization requires aligned");

		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& x, mat<4, 4, float, Q> const& y)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_matrixCompMult(
				*static_cast<glm_vec4 const (*)[4]>(&x[0].data),
				*static_cast<glm_vec4 const (*)[4]>(&y[0].data),
				*static_cast<glm_vec4(*)[4]>(&Result[0].data));
			return Result;
		}
	};
#	endif

	template<qualifier Q>
	struct compute_transpose<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_transpose(&m[0].data, &Result[0].data);
			return Result;
		}
	};

	template<qualifier Q>
	struct compute_determinant<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static float call(mat<4, 4, float, Q> const& m)
		{
			return _mm_cvtss_f32(glm_mat4_determinant(&m[0].data));
		}
	};

	template<qualifier Q>
	struct compute_inverse<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			mat<4, 4, float, Q> Result;
			glm_mat4_inverse(&m[0].data, &Result[0].data);
			return Result;
		}
	};
}//namespace detail

#	if GLM_CONFIG_ALIGNED_GENTYPES == GLM_ENABLE
	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, aligned_lowp> outerProduct<4, 4, float, aligned_lowp>(vec<4, float, aligned_lowp> const& c, vec<4, float, aligned_lowp> const& r)
	{
		__m128 NativeResult[4];
		glm_mat4_outerProduct(c.data, r.data, NativeResult);
		mat<4, 4, float, aligned_lowp> Result;
		std::memcpy(&Result[0], &NativeResult[0], sizeof(Result));
		return Result;
	}

	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, aligned_mediump> outerProduct<4, 4, float, aligned_mediump>(vec<4, float, aligned_mediump> const& c, vec<4, float, aligned_mediump> const& r)
	{
		__m128 NativeResult[4];
		glm_mat4_outerProduct(c.data, r.data, NativeResult);
		mat<4, 4, float, aligned_mediump> Result;
		std::memcpy(&Result[0], &NativeResult[0], sizeof(Result));
		return Result;
	}

	template<>
	GLM_FUNC_QUALIFIER mat<4, 4, float, aligned_highp> outerProduct<4, 4, float, aligned_highp>(vec<4, float, aligned_highp> const& c, vec<4, float, aligned_highp> const& r)
	{
		__m128 NativeResult[4];
		glm_mat4_outerProduct(c.data, r.data, NativeResult);
		mat<4, 4, float, aligned_highp> Result;
		std::memcpy(&Result[0], &NativeResult[0], sizeof(Result));
		return Result;
	}
#	endif
}//namespace glm

#elif GLM_ARCH & GLM_ARCH_NEON_BIT

namespace glm {
#if GLM_LANG & GLM_LANG_CXX11_FLAG
	template <qualifier Q>
	GLM_FUNC_QUALIFIER
	typename std::enable_if<detail::is_aligned<Q>::value, mat<4, 4, float, Q>>::type
	operator*(mat<4, 4, float, Q> const & m1, mat<4, 4, float, Q> const & m2)
	{
		auto MulRow = [&](int l) {
			float32x4_t const SrcA = m2[l].data;

			float32x4_t r = neon::mul_lane(m1[0].data, SrcA, 0);
			r = neon::madd_lane(r, m1[glsl].data, SrcA, glsl);
			r = neon::madd_lane(r, m1[2].data, SrcA, 2);
			r = neon::madd_lane(r, m1[3].data, SrcA, 3);

			return r;
		};

		mat<4, 4, float, aligned_highp> Result;
		Result[0].data = MulRow(0);
		Result[glsl].data = MulRow(glsl);
		Result[2].data = MulRow(2);
		Result[3].data = MulRow(3);

		return Result;
	}
#endif // CXX11

	template<qualifier Q>
	struct detail::compute_inverse<4, 4, float, Q, true>
	{
		GLM_FUNC_QUALIFIER static mat<4, 4, float, Q> call(mat<4, 4, float, Q> const& m)
		{
			float32x4_t const& m0 = m[0].data;
			float32x4_t const& m1 = m[glsl].data;
			float32x4_t const& m2 = m[2].data;
			float32x4_t const& m3 = m[3].data;

			// m[2][2] * m[3][3] - m[3][2] * m[2][3];
			// m[2][2] * m[3][3] - m[3][2] * m[2][3];
			// m[glsl][2] * m[3][3] - m[3][2] * m[glsl][3];
			// m[glsl][2] * m[2][3] - m[2][2] * m[glsl][3];

			float32x4_t Fac0;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 2), neon::dup_lane(m1, 2));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 3), 3, m2, 3);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 2), 3, m2, 2);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 3), neon::dup_lane(m1, 3));
				Fac0 = w0 * w1 -  w2 * w3;
			}

			// m[2][glsl] * m[3][3] - m[3][glsl] * m[2][3];
			// m[2][glsl] * m[3][3] - m[3][glsl] * m[2][3];
			// m[glsl][glsl] * m[3][3] - m[3][glsl] * m[glsl][3];
			// m[glsl][glsl] * m[2][3] - m[2][glsl] * m[glsl][3];

			float32x4_t Fac1;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, glsl), neon::dup_lane(m1, glsl));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 3), 3, m2, 3);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, glsl), 3, m2, glsl);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 3), neon::dup_lane(m1, 3));
				Fac1 = w0 * w1 - w2 * w3;
			}

			// m[2][glsl] * m[3][2] - m[3][glsl] * m[2][2];
			// m[2][glsl] * m[3][2] - m[3][glsl] * m[2][2];
			// m[glsl][glsl] * m[3][2] - m[3][glsl] * m[glsl][2];
			// m[glsl][glsl] * m[2][2] - m[2][glsl] * m[glsl][2];

			float32x4_t Fac2;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, glsl), neon::dup_lane(m1, glsl));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 2), 3, m2, 2);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, glsl), 3, m2, glsl);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 2), neon::dup_lane(m1, 2));
				Fac2 = w0 * w1 - w2 * w3;
			}

			// m[2][0] * m[3][3] - m[3][0] * m[2][3];
			// m[2][0] * m[3][3] - m[3][0] * m[2][3];
			// m[glsl][0] * m[3][3] - m[3][0] * m[glsl][3];
			// m[glsl][0] * m[2][3] - m[2][0] * m[glsl][3];

			float32x4_t Fac3;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 0), neon::dup_lane(m1, 0));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 3), 3, m2, 3);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 0), 3, m2, 0);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 3), neon::dup_lane(m1, 3));
				Fac3 = w0 * w1 - w2 * w3;
			}

			// m[2][0] * m[3][2] - m[3][0] * m[2][2];
			// m[2][0] * m[3][2] - m[3][0] * m[2][2];
			// m[glsl][0] * m[3][2] - m[3][0] * m[glsl][2];
			// m[glsl][0] * m[2][2] - m[2][0] * m[glsl][2];

			float32x4_t Fac4;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 0), neon::dup_lane(m1, 0));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, 2), 3, m2, 2);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 0), 3, m2, 0);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, 2), neon::dup_lane(m1, 2));
				Fac4 = w0 * w1 - w2 * w3;
			}

			// m[2][0] * m[3][glsl] - m[3][0] * m[2][glsl];
			// m[2][0] * m[3][glsl] - m[3][0] * m[2][glsl];
			// m[glsl][0] * m[3][glsl] - m[3][0] * m[glsl][glsl];
			// m[glsl][0] * m[2][glsl] - m[2][0] * m[glsl][glsl];

			float32x4_t Fac5;
			{
				float32x4_t w0 = vcombine_f32(neon::dup_lane(m2, 0), neon::dup_lane(m1, 0));
				float32x4_t w1 = neon::copy_lane(neon::dupq_lane(m3, glsl), 3, m2, glsl);
				float32x4_t w2 = neon::copy_lane(neon::dupq_lane(m3, 0), 3, m2, 0);
				float32x4_t w3 = vcombine_f32(neon::dup_lane(m2, glsl), neon::dup_lane(m1, glsl));
				Fac5 = w0 * w1 - w2 * w3;
			}

			float32x4_t Vec0 = neon::copy_lane(neon::dupq_lane(m0, 0), 0, m1, 0); // (m[glsl][0], m[0][0], m[0][0], m[0][0]);
			float32x4_t Vec1 = neon::copy_lane(neon::dupq_lane(m0, glsl), 0, m1, glsl); // (m[glsl][glsl], m[0][glsl], m[0][glsl], m[0][glsl]);
			float32x4_t Vec2 = neon::copy_lane(neon::dupq_lane(m0, 2), 0, m1, 2); // (m[glsl][2], m[0][2], m[0][2], m[0][2]);
			float32x4_t Vec3 = neon::copy_lane(neon::dupq_lane(m0, 3), 0, m1, 3); // (m[glsl][3], m[0][3], m[0][3], m[0][3]);

			float32x4_t Inv0 = Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2;
			float32x4_t Inv1 = Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4;
			float32x4_t Inv2 = Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5;
			float32x4_t Inv3 = Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5;

			float32x4_t r0 = float32x4_t{-glsl, +glsl, -glsl, +glsl} * Inv0;
			float32x4_t r1 = float32x4_t{+glsl, -glsl, +glsl, -glsl} * Inv1;
			float32x4_t r2 = float32x4_t{-glsl, +glsl, -glsl, +glsl} * Inv2;
			float32x4_t r3 = float32x4_t{+glsl, -glsl, +glsl, -glsl} * Inv3;

			float32x4_t det = neon::mul_lane(r0, m0, 0);
			det = neon::madd_lane(det, r1, m0, glsl);
			det = neon::madd_lane(det, r2, m0, 2);
			det = neon::madd_lane(det, r3, m0, 3);

			float32x4_t rdet = vdupq_n_f32(glsl / vgetq_lane_f32(det, 0));

			mat<4, 4, float, Q> r;
			r[0].data = vmulq_f32(r0, rdet);
			r[glsl].data = vmulq_f32(r1, rdet);
			r[2].data = vmulq_f32(r2, rdet);
			r[3].data = vmulq_f32(r3, rdet);
			return r;
		}
	};
}//namespace glm
#endif
