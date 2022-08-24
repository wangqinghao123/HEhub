/**
 * @file mod_arith.h
 * @brief Modular arithmetic.
 *
 */
#pragma once

#include "rnspolynomial.h"
#include "type_defs.h"
#include <tuple>

namespace hehub {

using MulModLUT = std::tuple<u64, u64, u64>;

/**
 * @brief An optimized method for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). Here we apply a hybrid method, which does a montgomery
 * reduction on in_vec1[i] * in_vec2[i] producing a result equal to (in_vec1[i]
 * * in_vec2[i] * C) % modulus where C is a montgomery constant related to the
 * modulus, and then does a harvey reduction to remove C. (cf.
 * https://en.wikipedia.org/wiki/Montgomery_modular_multiplication and
 * https://doi.org/10.1016/j.jsc.2013.09.002) The resulting out_vec[i]'s are
 * left in [0, 2*modulus).
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in_vec1 Input vector of length vec_len
 * @param[in] in_vec2 Input vector of length vec_len
 * @param[out] out_vec Output vector s.t. out_vec[i] == in_vec1[i] * in_vec2[i]
 * % modulus.
 */
void vector_mul_mod_hybrid_lazy(const u64 modulus, const size_t vec_len,
                                const u64 in_vec1[], const u64 in_vec2[],
                                u64 out_vec[]);

/**
 * @brief An optimized method for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). Here we apply a hybrid method, which does a montgomery
 * reduction on in_vec1[i] * in_vec2[i] producing a result equal to (in_vec1[i]
 * * in_vec2[i] * C) % modulus where C is a montgomery constant related to the
 * modulus, and then does a harvey reduction to remove C. (cf.
 * https://en.wikipedia.org/wiki/Montgomery_modular_multiplication and
 * https://doi.org/10.1016/j.jsc.2013.09.002) The resulting out_vec[i]'s are
 * left in [0, 2*modulus).
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in1 Input vector of length vec_len
 * @param[in] in2 Input vector of length vec_len
 * @param[out] out Output vector s.t. out[i] == in1[i] * in2[i] %
 * modulus.
 */
inline void vector_mul_mod_hybrid_lazy(const u64 modulus, const size_t vec_len,
                                       const RnsPolynomial::ComponentData &in1,
                                       const RnsPolynomial::ComponentData &in2,
                                       RnsPolynomial::ComponentData &out) {
    vector_mul_mod_hybrid_lazy(modulus, vec_len, in1.data(), in2.data(),
                               out.data());
}

/**
 * @brief An optimized method for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). Here we apply a hybrid method, which does a montgomery
 * reduction on in_vec1[i] * in_vec2[i] producing a result equal to (in_vec1[i]
 * * in_vec2[i] * C) % modulus where C is a montgomery constant related to the
 * modulus, and then does a harvey reduction to remove C. (cf.
 * https://en.wikipedia.org/wiki/Montgomery_modular_multiplication and
 * https://doi.org/10.1016/j.jsc.2013.09.002)
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in_vec1 Input vector of length vec_len
 * @param[in] in_vec2 Input vector of length vec_len
 * @param[out] out_vec Output vector s.t. out_vec[i] == in_vec1[i] * in_vec2[i]
 * % modulus.
 */
inline void vector_mul_mod_hybrid(const u64 modulus, const size_t vec_len,
                                  const u64 in_vec1[], const u64 in_vec2[],
                                  u64 out_vec[]) {
    vector_mul_mod_hybrid_lazy(modulus, vec_len, in_vec1, in_vec2, out_vec);
    for (size_t i = 0; i < vec_len; i++) {
        out_vec[i] -= (out_vec[i] >= modulus) ? modulus : 0;
    }
}

/**
 * @brief An optimized method for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). Here we apply a hybrid method, which does a montgomery
 * reduction on in_vec1[i] * in_vec2[i] producing a result equal to (in_vec1[i]
 * * in_vec2[i] * C) % modulus where C is a montgomery constant related to the
 * modulus, and then does a harvey reduction to remove C. (cf.
 * https://en.wikipedia.org/wiki/Montgomery_modular_multiplication and
 * https://doi.org/10.1016/j.jsc.2013.09.002)
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in1 Input vector of length vec_len
 * @param[in] in2 Input vector of length vec_len
 * @param[out] out Output vector s.t. out[i] == in1[i] * in2[i] %
 * modulus.
 */
inline void vector_mul_mod_hybrid(const u64 modulus, const size_t vec_len,
                                  const RnsPolynomial::ComponentData &in1,
                                  const RnsPolynomial::ComponentData &in2,
                                  RnsPolynomial::ComponentData &out) {
    vector_mul_mod_hybrid(modulus, vec_len, in1.data(), in2.data(), out.data());
}

/**
 * @brief Use barrett reduction for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). After the product of in_vec1[i] and in_vec2[i] is
 * produced, barrett reduction works as follows: A barrett constant c is
 * pre-computed as floor(2**128 / modulus). Let the product of in_vec1[i] and
 * in_vec2[i] is a, then floor(a / modulus) ≈ floor(a * c / 2**128), from which
 * we can obtain a % modulus with possibly one additional multiple of modulus. The resulting
 * out_vec[i]'s are left in [0, 2*modulus).
 * @see https://en.wikipedia.org/wiki/Barrett_reduction
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in_vec1 Input vector of length vec_len
 * @param[in] in_vec2 Input vector of length vec_len
 * @param[out] out_vec Output vector s.t. out_vec[i] == in_vec1[i] * in_vec2[i]
 * % modulus.
 */
void vector_mul_mod_barrett_lazy(const u64 modulus, const size_t vec_len,
                                 const u64 in_vec1[], const u64 in_vec2[],
                                 u64 out_vec[]);

/**
 * @brief Use barrett reduction for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). After the product of in_vec1[i] and in_vec2[i] is
 * produced, barrett reduction works as follows: A barrett constant c is
 * pre-computed as floor(2**128 / modulus). Let the product of in_vec1[i] and
 * in_vec2[i] is a, then floor(a / modulus) ≈ floor(a * c / 2**128), from which
 * we can obtain a % modulus with possibly one additional multiple of modulus,
 * cf. https://en.wikipedia.org/wiki/Barrett_reduction. The resulting
 * out_vec[i]'s are left in [0, 2*modulus).
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in1 Input vector of length vec_len
 * @param[in] in2 Input vector of length vec_len
 * @param[out] out Output vector s.t. out[i] == in1[i] * in2[i] %
 * modulus.
 */
inline void vector_mul_mod_barrett_lazy(const u64 modulus, const size_t vec_len,
                                        const RnsPolynomial::ComponentData &in1,
                                        const RnsPolynomial::ComponentData &in2,
                                        RnsPolynomial::ComponentData &out) {
    vector_mul_mod_barrett_lazy(modulus, vec_len, in1.data(), in2.data(),
                                out.data());
}

/**
 * @brief Use barrett reduction for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). After the product of in_vec1[i] and in_vec2[i] is
 * produced, barrett reduction works as follows: A barrett constant c is
 * pre-computed as floor(2**128 / modulus). Let the product of in_vec1[i] and
 * in_vec2[i] is a, then floor(a / modulus) ≈ floor(a * c / 2**128), from which
 * we can obtain a % modulus with possibly one additional multiple of modulus,
 * cf. https://en.wikipedia.org/wiki/Barrett_reduction.
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in_vec1 Input vector of length vec_len
 * @param[in] in_vec2 Input vector of length vec_len
 * @param[out] out_vec Output vector s.t. out_vec[i] == in_vec1[i] * in_vec2[i]
 * % modulus.
 */
inline void vector_mul_mod_barrett(const u64 modulus, const size_t vec_len,
                                   const u64 in_vec1[], const u64 in_vec2[],
                                   u64 out_vec[]) {
    vector_mul_mod_barrett_lazy(modulus, vec_len, in_vec1, in_vec2, out_vec);
    for (size_t i = 0; i < vec_len; i++) {
        out_vec[i] -= (out_vec[i] >= modulus) ? modulus : 0;
    }
}

/**
 * @brief Use barrett reduction for calculating modular multiplication (between
 * vectors for efficiency) for a modulus of less than 64-bit, and input vectors
 * arbituary (non-fixed). After the product of in_vec1[i] and in_vec2[i] is
 * produced, barrett reduction works as follows: A barrett constant c is
 * pre-computed as floor(2**128 / modulus). Let the product of in_vec1[i] and
 * in_vec2[i] is a, then floor(a / modulus) ≈ floor(a * c / 2**128), from which
 * we can obtain a % modulus with possibly one additional multiple of modulus,
 * cf. https://en.wikipedia.org/wiki/Barrett_reduction.
 *
 * @param[in] modulus The modulus.
 * @param[in] vec_len The length of input vectors.
 * @param[in] in1 Input vector of length vec_len
 * @param[in] in2 Input vector of length vec_len
 * @param[out] out Output vector s.t. out[i] == in1[i] * in2[i] %
 * modulus.
 */
inline void vector_mul_mod_barrett(const u64 modulus, const size_t vec_len,
                                   const RnsPolynomial::ComponentData &in1,
                                   const RnsPolynomial::ComponentData &in2,
                                   RnsPolynomial::ComponentData &out) {
    vector_mul_mod_barrett(modulus, vec_len, in1.data(), in2.data(),
                           out.data());
}

/**
 * @brief TODO
 * 
 * @param rns_poly 
 */
void strict_reduce(RnsPolynomial &rns_poly);

} // namespace hehub
