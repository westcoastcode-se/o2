//
// Part of the o2 Project, under the Apache License v2.0 with o2 Project Exceptions.
// See the LICENSE file in the project root for license terms
//

#pragma once

#include <string_view>
#include <string>
#include <cinttypes>

namespace o2
{
    typedef std::string_view string_view;
    typedef std::string string;

    // \brief modifiers for the token. For example if a string is multiline
    enum class token_modifier
    {
        none,
        // Normally used for strings and comments. Indicates that the token is a multiline value
        multiline,
        // Hint that the supplied number is an unsigned 64bit value
        hint_unsigned,
        // Hint that the supplied decimal value is a 32bit decimal
        hint_float,
		// Hint that the value is considered empty, such as an identity
		hint_empty,
    };

    enum class token_type
    {
        identity,

        scope_start,
        public_ /* public */,
        private_ /* private */,
		internal_ /* internal */,
        scope_end,

        keyword_start,
        type /* type */,
        typedef_ /* typedef */,
        macro /* macro */,
        func /* func */,
        cast /* cast */,
        interface /* interface */,
		trait /* trait */,
        if_ /* if */,
        else_ /* else */,
        var /* var */,
        for_ /* for */,
        import /* import */,
        return_ /* return */,
		struct_ /* struct */,
		as /* as */,
        keyword_end,

        storage_class_start,
        static_ /* static */,
        inline_ /* inline */,
        virtual_ /* virtual */,
        override_ /* override */,
        final_ /* final */,
        extern_ /* extern */,
        storage_class_end,

        type_modifier_start,
        const_ /* const */,
        ref /* & */,
        pointer /* * */,
        type_modifier_end,

        test_start,
        test_equals /* == */,
        test_not_equals /* != */,
        test_lt /* < */,
        test_lte /* <= */,
        test_gt /* > */,
        test_gte /* >= */ ,
        test_end,

        parant_left /* ( */,
        parant_right /* ) */,
        square_left /* [ */,
        square_right /* ] */,
        bracket_left /* { */,
        bracket_right /* } */,
        attribute_left /* [[ */,
        attribute_right /* ]] */,

        values_start,
        number,
        hex,
        boolean,
        decimal,
        string,
        values_end,

        bit_not /* ~ */,
        bit_or /* | */,
        bit_xor /* ^ */,

        dot /* . */,
        comma /* , */,
        colon /* : */,
        semicolon /* ; */,
        attribute /* @??? */,
        assign /* = */,
        separator /* :: */,
        not_ /* ! */,
        plus /* + */,
        minus /* - */,
        div /* / */,
        inc /* ++ */,
        dec /* -- */,

        escape /* \ */,
        comment /* single- and multiline */,
		newline /* \n */,

        unknown,
        eof,
    };

    /// <summary>
    /// 
    /// </summary>
    class lexer
    {
    public:
        lexer(string_view text)
            : _text(text) {}

        /// <returns>Get the first chacter in the string</returns>
        const string_view::value_type* first() const { return _text.data(); }

        /// <returns>The last character in the string</returns>
        const string_view::value_type* last() const { return _text.data() + _text.length(); }

    private:
        string_view _text;
    };

    class token
    {
    public:
        token(const lexer* l)
            : _lexer(l), _pos(l->first()), _type(token_type::unknown), _modifier(token_modifier::none),
            _string_start(_pos), _string_end(_pos),
            _line(0), _line_offset(_pos)
        {
        }

        token(const token& lhs) = default;

        /// <summary>
        /// Check if the supplied token type is the current token
        /// </summary>
        /// <param name="t"></param>
        /// <returns></returns>
        bool is(token_type t) const { return _type == t; }

        /// <returns>The current token type</returns>
        token_type type() const { return _type; }

        /**
         * \return a modifier associated with the current token
         */
        token_modifier get_modifier() const { return _modifier; }

        /// <returns>Get the token string</returns>
        string_view value() const { return string_view(_string_start, _string_end); }

		/**
		 * \return the length of the value
		 */
		int value_length() const { return _string_end - _string_start; }

        /// <returns>Get which line we are parsing at the moment</returns>
        inline int line() const { return _line; }

        /// <returns>Get the offset, in bytes, on the line we are parsing at the moment</returns>
        const string_view::value_type* line_start() const { return _line_offset; }

        /// <summary>
        /// Check to see if the supplied token is a scope keyword
        /// </summary>
        /// <param name="tt"></param>
        /// <returns></returns>
        static bool is_scope(token_type tt)
        {
            return tt > token_type::scope_start && tt < token_type::scope_end;
        }

        /// <summary>
        /// Is the supplied token a datatype
        /// </summary>
        /// <param name="tt"></param>
        /// <returns></returns>
        static bool is_value(token_type tt)
        {
            return tt > token_type::values_start && tt < token_type::values_end;
        }

        /// <summary>
        /// Check to see if the supplied token is a keyword
        /// </summary>
        /// <param name="tt"></param>
        /// <returns></returns>
        static bool is_keyword(token_type tt)
        {
            return tt > token_type::keyword_start && tt < token_type::keyword_end;
        }

        /// <summary>
        /// Check to see if the supplied token is a storage class keyword
        /// </summary>
        /// <param name="tt"></param>
        /// <returns></returns>
        static bool is_storage_class(token_type tt)
        {
            return tt > token_type::storage_class_start && tt < token_type::storage_class_end;
        }

        /// <summary>
        /// Check to see if the supplied token is a type modifier keyword, such as const
        /// </summary>
        /// <param name="tt"></param>
        /// <returns></returns>
        static bool is_type_modifier(token_type tt)
        {
            return tt > token_type::type_modifier_start && tt < token_type::type_modifier_end;
        }

        /// <returns>Get the offset, in bytes, on the line we are parsing at the moment</returns>
        int line_offset() const;

        /// <returns>Get the offset, in bytes, in the source code we are parsing at the moment</returns>
        int offset() const;

        /// <summary>
        /// Get the next token
        /// </summary>
        /// <returns></returns>
        token_type next();

		/**
		 * \brief next token until we reach one of the supplied token(s)
		 */
		token_type next_until(token_type t1);

		/**
		 * \brief next token until we reach one of the supplied token(s)
		 */
		token_type next_until(token_type t1, token_type t2);

		/**
		 * \brief next token until we reach a token that's not one of the supplied token(s)
		 */
		token_type next_until_not(token_type t1);

		/**
		 * \brief next token until we reach a token that's not one of the supplied token(s)
		 */
		token_type next_until_not(token_type t1, token_type t2);

		/**
		 * \brief next token until we reach a token that's not one of the supplied token(s)
		 */
		token_type next_until_not(token_type t1, token_type t2, token_type t3);

        /**
         * \brief 
         * \return the value as a 64bit integer
         */
        std::int64_t value_int64() const;

        /**
         * \brief
         * \return the value as a unsigned 64bit integer
         */
        std::uint64_t value_uint64() const;

        /**
         * \return the value as a boolean
         */
        bool value_bool() const;

        /**
         * \return the value as a float value
         */
        float value_float32() const;

        /**
         * \return the value as a double
         */
        double value_float64() const;

    private:
        /// <summary>
        /// Get the next token
        /// </summary>
        /// <returns></returns>
        void next0();

        /// <summary>
        /// Current token is an atom token
        /// </summary>
        /// <param name="type"></param>
        void atom(token_type type);

        /// <summary>
        /// Current token is an atom token
        /// </summary>
        void unknown();

        /// <summary>
        /// Reached the end of the string
        /// </summary>
        token_type eof();

        /// <summary>
        /// Peek at the next character
        /// </summary>
        /// <param name="n"></param>
        /// <returns></returns>
        string_view::value_type peek(int n) const;

        /// <summary>
        /// Seek the value assuming that it's a string value
        /// </summary>
        void next_string(char c);

        /// <summary>
        /// Seek the value assuming that it's a number
        /// </summary>
        void next_number();

        /// <summary>
        /// Seek the value assuming that it's a keyword
        /// </summary>
        void next_keyword();

        /**
         * \brief seek the next attribute
         */
        void next_attribute();

        /// <summary>
        /// Comment next?
        /// </summary>
        void next_comment_or_div();

        /**
         * \brief next token is a minus character (or sub)
         */
        void next_minus_or_dec();

        /**
         * \brief next token is a plus character (or an increment)
         */
        void next_plus_or_inc();

        void next_lt_or_lte();

        void next_gt_or_gte();

        void next_not_or_not_equals();

        /// <returns>Figure out the keyword type</returns>
        token_type hint_keyword_type() const;

    private:
        const lexer* const _lexer;
        const string_view::value_type* _pos;
        token_type _type;
        token_modifier _modifier;

        const string_view::value_type* _string_start;
        const string_view::value_type* _string_end;

        int _line;
        const string_view::value_type* _line_offset;
    };

    // \brief seek until one of the supplied type is found
    inline static token_type token_seek_until(token* t, token_type until)
    {
        auto tt = t->type();
        while (tt != until && tt != token_type::eof) tt = t->next();
        return tt;
    }

    // \brief seek until one of the two types are found
    inline static token_type token_seek_until2(token* t, token_type until1, token_type until2)
    {
        auto tt = t->type();
        while (tt != until1 && tt != until2 && tt != token_type::eof) tt = t->next();
        return tt;
    }

}