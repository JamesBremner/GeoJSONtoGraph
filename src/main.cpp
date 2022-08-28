#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>
#include <time.h>
#include "cRunwatch.h"

class cVertex
{
public:
    double x, y; // location
    int myIndex; // index

    void set(double x, double y);

    void display() const;

    /** Assign unique vertex index
     * @param[in] vertexVector unique vertices discovered so far
     *
     * Identically located vertices will be assigned the same index
     */
    void indexer(std::vector<cVertex> &vertexVector);
    void indexer(std::set<cVertex, cVertex> &vertexSet);

    /// Return true if they are so close as to be considered identical
    bool operator==(const cVertex &other) const;

    bool operator()(const cVertex &a, const cVertex &b) const;

private:
    std::tuple<double, double> t;
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
    void generateSmallTest();
    void generateLargeTest();

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
    void indexUniqueVertices2();

private:
    std::vector<std::string> myInput;
    std::vector<cLineString> myLineString;
};

void cSolution::generateSmallTest()
{
    std::vector<std::string> input{
        "{\"type\": \"Feature\", \"properties\": {\"id\": 1}, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [ [ 147.0, -4.8 ], [ 141.0, -2.0 ] ]}}",
        "{\"type\": \"Feature\", \"properties\": {\"id\": 2}, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [ [ 152.6, -5.2 ], [ 152.05, -3.8 ], [ 147.0, -4.8 ] ] } }",
        "{\"type\": \"Feature\", \"properties\": {\"id\": 3}, \"geometry\": { \"type\": \"LineString\", \"coordinates\": [ [ 147.0, -4.8 ], [ 144.73, 0.0 ] ] } }"};
    myInput = input;
}
void cSolution::generateLargeTest()
{
    srand(time(NULL));
    for (int k = 0; k < 10000; k++)
    {
        cEdge e;
        double x = rand() % 1000 - 500;
        double y = rand() % 1000 - 500;
        e.v1.set(x, y);
        x = rand() % 1000 - 500;
        y = rand() % 1000 - 500;
        e.v2.set(x, y);
        cLineString l("random generated");
        l.myEdge.push_back(e);
        myLineString.push_back(l);
    }
}

bool cVertex::operator==(const cVertex &other) const
{
    // check if the two vertices are so close that they can be considered identical
    const double min = 0.001;
    return (std::fabs(x - other.x) < min &&
            fabs(y - other.y) < min);
}

bool cVertex::operator()(const cVertex &a, const cVertex &b) const
{
    return a.t < b.t;
}

void cEdge::display()
{
    v1.display();
    std::cout << " to ";
    v2.display();
    std::cout << "\n";
}

void cVertex::set(double ix, double iy)
{
    x = ix;
    y = iy;
    t = std::make_tuple(x, y);
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
    double x, y;
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
            x = atof(myInput.substr(p + 1).c_str());
            p = myInput.find(",", p + 1);
            y = atof(myInput.substr(p + 1).c_str());
            edge.v1.set(x, y);
            p = myInput.find("[", p + 1);
        }
        else
        {
            edge.v1 = edge.v2;
        }
        x = atof(myInput.substr(p + 1).c_str());
        p = myInput.find(",", p + 1);
        y = atof(myInput.substr(p + 1).c_str());
        edge.v2.set(x, y);
        myEdge.push_back(edge);
    }

    display();
}

void cVertex::indexer(std::vector<cVertex> &vertexVector)
{
    std::vector<cVertex>::iterator it;
    {
        raven::set::cRunWatch aWatcher("vector find");
        it = std::find(
            vertexVector.begin(),
            vertexVector.end(),
            *this);
    }
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

void cVertex::indexer(std::set<cVertex, cVertex> &vertexSet)
{
    std::set<cVertex, cVertex>::iterator it;
    {
        raven::set::cRunWatch aWatcher("set find");
        it = std::find(
            vertexSet.begin(),
            vertexSet.end(),
            *this);
    }
    if (it == vertexSet.end())
    {
        // new unique vertex
        myIndex = vertexSet.size();
        vertexSet.insert(*this);
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

    // // display unique vertices found
    // std::cout << "unique vertices\n";
    // for (auto &v : vertexVector)
    // {
    //     v.display();
    //     std::cout << "\n";
    // }

    // for (auto &l : myLineString)
    // {
    //     l.display();
    // }
}

void cSolution::indexUniqueVertices2()
{
    std::set<cVertex, cVertex> vertexSet;

    // loop over all line strings
    for (auto &l : myLineString)
    {
        // loop over edges in line string
        for (auto &e : l.myEdge)
        {
            // index the vertices
            e.v1.indexer(vertexSet);
            e.v2.indexer(vertexSet);
        }
    }

    // display unique vertices found
    // std::cout << "unique vertices\n";
    // for (auto &v : vertexSet)
    // {
    //     v.display();
    //     std::cout << "\n";
    // }

    // for (auto &l : myLineString)
    // {
    //     l.display();
    // }
}

main()
{
    raven::set::cRunWatch::Start();

    cSolution S;
    S.generateSmallTest();
    S.extractLocation();
    S.indexUniqueVertices();

    std::cout << "\n============== S2 ===============\n";
    cSolution S2;
    S2.generateSmallTest();
    S2.extractLocation();
    S2.indexUniqueVertices2();

    std::cout << "\n============== S3 ===============\n";
    cSolution S3;
    S3.generateLargeTest();
    {
        raven::set::cRunWatch aWatcher("vector");
        S3.indexUniqueVertices();
    }

    std::cout << "\n============== S4 ===============\n";
    cSolution S4;
    S4.generateLargeTest();
    {
        raven::set::cRunWatch aWatcher("set");
        S4.indexUniqueVertices2();
    }

    raven::set::cRunWatch::Report();

    return 0;
}
