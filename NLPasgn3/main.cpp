#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

enum class Operation
{
	SUBSTITUTION,
	DELETION,
	INSERTION,
	NOTHING
};

class BackPointer
{
public:
	BackPointer() :
	d(0),
	op(Operation::NOTHING)
	{
	}
	
	BackPointer(unsigned distance, Operation operation) :
	d(distance),
	op(operation)
	{
	}
	
	unsigned distance()
	{ return d; }
	
	Operation operation()
	{	return op; }

private:
	unsigned d;
	Operation op;
};

class MatrixNxN
{
public:
	MatrixNxN(const std::string source, const std::string target) :
	src(source),
	trg(target),
	m()
	{
		// Trim source of ending '*' characters
		//while (src.at(src.length() - 1) == '*')
		//	src.pop_back();
		fillMatrix();
	}
	
	MatrixNxN(const char* const source, const char* const target) :
	src(source),
	trg(target),
	m()
	{
		// Trim source of ending '*' characters
		//while (src.at(src.length() - 1) == '*')
		//	src.pop_back();
		fillMatrix();
	}
	
	void print()
	{
		std::cout << ". #";
		for (unsigned i = 0; i < trg.length(); ++i)
			std::cout << " " << trg.at(i);
		std::cout << std::endl;
		
		for (unsigned i = 0; i <= src.length(); ++i)
		{
			if (i > 0) std::cout << src.at(i - 1) << " ";
			else std::cout << "# ";
			
			for (unsigned j = 0; j <= trg.length(); ++j)
				std::cout << m.at(i).at(j).distance() << " ";
			std::cout << std::endl;
		}
	}
	
	void printOperations()
	{
		// Print out the operation list
		unsigned i = src.length();
		unsigned j = trg.length();
		std::stack<char> st;

		while (i > 0 || j > 0)
		{
			switch (m.at(i).at(j).operation())
			{
			case Operation::SUBSTITUTION:
				st.push('s');
				--i;
				--j;
				break;
				
			case Operation::INSERTION:
				st.push('i');
				--j;
				break;
				
			case Operation::DELETION:
				st.push('d');
				--i;
				break;
				
			case Operation::NOTHING:
				st.push('_');
				--i;
				--j;
				break;
			}
		}
		while (st.size() > 0)
		{
			std::cout << st.top();
			st.pop();
		}
		std::cout << std::endl;
	}
	
	unsigned getMinDistance()
	{
		return m.at(src.length()).at(trg.length()).distance();
	}
	
private:
	std::string src;
	std::string trg;
	
	typedef std::vector<BackPointer> row;
	std::vector<row> m;
	
	void fillMatrix()
	{
		m = std::vector<row>(src.length() + 1, row(trg.length() + 1));
		
		m.at(0).at(0) = BackPointer(0, Operation::NOTHING);
		for (unsigned i = 1; i <= src.length(); ++i)
		{
			unsigned v = m.at(i - 1).at(0).distance();
			v += src.at(i - 1) == '*' ? 0 : 1;
			m.at(i).at(0) = BackPointer(v, Operation::DELETION);
		}
		
		for (unsigned j = 1; j <= trg.length(); ++j)
		{
			unsigned v = m.at(0).at(j - 1).distance();
			v += trg.at(j - 1) == '*' ? 0 : 1;
			m.at(0).at(j) = BackPointer(v, Operation::INSERTION);
		}
		
		for (unsigned i = 1; i <= src.length(); ++i)
			for (unsigned j = 1; j <= trg.length(); ++j)
			{
				unsigned del = m.at(i - 1).at(j).distance() + 1;
				unsigned sub = m.at(i - 1).at(j - 1).distance();
				if (src.at(i - 1) == '*')
					sub += 1;
				else
					sub += src.at(i - 1) == trg.at(j - 1) ? 0 : 2;
				unsigned ins = m.at(i).at(j - 1).distance() + 1;
				
				if (sub <= del)
				{
					if (sub <= ins)
					{
						// Use sub
						if (src.at(i - 1) != trg.at(j - 1))
							m.at(i).at(j) = BackPointer(sub, Operation::SUBSTITUTION);
						else
							m.at(i).at(j) = BackPointer(sub, Operation::NOTHING);
					}
					else
					{
						// Use ins
						m.at(i).at(j) = BackPointer(ins, Operation::INSERTION);
					}
				}
				else
				{
					if (del <= ins)
					{
						// Use del
						m.at(i).at(j) = BackPointer(del, Operation::DELETION);
					}
					else
					{
						// Use ins
						m.at(i).at(j) = BackPointer(ins, Operation::INSERTION);
					}
				}
			}
	}
};

int main(int argc, char** argv)
{
	// Check correct argument number
	if (argc != 3)
	{
		std::cout << "Error! Needs exactly two arguments." << std::endl;
		return -1;
	}
	
	// Rporting what we are doing
	std::cout << "Converting " << argv[1] << " to " << argv[2] << "." << std::endl;
	
	// Calculate Minimum Edit Distance
	MatrixNxN matrix(argv[1], argv[2]);
	
	// Print out the calculated matrix
	matrix.print();
	
	// Print out the alignment
	std::cout << argv[1] << std::endl << argv[2] << std::endl;
	matrix.printOperations();
	
	// Print out the minimum edit distance
	std:: cout << "The minimum edit distance is " << matrix.getMinDistance() << "." << std::endl;
	
	return 0;
}
