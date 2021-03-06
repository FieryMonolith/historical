/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2001, Daniel C. Nuffer
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#ifndef FIXED_SIZE_QUEUE
#define FIXED_SIZE_QUEUE

#include <cstdlib>
#include <iterator>
#include <cstddef>

#include <boost/spirit/core/assert.hpp> // for BOOST_SPIRIT_ASSERT

// FIXES for broken compilers
#include <boost/config.hpp>
#include <boost/iterator_adaptors.hpp>

//// fix for cygwin
//#ifdef size_t
//#undef size_t
//namespace std { typedef unsigned size_t; }
//#endif

#define BOOST_SPIRIT_ASSERT_FSQ_SIZE \
    BOOST_SPIRIT_ASSERT(((m_tail + N + 1) - m_head) % (N+1) == m_size % (N+1)) \
    /**/

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

///////////////////////////////////////////////////////////////////////////////
namespace impl {

template <class QueueT>
struct fsq_iter_policy
{
    // VC7 requires explicit public here (bug)
public:
    typedef typename QueueT::position position;

    template <class Base>
    void initialize(Base&)
    {}

    template <class IterT>
    typename IterT::reference dereference(const IterT& x) const
    {
        const position& p = x.base();

        return p.self->m_queue[p.pos];
    }

    template <class IterT>
    void increment(IterT& x)
    {
        position& p = x.base();

        ++p.pos;
        if (p.pos == QueueT::MAX_SIZE+1)
            p.pos = 0;
    }

    template <class IterT>
    void decrement(IterT& x)
    {
        position& p = x.base();

        if (p.pos == 0)
            p.pos = QueueT::MAX_SIZE;
        else
            --p.pos;
    }

    template <class Iter1T, class Iter2T>
    bool equal(const Iter1T& x1, const Iter2T& x2) const
    {
        const position& p1 = x1.base();
        const position& p2 = x2.base();

        return (p1.self == p2.self) && (p1.pos == p2.pos);
    }

    template <class IteratorAdaptor1, class IteratorAdaptor2>
    typename IteratorAdaptor1::difference_type
    distance(const IteratorAdaptor1& x1, const IteratorAdaptor2& x2) const
    {
        typedef typename IteratorAdaptor1::difference_type diff_t;

        const position& p1 = x1.base();
        const position& p2 = x2.base();
        std::size_t pos1 = p1.pos;
        std::size_t pos2 = p2.pos;

        // Undefined behaviour if the iterators come from different
        //  containers
        BOOST_SPIRIT_ASSERT(p1.self == p2.self);

        if (pos1 < p1.self->m_head)
            pos1 += QueueT::MAX_SIZE;
        if (pos2 < p2.self->m_head)
            pos2 += QueueT::MAX_SIZE;

        if (pos2 > pos1)
            return diff_t(pos2 - pos1);
        else
            return -diff_t(pos1 - pos2);
    }

    template <class IteratorAdaptor, class DifferenceType>
    void advance(IteratorAdaptor& x, DifferenceType n)
    {
        position& p = x.base();

        // Notice that we don't care values of n that can
        //  wrap around more than one time, since it would
        //  be undefined behaviour anyway (going outside
        //  the begin/end range). Negative wrapping is a bit
        //  cumbersome because we don't want to case p.pos
        //  to signed.
        if (n < 0)
    {
            n = -n;
            if (p.pos < (std::size_t)n)
                p.pos = QueueT::MAX_SIZE+1 - (n - p.pos);
            else
                p.pos -= n;
        }
        else
        {
            p.pos += n;
            if (p.pos >= QueueT::MAX_SIZE+1)
                p.pos -= QueueT::MAX_SIZE+1;
    }
    }
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace impl */

template <typename T, std::size_t N>
class fixed_size_queue
{
private:
    struct position
    {
        fixed_size_queue* self;
        std::size_t pos;

        position() {}

        // The const_cast here is just to avoid to have two different
        //  position structures for the const and non-const case.
        // The const semantic is guaranteed by the iterator itself
        position(const fixed_size_queue* s, std::size_t p)
            : self(const_cast<fixed_size_queue*>(s)), pos(p)
        {}
};

    //  JDG 4-15-03 Borland can't take fixed_size_queue
    //  w/o the template parameters.
    typedef impl::fsq_iter_policy<fixed_size_queue<T, N> > iter_policy_t;

public:
    // Declare the iterators
    typedef boost::iterator_adaptor
    <
        position,
        iter_policy_t,
        T, T&, T*,
        std::random_access_iterator_tag,
        std::ptrdiff_t
    > iterator;

    typedef boost::iterator_adaptor
    <
        position,
        iter_policy_t,
        const T, const T&, const T*,
        std::random_access_iterator_tag,
        std::ptrdiff_t
    > const_iterator;

public:
    fixed_size_queue();
    fixed_size_queue(const fixed_size_queue& x);
    fixed_size_queue& operator=(const fixed_size_queue& x);
    ~fixed_size_queue();

    void push_back(const T& e);
    void push_front(const T& e);
    void serve(T& e);
    void pop_front();

    bool empty() const
    {
        return m_size == 0;
    }

    bool full() const
    {
        return m_size == N;
    }

    iterator begin()
    {
        return iterator(position(this, m_head));
    }

    const_iterator begin() const
    {
        return const_iterator(position(this, m_head));
    }

    iterator end()
    {
        return iterator(position(this, m_tail));
    }

    const_iterator end() const
    {
        return const_iterator(position(this, m_tail));
    }

    std::size_t size() const
    {
        return m_size;
    }

    T& front()
    {
        return m_queue[m_head];
    }

    const T& front() const
    {
        return m_queue[m_head];
    }

private:
    // Some broken compilers do not see through typedefs when declaring
    // friend, so we don't use iter_policy_t here

    // JDG 4-15-03 Borland can't take in fixed_size_queue w/o
    // the template parameters.
    friend struct impl::fsq_iter_policy<fixed_size_queue<T, N> >;

    // Redefine the template parameters to avoid using partial template
    //  specialization on the iterator policy to extract N.
    BOOST_STATIC_CONSTANT(std::size_t, MAX_SIZE = N);

    std::size_t m_head;
    std::size_t m_tail;
    std::size_t m_size;
    T m_queue[N+1];
};

template <typename T, std::size_t N>
inline
fixed_size_queue<T, N>::fixed_size_queue()
    : m_head(0)
    , m_tail(0)
    , m_size(0)
{
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);
}

template <typename T, std::size_t N>
inline
fixed_size_queue<T, N>::fixed_size_queue(const fixed_size_queue& x)
    : m_head(x.m_head)
    , m_tail(x.m_tail)
    , m_size(x.m_size)
{
    copy(x.begin(), x.end(), begin());
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);
}

template <typename T, std::size_t N>
inline fixed_size_queue<T, N>&
fixed_size_queue<T, N>::operator=(const fixed_size_queue& x)
{
    if (this != &x)
    {
        m_head = x.m_head;
        m_tail = x.m_tail;
        m_size = x.m_size;
        copy(x.begin(), x.end(), begin());
    }
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);

    return *this;
}

template <typename T, std::size_t N>
inline
fixed_size_queue<T, N>::~fixed_size_queue()
{
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);
}

template <typename T, std::size_t N>
inline void
fixed_size_queue<T, N>::push_back(const T& e)
{
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);

    BOOST_SPIRIT_ASSERT(!full());

    m_queue[m_tail] = e;
    ++m_size;
    ++m_tail;
    if (m_tail == N+1)
        m_tail = 0;


    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);
}

template <typename T, std::size_t N>
inline void
fixed_size_queue<T, N>::push_front(const T& e)
{
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);

    BOOST_SPIRIT_ASSERT(!full());

    if (m_head == 0)
        m_head = N;
    else
        --m_head;

    m_queue[m_head] = e;
    ++m_size;

    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);
}


template <typename T, std::size_t N>
inline void
fixed_size_queue<T, N>::serve(T& e)
{
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);

    e = m_queue[m_head];
    pop_front();
}



template <typename T, std::size_t N>
inline void
fixed_size_queue<T, N>::pop_front()
{
    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);

    ++m_head;
    if (m_head == N+1)
        m_head = 0;
    --m_size;

    BOOST_SPIRIT_ASSERT(m_size <= N+1);
    BOOST_SPIRIT_ASSERT_FSQ_SIZE;
    BOOST_SPIRIT_ASSERT(m_head <= N+1);
    BOOST_SPIRIT_ASSERT(m_tail <= N+1);
}

///////////////////////////////////////////////////////////////////////////////
}} // namespace boost::spirit

#undef BOOST_SPIRIT_ASSERT_FSQ_SIZE

#endif
