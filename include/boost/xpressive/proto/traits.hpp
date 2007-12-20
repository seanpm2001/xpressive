///////////////////////////////////////////////////////////////////////////////
/// \file traits.hpp
/// Definition of the (meta-)functions for querying for expression tree properties.
//
//  Copyright 2007 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PROTO_TRAITS_HPP_EAN_10_28_2007
#define BOOST_PROTO_TRAITS_HPP_EAN_10_28_2007

#include <boost/type_traits.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/xpressive/proto/proto_fwd.hpp>
#include <boost/xpressive/proto/args.hpp>
#include <boost/xpressive/proto/detail/apply_args.hpp>
#include <boost/xpressive/proto/detail/nary_expr.hpp>
#include <boost/xpressive/proto/detail/is_callable.hpp>
#include <boost/xpressive/proto/detail/arg_c.hpp>
#include <boost/xpressive/proto/detail/define.hpp>

namespace boost { namespace proto
{

    namespace op
    {
        template<typename T>
        struct terminal
        {
            typedef expr<tag::terminal, term<T> > type;
            typedef type proto_base_expr;

            template<typename Sig>
            struct result;

            template<typename This, typename Expr, typename State, typename Visitor>
            struct result<This(Expr, State, Visitor)>
            {
                typedef Expr type;
            };

            template<typename Expr, typename State, typename Visitor>
            Expr const &
            operator()(Expr const &expr, State const &, Visitor &) const
            {
                return expr;
            }
        };

        template<typename Tag, typename T>
        struct unary_expr
        {
            typedef expr<Tag, args<T> > type;
            typedef type proto_base_expr;

            template<typename Sig>
            struct result;

            template<typename This, typename Expr, typename State, typename Visitor>
            struct result<This(Expr, State, Visitor)>
            {
                typedef expr<UNREF(Expr)::proto_tag, args<
                    typename boost::result_of<
                        T(typename result_of::arg_c<Expr, 0>::type, State, Visitor)
                    >::type
                > > type;
            };

            template<typename Expr, typename State, typename Visitor>
            typename result<unary_expr(Expr const &, State const &, Visitor &)>::type
            operator()(Expr const &expr, State const &state, Visitor &visitor) const
            {
                typename result<unary_expr(Expr const &, State const &, Visitor &)>::type that = {
                    {T()(proto::arg_c<0>(expr), state, visitor)}
                };
                return that;
            }
        };

        template<typename Tag, typename T, typename U>
        struct binary_expr
        {
            typedef expr<Tag, args<T, U> > type;
            typedef type proto_base_expr;

            template<typename Sig>
            struct result;

            template<typename This, typename Expr, typename State, typename Visitor>
            struct result<This(Expr, State, Visitor)>
            {
                typedef expr<UNREF(Expr)::proto_tag, args<
                    typename boost::result_of<T(typename result_of::arg_c<Expr, 0>::type, State, Visitor)>::type
                  , typename boost::result_of<U(typename result_of::arg_c<Expr, 1>::type, State, Visitor)>::type
                > > type;
            };

            template<typename Expr, typename State, typename Visitor>
            typename result<binary_expr(Expr const &, State const &, Visitor &)>::type
            operator()(Expr const &expr, State const &state, Visitor &visitor) const
            {
                typename result<binary_expr(Expr const &, State const &, Visitor &)>::type that = {
                    {T()(proto::arg_c<0>(expr), state, visitor)
                  , {U()(proto::arg_c<1>(expr), state, visitor)}}
                };
                return that;
            }
        };

        template<typename T> struct posit : unary_expr<tag::posit, T> {};
        template<typename T> struct negate : unary_expr<tag::negate, T> {};
        template<typename T> struct dereference : unary_expr<tag::dereference, T> {};
        template<typename T> struct complement : unary_expr<tag::complement, T> {};
        template<typename T> struct address_of : unary_expr<tag::address_of, T> {};
        template<typename T> struct logical_not : unary_expr<tag::logical_not, T> {};
        template<typename T> struct pre_inc : unary_expr<tag::pre_inc, T> {};
        template<typename T> struct pre_dec : unary_expr<tag::pre_dec, T> {};
        template<typename T> struct post_inc : unary_expr<tag::post_inc, T> {};
        template<typename T> struct post_dec : unary_expr<tag::post_dec, T> {};

        template<typename T, typename U> struct shift_left : binary_expr<tag::shift_left, T, U> {};
        template<typename T, typename U> struct shift_right : binary_expr<tag::shift_right, T, U> {};
        template<typename T, typename U> struct multiplies : binary_expr<tag::multiplies, T, U> {};
        template<typename T, typename U> struct divides : binary_expr<tag::divides, T, U> {};
        template<typename T, typename U> struct modulus : binary_expr<tag::modulus, T, U> {};
        template<typename T, typename U> struct plus : binary_expr<tag::plus, T, U> {};
        template<typename T, typename U> struct minus : binary_expr<tag::minus, T, U> {};
        template<typename T, typename U> struct less : binary_expr<tag::less, T, U> {};
        template<typename T, typename U> struct greater : binary_expr<tag::greater, T, U> {};
        template<typename T, typename U> struct less_equal : binary_expr<tag::less_equal, T, U> {};
        template<typename T, typename U> struct greater_equal : binary_expr<tag::greater_equal, T, U> {};
        template<typename T, typename U> struct equal_to : binary_expr<tag::equal_to, T, U> {};
        template<typename T, typename U> struct not_equal_to : binary_expr<tag::not_equal_to, T, U> {};
        template<typename T, typename U> struct logical_or : binary_expr<tag::logical_or, T, U> {};
        template<typename T, typename U> struct logical_and : binary_expr<tag::logical_and, T, U> {};
        template<typename T, typename U> struct bitwise_and : binary_expr<tag::bitwise_and, T, U> {};
        template<typename T, typename U> struct bitwise_or : binary_expr<tag::bitwise_or, T, U> {};
        template<typename T, typename U> struct bitwise_xor : binary_expr<tag::bitwise_xor, T, U> {};
        template<typename T, typename U> struct comma : binary_expr<tag::comma, T, U> {};
        template<typename T, typename U> struct mem_ptr : binary_expr<tag::mem_ptr, T, U> {};

        template<typename T, typename U> struct assign : binary_expr<tag::assign, T, U> {};
        template<typename T, typename U> struct shift_left_assign : binary_expr<tag::shift_left_assign, T, U> {};
        template<typename T, typename U> struct shift_right_assign : binary_expr<tag::shift_right_assign, T, U> {};
        template<typename T, typename U> struct multiplies_assign : binary_expr<tag::multiplies_assign, T, U> {};
        template<typename T, typename U> struct divides_assign : binary_expr<tag::divides_assign, T, U> {};
        template<typename T, typename U> struct modulus_assign : binary_expr<tag::modulus_assign, T, U> {};
        template<typename T, typename U> struct plus_assign : binary_expr<tag::plus_assign, T, U> {};
        template<typename T, typename U> struct minus_assign : binary_expr<tag::minus_assign, T, U> {};
        template<typename T, typename U> struct bitwise_and_assign : binary_expr<tag::bitwise_and_assign, T, U> {};
        template<typename T, typename U> struct bitwise_or_assign : binary_expr<tag::bitwise_or_assign, T, U> {};
        template<typename T, typename U> struct bitwise_xor_assign : binary_expr<tag::bitwise_xor_assign, T, U> {};
        template<typename T, typename U> struct subscript : binary_expr<tag::subscript, T, U> {};

    }

    namespace result_of
    {

        template<typename Expr, long N>
        struct value_at_c
          : detail::value_at_c<typename Expr::proto_args::cons_type, N>
        {};

        template<typename Expr, long N>
        struct value_at_c<Expr &, N>
          : detail::value_at_c<typename Expr::proto_args::cons_type, N>
        {};

        template<typename Expr, long N>
        struct arg_c
          : detail::arg_c<typename Expr::proto_args::cons_type, N>
        {};

        template<typename Expr, long N>
        struct arg_c<Expr const, N>
          : detail::arg_cv<typename Expr::proto_args::cons_type, N>
        {};

        template<typename Expr, long N>
        struct arg_c<Expr &, N>
          : arg_c<Expr, N>
        {};

        template<typename Expr>
        struct arg
          : arg_c<Expr, 0>
        {};

        template<typename Expr>
        struct left
          : arg_c<Expr, 0>
        {};

        template<typename Expr>
        struct right
          : arg_c<Expr, 1>
        {};

        // as_expr
        template<typename T, typename Domain, typename EnableIf>
        struct as_expr
        {
            typedef typename mpl::if_<
                is_function<UNREF(T)>
              , T
              , UNCVREF(T)
            >::type value_type;

            typedef expr<tag::terminal, term<value_type> > expr_type;
            typedef typename Domain::template apply<expr_type>::type type;

            static type const call(CVREF(T) t)
            {
                return Domain::make(proto::construct<expr_type>(t));
            }
        };

        template<typename T, typename Domain>
        struct as_expr<T, Domain, typename T::proto_is_expr_>
        {
            typedef typename T::proto_derived_expr type;  // strips the cv-qualification

            static type const call(T const &t)
            {
                return t;
            }
        };

        template<typename T, typename Domain>
        struct as_expr<T &, Domain, typename T::proto_is_expr_>
        {
            typedef typename T::proto_derived_expr type; // strips the cv-qualification

            static type const call(T &t)
            {
                return t;
            }
        };

        template<typename T, typename Domain, typename EnableIf>
        struct as_expr_ref
        {
            typedef expr<tag::terminal, term<CVREF(T) > > expr_type;
            typedef typename Domain::template apply<expr_type>::type type;

            static type const call(CVREF(T) t)
            {
                return Domain::make(proto::construct<expr_type>(t));
            }
        };

        template<typename T, typename Domain>
        struct as_expr_ref<T, Domain, typename T::proto_is_expr_>
        {
            typedef T const &type;

            static T const &call(T const &t)
            {
                return t;
            }
        };

        template<typename T, typename Domain>
        struct as_expr_ref<T &, Domain, typename T::proto_is_expr_>
        {
            typedef T &type;

            static T &call(T &t)
            {
                return t;
            }
        };

        template<typename T, typename EnableIf>
        struct is_expr
          : mpl::false_
        {};

        template<typename T>
        struct is_expr<T, typename T::proto_is_expr_>
          : mpl::true_
        {};

        template<typename T>
        struct is_expr<T &, typename T::proto_is_expr_>
          : mpl::true_
        {};

    }

    namespace functional
    {

        template<typename Domain>
        struct as_expr_ref
        {
            template<typename Sig>
            struct result {};

            template<typename This, typename T>
            struct result<This(T)>
              : result_of::as_expr_ref<T, Domain>
            {};

        #ifdef BOOST_HAS_RVALUE_REFS
            template<typename T>
            typename result_of::as_expr_ref<T, Domain>::type const
            operator ()(T &&t) const
            {
                return result_of::as_expr_ref<T, Domain>::call(t);
            }
        #else
            template<typename T>
            typename result_of::as_expr_ref<T &, Domain>::type const
            operator ()(T &t) const
            {
                return result_of::as_expr_ref<T &, Domain>::call(t);
            }

            template<typename T>
            typename result_of::as_expr_ref<T const &, Domain>::type const
            operator ()(T const &t) const
            {
                return result_of::as_expr_ref<T const &, Domain>::call(t);
            }
        #endif
        };

        template<typename Domain>
        struct as_expr
        {
            template<typename Sig>
            struct result {};

            template<typename This, typename T>
            struct result<This(T)>
              : result_of::as_expr<T, Domain>
            {};

        #ifdef BOOST_HAS_RVALUE_REFS
            template<typename T>
            typename result_of::as_expr<T, Domain>::type const
            operator ()(T &&t) const
            {
                return result_of::as_expr<T, Domain>::call(t);
            }
        #else
            template<typename T>
            typename result_of::as_expr<T &, Domain>::type const
            operator ()(T &t) const
            {
                return result_of::as_expr<T &, Domain>::call(t);
            }

            template<typename T>
            typename result_of::as_expr<T const &, Domain>::type const
            operator ()(T const &t) const
            {
                return result_of::as_expr<T const &, Domain>::call(t);
            }
        #endif
        };

        template<long N>
        struct arg_c
        {
            template<typename Sig>
            struct result {};

            template<typename This, typename Expr>
            struct result<This(Expr)>
              : result_of::arg_c<Expr, N>
            {};

            template<typename Expr>
            typename result_of::arg_c<Expr, N>::type
            operator ()(Expr &expr) const
            {
                return result_of::arg_c<Expr, N>::call(expr);
            }

            template<typename Expr>
            typename result_of::arg_c<Expr const, N>::type
            operator ()(Expr const &expr) const
            {
                return result_of::arg_c<Expr const, N>::call(expr);
            }
        };

        template<typename N>
        struct arg
          : arg_c<N::value>
        {};

        struct left
          : arg_c<0>
        {};

        struct right
          : arg_c<1>
        {};

    }

    template<typename Expr>
    typename result_of::arg<Expr>::type arg(Expr &expr)
    {
        return result_of::arg<Expr>::call(expr.proto_base().proto_args_);
    }

    template<typename Expr>
    typename result_of::arg<Expr const>::type arg(Expr const &expr)
    {
        return result_of::arg<Expr const>::call(expr.proto_base().proto_args_);
    }

    template<typename Expr>
    typename result_of::left<Expr>::type left(Expr &expr)
    {
        return result_of::left<Expr>::call(expr.proto_base().proto_args_);
    }

    template<typename Expr>
    typename result_of::left<Expr const>::type left(Expr const &expr)
    {
        return result_of::left<Expr const>::call(expr.proto_base().proto_args_);
    }

    template<typename Expr>
    typename result_of::right<Expr>::type right(Expr &expr)
    {
        return result_of::right<Expr>::call(expr.proto_base().proto_args_);
    }

    template<typename Expr>
    typename result_of::right<Expr const>::type right(Expr const &expr)
    {
        return result_of::right<Expr const>::call(expr.proto_base().proto_args_);
    }

    template<long N, typename Expr>
    typename result_of::arg_c<Expr, N>::type arg_c(Expr &expr)
    {
        return result_of::arg_c<Expr, N>::call(expr.proto_base().proto_args_);
    }

    template<long N, typename Expr>
    typename result_of::arg_c<Expr const, N>::type arg_c(Expr const &expr)
    {
        return result_of::arg_c<Expr const, N>::call(expr.proto_base().proto_args_);
    }

#ifdef BOOST_HAS_RVALUE_REFS
    template<typename T>
    typename result_of::as_expr<T>::type as_expr(T &&t)
    {
        return result_of::as_expr<T>::call(t);
    }

    template<typename Domain, typename T>
    typename result_of::as_expr<T, Domain>::type as_expr(T &&t)
    {
        return result_of::as_expr<T, Domain>::call(t);
    }

    template<typename T>
    typename result_of::as_expr_ref<T>::type as_expr_ref(T &&t)
    {
        return result_of::as_expr_ref<T>::call(t);
    }

    template<typename Domain, typename T>
    typename result_of::as_expr_ref<T, Domain>::type as_expr_ref(T &&t)
    {
        return result_of::as_expr_ref<T, Domain>::call(t);
    }
#else
    template<typename T>
    typename result_of::as_expr<T &>::type as_expr(T &t)
    {
        return result_of::as_expr<T &>::call(t);
    }

    template<typename Domain, typename T>
    typename result_of::as_expr<T &, Domain>::type as_expr(T &t)
    {
        return result_of::as_expr<T &, Domain>::call(t);
    }

    template<typename T>
    typename result_of::as_expr<T const &>::type as_expr(T const &t)
    {
        return result_of::as_expr<T const &>::call(t);
    }

    template<typename Domain, typename T>
    typename result_of::as_expr<T const &, Domain>::type as_expr(T const &t)
    {
        return result_of::as_expr<T const &, Domain>::call(t);
    }

    template<typename T>
    typename result_of::as_expr_ref<T &>::type as_expr_ref(T &t)
    {
        return result_of::as_expr_ref<T &>::call(t);
    }

    template<typename Domain, typename T>
    typename result_of::as_expr_ref<T &, Domain>::type as_expr_ref(T &t)
    {
        return result_of::as_expr_ref<T &, Domain>::call(t);
    }

    template<typename T>
    typename result_of::as_expr_ref<T const &>::type as_expr_ref(T const &t)
    {
        return result_of::as_expr_ref<T const &>::call(t);
    }

    template<typename Domain, typename T>
    typename result_of::as_expr_ref<T const &, Domain>::type as_expr_ref(T const &t)
    {
        return result_of::as_expr_ref<T const &, Domain>::call(t);
    }
#endif

    /// is_callable
    ///
    template<typename T>
    struct is_callable
      : proto::detail::is_callable_<T>
    {};

    template<>
    struct is_callable<callable>
      : mpl::false_
    {};

    /// is_aggregate
    ///
    template<typename T>
    struct is_aggregate
      : is_pod<T>
    {};

    template<typename Tag, typename Args, long N>
    struct is_aggregate<expr<Tag, Args, N> >
      : mpl::true_
    {};

}}

#include <boost/xpressive/proto/detail/undef.hpp>

#endif
