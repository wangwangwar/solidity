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

#pragma once

#include <libsolidity/ast/ASTVisitor.h>
#include <libsolidity/inlineasm/AsmData.h>
#include <libsolidity/interface/ErrorReporter.h>
#include <string>

namespace dev
{
namespace solidity
{
namespace julia
{

class SourceUnit;

class Compiler: private ASTConstVisitor
{
public:
	Compiler(ErrorReporter& _errorReporter): m_errorReporter(_errorReporter) {}

	/// resets the state
	void reset() { m_processed = false; }

	/// process a source unit
	void process(SourceUnit const& _source) { solAssert(!m_processed, ""); _source.accept(*this); m_processed = true; }

	/// @returns the JULIA block
	assembly::Block body() const { solAssert(m_processed, ""); return m_body; }

private:
	virtual bool visitNode(ASTNode const& _node) override
	{
		m_errorReporter.fatalJuliaCompilerError(_node, "ASTNode not supported");
		return false;
	}

	virtual bool visit(SourceUnit const&) override { return true; }
	virtual bool visit(PragmaDirective const&) override { return false; }

	bool m_processed = false;
	assembly::Block m_body;
	ErrorReporter& m_errorReporter;
};

}
}
}
