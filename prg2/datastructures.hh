// Datastructures.hh
//
// Student name: Santeri Laurila
// Student email: santeri.laurila@tuni.fi
// Student number: 274301

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <exception>
#include <functional>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

enum Colour
{
    gray,
    white,
    black
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Just gets the value from variable
    unsigned int town_count();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Needs to clear every item
    void clear_all();

    // Estimate of performance: O(1)
    // Short rationale for estimate: Just sets up the structure
    bool add_town(TownID id, Name const &name, Coord coord, int tax);

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    Name get_town_name(TownID id);

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    int get_town_tax(TownID id);

    // Estimate of performance: O(log N)
    // Short rationale for estimate: goes through every key in map
    std::vector<TownID> all_towns();

    // Estimate of performance: O(N)
    // Short rationale for estimate: for loops every key in map
    std::vector<TownID> find_towns(Name const &name);

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    bool change_town_name(TownID id, Name const &newname);

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: std::sort is N log N complex
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: std::sort is N log N complex
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(1)
    // Short rationale for estimate: .front() is constant complexity
    TownID min_distance();

    // Estimate of performance: O(1)
    // Short rationale for estimate: .back() is constant complexity
    TownID max_distance();

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    std::vector<TownID> get_town_vassals(TownID id);

    // Helper function for taxer_path
    void recursive_taxer_path(TownID id, std::vector<TownID> &);

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(1) on average, worst case linear in the size of the container.
    // Short rationale for estimate: std::unordered_map find is O(N) -operation
    bool remove_town(TownID id);

    // Estimate of performance: O(N log N)
    // Short rationale for estimate: sort is N log N
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    int total_net_tax(TownID id);

    // Phase 2 operations

    // Estimate of performance: O(N)
    // Short rationale for estimate: Needs to clear every item
    void clear_roads();

    // Estimate of performance: O(N)
    // Short rationale for estimate: Needs to iterate every item
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(N), or theta(1)
    // Short rationale for estimate: find from unordered_map is O(N), or theta(1)
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: O(N), or theta(1)
    // Short rationale for estimate: find from unordered_map is O(N), or theta(1)
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: O(V + E) = O(N)
    // Short rationale for estimate: BFS is O(N)
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: O(V + E) = O(N)
    // Short rationale for estimate: BFS is O(N)
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:
    struct Town
    {
        TownID id;
        Name name;
        Coord coord;
        double distance;      // from (0,0)
        double town_distance; // from town to coordinate
        int tax;
        TownID master;
        std::vector<TownID> *vassals;
        std::vector<TownID> roads; // what towns are connected to town

        // BFS
        unsigned int d;
        TownID pi;
        Colour colour;
    };

    std::unordered_map<TownID, struct Town> *all_towns_;
    unsigned int town_count_;

    std::set<std::pair<TownID, TownID>> *all_roads_;
    unsigned int road_count_;
};

#endif // DATASTRUCTURES_HH
