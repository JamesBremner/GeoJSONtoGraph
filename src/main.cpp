#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

class cVertex
{
public:
    double x, y; // location
    int myIndex; // index

    void display() const;

    /** Assign unique vertex index
     * @param[in] vertexVector unique vertices discovered so far
     * 
     * Identically located vertices will be assigned the same index
     */
    void indexer(std::vector<cVertex>& vertexVector);

    /// Return true if they are so close as to be considered identical
    bool operator==(const cVertex &other) const;
};

class cEdge
{
public:
    cVertex v1, v2;

    void display();
};
class cLineString
{
public:
    std::vector<cEdge> myEdge;
    std::string myInput;

    cLineString(const std::string &line)
        : myInput(line)
    {
    }
    void display();
    void extractEdges();
};
class cSolution
{
public:
    void generateSmallTest()
    {
        std::vector<std::string> input{
            "{\"type\": \"Feature\", \"properties\": {\"id\": 1}, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [ [ 147.0, -4.8 ], [ 141.0, -2.0 ] ]}}",
            "{\"type\": \"Feature\", \"properties\": {\"id\": 2}, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [ [ 152.6, -5.2 ], [ 152.05, -3.8 ], [ 147.0, -4.8 ] ] } }",
            "{\"type\": \"Feature\", \"properties\": {\"id\": 3}, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [ [ 147.0, -4.8 ], [ 144.73, 0.0 ] ] } }"};
        myInput = input;
    }
    void extractLocation()
    {
        for (auto &l : myInput)
        {
            cLineString ls(l);
            ls.extractEdges();
            myLineString.push_back(ls);
        }
    }

    void indexUniqueVertices();

private:
    std::vector<std::string> myInput;
    std::vector<cLineString> myLineString;
};

bool cVertex::operator==(const cVertex &other) const
{
    // check if the two vertices are so close that they can be considered identical
    const double min = 0.001;
    return (std::fabs(x - other.x) < min &&
            fabs(y - other.y) < min);
}

void cEdge::display()
{
    v1.display();
    std::cout << " to ";
    v2.display();
    std::cout << "\n";
}
void cVertex::display() const
{
    std::cout << myIndex << " (" << x << " " << y << ")";
}
void cLineString::display()
{
    std::cout << myInput
              << "\nhas edges:\n";
    for (auto &e : myEdge)
    {
        e.display();
    }
    std::cout << "\n";
}

void cLineString::extractEdges()
{
    cEdge edge;
    int p = myInput.find("[");
    bool first = true;
    while (true)
    {
        auto dbg = myInput.substr(p + 1);
        p = myInput.find("[", p + 1);
        if (p == -1)
            break;
        if (first)
        {
            first = false;
            edge.v1.x = atof(myInput.substr(p + 1).c_str());
            p = myInput.find(",", p + 1);
            edge.v1.y = atof(myInput.substr(p + 1).c_str());
            p = myInput.find("[", p + 1);
        }
        else
        {
            edge.v1 = edge.v2;
        }
        edge.v2.x = atof(myInput.substr(p + 1).c_str());
        p = myInput.find(",", p + 1);
        edge.v2.y = atof(myInput.substr(p + 1).c_str());
        myEdge.push_back(edge);
    }

    display();
}

void cVertex::indexer(std::vector<cVertex>& vertexVector)
{
    auto it = std::find(
        vertexVector.begin(),
        vertexVector.end(),
        *this);
    if (it == vertexVector.end())
    {
        // new unique vertex
        myIndex = vertexVector.size();
        vertexVector.push_back(*this);
    }
    else
    {
        // duplicate vertex
        myIndex = it->myIndex;
    }
}

void cSolution::indexUniqueVertices()
{
     std::vector<cVertex> vertexVector;

    // loop over all line strings
    for (auto &l : myLineString)
    {
        // loop over edges in line string
        for (auto &e : l.myEdge)
        {
            // index the vertices
            e.v1.indexer(vertexVector);
            e.v2.indexer(vertexVector);
        }
    }

    // display unique vertices found
    std::cout << "unique vertices\n";
    for (auto &v : vertexVector)
    {
        v.display();
        std::cout << "\n";
    }

    for (auto &l : myLineString)
    {
        l.display();
    }
}

main()
{
    cSolution S;
    S.generateSmallTest();
    S.extractLocation();
    S.indexUniqueVertices();

    return 0;
}
