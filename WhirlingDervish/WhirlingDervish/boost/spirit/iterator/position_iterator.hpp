/*=============================================================================
    Spirit V1.6.0
    Copyright (c) 2002 Juan Carlos Arevalo-Baeza
    Copyright (c) 2002-2003 Hartmut Kaiser
    Copyright (c) 2003 Giovanni Bajo
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software
    is granted provided this copyright notice appears in all copies.
    This software is provided "as is" without express or implied
    warranty, and with no claim as to its suitability for any purpose.

    See spirit.hpp for full copyright notices.
=============================================================================*/
#ifndef BOOST_SPIRIT_POSITION_ITERATOR_HPP
#define BOOST_SPIRIT_POSITION_ITERATOR_HPP

///////////////////////////////////////////////////////////////////////////////
#include <string>

#include <boost/config.hpp>
#include <boost/concept_check.hpp>
#include <boost/spirit/core/impl/msvc.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

///////////////////////////////////////////////////////////////////////////////
//
//  file_position_without_column
//
//  A structure to hold positional information. This includes the file,
//  and the line number
//
///////////////////////////////////////////////////////////////////////////////

struct file_position_without_column {
    std::string file;
    int line;

    file_position_without_column(std::string const& file_ = std::string(),
                  int line_ = 1):
        file    (file_),
        line    (line_)
    {}

    bool operator==(const file_position_without_column& fp) const
    { return line == fp.line && file == fp.file; }
};

///////////////////////////////////////////////////////////////////////////////
//
//  file_position
//
//  This structure holds complete file position, including file name,
//  line and column number
//
///////////////////////////////////////////////////////////////////////////////

struct file_position : public file_position_without_column {
    int column;

    file_position(std::string const& file_ = std::string(),
                  int line_ = 1, int column_ = 1):
        file_position_without_column (file_, line_),
        column                       (column_)
    {}

    bool operator==(const file_position& fp) const
    { return column == fp.column && line == fp.line && file == fp.file; }
};


///////////////////////////////////////////////////////////////////////////////
//
//  position_policy<>
//
//  This template is the policy to handle the file position. It is specialized
//  on the position type. Providing a custom file_position also requires
//  providing a specialization of this class.
//
//  Policy interface:
//
//    Default constructor of the custom position class must be accessible.
//    set_tab_chars(unsigned int chars) - Set the tabstop width
//    next_char(PositionT& pos)  - Notify that a new character has been
//      processed
//    tabulation(PositionT& pos) - Notify that a tab character has been
//      processed
//    next_line(PositionT& pos)  - Notify that a new line delimiter has
//      been reached.
//
///////////////////////////////////////////////////////////////////////////////

template <typename PositionT>
class position_policy;


// Foward declaration
template <typename ForwardIteratorT, typename PositionT, typename SelfT>
class position_iterator;

///////////////////////////////////////////////////////////////////////////////
}} /* namespace boost::spirit */


// This must be included here for full compatibility with old MSVC
#include "impl/position_iterator.ipp"


///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

///////////////////////////////////////////////////////////////////////////////
//
//  position_iterator
//
//  It wraps an iterator, and keeps track of the current position in the input,
//  as it gets incremented.
//
//  The wrapped iterator must be at least a Forward iterator. The position
//  iterator itself will always be a non-mutable Forward iterator.
//
//  In order to have begin/end iterators constructed, the end iterator must be
//  empty constructed. Similar to what happens with stream iterators. The begin
//  iterator must be constructed from both, the begin and end iterators of the
//  wrapped iterator type. This is necessary to implement the lookahead of
//  characters necessary to parse CRLF sequences.
//
//  In order to extract the current positional data from the iterator, you may
//  use the get_position member function.
//
//  You can also use the set_position member function to reset the current
//  position to something new.
//
//  The structure that holds the current position can be customized through a
//  template parameter, and the class position_policy must be specialized
//  on the new type to define how to handle it. Currently, it's possible
//  to choose between the file_position and file_position_without_column
//  (which saves some overhead if managing current column is not required).
//
///////////////////////////////////////////////////////////////////////////////

template
<
    typename ForwardIteratorT,
    typename PositionT = file_position,
    typename SelfT = nil_t
>
class position_iterator
    :
      public iterator_::impl::position_iterator_generator
      <
        SelfT,
        ForwardIteratorT,
        PositionT
      >::type
{
private:

#if !defined(__BORLANDC__) || (__BORLANDC__ > 0x551) // JDG 4-15-03
    BOOST_CLASS_REQUIRE(ForwardIteratorT, boost, ForwardIteratorConcept);
#endif

    typedef typename iterator_::impl::position_iterator_generator
    <
        SelfT,
        ForwardIteratorT,
        PositionT
    >::type base_t;

protected:
    typedef typename base_t::policies_type iter_policy_t;

public:

    typedef PositionT position_t;

    position_iterator()
    {}

    position_iterator(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end):
        base_t(begin, iter_policy_t(PositionT(), end))
    {}

    template <typename FileNameT>
    position_iterator(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        FileNameT fileName):
        base_t(begin, iter_policy_t(PositionT(fileName), end))
    {}

    template <typename FileNameT, typename LineT>
    position_iterator(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        FileNameT fileName, LineT line):
        base_t(begin, iter_policy_t(PositionT(fileName, line), end))
    {}

    template <typename FileNameT, typename LineT, typename ColumnT>
    position_iterator(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        FileNameT fileName, LineT line, ColumnT column):
        base_t(begin, iter_policy_t(PositionT(fileName, line, column), end))
    {}

    position_iterator(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        const PositionT& pos):
        base_t(begin, iter_policy_t(PositionT(pos), end))
    {}

    position_iterator(const position_iterator& iter)
        : base_t(iter.base(), iter.policies())
    {}

    position_iterator& operator=(const position_iterator& iter)
    {
        base_t::operator=(iter);
        return *this;
    }

    void set_position(PositionT const& newpos)
    { this->policies()._pos = newpos; }

    PositionT& get_position()
    { return this->policies()._pos; }

    PositionT const& get_position() const
    { return this->policies()._pos; }

    void set_tabchars(unsigned int chars)
    {
        // This function (which comes from the position_policy) has a
        //  different name on purpose, to avoid messing with using
        //  declarations or qualified calls to access the base template
        //  function, which might break some compilers.
        this->policies().set_tab_chars(chars);
    }

protected:

    //  JDG 4-15-03
    //  Borland can't take in position_iterator without
    //  the template parameters.
    typedef position_iterator<ForwardIteratorT, PositionT, SelfT> self_t;
    friend struct iterator_::impl::position_iterator_policy<
            self_t, ForwardIteratorT, PositionT>;

    void newline(void)
    {}
};


///////////////////////////////////////////////////////////////////////////////
//
//  position_iterator2
//
//  Equivalent to position_iterator, but it is able to extract the current
//  line into a string. This is very handy for error reports.
//
//  Notice that the footprint of this class is higher than position_iterator,
//  (how much depends on how bulky the underlying iterator is), so it should
//  be used only if necessary.
//
///////////////////////////////////////////////////////////////////////////////

template
<
    typename ForwardIteratorT,
    typename PositionT = file_position
>
class position_iterator2
    : public position_iterator
    <
        ForwardIteratorT,
        PositionT,
        position_iterator2<ForwardIteratorT, PositionT>
    >
{
    typedef position_iterator
    <
        ForwardIteratorT,
        PositionT,
        position_iterator2<ForwardIteratorT, PositionT> // JDG 4-15-03
    >  base_t;

public:
    typedef typename base_t::value_type value_type;
    typedef PositionT position_t;

    position_iterator2()
    {}

    position_iterator2(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end):
        base_t(begin, end),
        _startline(begin)
    {}

    template <typename FileNameT>
    position_iterator2(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        FileNameT file):
        base_t(begin, end, file),
        _startline(begin)
    {}

    template <typename FileNameT, typename LineT>
    position_iterator2(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        FileNameT file, LineT line):
        base_t(begin, end, file, line),
        _startline(begin)
    {}

    template <typename FileNameT, typename LineT, typename ColumnT>
    position_iterator2(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        FileNameT file, LineT line, ColumnT column):
        base_t(begin, end, file, line, column),
        _startline(begin)
    {}

    position_iterator2(
        const ForwardIteratorT& begin,
        const ForwardIteratorT& end,
        const PositionT& pos):
        base_t(begin, end, pos),
        _startline(begin)
    {}

    position_iterator2(const position_iterator2& iter)
        : base_t(iter), _startline(iter._startline)
    {}

    position_iterator2& operator=(const position_iterator2& iter)
    {
        base_t::operator=(iter);
        _startline = iter._startline;
        return *this;
    }

    ForwardIteratorT get_currentline_begin(void) const
    { return _startline; }

    ForwardIteratorT get_currentline_end(void) const
    { return get_endline(); }

    std::basic_string<value_type> get_currentline(void) const
    {
        return std::basic_string<value_type>
            (get_currentline_begin(), get_currentline_end());
    }

protected:
    ForwardIteratorT _startline;

    ForwardIteratorT get_endline() const
    {
        ForwardIteratorT endline = _startline;
        while (endline != this->policies()._end && *endline != '\r' &&
            *endline != '\n')
        {
            ++endline;
        }
        return endline;
    }

    //  JDG 4-15-03
    //  Borland can't take in position_iterator2 without
    //  the template parameters.
    typedef position_iterator2<ForwardIteratorT, PositionT> self_t;
    friend struct iterator_::impl::position_iterator_policy<
            self_t, ForwardIteratorT, PositionT>;

    void newline(void)
    { _startline = this->base(); }
};

///////////////////////////////////////////////////////////////////////////////
}} // namespace boost::spirit

#endif
