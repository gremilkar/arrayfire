/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <Array.hpp>
#include <optypes.hpp>
#include <err_cpu.hpp>
#include <jit/UnaryNode.hpp>
#include <cmath>

namespace cpu
{

template<typename T>
T sigmoid(T in)
{
    return (1.0) / (1 + std::exp(-in));
}

#define UNARY_OP_FN(op, fn)                         \
    template<typename T>                            \
    struct UnOp<T, T, af_##op##_t>                  \
    {                                               \
        void eval(jit::array<T> &out,               \
                  const jit::array<T> &in, int lim) \
        {                                           \
            for (int i = 0; i < lim; i++) {         \
                out[i] = fn(in[i]);                 \
            }                                       \
        }                                           \
    };                                              \

#define UNARY_OP(op) UNARY_OP_FN(op, std::op)

UNARY_OP(sin)
UNARY_OP(cos)
UNARY_OP(tan)

UNARY_OP(asin)
UNARY_OP(acos)
UNARY_OP(atan)

UNARY_OP(sinh)
UNARY_OP(cosh)
UNARY_OP(tanh)

UNARY_OP(asinh)
UNARY_OP(acosh)
UNARY_OP(atanh)

UNARY_OP(round)
UNARY_OP(trunc)
UNARY_OP(signbit)
UNARY_OP(floor)
UNARY_OP(ceil)

UNARY_OP(exp)
UNARY_OP_FN(sigmoid, sigmoid)
UNARY_OP(expm1)
UNARY_OP(erf)
UNARY_OP(erfc)

UNARY_OP(log)
UNARY_OP(log10)
UNARY_OP(log1p)
UNARY_OP(log2)

UNARY_OP(sqrt)
UNARY_OP(cbrt)

UNARY_OP(tgamma)
UNARY_OP(lgamma)

#undef UNARY_OP
#undef UNARY_OP_FN

    template<typename T, af_op_t op>
    Array<T> unaryOp(const Array<T> &in)
    {
        jit::Node_ptr in_node = in.getNode();
        jit::UnaryNode<T, T, op> *node = new jit::UnaryNode<T, T, op>(in_node);

        return createNodeArray<T>(in.dims(), jit::Node_ptr(node));
    }

#define iszero(a) ((a) == 0)

#define CHECK_FN(name ,op)                          \
    template<typename T>                            \
    struct UnOp<char, T, af_##name##_t>             \
    {                                               \
        void eval(jit::array<char> &out,            \
                  const jit::array<T> &in, int lim) \
        {                                           \
            for (int i = 0; i < lim; i++) {         \
                out[i] = op(in[i]);                 \
            }                                       \
        }                                           \
    };                                              \

    CHECK_FN(isinf, std::isinf)
    CHECK_FN(isnan, std::isnan)
    CHECK_FN(iszero, iszero)

    template<typename T, af_op_t op>
    Array<char> checkOp(const Array<T> &in)
    {
        jit::Node_ptr in_node = in.getNode();
        jit::UnaryNode<char, T, op> *node = new jit::UnaryNode<char, T, op>(in_node);

        return createNodeArray<char>(in.dims(), jit::Node_ptr(node));
    }

}
