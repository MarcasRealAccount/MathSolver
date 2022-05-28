#include "Equation.h"
#include "Terms.h"
#include "Utils/Core.h"

#include <iostream>

int main(int argc, char** argv)
{
	Equation equation {
		MultiplicationTerm {
		    ValueTerm { 2, 3 },
		    ValueTerm { 2, 1 } },
		ValueTerm { "x" }
	};

	std::cout << equation.toString() << '\n';
	if (equation.simplify())
		std::cout << equation.toString() << '\n';
}