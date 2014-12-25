
#ifndef ALPHABET
#define ALPHABET
namespace Alphabet{

	enum Type {
		HEXADECIMAL_DIGIT,
		DECIMAL_DIGIT,
		HEXADECIMAL_ADDITIONAL_DIGIT,
		NONZERO_DECIMAL_DIGIT,
		OCTAL_DIGIT,
		NUMBER_CHAR,
		ZERO,
		BINARY_DIGIT,
		UNITY,
		UNDERSCORE,
		LETTER,
		LATIN_LETTER,
		SPECIAL,
		NEWLINE, 
		WHITESPACE, 
		SIGN
	};

	template <Type t>
	bool is(char c);


	template<> bool is<NUMBER_CHAR>(char c);
	template <> bool is<HEXADECIMAL_DIGIT>(char c);
	template <> bool is<DECIMAL_DIGIT>(char c);
	template <> bool is<BINARY_DIGIT>(char c);
	template <> bool is<OCTAL_DIGIT>(char c);

	template<> bool is<HEXADECIMAL_ADDITIONAL_DIGIT>(char c);

	template<> bool is<NONZERO_DECIMAL_DIGIT>(char c);

	template<> bool is<ZERO>(char c);

	template<> bool is<UNDERSCORE>(char c);

	template<> bool is<LETTER>(char c);

	template<> bool is<LATIN_LETTER>(char c);

	template<> bool is<NEWLINE>(char c);

	template<> bool is<WHITESPACE>(char c);

	template<> bool is<SIGN>(char c);
}
#endif

 