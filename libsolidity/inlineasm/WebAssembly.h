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
 * Julia to WebAssembly code generator.
 */

#pragma once

#include <libsolidity/inlineasm/AsmAnalysis.h>
#include <libsolidity/interface/Exceptions.h>

#include <functional>

namespace dev
{
namespace eth
{
class Assembly;
}
namespace solidity
{
namespace assembly
{
struct Block;

class WebAssembly
{
public:
	WebAssembly(ErrorList& _errors): m_errors(_errors) {}
	/// Performs code generation and @returns S-expressions
	std::string assemble(Block const& _block);
	/// Performs code generator and @returns WebAssembly binary
	///bytes assemble(Block const& _block);
private:
	ErrorList& m_errors;
};

}
}
}
