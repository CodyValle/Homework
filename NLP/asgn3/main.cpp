// Authors: Cody Valle and Jinous Esmaeili
// File: asgn3.cpp
// Description: Takes two arguments and computes the minimum edit distance and alignment

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <deque>

// Calculates the minimum edit distance and operation list
class MinEditDistance
{
public:
	// Constructor
	MinEditDistance(const std::string source, const std::string target) :
	src(source),
	trg(target),
	m(),
	operations()
	{
		fillMatrix();
	}
	
	// Constructor
	MinEditDistance(const char* const source, const char* const target) :
	src(source),
	trg(target),
	m(),
	operations()
	{
		fillMatrix();
	}
	
	// Prints out the matrix
	void printMatrix()
	{
		std::cout << ". #"; // First chunk of the first row
		
		// Print out the target string with spaces between each character
		for (unsigned i = 0; i < trg.length(); ++i)
			std::cout << " " << trg.at(i);
		std::cout << std::endl;
		
		// Print out the source string and matrix rows
		for (unsigned i = 0; i <= src.length(); ++i)
		{
			// The soruce string part
			if (i > 0) std::cout << src.at(i - 1) << " ";
			else std::cout << "# ";
			
			// The matrix row part
			for (unsigned j = 0; j <= trg.length(); ++j)
				std::cout << m.at(i).at(j).distance() << " ";
			std::cout << std::endl;
		}
	}
	
	// Prints out the alignment
	void printAlignment()
	{
		// Print source string with aligning asterisks 
		unsigned curChar = 0; 
		for (unsigned i = 0; i < operations.size(); ++i)
			std::cout << (operations.at(i) == 'i' ? '*' : src.at(curChar++));    
		std::cout << std::endl;
			       
		// Print target string with aligning asterisks
		curChar = 0;      
		for (unsigned i = 0; i < operations.size(); ++i)
			std::cout << (operations.at(i) == 'd' ? '*' : trg.at(curChar++));
		std::cout << std::endl;
			                                       	
		// Print operation sequence
		for (unsigned i = 0; i < operations.size(); ++i)
			std::cout << operations.at(i);
		std::cout << std::endl;
	}
	
	// Gets the minimum edit distance
	unsigned getMinDistance()
	{ return m.at(src.length()).at(trg.length()).distance(); }
	
	// Gets the operation list
	std::deque<char> getOperations()
	{ return operations; }
	
private:
	// Operations for the editing the strings
	enum class Operation
	{
		SUBSTITUTION,
		DELETION,
		INSERTION,
		NOTHING
	};

	// An element of the minimum edit distance matrix
	class BackPointer
	{
	public:
		// Constructor
		BackPointer() :
		d(0),
		op(Operation::NOTHING)
		{}
	
		// Constructor
		BackPointer(unsigned distance, Operation operation) :
		d(distance),
		op(operation)
		{}
	
		// Gets the distance at this current element
		unsigned distance()
		{ return d; }
	
		// Gets the operation to get to this current element
		Operation operation()
		{	return op; }

	private:
		unsigned d;
		Operation op;
	};
	
	std::string src; // The string to start with
	std::string trg; // The string to end with
	
	typedef std::vector<BackPointer> Row; // Every row in the matrix is a vector of BackPointers
	std::vector<Row> m; // The matrix is a vector of Rows
	
	std::deque<char> operations; // The operation list
	
	void fillMatrix()
	{
		// Instantiate the matrix
		m = std::vector<Row>(src.length() + 1, Row(trg.length() + 1));
		
		// Create the BackPointer in the (0, 0) spot
		m.at(0).at(0) = BackPointer(0, Operation::NOTHING);
		
		// Fill in the first column
		for (unsigned i = 1; i <= src.length(); ++i)
			m.at(i).at(0) = BackPointer(i, Operation::DELETION);
		
		// Fill in the first row
		for (unsigned j = 1; j <= trg.length(); ++j)
			m.at(0).at(j) = BackPointer(j, Operation::INSERTION);
		
		// Fill the matrix
		for (unsigned i = 1; i <= src.length(); ++i)
			for (unsigned j = 1; j <= trg.length(); ++j)
			{
				// Calculate the operation costs
				unsigned del = m.at(i - 1).at(j).distance() + 1;
				unsigned sub = m.at(i - 1).at(j - 1).distance();
				sub += src.at(i - 1) == trg.at(j - 1) ? 0 : 2;
				unsigned ins = m.at(i).at(j - 1).distance() + 1;
				
				Operation op; // The type of operation we will use
				
				// Determine the type of operation
				if (sub <= del)
				{
					if (sub <= ins)
						op = Operation::SUBSTITUTION;
					else if (ins <= del)
						op = Operation::INSERTION;
					else
						op = Operation::DELETION;
				}
				else
				{
					if (del <= ins)
						op = Operation::DELETION;
					else
						op = Operation::INSERTION;
				}
				
				// Create and store the BackPointer
				switch (op)
				{
				case Operation::NOTHING:
				case Operation::SUBSTITUTION: // Substitution
					if (src.at(i - 1) != trg.at(j - 1))
					  m.at(i).at(j) = BackPointer(sub, Operation::SUBSTITUTION);
					else
						m.at(i).at(j) = BackPointer(sub, Operation::NOTHING);
					break;
					
				
				case Operation::DELETION: // Deletion
					m.at(i).at(j) = BackPointer(del, Operation::DELETION);
					break;
					
				
				case Operation::INSERTION: // Insertion
					m.at(i).at(j) = BackPointer(ins, Operation::INSERTION);
					break;
				}
			}
			
			calculateOperations(); // Calculate the operations
	}
	
	// Calculates the operations
	void calculateOperations()
	{
		unsigned i = src.length(); // i is our row
		unsigned j = trg.length(); // j is our column

		while (i > 0 || j > 0) // While not at (0, 0)
		{
			switch (m.at(i).at(j).operation())
			{
			case Operation::SUBSTITUTION:
				operations.push_front('s');
				--i;
				--j;
				break;
				
			case Operation::INSERTION:
				operations.push_front('i');
				--j;
				break;
				
			case Operation::DELETION:
				operations.push_front('d');
				--i;
				break;
				
			case Operation::NOTHING:
				operations.push_front('_');
				--i;
				--j;
				break;
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
	MinEditDistance matrix(source, target);
	
	// Print out the calculated matrix
	matrix.printMatrix();
	
	/// Print out the alignment
	matrix.printAlignment();
	
	// Print out the minimum edit distance
	std:: cout << "The minimum edit distance is " << matrix.getMinDistance() << "." << std::endl;
	
	return 0;
}
