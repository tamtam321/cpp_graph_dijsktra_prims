#ifndef CODE_MY_CLASS_GRAPH_HPP
#define CODE_MY_CLASS_GRAPH_HPP

#include <iostream>
#include <map>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <utility>
#include <tuple>
#include <vector>
#include <set>

/// Infinity value
# define INF 0x3f3f3f3f

#include <exception>

class InvalidFileName : public std::exception
{
public:
    const char *what() const noexcept { return "Invalid filename!"; }
};

class UnderFlowException : public std::exception
{
public:
    const char *what() const noexcept { return "Graph is empty, you can't remove any node!"; }
};

class EmptyGraph : public std::exception
{
public:
    const char *what() const noexcept { return "Graph is empty, can't do Dijkstra or Prim!"; }
};

class myGraph
{
    // Mindegyik Node-nak van értéke és egy map, amivel számon tartom, hogy
    // az adott csúcs a szomszédaival milyen súlyozású.
    struct Node
    {
        char value = ' ';
        std::map<char, int> adj;

        Node() = default;
        explicit Node(char vrtx);
        Node(char vrtx, std::map<char, int> adjacency);

        Node(const Node &n);
        Node &operator=(const Node &n) = delete;
        Node(Node &&n) = delete;
        Node &operator=(Node &&n) = delete;
    };

    // A gráf ebben tárolja a csúcsokat.
    std::map<char, Node*> vertexes;

    void erase_graph(std::map<char, Node*>&);
    void copy_helper(const myGraph&);

    bool isEmpty();
    void update_adjacency_after_insert(Node *p);
    void update_adjacency_after_remove(Node *p);

    // Depth First Search -> Ennek segítségével járom be a gráfot mélységében és térképezem fel, hogy melyek
    // azok a csúcsok amelyeket nem lehet törölni, mert izoláció keletkezne.
    void DFS(std::set<char> &visited, std::set<char> &articulationPoints, Node *vertex,
             std::map<char, int> &visitedTime, std::map<char, int> &lowTime,
             std::map<char, char> &parent, int &time);

    // Itt jelzem boollal, hogy alkalmas-e törlésre.
    bool is_articulation_point(Node *p);

    // Ennek segítségével adom meg a Dijkstra kiiratásánál a kezdő és végpont közti legrövidebb útvonalat.
    std::string dijkstra_path(std::map<char, char> parent, char from, char to);

public:
    myGraph() = default;
    ~myGraph();
    explicit myGraph(const std::string &file_name);

    myGraph(const myGraph &mg);
    myGraph &operator=(const myGraph &mg);
    myGraph &operator=(myGraph &&mg) = delete;
    myGraph(myGraph &&mg) = delete;

    void addVertex(char vrtx, const std::map<char, int> &adjacency);
    void removeVertex(char vrtx);
    void dijkstra(char src);
    void prim(char src);

    void print_graph();
};

myGraph::Node::Node(char vrtx)
{
    this->value = vrtx;
}

myGraph::Node::Node(char vrtx, std::map<char, int> adjacency)
{
    this->value = vrtx;
    this->adj = std::move(adjacency);
}

myGraph::Node::Node(const Node &n)
{
    if (n.value)
    {
        this->value = n.value;
        this->adj = n.adj;
    }
}

void myGraph::erase_graph(std::map<char, Node*> &d)
{
    std::map<char, Node*>::iterator it;
    Node *tmp;

    for (it = d.begin(); it != d.end(); it++)
    {
        tmp = it->second;
        delete tmp;
    }

    d.clear();
}

myGraph::~myGraph()
{
    erase_graph(vertexes);
}

// Beolvasom a fájlt és a while ciklus minden iterációjánál 3 sort dolgozok fel (csúcs, szomszédok, súlyok) és
// ennek megfelelően hozom létre a node-dot és töltöm fel a vertexes mappet.
myGraph::myGraph(const std::string &file_name)
{
    std::ifstream f_in(file_name);

    if (f_in.fail())
    {
        throw InvalidFileName();
    }

    std::string str_vertex;
    std::string str_adjacent;
    std::string str_distance;
    std::stringstream ss;
    std::stringstream ss2;

    while (!f_in.eof())
    {
        Node *p = new Node();

        std::getline(f_in, str_vertex);
        std::getline(f_in, str_adjacent);
        std::getline(f_in, str_distance);

        p->value = str_vertex[0];

        ss << str_adjacent;
        ss2 << str_distance;

        // Mivel annyi súly van, mint ahány szomszéd, ezért elég ha a ciklus
        // annyiszor iterál, ameddig van szomszédot beolvasni.
        // Megadom a Node-ban lévő adj mapnek a hozzá tartozó szomszédokat meg a megfelelő súlyt.
        for (char ch_adj; ss >> ch_adj;)
        {
            int adj_dis;
            ss2 >> adj_dis;
            p->adj.insert(std::pair<char, int>(ch_adj, adj_dis));

            // Itt szedem ki a vesszőket.
            if (ss.peek() == ',' && ss2.peek() == ',')
            {
                ss.ignore();
                ss2.ignore();
            }
        }

        ss.clear();
        ss2.clear();

        // Itt tartom számon a gráfnak, hogy milyen node-jai vannak.
        vertexes[p->value] = p;
    }
}

void myGraph::copy_helper(const myGraph &c)
{
    if (!c.vertexes.empty())
    {
        for (auto [vertex_value, node] : c.vertexes)
        {
            Node *vertex = new Node(node->value, node->adj);
            this->vertexes.insert(std::make_pair(vertex->value, vertex));
        }
    }
}

myGraph::myGraph(const myGraph &mg)
{
    copy_helper(mg);
}

myGraph &myGraph::operator=(const myGraph &mg)
{
    if (this != &mg)
    {
        erase_graph(this->vertexes);

        copy_helper(mg);
    }

    return *this;
}

// Új node befűzése után, a szomszédjaihoz hozzáadom az új node értékét és a súlyt.
// Szóval frissítem a szomszédságokat.
void myGraph::update_adjacency_after_insert(Node *p)
{
    for (auto [v_key, v_val] : p->adj)
    {
        vertexes[v_key]->adj.insert(std::pair<char, int>(p->value, v_val));
    }
}

// Üres-e a gráf
bool myGraph::isEmpty()
{
    return vertexes.empty();
}

// Ha üres a gráf, akkor csak beszúrom a node-dot és  nem frissítek szomszédságot.
// Különben szomszédságot is frissíteni kell.
void myGraph::addVertex(char vrtx, const std::map<char, int> &adjacency)
{
    if (isEmpty())
    {
        Node *p = new Node(vrtx);
        vertexes[p->value] = p;
        return;
    }

    for (auto [vertex, distance] : adjacency)
    {
        if (vertexes.find(vertex) == vertexes.end())
        {
            std::string to(1, vertex);
            std::string from(1, vrtx);
            std::cout << "The \"" + to + "\" node is not exist, you can't connect it to node \"" + from + "\"!\n";
            return;
        }
    }

    Node *p = new Node(vrtx, adjacency);
    update_adjacency_after_insert(p);
    vertexes[p->value] = p;
}

// A törölt node-nak a szomszédjaiból kiszedem az adott node-dot.
// Szóval frissítem törlés után a szomszédságot.
void myGraph::update_adjacency_after_remove(Node *p)
{
    for (auto [v_key, v_val] : p->adj)
    {
        vertexes[v_key]->adj.erase(p->value);
    }
}

// Ha üres akkor nem lehet törölni és exception-t dobok.
// Ha nem létező node-dot akarok kitörölni azt is jelzem.
// Utána megnézem hogy a node articulation pont-e, mert ha igen akkor törlés után
// más node-dok vagy részgráfok izolálódnak, amit nem engedek és jelzem.
// Csak olyan node-dot szabad törölni, ami nem jár más node-dok és részgráfok izolációjával.
// Különben meg törlöm a node-dot a gráfból és frissítem a szomszédságot.
void myGraph::removeVertex(char vrtx)
{
    if (isEmpty())
    {
        throw UnderFlowException();
    }
    else if(vertexes.find(vrtx) == vertexes.end())
    {
        try
        {
            throw vrtx;
        }
        catch (char ex)
        {
            std::string s(1, ex);
            std::cout << "The \"" + s + "\" node is not exist, you can't remove it!\n";
        }

        return;
    }

    Node *p = vertexes[vrtx];

    if (is_articulation_point(p))
    {
        try
        {
            throw vrtx;
        }
        catch (char ex)
        {
            std::string s(1, ex);
            std::cout << "Can't remove \"" + s + "\" node, because\n"
                         "this is an articulation point!\n"
                         "Deletion is allowed, if no other nodes or subtrees are isolated after remove.\n" << std::endl;
        }

        return;
    }
    else
    {
        update_adjacency_after_remove(p);
        vertexes.erase(p->value);
        delete p;
    }
}

// Mélységi kereséssel derítem fel, hogy a gráfban melyek azok a csúcsok, amik izolációhoz vezethet.
void myGraph::DFS(std::set<char> &visited, std::set<char> &articulationPoints,
                  Node *vertex, std::map<char, int> &visitedTime,
                  std::map<char, int> &lowTime, std::map<char, char> &parent, int &time)
{
    // Itt tartom számon a meglátogatott csúcsokat.
    visited.insert(vertex->value);

    // Ezzel jelölöm meg, hogy a csúcs a hanyadik meglátogatott node.
    visitedTime.insert({vertex->value, time});

    // Az összes eddig meglátogatott szomszédok, amik az adott törlendő csúcson
    // keresztül elérhetőek, azok közül a minimum meglátogatott érték.
    // Ennek a segítségével, meg tudjuk mondani, hogy melyik pont okoz izolációt.
    lowTime.insert({vertex->value, time});

    // Meglátogatott érték növelése minden eddig nem látogatott node meglátogatás után.
    time++;

    // Egy node-nak hány eddig nem látogatott gyereke van.
    int childCount = 0;

    // Izolációt okozó pont-e ?
    bool isArticulationPoint = false;

    // Elindulok a törlendő node szomszédjai felé és rekurzívan bejárom onnantól a gráfot.
    for (auto [_adj, distance] : vertex->adj)
    {
        // Ha vertexnek van szülője és ha a szomszédja a szülője akkor nem foglalkozunk vele és megyünk
        // a következő szomszédra.
        if (parent.find(vertex->value) != parent.end() && _adj == parent[vertex->value])
        {
            continue;
        }

        // Ha még nincs meglátogatva a szomszéd node, akkor beállítom a szülőjét, aki az előző node ahonnan elléptünk.
        // A szülőnek megnöveljük a gyerekszámlálóját.
        // Aztán rekurzívan megyünk tovább a gyerek szomszédjaira.
        if (!visited.contains(_adj))
        {
            parent.insert({_adj, vertex->value});
            childCount++;
            DFS(visited, articulationPoints, vertexes[_adj], visitedTime, lowTime, parent, time);

            // Ha a vertexnek a meglátogatott értéke az kisebb vagy egyenlő, mint a
            // szomszédjának a min meglátogatott értéke, akkor az egy lehetséges izolációt okozó pont.
            if (visitedTime[vertex->value] <= lowTime[_adj])
            {
                isArticulationPoint = true;
            }
            else // Különben, a vertex és szomszéd lowTime-ja között kiválasztom a legkisebbet és azzal írom felül a vertex lowTime-ját.
            {
                lowTime[vertex->value] = std::min(lowTime[vertex->value], lowTime[_adj]);
            }
        }
        else // Ha már meg van látogatva a vertex, akkor ugyanúgy felülírom a lowTime-ját, ha szomszédnak kisebb a lowTime-ja.
        {
            lowTime[vertex->value] = std::min(lowTime[vertex->value], lowTime[_adj]);
        }
    }

    // Ha a vertexnek nincs szülője vagyis az a root és 2 vagy több gyereke van akkor az egy
    // izolációt okozó pont.
    // Vagy ha vertexnek van szülője és izolációt tud okozni, akkor az egy izolációt okozó pont.
    if ((parent.find(vertex->value) == parent.end() && childCount >= 2) ||
    (parent.find(vertex->value) != parent.end() && isArticulationPoint))
    {
        articulationPoints.insert(vertex->value);
    }
}

// Itt adom meg, hogy a node az izolációt okoz-e ha törölném.
bool myGraph::is_articulation_point(Node *p)
{
    // solution
    bool sol = false;

    // A DFS metódusnál jellemeztem
    int time = 0;
    std::set<char> visited;
    std::map<char, int> visitedTime;
    std::map<char, int> lowTime;

    // Itt tárolom a gráfban talált izolációt okozó pontokat.
    std::set<char> articulationPoints;

    // A csúcsnak a szülője az az előző node ahonnan elindulva eljutottam hozzá.
    std::map<char, char> parent;

    // Törlendő node és egyben ő lesz a root is.
    Node *startVertex = p;

    // Ezzel töltöm fel az articulationPoints settet.
    DFS(visited, articulationPoints, startVertex, visitedTime, lowTime, parent, time);

    // Ha a törlendő csúcs benne van a setben akkor jelezzük, hogy azt nem szabad törölni.
    if (articulationPoints.contains(p->value))
    {
        sol = true;
    }

    return sol;
}

// Itt adom meg a legrövidebb út bejárását a source és a végpont között, amit majd kiiratok.
// Ha parent map segítségével építem fel az utat.
// A végponttól indulok aztán a szülőn keresztül visszafele lépegetve eljutok a source-ig.
std::string myGraph::dijkstra_path(std::map<char, char> parent, char from, char to)
{
    std::string path = "";

    path += to;
    std::string arrow = "--->";

    while (from != to)
    {
        path = parent[to] + arrow + path;
        to = parent[to];
    }

    return path;
}

void myGraph::dijkstra(char src)
{
    // Ha üres a gráf akkor nem lehet rajta elvégezni Dijkstra-t.
    // Vagy ha olyan kiindulási csúcsot adok meg ami nem is létezik, akkor jelzem a hibát és nem hajtom
    // végre a Dijkstra-t.
    if (isEmpty())
    {
        throw EmptyGraph();
    }
    else if (vertexes.find(src) == vertexes.end())
    {
        std::string s(1, src);
        std::cout << "The \"" + s + "\" node is not exist, so it could not be the source of the Dijkstra's algorithm.\n\n";
        return;
    }

    // Melyik csúcsok lettek eddig meglátogatva
    std::map<char, bool> visited;

    // Csúcsok szülője, vagyis melyik előző csúcsból jutottam oda.
    std::map<char, char> parent;

    // Itt tárolom el a végpontokat és, hogy milyen hosszú az odavezető út.
    std::map<char, int> dist;

    // Itt tartom számon, hogy melyik csúcs még nincs meglátogatva.
    std::set<char> unvisited;

    // Ebben tárolom el, hogy jelenleg, az adott esetben melyik
    // melyik a rövidebb út az csúcshoz.
    int curr_shortest_dis = INF;

    // Ennek a segítségével adom meg a következő csúcsot amivel dolgozni fogok.
    std::pair<char, int> next_vertex_to_process;

    // Csinálok egy másolatot a vertexes-ről, hogy majd azzal dolgozzak, mert
    // előfordul hogy map-ba bekeről egy felesleges elem és nem szeretném hogy az eredetin változtasson.
    // Azzal nincs baj ha a másolatot változtatja.
    std::map<char, Node*> vertexes_copy = vertexes;

    // Beállítások:
    // * dist-ben hogy a csúcsokhoz vezető út az elején mindegyik végtelen.
    // * visited-ben mindegyik csúcsot false-ra állítani, mert még nincsenek meglátogatva.
    // * unvisited settet feltölteni
    for (auto [vertex, distance] : vertexes_copy)
    {
        dist.insert(std::make_pair(vertex, INF));
        visited.insert(std::make_pair(vertex, false));
        unvisited.insert(vertex);
    }

    // A source távolsága magától az nulla.
    dist[src] = 0;

    // A source-tól fogunk elindulni.
    // A curr_vertex amelyik csúcson éppen vagyunk.
    Node *curr_vertex = vertexes_copy[src];

    // Amíg nem látogattuk meg az összes csúcsot, addig pörög a ciklus.
    while (!unvisited.empty())
    {
        // Ahol vagyok megnézem a szomszédokat.
        for (auto [adjacent, adj_distance] : curr_vertex->adj)
        {
            // Megnézem, hogy tudok-e rövidebb úton odamenni
            curr_shortest_dis = std::min(dist[curr_vertex->value] + adj_distance, dist[adjacent]);

            // Ha rövidebb az új út, mint a régebbi, akkor
            // ha még nincs szülője, akkor beállítom neki és frissítem a távolságát,
            // ha meg van szülője akkor csak frisstíem a szülőt és a távolságot.
            if (curr_shortest_dis < dist[adjacent])
            {
                if (parent.find(adjacent) == parent.end())
                {
                    parent.insert(std::make_pair(adjacent, curr_vertex->value));
                    dist[adjacent] = curr_shortest_dis;
                }
                else
                {
                    parent[adjacent] = curr_vertex->value;
                    dist[adjacent] = curr_shortest_dis;
                }
            }
        }

        // Frissítem az unvisited és a visited tárolókat.
        unvisited.erase(curr_vertex->value);
        visited[curr_vertex->value] = true;

        // initialize curr_process_vertex
        // reset curr_process_vertex
        next_vertex_to_process = std::make_pair('!', INF);

        // Megkeresem azt a csúcsot, aminek a legkisebb a súlya és még nincs meglátogatva.
        for (auto [vertex, distance] : dist)
        {
            if (!visited[vertex])
            {
                if (next_vertex_to_process.second >= distance)
                {
                    next_vertex_to_process = std::make_pair(vertex, distance);
                }
            }
        }

        // Ha megvan a következő csúcs, amivel dolgozni fogok, akkor odalépek.
        curr_vertex = vertexes_copy[next_vertex_to_process.first];
    }

    // Kiíratás:
    std::cout << "Dijkstra's Shortest Path:" << std::endl;
    std::cout << "Direction     Distance       Path" << std::endl;

    for (auto [vertex, distance] : dist)
    {
        std::string path = dijkstra_path(parent, src, vertex);
        std::cout << src << "->" << vertex << "          " << distance << "              " << path << std::endl;
    }

    std::cout << std::endl;
}

void myGraph::prim(char src)
{
    // Ha üres a gráf akkor nem lehet rajta elvégezni Primet.
    // Vagy ha olyan kiindulási csúcsot adok meg ami nem is létezik, akkor jelzem a hibát és nem hajtom
    // végre a Primet.
    if (isEmpty())
    {
        throw EmptyGraph();
    }
    else if (vertexes.find(src) == vertexes.end())
    {
        std::string s(1, src);
        std::cout << "The \"" + s + "\" node is not exist, so it could not be the source of the Prim's algorithm.\n\n";
        return;
    }

    // Másolatot készítek a vertexes-ről, hogy az eredetibe ne kerüljön nem kívánt elem.
    std::map<char, Node*> vertexes_copy = vertexes;

    // Az adott csúcshoz melyik csúcsból indultam el, az lesz a szülő.
    std::map<char, char> parent;

    // Melyik csúcsókra léptem eddig
    std::map<char, bool> visited;

    // Csúcs ahová léptem és mennyi volt a súlya.
    std::map<char, int> mst;

    // Még nem látogatott csúcsok
    std::set<char> unvisited;

    // Beállítás:
    // * mst-ben beállítom, hogy midegyik csúcshoz vezető élnek a súlya legyen az elején végtelen.
    // * visited-ben, hogy a csúcsok még nincsenek meglátogatva.
    // * parent-ben a csúcsok parentje '!' az elején, aztán korrigáljuk.
    // * unvisited-be betesszük a csúcsokat.
    for (auto [vertex, distance] : vertexes_copy)
    {
        mst.insert(std::make_pair(vertex, INF));
        visited.insert(std::make_pair(vertex, false));
        parent.insert(std::make_pair(vertex, '!'));
        unvisited.insert(vertex);
    }

    // A kiindulási pont magába vezető út hossza 0
    mst[src] = 0;

    // Ezen a csúcson vagyok
    Node *curr_vertex = vertexes_copy[src];

    // Ez mondja meg, hogy az eddig feltérképezett utak közül, melyik útnak a súlya a legkisebb és
    // arra felé megyünk és folytatjuk a feltérképezést, ha még nem jártunk annál a csúcsnál.
    std::pair<char, int> curr_min_vertex = std::make_pair('!', INF);

    // A kiindulási pontot érintettük
    unvisited.erase(src);
    visited[curr_vertex->value] = true;

    // A kiindulási pont parentje az önmaga.
    parent[src] = src;

    while (!unvisited.empty())
    {
        // Feltérképezem a csúcsnak a szomszédjait.
        for (auto [adjacent, adj_distance] : curr_vertex->adj)
        {
            // Amelyik még nincs meglátogatva és az abba vezető élnek a súlya kisebb-e, mint
            // az előző legkisebb él ami érinti, ha igen akkor találtunk egy olyan élt aminek a költsége kevesebb
            // és frissítjük a csúcsba vezető él súly értékét az mst-ben és a szülőjét.
            if (!visited[adjacent])
            {
                if (adj_distance < mst[adjacent])
                {
                    mst[adjacent] = adj_distance;
                    parent[adjacent] = curr_vertex->value;
                }
            }
        }

        // Megkeressük a következő legkisebb olyan feltérképezett élét a gráfnak, ami olyan csúcshoz vezet, amit
        // még nem érintettünk.
        for (auto [vertex, distance] : mst)
        {
            if (curr_min_vertex.second > distance)
            {
                if (!visited[vertex])
                {
                    curr_min_vertex = std::make_pair(vertex, distance);
                }
            }
        }

        // Frissítem a visited & unvisited tárolót.
        unvisited.erase(curr_min_vertex.first);
        visited[curr_min_vertex.first] = true;

        // Odalépek a következő csúcshoz
        curr_vertex = vertexes_copy[curr_min_vertex.first];

        // reset curr_min_vertex
        curr_min_vertex = std::make_pair('!', INF);
    }

    // Kiíratás:
    std::cout << "Prim's Minimum Spanning Tree:" << std::endl;
    std::cout << "Path" << "      " << "Weight" << std::endl;

    for (auto [vertex, distance] : mst)
    {
        std::cout << parent[vertex] << "-->" << vertex << "      " << distance << std::endl;
    }

    std::cout << std::endl;
}

// Kiíratom a gráfot, úgy ahogy a feladatleírásban a
// gráfot reprezentáló fájl formátumban megvan adva.
void myGraph::print_graph()
{
    if (!isEmpty())
    {
        std::cout << "A Graf a kovetkezokeppen nez ki: " << std::endl;

        for (auto [vertex_value, node] : vertexes)
        {
            std::cout << vertex_value << std::endl;

            for (auto adjacent = node->adj.begin(); adjacent != node->adj.end(); adjacent++)
            {
                std::cout << adjacent->first;

                // Végén ne legyen vessző
                if (adjacent != std::prev(node->adj.end()))
                {
                    std::cout << ",";
                }
            }

            std::cout << std::endl;

            // Végén ne legyen vessző
            for (auto distance = node->adj.begin(); distance != node->adj.end(); distance++)
            {
                std::cout << distance->second;

                // Végén ne legyen vessző
                if (distance != std::prev(node->adj.end()))
                {
                    std::cout << ",";
                }
            }

            std::cout << std::endl << std::endl;
        }

        std::cout << std::endl;
    }
    else
    {
        std::cout << "A graf ures!" << std::endl << std::endl;
    }
}

#endif //CODE_MY_CLASS_GRAPH_HPP
