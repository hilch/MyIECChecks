#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "MyChecks.h"
#ifdef __cplusplus
	};
#endif

#include <cstring>
#include <cstdio>
#include <cmath>
#include <limits>


unsigned long bur_heap_size = 0x10000;

//#define GET_BACKTRACE_DATA(x) *((char*)0) = 0; /* create a page fault to get the backtrace data */
#define GET_BACKTRACE_DATA {int x = 1/0;}

/* helper function to make an entry into system logger */
UDINT MakeEntry(UINT number, UDINT errorInfo, STRING* text);


/* Check divisor for DINT division */
DINT CheckDivDint(DINT divisor) {
	if( divisor == 0 ){
		MakeEntry( 55555, divisor, "'Divide by Zero' with divisor of type DINT" );
		DINT zero = 0;
		DINT x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}

/* Check divisor for UDINT division */
UDINT CheckDivUdint(UDINT divisor) {
	if( divisor == 0 ){
		MakeEntry( 55555, divisor, "'Divide by Zero' with divisor of type UDINT" );
		UDINT zero = 0;
		UDINT x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}



/* Check divisor for INT division */
INT CheckDivInt(INT divisor){
	if( divisor == 0 ){
		MakeEntry( 55555, divisor, "'Divide by Zero' with divisor of type INT" );
		INT zero = 0;
		INT x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}


/* Check divisor for UINT division */
UINT CheckDivUint(UINT divisor){
	if( divisor == 0 ){
		MakeEntry( 55555, divisor, "'Divide by Zero' with divisor of type UINT" );
		UINT zero = 0;
		UINT x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}



SINT CheckDivSint(SINT divisor){
	if( divisor == 0 ){
		MakeEntry( 55555, divisor, "'Divide by Zero' with divisor of type SINT" );
		SINT zero = 0;
		SINT x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}

USINT CheckDivUsint(USINT divisor){
	if( divisor == 0 ){
		MakeEntry( 55555, divisor, "'Divide by Zero' with divisor of type USINT" );
		USINT zero = 0;
		USINT x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}

/* Check divisor for REAL division */
REAL CheckDivReal(REAL divisor){
	if( std::abs(divisor) < std::numeric_limits<float>::epsilon() ){
		MakeEntry( 55555, 0, "'Divide by Zero' with divisor of type REAL" );
		int zero = 0;
		float x = 1/zero; /* create division error 25300 to get backtrace data */
		(void) x;
	}
	return divisor;
}

/* Check divisor for LREAL division */
LREAL CheckDivLReal(LREAL divisor){
	if( std::abs(divisor) < std::numeric_limits<double>::epsilon() ){
		MakeEntry( 55555, 0, "'Divide by Zero' with divisor of type LREAL" );
		int zero = 0;
		double x = 1/zero; /* create division error 25300 to get backtrace data */ 
		(void) x;
	}
	return divisor;
}

/* check range for array access */
DINT CheckBounds(DINT index, DINT lower, DINT upper){
	if( index < lower || index > upper ){
		char info[80];
		sprintf( info, "Array Index %d is outside [%d..%d]", index, lower, upper );
		MakeEntry( 55555, index, info );
		*((char*)0) = 0; /* create a page fault 25314 to get the backtrace data */
	}
	return index;
}

/* check range for enumeration */
DINT CheckRange(DINT value, DINT lower, DINT upper){
	if( value < lower || value > upper ){
		char info[80];
		sprintf( info, "Enumeration value %d is outside [%d..%d]", value, lower, upper );
		MakeEntry( 55555, value, info );
		int zero = 0;
		double x = 1/zero; /* create division error 25300 to get backtrace data */ 
		(void) x;
	}
	return value;
}

/* Check range for subrange of signed data types */
DINT CheckSignedSubrange(DINT value, DINT lower, DINT upper){
	if( value < lower || value > upper ){
		char info[80];
		sprintf( info, "Signed Subrange Index %d is outside [%d..%d]", value, lower, upper );
		MakeEntry( 55555, value, info );
		int zero = 0;
		double x = 1/zero; /* create division error 25300 to get backtrace data */ 
		(void) x;
	}
	return value;
}

/* Check range for subrange of unsigned data types */
UDINT CheckUnsignedSubrange(UDINT value, UDINT lower, UDINT upper){
	if( value < lower || value > upper ){
		char info[80];
		sprintf( info, "Unsigned Subrange Index %d is outside [%d..%d]", value, lower, upper );
		MakeEntry( 55555, value, info );
		int zero = 0;
		double x = 1/zero; /* create division error 25300 to get backtrace data */ 
		(void) x;
	}
	return value;
}


UDINT CheckReadAccess(UDINT address){
	if( address == 0 ){
		MakeEntry( 55555, 0, "Attempt to read from address of zero / NULL" );
		*((char*)0) = 0; /* create a page fault 25314 to get the backtrace data */
	}
	return address;
}

UDINT CheckWriteAccess(UDINT address){
	if( address == 0 ){
		MakeEntry( 55555, 0, "Attempt to write to address of zero / NULL" );
		*((char*)0) = 0; /* create a page fault 25314 to get the backtrace data */
	}
	return address;
}

UDINT MakeEntry(UINT number, UDINT errorInfo, STRING* text){

	STRING taskName[30];
	USINT group;
	ST_name( 0, taskName, &group );

	STRING out[255];
	std::sprintf( out, "%s in task '%s'", text, taskName );

	ERRxwarning( number, errorInfo, out );
	return 0;
}

