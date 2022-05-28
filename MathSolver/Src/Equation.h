#pragma once

#include "ITerm.h"

#include <memory>

struct Equation
{
public:
	template <Term Lhs, Term Rhs>
	Equation(Lhs&& lhs, Rhs&& rhs);

	bool        simplify();
	std::string toString() const;

public:
	std::unique_ptr<ITerm> m_Lhs, m_Rhs;
};

//----------------
// Implementation
//----------------

template <Term Lhs, Term Rhs>
Equation::Equation(Lhs&& lhs, Rhs&& rhs)
    : m_Lhs(std::make_unique<Lhs>(std::move(lhs))), m_Rhs(std::make_unique<Rhs>(std::move(rhs)))
{
}