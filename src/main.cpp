#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

typedef std::pair<double,double> vertex_t;

class cEdge
{
public:
    vertex_t v1, v2;

    void display();
};
class cLineString
{
public:
    std::vector<cEdge> myEdge;
    std::string myInput;

    cLineString( const std::string& line)
    : myInput( line )
    {}
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
        for (auto &l : myInput) {
            cLineString ls( l );
            ls.extractEdges();
            myLineStringLocations.push_back(ls);
        }
    }

    void constructUniqueVertices()
    {
        // loop over all line string locations
        for (auto &l : myLineStringLocations)
        {
            // loop over edges in string location
            bool first = true;
            for (auto &e : l.myEdge)
            {
                if (first)
                {
                    first = false;
                    // save, dropping duplicates, the first vertex of the first edge
                    myVertexSet.insert(e.v1);
                }
                // save, dropping duplicates, the second vertex
                myVertexSet.insert(e.v2);
            }
        }

        // display unique vertices found
        std::cout << "unique vertices\n";
        for( auto& v : myVertexSet )
        {
            std::cout << v.first <<" "<< v.second << "\n";
        }
    }

private:
    std::vector<std::string> myInput;
    std::vector<cLineString> myLineStringLocations;
    std::set<vertex_t> myVertexSet;
};

    void cEdge::display()
    {
        std::cout << v1.first << " " << v1.second << ", " 
            << v2.first << " " << v2.second;
    }
    void cLineString::display()
    {
        std::cout << myInput 
            << "\nhas edges:\n";
        for (auto &e : myEdge)
        {
            e.display();
            std::cout << "\n";
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
                edge.v1.first = atof(myInput.substr(p + 1).c_str());
                p = myInput.find(",", p + 1);
                edge.v1.second = atof(myInput.substr(p + 1).c_str());
                p = myInput.find("[", p + 1);
            }
            else
            {
                edge.v1 = edge.v2;
            }
            edge.v2.first = atof(myInput.substr(p + 1).c_str());
            p = myInput.find(",", p + 1);
            edge.v2.second = atof(myInput.substr(p + 1).c_str());
            myEdge.push_back(edge);
        }

        display();
    }
 

main()
{
    cSolution S;
    S.generateSmallTest();
    S.extractLocation();
    S.constructUniqueVertices();

    return 0;
}
