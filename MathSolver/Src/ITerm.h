#pragma once

#include "Utils/Tuple.h"

#include <string>
#include <utility>

struct Variable
{
public:
	Variable(const std::string& name) : m_Name(name) {}
	Variable(std::string&& name) : m_Name(std::move(name)) {}

	const std::string& toString() const { return m_Name; }

public:
	std::string m_Name;
};

enum class ETermType : std::uint32_t
{
	Function,
	Value,
	Power,
	Add,
	Multiplication,
	Division
};

struct ITerm
{
public:
	virtual ~ITerm() = default;

	virtual ETermType getType() const = 0;

	virtual std::size_t termCount() const           = 0;
	virtual bool        isFirstTermNegative() const = 0;

	virtual bool isDefined() const                      = 0;
	virtual void flipSign()                             = 0;
	virtual bool simplify(std::unique_ptr<ITerm>& self) = 0;

	virtual std::string toString(bool first = true) const = 0;
};

template <class T>
concept Term = std::is_base_of_v<ITerm, T>;