# include "TokenProcessor.h"

int main() {

    TokenProcessor * tp = new TokenProcessor() ;

    std::fstream inFile( "../../test_input/test01.txt", std::ios::in ) ;

    if ( inFile ) {
        tp->CutFile( inFile ) ;
        tp->Output() ;
        inFile.close() ;
    } // if
    else {
        std::cout << "The file can not open !" << "\n" ;
    } // else

    delete tp ;
    tp = NULL ;

} // main()


