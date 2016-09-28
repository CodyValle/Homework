#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <deque>

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
		fillMatrix();
	}
	
	MatrixNxN(const char* const source, const char* const target) :
	src(source),
	trg(target),
	m()
	{
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
	
	std::deque<char> getOperations()
	{
		// Print out the operation list
		unsigned i = src.length();
		unsigned j = trg.length();
		std::deque<char> st;

		while (i > 0 || j > 0)
		{
			switch (m.at(i).at(j).operation())
			{
			case Operation::SUBSTITUTION:
				st.push_front('s');
				--i;
				--j;
				break;
				
			case Operation::INSERTION:
				st.push_front('i');
				--j;
				break;
				
			case Operation::DELETION:
				st.push_front('d');
				--i;
				break;
				
			case Operation::NOTHING:
				st.push_front('_');
				--i;
				--j;
				break;
			}
		}
		
		return st;
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
			m.at(i).at(0) = BackPointer(i, Operation::DELETION);
		
		for (unsigned j = 1; j <= trg.length(); ++j)
			m.at(0).at(j) = BackPointer(j, Operation::INSERTION);
		
		for (unsigned i = 1; i <= src.length(); ++i)
			for (unsigned j = 1; j <= trg.length(); ++j)
			{
				unsigned del = m.at(i - 1).at(j).distance() + 1;
				unsigned sub = m.at(i - 1).at(j - 1).distance();
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
					else if (ins <= del)
					{
						// Use ins
						m.at(i).at(j) = BackPointer(ins, Operation::INSERTION);
					}
					else
					{   
						// Use del
						m.at(i).at(j) = BackPointer(del, Operation::DELETION);
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
	
	// Set up variables
	std::string source(argv[1]);
	std::string target(argv[2]);
	
	// Rporting what we are doing
	std::cout << "Converting " << source << " to " << target << "." << std::endl;
	
	// Calculate Minimum Edit Distance
	MatrixNxN matrix(source, target);
	
	// Print out the calculated matrix
	matrix.print();
	
	/// Print out the alignment
	// Get the operation sequence
	std::deque<char> ops = matrix.getOperations();
	
	// Print source string with aligning asterisks 
	int curChar = 0; 
	for (unsigned i = 0; i < ops.size(); ++i)
	  std::cout << (ops.at(i) == 'i' ? '*' : source.at(curChar++));    
	std::cout << std::endl;
	         
	// Print target string with aligning asterisks
	curChar = 0;      
	for (unsigned i = 0; i < ops.size(); ++i)
	  std::cout << (ops.at(i) == 'd' ? '*' :target.at(curChar++));
	std::cout << std::endl;
	                                         	
	// Print operation sequence
	for (unsigned i = 0; i < ops.size(); ++i)
		std::cout << ops.at(i);
	std::cout << std::endl;
	
	
	// Print out the minimum edit distance
	std:: cout << "The minimum edit distance is " << matrix.getMinDistance() << "." << std::endl;
	
	return 0;
}
