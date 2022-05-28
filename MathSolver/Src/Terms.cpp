#include "Terms.h"

#include "MathUtils.h"

std::size_t FunctionTerm::termCount() const
{
	return m_Arguments.size() == 1 && m_Arguments[0]->termCount() <= 1 ? 2 : 1;
}

bool FunctionTerm::isDefined() const
{
	// TODO(MarcasRealAccount): Implement function wise definition
	return true;
}

void FunctionTerm::flipSign()
{
}

bool FunctionTerm::simplify(std::unique_ptr<ITerm>& self)
{
	bool changed = false;
	for (auto& arg : m_Arguments)
		changed |= arg->simplify(arg);

	// TODO(MarcasRealAccount): Implement function wise simplification
	return changed;
}

std::string FunctionTerm::toString([[maybe_unused]] bool first) const
{
	std::string str = m_Name;

	if (m_Arguments.empty())
	{
		str += "()";
	}
	else if (first && m_Arguments.size() == 1 && m_Arguments[0]->termCount() <= 1)
	{
		str += ' ' + m_Arguments[0]->toString();
	}
	else
	{
		str += '(';
		bool addComma = false;
		for (auto& argument : m_Arguments)
		{
			if (addComma)
				str += ", ";
			addComma = true;
			str += argument->toString();
		}

		str += ')';
	}

	return str;
}

std::size_t ValueTerm::termCount() const
{
	std::size_t count = 1;
	if (!m_Variable.m_Name.empty() && ((m_Numerator != -1 && m_Numerator != 1) || m_Denomenator != 1))
		++count;
	if (m_Exponent != 1)
		++count;
	return count;
}

bool ValueTerm::isDefined() const
{
	return m_Denomenator != 0;
}

void ValueTerm::flipSign()
{
	m_Numerator = -m_Numerator;
}

bool ValueTerm::simplify(std::unique_ptr<ITerm>& self)
{
	bool changed = false;
	if (m_Exponent == 0)
	{
		if (m_Variable.m_Name.empty() && m_Numerator != 1 && m_Denomenator != 1)
		{
			// Simplify (n/d)^0 => 1
			m_Numerator   = 1;
			m_Denomenator = 1;
			m_Exponent    = 1;
			changed       = true;
		}
		else
		{
			// Simplify n/d * x^0 => n/d
			m_Variable.m_Name = "";
			changed           = true;
		}
	}
	else if (m_Exponent < 0 && m_Variable.m_Name.empty())
	{
		// Simplify (n/d)^(-e) => d/n
		std::uint64_t num = std::abs(m_Numerator);
		m_Numerator       = m_Numerator < 0 ? -static_cast<std::int64_t>(m_Denomenator) : m_Denomenator;
		m_Denomenator     = num;
		m_Exponent        = -m_Exponent;
		changed           = true;
	}

	// Simplify fraction
	std::uint64_t gcd = MathUtils::GCD(std::abs(m_Numerator), m_Denomenator);
	m_Numerator /= gcd;
	m_Denomenator /= gcd;
	changed |= gcd != 1;

	return changed;
}

std::string ValueTerm::toString(bool first) const
{
	std::string str;

	if (first && m_Numerator < 0 && m_Exponent == 1)
		str += '-';

	if (m_Variable.m_Name.empty())
	{
		if (m_Exponent != 1)
		{
			str += '(';
			if (m_Numerator < 0)
				str += '-';
		}

		str += std::to_string(std::abs(m_Numerator));
		if (m_Denomenator != 1)
			str += '/' + std::to_string(m_Denomenator);

		if (m_Exponent != 1)
			str += ")^" + std::to_string(m_Exponent);
	}
	else
	{
		str += m_Variable.toString();

		if (m_Denomenator != 1 || (m_Numerator != 1 && m_Numerator != -1))
			str += std::to_string(std::abs(m_Numerator));
		if (m_Denomenator != 1)
			str += '/' + std::to_string(m_Denomenator) + " * ";

		if (m_Exponent != 1)
			str += '^' + std::to_string(m_Exponent);
	}

	return str;
}

std::size_t PowerTerm::termCount() const
{
	return 1;
}

bool PowerTerm::isDefined() const
{
	return m_Base->isDefined() && m_Exponent->isDefined();
}

void PowerTerm::flipSign()
{
}

bool PowerTerm::simplify(std::unique_ptr<ITerm>& self)
{
	bool changed = false;
	changed |= m_Base->simplify(m_Base);
	changed |= m_Exponent->simplify(m_Exponent);

	if (m_Base->isDefined())
	{
		if (m_Exponent->isDefined())
		{
			if (m_Exponent->getType() == ETermType::Value)
			{
				ValueTerm* exponentT = reinterpret_cast<ValueTerm*>(m_Exponent.get());
				// Simplify b^0 => 1
				// 0/d * x^e => 0, b > 0
				if (exponentT->m_Numerator == 0 && exponentT->m_Denomenator != 0)
				{
					self    = std::make_unique<ValueTerm>(1);
					changed = true;
				}
			}
			else if (m_Exponent->isFirstTermNegative())
			{
				// Simplify b^-e => 1/(b^e)
				m_Exponent->flipSign();
				self    = std::make_unique<DivisionTerm>(std::make_unique<ValueTerm>(1), std::move(self));
				changed = true;
			}
		}
	}
	return changed;
}

std::string PowerTerm::toString([[maybe_unused]] bool first) const
{
	std::string str;
	if (m_Base->termCount() > 1)
		str += '(' + m_Base->toString() + ')';
	else
		str += m_Base->toString();
	str += '^';
	if (m_Exponent->termCount() > 1)
		str += '(' + m_Exponent->toString() + ')';
	else
		str += m_Exponent->toString();
	return str;
}

std::size_t AddTerm::termCount() const
{
	std::size_t terms = 0;
	for (auto& term : m_Terms)
		terms += term->termCount();
	return terms;
}

bool AddTerm::isDefined() const
{
	for (auto& term : m_Terms)
		if (!term->isDefined())
			return false;
	return true;
}

void AddTerm::flipSign()
{
	for (auto& term : m_Terms)
		term->flipSign();
}

bool AddTerm::simplify(std::unique_ptr<ITerm>& self)
{
	bool changed = false;
	for (auto& term : m_Terms)
		changed |= term->simplify(term);

	// Combine identical terms a + a => 2a, 3/5 * a + 5/3 * a => 9/15 * a + 25/15 * a => 34/15 * a
	// a^2 + a => a^2 + a


	return changed;
}

std::string AddTerm::toString(bool first) const
{
	std::string str;
	bool        sfirst = true;
	for (auto& term : m_Terms)
	{
		if (!sfirst)
			str += term->isFirstTermNegative() ? " - " : " + ";
		str += term->toString(first);
		first  = false;
		sfirst = false;
	}
	return str;
}

std::size_t MultiplicationTerm::termCount() const
{
	std::size_t terms = 0;
	for (auto& term : m_Terms)
		terms += term->termCount();
	return terms;
}

bool MultiplicationTerm::isDefined() const
{
	for (auto& term : m_Terms)
		if (!term->isDefined())
			return false;
	return true;
}

void MultiplicationTerm::flipSign()
{
	if (!m_Terms.empty())
		m_Terms[0]->flipSign();
}

bool MultiplicationTerm::simplify(std::unique_ptr<ITerm>& self)
{
	bool changed = false;
	for (auto& term : m_Terms)
		changed |= term->simplify(term);

	// Combine all fractions together
	std::int64_t  numerator   = 1;
	std::uint64_t denomenator = 1;

	for (auto itr = m_Terms.begin(); itr != m_Terms.end();)
	{
		auto& term = *itr;
		if (term->getType() == ETermType::Value)
		{
			ValueTerm* termT = reinterpret_cast<ValueTerm*>(term.get());
			if (termT->m_Variable.m_Name.empty())
			{
				if (termT->m_Exponent == 1)
				{
					numerator *= termT->m_Numerator;
					denomenator *= termT->m_Denomenator;
					termT->m_Numerator   = 1;
					termT->m_Denomenator = 1;
					changed              = true;
					itr                  = m_Terms.erase(itr);
				}
				else if (termT->m_Exponent == -1)
				{
					numerator *= termT->m_Denomenator;
					denomenator *= termT->m_Numerator;
					termT->m_Numerator   = 1;
					termT->m_Denomenator = 1;
					termT->m_Exponent    = 1;
					changed              = true;
					itr                  = m_Terms.erase(itr);
				}
				else
				{
					++itr;
				}
			}
			else
			{
				numerator *= termT->m_Numerator;
				denomenator *= termT->m_Denomenator;
				termT->m_Numerator   = 1;
				termT->m_Denomenator = 1;
				changed              = true;
				++itr;
			}
		}
		else
		{
			++itr;
		}
	}

	if (numerator != 1 || denomenator != 1)
	{
		auto& fraction = (*m_Terms.insert(0, std::make_unique<ValueTerm>(numerator, denomenator)));
		changed |= fraction->simplify(fraction);
	}

	return changed;
}

std::string MultiplicationTerm::toString(bool first) const
{
	std::string str;
	bool        sfirst = true;
	for (auto& term : m_Terms)
	{
		if (!sfirst)
			str += " * ";
		str += term->toString(first);
		first  = false;
		sfirst = false;
	}
	return str;
}

std::size_t DivisionTerm::termCount() const
{
	return m_Numerator->termCount() + m_Denomenator->termCount();
}

bool DivisionTerm::isDefined() const
{
	return m_Numerator->isDefined() && m_Denomenator->isDefined();
}

void DivisionTerm::flipSign()
{
	m_Numerator->flipSign();
}

bool DivisionTerm::simplify(std::unique_ptr<ITerm>& self)
{
	bool changed = false;
	changed |= m_Numerator->simplify(m_Numerator);
	changed |= m_Denomenator->simplify(m_Denomenator);
	return changed;
}

std::string DivisionTerm::toString(bool first) const
{
	std::string str;
	if (first && m_Numerator->termCount() == 1 && m_Numerator->isFirstTermNegative())
		str += '-';

	if (m_Numerator->termCount() > 1)
		str += '(' + m_Numerator->toString() + ')';
	else
		str += m_Numerator->toString(false);
	str += '/';
	if (m_Denomenator->termCount() > 1)
		str += '(' + m_Denomenator->toString() + ')';
	else
		str += m_Denomenator->toString();
	return str;
}