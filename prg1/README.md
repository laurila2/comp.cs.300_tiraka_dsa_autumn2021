# Ohjelmointiprojekti vaihe 1: Game of Taxes



## datastructures.hh

### std::unordered_map<TownID, struct Town> *all_towns_;
Päädyin yleisesti unordered_map, jonka avaimena on struct -rakenteeseen, työssä. Haku unordered_mapista pitäisi olla keskimäärin nopeampaa. 

### struct Town
Townin struct-rakenteeseen kertyi paljon tavaraa, mm. town_distance, johon voidaan tallentaa townin etäisyys tietystä koordinaatista.

## datastructures.cc
Koodin rakenteet eivät kaivanneet mielestäni hirveästi kommentointia, sillä ne ovat melko selkeitä.

### bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
add_town -rakenteessa en käyttänyt neliöjuurta etäisyyksien selvittämiseen, sillä neliöjuuri on hidas operaatio. Tieto siitä järjestyksestä,
missä kaupungit ovat toisistaan riittää. Sama juttu muissa kohdissa joissa lasketaan etäisyyksiä.


----

## Yleinen palaute projektista
Yleisenä palautteena kooditorioita olisi voinut olla aiemmin, kuin vain viimeisellä viikolla, lisäksi itselle oli vaikeaa päästä alkuun projektissa mutta kun lopulta pääsi alkuun niin sitten eteni ihan hyvin.

Projekti oli myös oikein mielekäs ja kiva tehdä, mutta töiden + muiden kurssien takia sen tekeminen jäi usein ikävästi illalle.

Esim. Ohjelmointi 2 -kurssilla kooditoriot toimivat paremmin mielestäni, siitä voisi ottaa mallia. Nyt oli esim. yksi kerta jolloin jonotin 2h kooditoriossa mutta sitten aika ehti loppua enkä saanut vastausta kysymykseeni.
