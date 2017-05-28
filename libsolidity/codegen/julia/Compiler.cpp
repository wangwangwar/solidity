/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @author Alex Beregszaszi
 * @date 2017
 * Component that translates Solidity code into JULIA.
 */

#include <libsolidity/codegen/julia/Compiler.h>
#include <boost/algorithm/string/predicate.hpp>

using namespace std;
using namespace dev;
using namespace dev::solidity;
using namespace dev::solidity::julia;

bool Compiler::visit(ContractDefinition const& _contract)
{
	if (!_contract.baseContracts().empty())
		m_errorReporter.juliaCompilerError(*_contract.baseContracts().front(), "Inheritance not supported.");
	if (!_contract.definedStructs().empty())
		m_errorReporter.juliaCompilerError(*_contract.definedStructs().front(), "User-defined types not supported.");
	if (!_contract.definedEnums().empty())
		m_errorReporter.juliaCompilerError(*_contract.definedEnums().front(), "User-defined types not supported.");
	if (!_contract.events().empty())
		m_errorReporter.juliaCompilerError(*_contract.events().front(), "Events not supported.");
	if (!_contract.functionModifiers().empty())
		m_errorReporter.juliaCompilerError(*_contract.functionModifiers().front(), "Modifiers not supported.");

	ASTNode::listAccept(_contract.definedFunctions(), *this);

	return false;
}

bool Compiler::visit(FunctionDefinition const& _function)
{
	if (!_function.isImplemented())
	{
		m_errorReporter.juliaCompilerError(_function, "Unimplemented functions not supported.");
		return false;
	}
	if (_function.name().empty())
	{
		m_errorReporter.juliaCompilerError(_function, "Fallback functions not supported.");
		return false;
	}
	if (!_function.modifiers().empty())
	{
		m_errorReporter.juliaCompilerError(_function, "Modifiers not supported.");
		return false;
	}
	if (!_function.parameters().empty())
	{
		m_errorReporter.juliaCompilerError(_function, "Parameters not supported.");
		return false;
	}
	if (!_function.returnParameters().empty())
	{
		m_errorReporter.juliaCompilerError(_function, "Return parameters not supported.");
		return false;
	}

	assembly::FunctionDefinition funDef;
	funDef.name = _function.name();
	funDef.location = _function.location();
	m_currentFunction = funDef;
	_function.body().accept(*this);
	return false;
}

void Compiler::endVisit(FunctionDefinition const&)
{
	// invalidate m_currentFunction
	m_body.statements.emplace_back(m_currentFunction);
}

bool Compiler::visit(Block const& _node)
{
	for (size_t i = 0; i < _node.statements().size(); ++i)
		_node.statements()[i]->accept(*this);
	return false;
}

bool Compiler::visit(Throw const& _throw)
{
	assembly::Literal zero;
	zero.kind = assembly::LiteralKind::Number;
	zero.value = "0";
	zero.type = "u256";

	assembly::FunctionCall funCall;
	funCall.functionName.name = "revert";
	funCall.arguments.push_back(zero);
	funCall.arguments.push_back(zero);
	funCall.location = _throw.location();
	m_currentFunction.body.statements.emplace_back(funCall);
	return false;
}
