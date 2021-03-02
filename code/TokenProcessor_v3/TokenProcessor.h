# ifndef TOKEN_PROCESSOR_H_INCLUDED
# define TOKEN_PROCESSOR_H_INCLUDED

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <stdlib.h>

/* TokenProcessor Version 3 */

typedef int TokenType ;

# define UNKNOWN 0
# define IDENTIFIER 1
# define INTEGER 2
# define DOUBLE 3
# define DELIMITER 4
# define STRING 5
# define CHAR 6

struct TokenData {
    std::string token ;
    int iValue ;
    double dValue ;
    std::string contents ;
    TokenType type ;
    TokenData() : token( "" ), iValue( 0 ), dValue( 0.0 ), contents( "" ), type( UNKNOWN ) {} // constructor
    TokenData( std::string tkn, int i, double d, std::string cnt, TokenType t ) :
        token( tkn ), iValue( i ), dValue( d ), contents( cnt ), type( t ) {} // constructor
} ; // struct TokenData

class TokenProcessor {

private:

    std::string mDelimitersList ;
    std::vector<std::string> mOriginalContent ;

    std::vector< std::vector<TokenData> > mTokenBox ;
    size_t mRowIndex ;
    size_t mColumnIndex ;

    bool IsWhite( char ch ) ;
    bool IsLetter( char ch ) ;
    bool IsDigit( char ch ) ;
    bool IsDelimiter( char ch ) ;

    char ChangeChar( char ch ) ;

    TokenData GetLetterToken( char firstChar, size_t & index, std::string & source ) ;
    TokenData GetNumberToken( char firstChar, size_t & index, std::string & source, bool beginWithInteger ) ;
    TokenData GetDelimiterToken( char firstChar, size_t & index, std::string & source ) ;
    TokenData GetStringToken( char boundaryChar, size_t & index, std::string & source, TokenType type ) ;

public:

    TokenProcessor() ; // constructor

    void CutLine( std::string str ) ;
    void CutFile( std::fstream & inFile ) ;

    bool GetToken( TokenData & token ) ;
    bool GetToken( TokenData & token, int times ) ;
    bool PeekToken( TokenData & token ) ;

    void Output( bool setBracket, bool setBoundary ) ;
    void OutputOriginal( bool setBoundary ) ;

    void Clear() ;
    void BackToBeginning() ;

} ; // class TokenProcessor

TokenProcessor::TokenProcessor() { // constructor

    mDelimitersList = "!#$%&()*+,-./:;<=>?@[\\]^`{|}~" ; // no _ " '

    mRowIndex = mColumnIndex = 0 ;

} // TokenProcessor::TokenProcessor()

void TokenProcessor::CutLine( std::string str ) {

    mOriginalContent.push_back( str ) ;

    std::vector<TokenData> tokenList ;

    for ( size_t i = 0 ; i < str.length() ; i++ ) {

        if ( IsWhite( str[i] ) ) {
            ;
        } // if
        else if ( str[i] == '_' || IsLetter( str[i] ) ) {
            tokenList.push_back( GetLetterToken( str[i], i, str ) ) ;
        } // else if
        else if ( IsDigit( str[i] ) ) {
            tokenList.push_back( GetNumberToken( str[i], i, str, true ) ) ;
        } // else if
        else if ( str[i] == '\"' ) {
            tokenList.push_back( GetStringToken( str[i], i, str, STRING ) ) ;
        } // else if
        else if ( str[i] == '\'' ) {
            tokenList.push_back( GetStringToken( str[i], i, str, CHAR ) ) ;
        } // else if
        else { // IsDelimiter( str[i] )
            tokenList.push_back( GetDelimiterToken( str[i], i, str ) ) ;
        } // else

    } // for

    if ( tokenList.size() != 0 ) {
        mTokenBox.push_back( tokenList ) ;
        tokenList.clear() ;
    } // if

} // TokenProcessor::CutLine()

void TokenProcessor::CutFile( std::fstream & inFile ) {

    std::string line = "" ;

    while ( getline( inFile, line ) ) {
        CutLine( line ) ;
    } // while

} // TokenProcessor::CutFile()

TokenData TokenProcessor::GetLetterToken( char firstChar, size_t & index, std::string & source ) {

    std::string token = "" ;
    token += firstChar ;

    for ( index++ ; index < source.length() &&
                    ( IsLetter( source[index] ) || IsDigit( source[index] ) || source[index] == '_' ) ;
          index++ ) {
        token += source[index] ;
    } // for

    if ( index < source.length() && ( !IsWhite( source[index] ) ) ) {
        index-- ;
    } // if

    return TokenData( token, 0, 0.0, "", IDENTIFIER ) ;

} // TokenProcessor::GetLetterToken()

TokenData TokenProcessor::GetNumberToken( char firstChar, size_t & index, std::string & source, bool beginWithInteger ) {

    std::string token = "" ;
    bool isInt = beginWithInteger, running = true ;

    token += firstChar ;
    index++ ;

    while ( index < source.length() && running ) {

        if ( IsDigit( source[index] ) ) {
            token += source[index++] ;
        } // if
        else if ( source[index] == '.' ) {
            if ( isInt ) {
                isInt = false ;
                token += source[index++] ;
            } // if
            else {
                running = false ;
            } // else
        } // else if
        else {
            running = false ;
        } // else
        
    } // while

    if ( index < source.length() && ( !IsWhite( source[index] ) ) ) {
        index-- ;
    } // if

    return ( isInt ? TokenData( token, atoi( token.c_str() ), 0.0, "", INTEGER ) :
                     TokenData( token, 0, atof( token.c_str() ), "", DOUBLE ) ) ;

} // TokenProcessor::GetNumberToken()

TokenData TokenProcessor::GetStringToken( char boundaryChar, size_t & index, std::string & source, TokenType type ) {

    std::string token = "", contents = "" ;
    token += boundaryChar ; // left boundary

    for ( index++ ; index < source.length() && source[index] != boundaryChar ; index++ ) {

        if ( source[index] == '\\' ) {

            token += source[index] ;

            if ( index + 1 < source.length() ) {
                index++ ;
                token += source[index] ;
                contents += ChangeChar( source[index] ) ;
            } // if
            else {
                contents += source[index] ;
            } // else

        } // if
        else {
            token += source[index] ;
            contents += source[index] ;
        } // else

    } // for

    if ( index < source.length() ) {
        token += source[index] ; // right boundary
    } // if
    else {
        ; // right boundary does not exist
    } // else

    return TokenData( token, 0, 0.0, contents, type ) ;

} // TokenProcessor::GetStringToken()

TokenData TokenProcessor::GetDelimiterToken( char firstChar, size_t & index, std::string & source ) {

    std::string token = "" ;
    token += firstChar ;

    if ( firstChar == '+' ) {

        if ( index + 1 < source.length() && ( source[index + 1] == '=' || source[index + 1] == '+' ) ) {
            token += source[++index] ; // += ++
        } // if

    } // if
    else if ( firstChar == '-' ) {

        if ( index + 1 < source.length() ) {
            if ( source[index + 1] == '=' || source[index + 1] == '-' ) {
                token += source[++index] ; // -= --
            } // if
            else if ( source[index + 1] == '>' ) {
                token += source[++index] ; // ->
                if ( index + 1 < source.length() && source[index + 1] == '*' ) {
                    token += source[++index] ; // ->*
                } // if
            } // else if
            else {
                ;
            } // else            
        } // if

    } // else if
    else if ( firstChar == '*' ) {

        if ( index + 1 < source.length() && ( source[index + 1] == '=' || source[index + 1] == '/' ) ) {
            token += source[++index] ; // *= */
        } // if

    } // else if
    else if ( firstChar == '/' ) {

        if ( index + 1 < source.length() && ( source[index + 1] == '=' || source[index + 1] == '/' || source[index + 1] == '*' ) ) {
            token += source[++index] ; // /= // /*
        } // if

    } // else if
    else if ( firstChar == '%' ) {

        if ( index + 1 < source.length() && source[index + 1] == '=' ) {
            token += source[++index] ; // %=
        } // if

    } // else if
    else if ( firstChar == '<' ) {

        if ( index + 1 < source.length() ) {
            if ( source[index + 1] == '=' ) {
                token += source[++index] ; // <=
            } // if
            else if ( source[index + 1] == '<' ) {
                token += source[++index] ; // <<
                if ( index + 1 < source.length() && source[index + 1] == '=' ) {
                    token += source[++index] ; // <<=
                } // if
            } // else if
            else {
                ;
            } // else
        } // if

    } // else if
    else if ( firstChar == '>' ) {

        if ( index + 1 < source.length() ) {
            if ( source[index + 1] == '=' ) {
                token += source[++index] ; // >=
            } // if
            else if ( source[index + 1] == '>' ) {
                token += source[++index] ; // >>
                if ( index + 1 < source.length() && source[index + 1] == '=' ) {
                    token += source[++index] ; // >>=
                } // if
            } // else if
            else {
                ;
            } // else
        } // if

    } // else if
    else if ( firstChar == '!' ) {

        if ( index + 1 < source.length() && source[index + 1] == '=' ) {
            token += source[++index] ; // !=
        } // if

    } // else if
    else if ( firstChar == '=' ) {

        if ( index + 1 < source.length() && source[index + 1] == '=' ) {
            token += source[++index] ; // ==
        } // if

    } // else if
    else if ( firstChar == '&' ) {

        if ( index + 1 < source.length() && ( source[index + 1] == '&' || source[index + 1] == '=' ) ) {
            token += source[++index] ; // && &=
        } // if

    } // else if
    else if ( firstChar == '|' ) {

        if ( index + 1 < source.length() && ( source[index + 1] == '|' || source[index + 1] == '=' ) ) {
            token += source[++index] ; // || |=
        } // if

    } // else if
    else if ( firstChar == ':' ) {

        if ( index + 1 < source.length() && source[index + 1] == ':' ) {
            token += source[++index] ; // ::
        } // if

    } // else if
    else if ( firstChar == '#' ) {

        if ( index + 1 < source.length() && source[index + 1] == '#' ) {
            token += source[++index] ; // ##
        } // if

    } // else if
    else if ( firstChar == '.' ) {

        if ( index + 1 < source.length() ) {
            if ( source[index + 1] == '*' ) {
                token += source[++index] ; // .*
            } // if
            else if ( IsDigit( source[index + 1] ) ) {
                return GetNumberToken( firstChar, index, source, false ) ; // number
            } // else if
            else {
                ;
            } // else            
        } // if

    } // else if
    else if ( firstChar == '^' ) {

        if ( index + 1 < source.length() && source[index + 1] == '=' ) {
            token += source[++index] ; // ^=
        } // if

    } // else if
    else {
        ;
    } // else

    return TokenData( token, 0, 0.0, "", DELIMITER ) ; ;

} // TokenProcessor::GetDelimiterToken()

bool TokenProcessor::GetToken( TokenData & token ) {

    if ( mRowIndex < mTokenBox.size() ) {

        token = mTokenBox[mRowIndex][mColumnIndex] ;
        mColumnIndex++ ;

        if ( mColumnIndex >= mTokenBox[mRowIndex].size() ) {
            mRowIndex++ ;
            mColumnIndex = 0 ;
        } // if

        return true ;

    } // if
    else {
        token = TokenData() ;
        return false ;
    } // else

} // TokenProcessor::GetToken()

bool TokenProcessor::GetToken( TokenData & token, int times ) {

    if ( times <= 0 ) {
        return false ;
    } // if

    bool successOrNot = true ;

    while ( ( times-- ) && successOrNot ) {
        successOrNot = GetToken( token ) ;
    } // while

    return successOrNot ;

} // TokenProcessor::GetToken()

bool TokenProcessor::PeekToken( TokenData & token ) {

    if ( mRowIndex < mTokenBox.size() ) {
        token = mTokenBox[mRowIndex][mColumnIndex] ;
        return true ;
    } // if
    else {
        token = TokenData() ;
        return false ;
    } // else

} // TokenProcessor::PeekToken()

void TokenProcessor::Output( bool setBracket = true, bool setBoundary = true ) {

    if ( setBoundary ) {
        std::cout << "---------- Tokens ----------" << std::endl ;
    } // if

    for ( size_t i = 0 ; i < mTokenBox.size() ; i++ ) {

        for ( size_t j = 0 ; j < mTokenBox[i].size() ; j++ ) {

            if ( setBracket ) {
                std::cout << "[" << mTokenBox[i][j].token << "]" << "  " ;
            } // if
            else {
                std::cout << mTokenBox[i][j].token << "  " ;
            } // else

        } // for

        std::cout << std::endl ;

    } // for

    if ( setBoundary ) {
        std::cout << "----------------------------" << std::endl ;
    } // if

} // TokenProcessor::Output()

void TokenProcessor::OutputOriginal( bool setBoundary = true ) {

    if ( setBoundary ) {
        std::cout << "---------- Original ----------" << std::endl ;
    } // if

    for ( size_t i = 0 ; i < mOriginalContent.size() ; i++ ) {
        std::cout << mOriginalContent[i] << std::endl ;
    } // for

    if ( setBoundary ) {
        std::cout << "------------------------------" << std::endl ;
    } // if

} // TokenProcessor::OutputOriginal() 

void TokenProcessor::Clear() {

    mOriginalContent.clear() ;
    mTokenBox.clear() ;
    mRowIndex = mColumnIndex = 0 ;

} // TokenProcessor::Clear()

void TokenProcessor::BackToBeginning() {
    mRowIndex = mColumnIndex = 0 ;
} //  TokenProcessor::BackToBeginning()

char TokenProcessor::ChangeChar( char ch ) {

    if ( ch == 'n' ) {
        return '\n' ;
    } // if
    else if ( ch == 'r' ) {
        return '\r' ;
    } // else if
    else if ( ch == 't' ) {
        return '\t' ;
    } // else if
    else if ( ch == 'a' ) {
        return '\a' ;
    } // else if
    else if ( ch == 'b' ) {
        return '\b' ;
    } // else if
    else if ( ch == 'f' ) {
        return '\f' ;
    } // else if
    else if ( ch == 'v' ) {
        return '\v' ;
    } // else if
    else {
        return ch ;
    } // else

} // TokenProcessor::ChangeChar()

bool TokenProcessor::IsWhite( char ch ) {
    return ( ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' ) ;
} // TokenProcessor::IsWhite()

bool TokenProcessor::IsLetter( char ch ) {
    return ( 'a' <= ch && ch <= 'z' ) || ( 'A' <= ch && ch <= 'Z' ) ;
} // TokenProcessor::IsLetter()

bool TokenProcessor::IsDigit( char ch ) {
    return ( '0' <= ch && ch <= '9' ) ;
} // TokenProcessor::IsDigit()

bool TokenProcessor::IsDelimiter( char ch ) {

    for ( size_t i = 0 ; i < mDelimitersList.length() ; i++ ) {
        if ( mDelimitersList[i] == ch ) {
            return true ;
        } // if
    } // for

    return false ;

} // TokenProcessor::IsDelimiter()

# endif // TOKEN_PROCESSOR_H_INCLUDED

