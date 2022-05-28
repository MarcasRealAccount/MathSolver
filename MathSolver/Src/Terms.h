#pragma once

#include "ITerm.h"

#include <memory>

struct Terms
{
public:
	template <Term... Ts>
	Terms(Ts&&... terms);
	template <Term... Ts>
	Terms(std::unique_ptr<Ts>&&... terms);
	template <Term... Ts>
	Terms(Utils::Tuple<Ts...>&& terms);

	auto insert(std::size_t index, std::unique_ptr<ITerm>&& term) { return m_Terms.insert(m_Terms.begin() + index, std::move(term)); }
	auto erase(std::vector<std::unique_ptr<ITerm>>::const_iterator iter) { return m_Terms.erase(iter); }

	auto& operator[](std::size_t index) { return m_Terms[index]; }
	auto& operator[](std::size_t index) const { return m_Terms[index]; }

	auto begin() { return m_Terms.begin(); }
	auto begin() const { return m_Terms.begin(); }
	auto cbegin() const { return m_Terms.cbegin(); }
	auto end() { return m_Terms.end(); }
	auto end() const { return m_Terms.end(); }
	auto cend() const { return m_Terms.cend(); }
	auto rbegin() { return m_Terms.rbegin(); }
	auto rbegin() const { return m_Terms.rbegin(); }
	auto crbegin() const { return m_Terms.crbegin(); }
	auto rend() { return m_Terms.rend(); }
	auto rend() const { return m_Terms.rend(); }
	auto crend() const { return m_Terms.crend(); }

	auto size() const { return m_Terms.size(); }
	auto empty() const { return m_Terms.empty(); }

public:
	std::vector<std::unique_ptr<ITerm>> m_Terms;
};

struct FunctionTerm : public ITerm
{
public:
	template <Term... Ts>
	FunctionTerm(const std::string& name, Ts&&... arguments);
	template <Term... Ts>
	FunctionTerm(std::string&& name, Ts&&... arguments);
	template <Term... Ts>
	FunctionTerm(const std::string& name, std::unique_ptr<Ts>&&... arguments);
	template <Term... Ts>
	FunctionTerm(std::string&& name, std::unique_ptr<Ts>&&... arguments);
	FunctionTerm(const std::string& name, Terms&& arguments) : m_Name(name), m_Arguments(std::move(arguments)) {}
	FunctionTerm(std::string&& name, Terms&& arguments) : m_Name(std::move(name)), m_Arguments(std::move(arguments)) {}

	virtual ETermType getType() const override { return ETermType::Function; }

	virtual std::size_t termCount() const override;
	virtual bool        isFirstTermNegative() const override { return false; }

	virtual bool isDefined() const override;
	virtual void flipSign() override;
	virtual bool simplify(std::unique_ptr<ITerm>& self) override;

	virtual std::string toString(bool first = true) const override;

public:
	std::string m_Name;
	Terms       m_Arguments;
};

struct ValueTerm : public ITerm
{
public:
	ValueTerm(std::int64_t numerator = 1, std::uint64_t denomenator = 1, std::int64_t exponent = 1) : m_Variable(""), m_Numerator(numerator), m_Denomenator(denomenator), m_Exponent(exponent) {}
	ValueTerm(const std::string& variable, std::int64_t numerator = 1, std::uint64_t denomenator = 1, std::int64_t exponent = 1) : m_Variable(variable), m_Numerator(numerator), m_Denomenator(denomenator), m_Exponent(exponent) {}
	ValueTerm(std::string&& variable, std::int64_t numerator = 1, std::uint64_t denomenator = 1, std::int64_t exponent = 1) : m_Variable(std::move(variable)), m_Numerator(numerator), m_Denomenator(denomenator), m_Exponent(exponent) {}

	virtual ETermType getType() const override { return ETermType::Value; }

	virtual std::size_t termCount() const override;
	virtual bool        isFirstTermNegative() const override { return m_Numerator < 0; }

	virtual bool isDefined() const override;
	virtual void flipSign() override;
	virtual bool simplify(std::unique_ptr<ITerm>& self) override;

	virtual std::string toString(bool first = true) const override;

public:
	Variable      m_Variable;
	std::int64_t  m_Numerator;
	std::uint64_t m_Denomenator;
	std::int64_t  m_Exponent;
};

struct PowerTerm : public ITerm
{
public:
	template <Term Base, Term Exponent>
	PowerTerm(Base&& base, Exponent&& exponent);
	template <Term Base, Term Exponent>
	PowerTerm(std::unique_ptr<Base>&& base, std::unique_ptr<Exponent>&& exponent);

	virtual ETermType getType() const override { return ETermType::Power; }

	virtual std::size_t termCount() const override;
	virtual bool        isFirstTermNegative() const override { return false; }

	virtual bool isDefined() const override;
	virtual void flipSign() override;
	virtual bool simplify(std::unique_ptr<ITerm>& self) override;

	virtual std::string toString(bool first = true) const override;

public:
	std::unique_ptr<ITerm> m_Base, m_Exponent;
};

struct AddTerm : public ITerm
{
public:
	template <Term... Ts>
	AddTerm(Ts&&... terms);
	template <Term... Ts>
	AddTerm(std::unique_ptr<Ts>&&... terms);
	AddTerm(Terms&& terms) : m_Terms(std::move(terms)) {}

	virtual ETermType getType() const override { return ETermType::Add; }

	virtual std::size_t termCount() const override;
	virtual bool        isFirstTermNegative() const override { return !m_Terms.empty() ? m_Terms[0]->isFirstTermNegative() : false; }

	virtual bool isDefined() const override;
	virtual void flipSign() override;
	virtual bool simplify(std::unique_ptr<ITerm>& self) override;

	virtual std::string toString(bool first = true) const override;

public:
	Terms m_Terms;
};

struct MultiplicationTerm : public ITerm
{
public:
	template <Term... Ts>
	MultiplicationTerm(Ts&&... terms);
	template <Term... Ts>
	MultiplicationTerm(std::unique_ptr<Ts>&&... terms);
	MultiplicationTerm(Terms&& terms) : m_Terms(std::move(terms)) {}

	virtual ETermType getType() const override { return ETermType::Multiplication; }

	virtual std::size_t termCount() const override;
	virtual bool        isFirstTermNegative() const override { return !m_Terms.empty() ? m_Terms[0]->isFirstTermNegative() : false; }

	virtual bool isDefined() const override;
	virtual void flipSign() override;
	virtual bool simplify(std::unique_ptr<ITerm>& self) override;

	virtual std::string toString(bool first = true) const override;

public:
	Terms m_Terms;
};

struct DivisionTerm : public ITerm
{
public:
	template <Term Numerator, Term Denomenator>
	DivisionTerm(Numerator&& numerator, Denomenator&& denomenator);
	template <Term Numerator, Term Denomenator>
	DivisionTerm(std::unique_ptr<Numerator>&& numerator, std::unique_ptr<Denomenator>&& denomenator);

	virtual ETermType getType() const override { return ETermType::Division; }

	virtual std::size_t termCount() const override;
	virtual bool        isFirstTermNegative() const override { return m_Numerator->termCount() == 1 && m_Numerator->isFirstTermNegative(); }

	virtual bool isDefined() const override;
	virtual void flipSign() override;
	virtual bool simplify(std::unique_ptr<ITerm>& self) override;

	virtual std::string toString(bool first = true) const override;

public:
	std::unique_ptr<ITerm> m_Numerator, m_Denomenator;
};

//----------------
// Implementation
//----------------

template <Term... Ts>
Terms::Terms(Ts&&... terms)
    : Terms(Utils::Tuple<Ts...> { std::move(terms)... })
{
}

template <Term... Ts>
Terms::Terms(std::unique_ptr<Ts>&&... terms)
    : m_Terms({ std::move(terms)... })
{
}

template <Term... Ts>
Terms::Terms(Utils::Tuple<Ts...>&& terms)
    : m_Terms(Utils::Details::make_unique_ptrs<ITerm>(std::move(terms)))
{
}

template <Term... Ts>
FunctionTerm::FunctionTerm(const std::string& name, Ts&&... arguments)
    : m_Name(name), m_Arguments(std::move(arguments)...)
{
}

template <Term... Ts>
FunctionTerm::FunctionTerm(std::string&& name, Ts&&... arguments)
    : m_Name(std::move(name)), m_Arguments(std::move(arguments)...)
{
}

template <Term... Ts>
FunctionTerm::FunctionTerm(const std::string& name, std::unique_ptr<Ts>&&... arguments)
    : m_Name(name), m_Arguments(std::move(arguments)...)
{
}

template <Term... Ts>
FunctionTerm::FunctionTerm(std::string&& name, std::unique_ptr<Ts>&&... arguments)
    : m_Name(std::move(name)), m_Arguments(std::move(arguments)...)
{
}

template <Term Base, Term Exponent>
PowerTerm::PowerTerm(Base&& base, Exponent&& exponent)
    : m_Base(std::make_unique<Base>(std::move(base))), m_Exponent(std::make_unique<Exponent>(std::move(exponent)))
{
}

template <Term Base, Term Exponent>
PowerTerm::PowerTerm(std::unique_ptr<Base>&& base, std::unique_ptr<Exponent>&& exponent)
    : m_Base(std::move(base)), m_Exponent(std::move(exponent))
{
}

template <Term... Ts>
AddTerm::AddTerm(Ts&&... terms)
    : m_Terms(std::move(terms)...)
{
}

template <Term... Ts>
AddTerm::AddTerm(std::unique_ptr<Ts>&&... terms)
    : m_Terms(std::move(terms)...)
{
}

template <Term... Ts>
MultiplicationTerm::MultiplicationTerm(Ts&&... terms)
    : m_Terms(std::move(terms)...)
{
}

template <Term... Ts>
MultiplicationTerm::MultiplicationTerm(std::unique_ptr<Ts>&&... terms)
    : m_Terms(std::move(terms)...)
{
}

template <Term Numerator, Term Denomenator>
DivisionTerm::DivisionTerm(Numerator&& numerator, Denomenator&& denomenator)
    : m_Numerator(std::make_unique<Numerator>(std::move(numerator))), m_Denomenator(std::make_unique<Denomenator>(std::move(denomenator)))
{
}

template <Term Numerator, Term Denomenator>
DivisionTerm::DivisionTerm(std::unique_ptr<Numerator>&& numerator, std::unique_ptr<Denomenator>&& denomenator)
    : m_Numerator(std::move(numerator)), m_Denomenator(std::move(denomenator))
{
}