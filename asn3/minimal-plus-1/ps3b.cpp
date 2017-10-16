/*
    QCP to SAT implementation - cpp file
*/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;


typedef struct atom_t   // create an atom type
{
    unsigned int pos_i;  // position i,j in latin square
    unsigned int pos_j;
    unsigned int value;  // value in C(i,j)
    long mappedVal; // mapped value
}Atom;

typedef vector<Atom> clause;    // clause containing a set of the above
typedef vector<clause> cnf;     // vector containing a cnf set



// maps every possibility of each cell of latin square to an integer
// using a map (logarithmic time)
void ls_map(unsigned int &order, map<string, unsigned long> &Map)
{
    unsigned long numVariables = 1;
//    map<string, unsigned int>::iterator it;

    for (int i = 1; i <= order; i++)
    {
        for (int j = 1; j <= order; j++)
        {
            for (int k = 1; k <= order; k++)
            {
                string s = to_string(i) + "," + to_string(j) + "," + to_string(k);
                Map.insert(make_pair(s, numVariables));
                numVariables++;
            }
        }
    }

//
//    for (it = Map.begin(); it != Map.end(); it++)
//        cout << it->first << "," << it->second << endl;
}




// input data from input file
// param :  order - Latin square order, an unsigned integer
//          unit_clause - information extracted from Latin square
// post: cells with numbers are all extracted
void inputData(const char* fileIn,
               unsigned int &order,
               clause &unit_clause,
               map<string, unsigned long> &Map)
{
    string str = "";
    istringstream iss;  // initialize input stringstream

    // input data
    ifstream input;
    input.open(fileIn);
    if (!input)
    {
        cerr << "failed to read text file" << endl;
        exit(1);
    }

    // get the order
    input.ignore(10, ' ');
    getline(input, str);
    order = atoi(str.c_str());
//    cout << order << endl;
//    cout << &order << endl;
    ls_map(order, Map);


    // map contents to unit clauses
    unsigned int i = 1;  // pos i
    unsigned int j = 1;  // pos j
    Atom atom;
//    clause unit_clause;
    while (i <= order)
    {
        getline(input,str);
        iss.str(str);

        string k;  // initialize value stored in cell (i,j)

        while (iss >> k)
        {
            // reset j if j > order
            if (j > order)
                j = 1;

//            cout << "C(" << i << "," << j << ") contains " << k << endl;

            // extract existing numbers already in latin square
            // convert them into unit clause
            if (k[0] != '.')
            {
                atom.pos_i = i;
                atom.pos_j = j;
                atom.value = stoul(k);
                string s = to_string(i) + "," + to_string(j) + "," + k;
                atom.mappedVal = Map.find(s)->second;
//                cout << atom.mappedVal << endl;
                unit_clause.push_back(atom);


//                index = unit_clause.size() - 1;
//                cout << "index " << index << " stores position (" << unit_clause[index].pos_i;
//                cout << "," << unit_clause[index].pos_j << ") in latin square";
//                cout << ", which contains " << unit_clause[index].value;
//                cout << " and mapped to " << atom.mappedVal << endl;
            }

            j++;

            if (iss.peek() == ' ') // if next character is ' ', ignore
                iss.ignore();
        }

        i++;
        iss.clear();
    }

//    cout << "there are " << unit_clause.size() << " elements in the unit clause";

    input.close();  // close file
}






// property A - each cell of square contains a number in [n]
void propertyA(const unsigned int &order, cnf &subset)
{
    clause c(order);
    Atom atom;
    long numVariables = 0;
    for (int i = 1; i <= order; i++)
    {
        atom.pos_i = i;
        for (int j = 1; j <= order; j++)
        {
            atom.pos_j = j;
            for (int k = 1; k <= order; k++)
            {
                atom.value = k;
                atom.mappedVal = ++numVariables;
//                cout << i << j << k << endl;
                c[k-1] = atom;
//                cout << atom.mappedVal << " ";
            }

            // push clauses into cnf set
            subset.push_back(c);
//            cout << "0" << endl;
        }
    }

//    for (int index = 0; index < subset.size(); index++)
//    {
//        for (int index2 = 0; index2 < subset[index].size(); index2++)
//            cout << subset[index][index2].mappedVal << ",";
//    }

}



// property B - no cell contains two distinct numbers
void propertyB(const unsigned int &order, cnf &subset)
{
    clause c(order);
    Atom atom;
    long numVariables = 0;
    for (int i = 1; i <= order; i++)
    {
        atom.pos_i = i;
        for (int j = 1; j <= order; j++)
        {
            atom.pos_j = j;
            for (int k = 1; k <= order; k++)
            {
                atom.value = k;
                atom.mappedVal = -(++numVariables);
//                cout << "-" <<i << j << k << endl;
                c[k-1] = atom;
//                cout << "-" << atom.mappedVal << " ";
            }

            // push clauses into cnf set
            subset.push_back(c);
//            cout << "0" << endl;
        }
    }
}


// property C - no row has two cells containing the same number
void propertyC(const unsigned int &order,
               cnf &subset,
               const map<string, unsigned long> &Map)
{
    clause c(order);
    Atom atom;
    for (int i = 1; i <= order; i++)
    {
        atom.pos_i = i;
        for (int k = 1; k <= order; k++)
        {
            atom.value = k;
            for (int j = 1; j <= order; j++)
            {
                atom.pos_j = j;
                string s = to_string(i) + "," + to_string(j) + "," + to_string(k);
                atom.mappedVal = -(Map.find(s)->second);
//                cout << atom.mappedVal << endl;
//                cout << "-" <<i << j << k << ", mapped to " << atom.mappedVal << endl;

                c[j-1] = atom;
            }

            // push clauses into cnf set
            subset.push_back(c);
//            cout << "0" << endl;
        }
    }
}



// property D - no column has two cells containing the same number
void propertyD(const unsigned int &order,
               cnf &subset,
               const map<string, unsigned long> &Map)
{
    clause c(order);
    Atom atom;
    for (int k = 1; k <= order; k++)
    {
        atom.value = k;
        for (int j = 1; j <= order; j++)
        {
            atom.pos_j = j;
            for (int i = 1; i <= order; i++)
            {
                atom.pos_i = i;
                string s = to_string(i) + "," + to_string(j) + "," + to_string(k);
                atom.mappedVal = -(Map.find(s)->second);
//                cout << atom.mappedVal << endl;
//                cout << "-" <<i << j << k << endl;
                c[i-1] = atom;
            }


            // push clauses into cnf set
            subset.push_back(c);
//            cout << "0" << endl;
        }
    }
}




void outputData(const char* fileOut,
                const unsigned int &order,
                const clause &unit_clause,
                const cnf &subset1,
                const cnf &subset2,
                const cnf &subset3,
                const cnf &subset4)
{
    long numClause = unit_clause.size() + subset1.size() + subset2.size() + subset3.size() + subset4.size();
    ofstream output;
    output.open(fileOut);
    if (!output)
    {
        cerr << "failed to write to output.txt" << endl;
        exit(1);
    }

    output << "c" << endl;
    output << "c QCP instance of order " << order << " in DIMACS format" << endl;
    output << "c" << endl;
    // maximum number of variables = order^3
    output << "p cnf " << order * order * order << " " << numClause << endl;   //

    // output all unit clauses
    for (int i = 0; i < unit_clause.size(); i++)
        output << unit_clause[i].mappedVal << " 0" << endl;

    // output other clauses
    for (int i = 0; i < subset1.size(); i++)
    {
        for (int j = 0; j < subset1[i].size(); j++)
            output << subset1[i][j].mappedVal << " ";

        output << "0" << endl;
    }

    for (int i = 0; i < subset2.size(); i++)
    {
        for (int j = 0; j < subset2[i].size(); j++)
            output << subset2[i][j].mappedVal << " ";

        output << "0" << endl;
    }

    for (int i = 0; i < subset3.size(); i++)
    {
        for (int j = 0; j < subset3[i].size(); j++)
            output << subset3[i][j].mappedVal << " ";

        output << "0" << endl;
    }


    for (int i = 0; i < subset4.size(); i++)
    {
        for (int j = 0; j < subset4[i].size(); j++)
            output << subset4[i][j].mappedVal << " ";

        output << "0" << endl;
    }


    output.close(); // close file
}



int main(int argc, char *argv[])
{
    // initialize
    unsigned int *order = new unsigned int;  // allocate memory for order of latin square
//    cout << order << endl;
//    cout << *order << endl;
    clause unit_clause;
    cnf subsetA, subsetB, subsetC, subsetD;
    map<string, unsigned long> Map;



    // extract unit clauses and order
    inputData(argv[1], *order, unit_clause, Map);

    // generate minimal subset of latin square cnf
    propertyA(*order, subsetA);
    propertyB(*order, subsetB);
    propertyC(*order, subsetC, Map);
    propertyD(*order, subsetD, Map);


    // output data
    outputData(argv[2], *order, unit_clause, subsetA, subsetC, subsetD, subsetB);


    // deallocate memory
    delete order;

    return 0;
}
