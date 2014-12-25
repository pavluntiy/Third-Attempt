#include "alphabet.hpp"

namespace Alphabet{
	template<> bool is<NUMBER_CHAR>(char c){
		return is<DECIMAL_DIGIT>(c);
	}

	template <> bool is<HEXADECIMAL_DIGIT>(char c){
		return is<DECIMAL_DIGIT>(c) || is<HEXADECIMAL_ADDITIONAL_DIGIT>(c);
	}

	template <> bool is<DECIMAL_DIGIT>(char c){
		return is<NONZERO_DECIMAL_DIGIT>(c) || is<ZERO>(c);
	}

	template <> bool is<BINARY_DIGIT>(char c){
		return c == '0' || c == '1';
	}


	template <> bool is<OCTAL_DIGIT>(char c){
		return '0' <= c && c <= '7';
	}

	template<> bool is<HEXADECIMAL_ADDITIONAL_DIGIT>(char c){
		return ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
	}

	template<> bool is<NONZERO_DECIMAL_DIGIT>(char c){
		return '1' <= c && c <= '9';
	}

	template<> bool is<ZERO>(char c){
		return c == '0';
	}

	template<> bool is<UNDERSCORE>(char c){
		return c == '_';
	}

	template<> bool is<LETTER>(char c){
		return is<LATIN_LETTER>(c) || is<UNDERSCORE>(c);
	}

	template<> bool is<LATIN_LETTER>(char c){
		return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
	}

	template<> bool is<NEWLINE>(char c){
		return c == '\n';
	}

	template<> bool is<WHITESPACE>(char c){
		return c == '\n' || c == ' ' || c == '\t';
	}

	template<> bool is<SIGN>(char c){
		return c == '-' || c == '+';
	}
}

 