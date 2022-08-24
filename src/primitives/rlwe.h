/**
 * @file rlwe.h
 * @brief Basics of the RLWE scheme.
 */

#include "common/rnspolynomial.h"
#include <array>

namespace hehub {

using RlwePt = RnsPolynomial;

using RlweCt = std::array<RnsPolynomial, 2>;

/**
 * @brief RLWE ternary secret key, which is a polynomial with coefficients
 * randomly sampled from {-1, 0, 1}. For convenience this is represented in RNS
 * and transformed to NTT form after sampling.
 */
class RlweSk : public RnsPolynomial {
public:
    RlweSk();

    /// Initialize the polynomial parameters and sample ternary coefficients.
    RlweSk(const PolyDimensions &poly_dim);
};

} // namespace hehub
