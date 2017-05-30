#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include <string>
#include <iosfwd>

class BigNumber
{
    private:

        typedef signed char                     digitType;
        typedef std::basic_string<digitType>   bufferType;
        typedef BigNumber           value;
        typedef BigNumber&          reference;
        typedef const BigNumber&    constantRef;
        typedef std::string                     string;

        static bufferType& remove_zero ( bufferType& );

        bool sign_;
        bufferType buffer;

        static bufferType conv_ulong ( unsigned long );
        static bufferType conv_float ( double );
        static bufferType conv_string ( const string& );

        void overflow ( unsigned begin = 0 );
        void underflow ( unsigned begin = 0 );
        reference normalize();
        reference multiply_by_digit ( digitType );


    public:
        static BigNumber zero;
        // constructors
        BigNumber ( unsigned long = 0ul );
        BigNumber ( signed long );
        BigNumber ( int );
        BigNumber ( double );
        BigNumber ( const string& );
        BigNumber ( const char* );

        // assignment
        reference   operator= ( unsigned long );
        reference   operator= ( signed long );
        reference   operator= ( int );
        reference   operator= ( string );
        reference   operator= ( const char* );
        reference   operator= ( double );

        // conversion
        signed long slong() const;
        unsigned long ulong() const;
        string      str() const;

        // sign
        signed      sign() const;
        reference   abs()           { return sign_ = false, *this; }
        friend value abs( value n ) { return n.abs(); }
        reference   neg();
        value       operator-() const;

        // arithmetic
        reference   operator+= ( constantRef );
        reference   operator-= ( constantRef );
        reference   operator*= ( constantRef );
        reference   operator/= ( value );

        reference operator++ ();
        reference operator-- ();

        value       operator+ ( constantRef );
        value       operator- ( constantRef );
        value       operator* ( constantRef );
        value       operator/ ( constantRef );

        // comparison
        bool        operator<  ( constantRef ) const;
        bool        operator<= ( constantRef ) const;
        bool        operator== ( constantRef ) const;
        bool        operator!= ( constantRef ) const;
        bool        operator>= ( constantRef ) const;
        bool        operator>  ( constantRef ) const;

};

// stream
std::ostream& operator<< ( std::ostream& os, const BigNumber& sdbn );
std::istream& operator>> ( std::istream& is, const BigNumber& sdbn );

#endif // BIGNUMBER_H