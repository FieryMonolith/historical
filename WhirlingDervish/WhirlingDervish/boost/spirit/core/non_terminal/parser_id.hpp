/*=============================================================================
    Spirit v1.7.0
    Copyright (c) 2001-2003 Joel de Guzman
    Copyright (c) 2001 Daniel Nuffer
    http://spirit.sourceforge.net/

    Permission to copy, use, modify, sell and distribute this software is
    granted provided this copyright notice appears in all copies. This
    software is provided "as is" without express or implied warranty, and
    with no claim as to its suitability for any purpose.
=============================================================================*/
#if !defined(BOOST_SPIRIT_PARSER_ID_HPP)
#define BOOST_SPIRIT_PARSER_ID_HPP

///////////////////////////////////////////////////////////////////////////////
namespace boost { namespace spirit {

    class parser_id
    {
    public:
                    parser_id()                     : p(0) {}
        explicit    parser_id(void const* prule)    : p(prule) {}
                    parser_id(std::size_t l_)       : l(l_) {}

        bool operator==(parser_id const& x) const   { return p == x.p; }
        bool operator!=(parser_id const& x) const   { return !(*this == x); }
        bool operator<(parser_id const& x) const    { return p < x.p; }
        std::size_t to_long() const                 { return l; }

    private:

        union
        {
            void const* p;
            std::size_t l;
        };
    };

    #if defined(BOOST_SPIRIT_DEBUG)
    inline std::ostream&
    operator<<(std::ostream& out, parser_id const& rid)
    {
        out << rid.to_long();
        return out;
    }
    #endif

    ///////////////////////////////////////////////////////////////////////////
    //
    //  parser_address_tag class: tags a parser with its address
    //
    ///////////////////////////////////////////////////////////////////////////
    struct parser_address_tag
    {
        parser_id id() const
        { return parser_id(reinterpret_cast<std::size_t>(this)); }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  parser_tag class: tags a parser with an integer ID
    //
    ///////////////////////////////////////////////////////////////////////////
    template <int N>
    struct parser_tag
    {
        static parser_id id()
        { return parser_id(std::size_t(N)); }
    };

    ///////////////////////////////////////////////////////////////////////////
    //
    //  dynamic_parser_tag class: tags a parser with a dynamically changeable
    //  integer ID
    //
    ///////////////////////////////////////////////////////////////////////////
    class dynamic_parser_tag {
    
    public:
        dynamic_parser_tag() : tag(std::size_t(0)) {}
        
        parser_id id() const
        { return tag.to_long() ? tag : parser_id(reinterpret_cast<std::size_t>(this)); }

        void set_id(parser_id id)
        { tag = id; } 
        
    private:
        parser_id tag;
    };

///////////////////////////////////////////////////////////////////////////////
}} // namespace boost::spirit

#endif

