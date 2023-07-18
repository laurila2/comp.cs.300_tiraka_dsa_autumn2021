# Ohjelmointiprojekti vaihe 2: Connecting Towns


## datastructures.hh

### std::unordered_map<TownID, struct Town> *all_towns_;
Päädyin yleisesti unordered_map, jonka avaimena on struct -rakenteeseen, työssä. Haku unordered_mapista pitäisi olla keskimäärin nopeampaa. 

### std::set<std::pair<TownID, TownID>> *all_roads_;
Tein vastaavasti all_roads -rakenteen kuten olin tehnyt 1. osassa all towns -rakenteen.


### struct Town
Townin struct-rakenteeseen kertyi paljon tavaraa, mm. town_distance, johon voidaan tallentaa townin etäisyys tietystä koordinaatista. 2. vaiheessa rakenteesesen lisättiin BFS:ssä tarvittavia lisätietoja, d, pi ja colour.

### enum Colour
Lisäsin Colour-kentän auttamaan myöhemmin käytettävässä BFS-algoritmissa.

## datastructures.cc

### std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
Päädyin tekemään any_routen BFS-algoritmilla. Se oli ehkä haastavin asia mitä tein tällä kurssilla.

Koodin muut rakenteet eivät kaivanneet mielestäni hirveästi kommentointia, sillä ne ovat melko selkeitä.


----

## Yleinen palaute projektista
Projekti oli myös oikein mielekäs ja kiva tehdä, mutta töiden + muiden kurssien takia sen tekeminen jäi usein ikävästi illalle.