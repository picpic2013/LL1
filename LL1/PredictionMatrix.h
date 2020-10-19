#pragma once

#include <string>
#include <map>
#include "Status.h"

/**
*  Data structure for the prediction matrix
*  @Author: Liming Liu
*/
class PredictionMatrix {
public:
	// Constructor
	PredictionMatrix();
	// Deep Copy Constructor
	PredictionMatrix(const PredictionMatrix& copy);
	// Destructor
	~PredictionMatrix();

	/**
	*  For the given example: @Select(S -> aBc) = {a, c}
	*  
	*  @Param S: S in the example
	*  @Param a: a in the expmple ({a, c})
	*  @Returns: whether there is certain element in the matrix
	*/
	bool has(const std::string& S, const std::string& a);

	/**
	*  Get grammar derivation by row character and column string
	*  For the given example: @Select(S -> aBc) = {a, c}
	*
	*  @Param S: row character ( S in the example )
	*  @Param a: column string ( a and c in the expmple ({a, c}) )
	*  @Returns: grammar derivation ( "aBc" for the example. )
	*/
	std::string get(const std::string& S, const std::string& a);

	/**
	*  add grammar derivation by row character and column string
	*  For the given example: @Select(S -> aBc) = {a, c}
	*
	*  @Param S: row character ( S in the example )
	*  @Param a: column string ( a and c in the expmple ({a, c}) )
	*  @Param exp: grammar derivation ( "aBc" for the example. )
	*/
	void add(const std::string& S, const std::string& a, const std::string& exp);

private:
	// inner data structure
	// S => { a => 'aB'}
	std::map<std::string, std::map<std::string, std::string> > data;
};