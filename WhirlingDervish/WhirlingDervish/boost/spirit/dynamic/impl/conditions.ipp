/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2002-2003 Martin Wille
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef BOOST_SPIRIT_CONDITIONS_IPP
#define BOOST_SPIRIT_CONDITIONS_IPP

///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/core/meta/parser_traits.hpp>
#include <boost/spirit/core/composite/epsilon.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    namespace impl {

///////////////////////////////////////////////////////////////////////////////
//
// condition evaluation
//
///////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////
    // condition_parser_selector, decides which parser to use for a condition
    // If the template argument is a parser then that parser is used.
    // If the template argument is a functor then a condition parser using
    // the functor is chosen

    template <typename T> struct embed_t_accessor
    {
        typedef typename T::embed_t type;
    };

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
    template <> struct embed_t_accessor<int>
    {
        typedef int type;
    };
#endif

    template <typename ConditionT>
    struct condition_parser_selector
    {
        typedef
            typename mpl::if_<
                    is_parser<ConditionT>,
                    ConditionT,
                    condition_parser<ConditionT>
                >::type
            type;

        typedef typename embed_t_accessor<type>::type embed_t;
    };

    //////////////////////////////////
    // condition_evaluator, uses a parser to check wether a condition is met
    // takes a parser or a functor that can be evaluated in boolean context
    // as template parameter.

    // JDG 4-15-03 refactored
    template <typename ConditionT>
    struct condition_evaluator
    {
        typedef condition_parser_selector<ConditionT>       selector_t;
        typedef typename selector_t::type                   selected_t;
        typedef typename selector_t::embed_t                cond_embed_t;

        typedef typename boost::call_traits<cond_embed_t>::param_type
            param_t;

        condition_evaluator(param_t s) : cond(s) {}

        /////////////////////////////
        // evaluate, checks wether condition is met
        // returns length of a match or a negative number for no-match
        template <typename ScannerT>
        int
        evaluate(ScannerT const &scan) const
        {
            typedef typename ScannerT::iterator_t iterator_t;
            typedef typename parser_result<selected_t, ScannerT>::type cres_t;
            iterator_t save(scan.first);
            cres_t result = cond.parse(scan);
            if (!result)            // reset the position if evaluation
                scan.first = save;  // fails.
            return result.length();
        }

        cond_embed_t cond;
    };

///////////////////////////////////////////////////////////////////////////////
    } // namespace impl

}} // namespace boost::spirit

#endif
