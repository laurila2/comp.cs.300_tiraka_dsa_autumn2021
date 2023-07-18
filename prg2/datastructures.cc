// Datastructures.cc
//
// Student name: Santeri Laurila
// Student email: santeri.laurila@tuni.fi
// Student number: 274301

#include "datastructures.hh"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
#include <set>
#include <unordered_set>
#include <queue>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

Datastructures::Datastructures()
{
    all_towns_ = new std::unordered_map<TownID, struct Town>;
    all_roads_ = new std::set<std::pair<TownID, TownID>>;
}

Datastructures::~Datastructures()
{
    delete all_towns_;
    delete all_roads_;
}

unsigned int Datastructures::town_count()
{
    return town_count_;
}

void Datastructures::clear_all()
{
    town_count_ = 0;
    road_count_ = 0;
    all_towns_->clear();
    all_roads_->clear();
}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    int x = coord.x;
    int y = coord.y;
    double distance = x * x + y * y; // no sqrt because its a slow operation
    double town_distance = -1;

    unsigned int d = 0;
    TownID pi;

    auto return_value = all_towns_->insert(
        {id,
         {id,
          name,
          coord,
          distance,
          town_distance,
          tax,
          TownID(),                  // for town's master
          new std::vector<TownID>(), // for town's vassals
          std::vector<TownID>(),     // for which towns you can travel from town
          d,
          pi,
          white}});

    town_count_ += 1;

    return return_value.second; // returns boolean value
}

Name Datastructures::get_town_name(TownID id)
{
    auto search = all_towns_->find(id);
    if (search != all_towns_->end())
    {
        auto name = search->second.name;
        return name;
    }
    else
    {
        return NO_NAME;
    }
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    auto towns_iter = all_towns_->find(id);
    if (towns_iter != all_towns_->end())
    {
        auto coordinates = towns_iter->second.coord;
        return coordinates;
    }
    else
    {
        return NO_COORD;
    }
}

int Datastructures::get_town_tax(TownID id)
{
    auto towns_iter = all_towns_->find(id);
    if (towns_iter != all_towns_->end())
    {
        auto tax = towns_iter->second.tax;
        return tax;
    }
    else
    {
        return NO_VALUE;
    }
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> all_towns_vec;
    for (const auto &pair : *all_towns_)
    {
        all_towns_vec.push_back(pair.first);
    }

    return all_towns_vec;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> towns;
    for (const auto &pair : *all_towns_)
    {
        if (name == pair.second.name)
        {
            towns.push_back(pair.first);
        }
    }

    return towns;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    auto towns_iter = all_towns_->find(id);
    if (towns_iter != all_towns_->end())
    {
        towns_iter->second.name = newname;
        return true;
    }

    return false;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    std::vector<std::pair<TownID, Name>> towns_structs;
    std::vector<TownID> towns_ordered;

    for (auto const &town : *all_towns_)
    {
        towns_structs.push_back(
            std::make_pair(town.second.id, town.second.name));
    }

    std::sort(towns_structs.begin(),
              towns_structs.end(),
              [](std::pair<TownID, Name> a, std::pair<TownID, Name> b) {
                  return a.second < b.second;
              });

    for (auto const &i : towns_structs)
    {
        towns_ordered.push_back(i.first);
    }

    return towns_ordered;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    std::vector<Town> towns;
    std::vector<TownID> towns_distance_increasing;

    for (const auto &pair : *all_towns_)
    {
        towns.push_back(pair.second); // vector full of structs
    }

    std::sort(towns.begin(), towns.end(), [](Town &a, Town &b) {
        return a.distance < b.distance;
    });

    for (const auto &town_struct : towns)
    {
        towns_distance_increasing.push_back(town_struct.id);
    }

    return towns_distance_increasing;
}

TownID Datastructures::min_distance()
{
    if (town_count_ == 0)
    {
        return NO_TOWNID;
    }

    else
    {
        TownID closest_town = towns_distance_increasing().front();
        return closest_town;
    }
}

TownID Datastructures::max_distance()
{
    if (town_count_ == 0)
    {
        return NO_TOWNID;
    }
    else
    {
        TownID farthest_town = towns_distance_increasing().back();
        return farthest_town;
    }
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    auto search = all_towns_->find(masterid);
    auto search2 = all_towns_->find(vassalid);

    if ((search != all_towns_->end()) && (search2 != all_towns_->end()))
    {
        search->second.vassals->push_back(vassalid);
        search2->second.master = masterid;

        return true;
    }

    return false;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> empty;
    std::vector<TownID> town_vassals;

    auto search = all_towns_->find(id);

    if (search != all_towns_->end())
    {
        town_vassals = *search->second.vassals;
        return town_vassals;
    }
    else
    {
        empty.push_back(NO_TOWNID);
        return empty;
    }
}

void Datastructures::recursive_taxer_path(TownID id,
                                          std::vector<TownID> &taxer_path)
{
    auto search = all_towns_->find(id);

    if ((taxer_path.size() == 0) && (search == all_towns_->end()))
    {
        taxer_path.push_back(NO_TOWNID);
    }

    if (search != all_towns_->end())
    {
        taxer_path.push_back(id);
        recursive_taxer_path(search->second.master, taxer_path);
    }
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxer_path_vec;
    recursive_taxer_path(id, taxer_path_vec);

    return taxer_path_vec;
}

bool Datastructures::remove_town(TownID id)
{
    auto search = all_towns_->find(id);

    if (search != all_towns_->end())
    {
        // if town has vassals and a master
        if (search->second.vassals->size() > 0
            and search->second.master.size() > 0)
        {
            for (auto const &vassal : *all_towns_->at(id).vassals)
            {
                all_towns_->at(search->second.master).vassals->push_back(vassal);
            }
        }

        all_towns_->erase(id);

        return true;
    }
    else
    {
        return false;
    }
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    std::vector<TownID> towns_nearest_vec;
    std::vector<Town> towns;

    for (auto &town : *all_towns_)
    {
        int town_x = town.second.coord.x;
        int town_y = town.second.coord.y;

        double towns_distance = pow((town_x - coord.x), 2)
                                - pow((town_y - coord.y), 2);
        town.second.town_distance = towns_distance;

        towns.push_back(town.second); // vector full of structs
    }

    std::sort(towns.begin(), towns.end(), [](Town &a, Town &b) {
        return a.town_distance < b.town_distance;
    });

    for (const auto &town_struct : towns)
    {
        towns_nearest_vec.push_back(town_struct.id);
    }

    return towns_nearest_vec;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("longest_vassal_path()");
}

int Datastructures::total_net_tax(TownID /*id*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("total_net_tax()");
}

//
// Phase 2 operations
//

void Datastructures::clear_roads()
{
    all_roads_->clear();
    road_count_ = 0;

    for (auto &town : *all_towns_)
    {
        town.second.roads.clear();
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> all_roads_vec;

    for (const auto &town_pair : *all_roads_)
    {
        all_roads_vec.push_back(town_pair);
    }

    return all_roads_vec;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    auto search_town1 = all_towns_->find(town1);
    auto search_town2 = all_towns_->find(town2);

    // If town names are identical, or one or both of the towns cannot be found,
    // return false
    if ((town1 == town2) || (search_town1 == all_towns_->end())
        || (search_town2 == all_towns_->end()))
    {
        return false;
    }

    else
    {
        std::vector<TownID> town1_roads;
        std::vector<TownID> town2_roads;

        town1_roads = search_town1->second.roads;
        town2_roads = search_town2->second.roads;

        // If there is a road already between towns, return false
        if (std::find(town1_roads.begin(), town1_roads.end(), town2)
            != town1_roads.end())
        {
            return false;
        }

        else if (std::find(town2_roads.begin(), town2_roads.end(), town1)
                 != town2_roads.end())
        {
            return false;
        }

        all_towns_->at(town1).roads.push_back(town2);
        all_towns_->at(town2).roads.push_back(town1);

        if (town1 < town2)
        {
            all_roads_->insert(std::make_pair(town1, town2));
        }

        else
        {
            all_roads_->insert(std::make_pair(town2, town1));
        }

        road_count_ += 1;
        return true;
    }
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    std::vector<TownID> empty;
    std::vector<TownID> roads;

    auto search = all_towns_->find(id);

    if (search != all_towns_->end())
    {
        roads = all_towns_->at(id).roads;
        return roads;
    }

    else
    {
        empty.push_back(NO_TOWNID);
        return empty;
    }
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    std::vector<TownID> route;
    bool route_found = false;

    auto search_town1 = all_towns_->find(fromid);
    auto search_town2 = all_towns_->find(toid);

    if ((search_town1 == all_towns_->end())
        || (search_town2 == all_towns_->end()))
    {
        route.push_back(NO_TOWNID);
        return route;
    }

    // BFS
    std::vector<TownID> que;
    TownID u = NO_TOWNID;

    for (auto &town : *all_towns_)
    {
        town.second.colour = white;
        town.second.d = -1;
        town.second.pi = NO_TOWNID;
    }

    all_towns_->at(fromid).colour = gray;
    all_towns_->at(fromid).d = 0;
    que.push_back(fromid);

    while (!que.empty())
    {
        if (route_found)
            break;

        u = que.back(); // Oldest item from que
        que.pop_back();

        for (auto &v : all_towns_->at(u).roads)
        {
            if (all_towns_->at(v).colour == white)
            {
                all_towns_->at(v).colour = gray;
                all_towns_->at(v).d = all_towns_->at(u).d + 1;
                all_towns_->at(v).pi = u;
                que.push_back(v);

                if (v == toid)
                {
                    route_found = true;
                    break;
                }
            }
        }
        all_towns_->at(u).colour = black;
    }

    if (route_found)
    {
        // Collect the route
        TownID crawl = toid;
        route.push_back(toid);

        while (crawl != fromid)
        {
            route.push_back(all_towns_->at(crawl).pi);
            crawl = all_towns_->at(crawl).pi;
        }

        // Swap vector to fromid to toid
        std::vector<TownID> route_swapped;
        std::vector<TownID>::reverse_iterator it = route.rbegin();
        while (it != route.rend())
        {
            route_swapped.push_back(*it);
            it++;
        }
        return route_swapped;
    }
    else
    {
        route.clear();
        return route;
    }
}

bool Datastructures::remove_road(TownID /*town1*/, TownID /*town2*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("remove_road()");
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    // Use any_route's BFS
    return any_route(fromid, toid);
}

std::vector<TownID> Datastructures::road_cycle_route(TownID /*startid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("road_cycle_route()");
}

std::vector<TownID> Datastructures::shortest_route(TownID /*fromid*/,
                                                   TownID /*toid*/)
{
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )
    throw NotImplemented("shortest_route()");
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}
