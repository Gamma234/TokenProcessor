# ifndef TOKEN_PROCESSOR_H_INCLUDED
# define TOKEN_PROCESSOR_H_INCLUDED

# include <iostream>
# include <fstream>
# include <string>
# include <vector>
# include <stdlib.h>

typedef int TokenType ;

# define STRING 1
# define INTEGER 2
# define DELIMITER 3

struct TokenData {
    std::string str ;
    int iValue ;
    TokenType type ;
    TokenData() : str( "" ), iValue( 0 ), type( STRING ) {} // constructor
    TokenData( std::string s, int i, TokenType t ) : str( s ), iValue( i ), type( t ) {} // constructor
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

    TokenData GetLetterToken( char firstChar, size_t & index, std::string& source ) ;
    TokenData GetDigitToken( char firstChar, size_t & index, std::string& source ) ;
    TokenData GetDelimiterToken( char firstChar, size_t & index, std::string& source ) ;

public:

    TokenProcessor() ; // constructor

    void CutLine( std::string str ) ;
    void CutFile( std::fstream& inFile ) ;

    bool GetToken( TokenData& token ) ;
    bool GetToken( TokenData& token, int times ) ;
    bool PeekToken( TokenData& token ) ;

    void Output( bool setBracket, bool setBoundary ) ;
    void OutputOriginal( bool setBoundary ) ;

    void Clear() ;
    void BackToBeginning() ;

} ; // class TokenProcessor

TokenProcessor::TokenProcessor() { // constructor

    mDelimitersList = "!\"#$%&\'()*+,-./:;<=>?@[\\]^`{|}~" ; // no '_'

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
            tokenList.push_back( GetDigitToken( str[i], i, str ) ) ;
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

void TokenProcessor::CutFile( std::fstream& inFile ) {

    std::string line = "" ;

    while ( getline( inFile, line ) ) {
        CutLine( line ) ;
    } // while

} // TokenProcessor::CutFile()

TokenData TokenProcessor::GetLetterToken( char firstChar, size_t & index, std::string& source ) {

    std::string token = "" ;
    token += firstChar ;

    for ( index++ ; index < source.length() && 
                    ( IsLetter( source[index] ) || IsDigit( source[index] ) || source[index] == '_' ) ;
          index++ ) {
        token += source[index] ;
    } // for

    if ( index < source.length() && ( ! IsWhite( source[index] ) ) ) {
        index-- ;
    } // if

    return TokenData( token, 0, STRING ) ;

} // TokenProcessor::GetLetterToken()

TokenData TokenProcessor::GetDigitToken( char firstChar, size_t & index, std::string& source ) {

    std::string token = "" ;
    token += firstChar ;

    for ( index++ ; index < source.length() && IsDigit( source[index] ) ; index++ ) {
        token += source[index] ;
    } // for

    if ( index < source.length() && ( ! IsWhite( source[index] ) ) ) {
        index-- ;
    } // if

    return TokenData( token, atoi( token.c_str() ), INTEGER ) ;

} // TokenProcessor::GetDigitToken()

TokenData TokenProcessor::GetDelimiterToken( char firstChar, size_t & index, std::string& source ) {

    std::string token = "" ;
    token += firstChar ;

    return TokenData( token, 0, DELIMITER ) ; ;

} // TokenProcessor::GetDelimiterToken()

bool TokenProcessor::GetToken( TokenData& token ) {

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

bool TokenProcessor::GetToken( TokenData& token, int times ) {

    if ( times <= 0 ) {
        return false ;
    } // if

    bool successOrNot = true ;

    while ( ( times-- ) && successOrNot ) {
        successOrNot = GetToken( token ) ;
    } // while

    return successOrNot ;

} // TokenProcessor::GetToken()

bool TokenProcessor::PeekToken( TokenData& token ) {

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
                std::cout << "[" << mTokenBox[i][j].str << "]" << "  " ;
            } // if
            else {
                std::cout << mTokenBox[i][j].str << "  " ;
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

