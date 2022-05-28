#include "Equation.h"

bool Equation::simplify()
{
	bool changed = false;
	changed |= m_Lhs->simplify(m_Lhs);
	changed |= m_Rhs->simplify(m_Rhs);
	return changed;
}

std::string Equation::toString() const
{
	return m_Lhs->toString() + " = " + m_Rhs->toString();
}