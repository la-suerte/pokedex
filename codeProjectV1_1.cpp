//test.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <memory>
#include <map>
#include <limits>
#include "asciiAff.h"

using namespace std;

// Interface pour les entités pouvant être interagies
class Interagir {
public:
    virtual std::string interagir() const = 0;
    virtual ~Interagir() {}  // Virtual destructor with implementation
};

// Classe de base pour les Pokémon
class Pokemon : public Interagir {
protected:
    std::string nom;
    std::vector<std::string> types;
    int hp;
    int hpMax;
    std::string nomAttaque;
    int degatsAttaque;
    std::map<std::string, float> tableEfficacite;
    
    int numPokedex;

public:
    Pokemon(const std::string& nom, const std::vector<std::string>& types, 
            int hp, const std::string& nomAttaque, int degatsAttaque, int numPokedex)
        : nom(nom), types(types), hp(hp), hpMax(hp), nomAttaque(nomAttaque), degatsAttaque(degatsAttaque), numPokedex(numPokedex) {
        initTableEfficacite();
    }

    virtual ~Pokemon() {}  // Virtual destructor with implementation

    virtual void initTableEfficacite() {
        // Par défaut, tous les types ont une efficacité normale (x1)
        std::vector<std::string> allTypes;
        allTypes.push_back("Feu");
        allTypes.push_back("Eau");
        allTypes.push_back("Plante");
        allTypes.push_back("Électrik");
        allTypes.push_back("Glace");
        allTypes.push_back("Combat");
        allTypes.push_back("Poison");
        allTypes.push_back("Sol");
        allTypes.push_back("Vol");
        allTypes.push_back("Psy");
        allTypes.push_back("Insecte");
        allTypes.push_back("Roche");
        allTypes.push_back("Spectre");
        allTypes.push_back("Dragon");
        allTypes.push_back("Ténèbres");
        allTypes.push_back("Acier");
        allTypes.push_back("Fée");
        
        for (size_t i = 0; i < allTypes.size(); ++i) {
            tableEfficacite[allTypes[i]] = 1.0f;
        }
    }

    const std::string& getNom() const { return nom; }
    const std::vector<std::string>& getTypes() const { return types; }
    int getHp() const { return hp; }
    int getHpMax() const { return hpMax; }
    const std::string& getNomAttaque() const { return nomAttaque; }
    int getDegatsAttaque() const { return degatsAttaque; }
    int getNumPokedex() const { return numPokedex; }

    void setHp(int nouveauHp) { 
        hp = nouveauHp; 
        if (hp > hpMax) hp = hpMax;
        if (hp < 0) hp = 0;
    }

    void recupererVie() { hp = hpMax; }

    bool estVivant() const { return hp > 0; }

    virtual std::string getTypeAttaque() const {
        return !types.empty() ? types[0] : "Normal";
    }

    float getMultiplicateurEfficacite(const std::string& typeAttaque) const {
        float multiplicateur = 1.0f;
        
        for (size_t i = 0; i < types.size(); ++i) {
            const std::string& type = types[i];
            if (type == "Feu") {
                if (typeAttaque == "Eau" || typeAttaque == "Roche" || typeAttaque == "Sol") multiplicateur *= 2.0f;
                if (typeAttaque == "Plante" || typeAttaque == "Glace" || typeAttaque == "Insecte" || 
                    typeAttaque == "Acier" || typeAttaque == "Fée") multiplicateur *= 0.5f;
            }
            else if (type == "Eau") {
                if (typeAttaque == "Plante" || typeAttaque == "Électrik") multiplicateur *= 2.0f;
                if (typeAttaque == "Feu" || typeAttaque == "Eau" || typeAttaque == "Glace" || 
                    typeAttaque == "Acier") multiplicateur *= 0.5f;
            }
            else if (type == "Plante") {
                if (typeAttaque == "Feu" || typeAttaque == "Glace" || typeAttaque == "Poison" || 
                   typeAttaque == "Vol" || typeAttaque == "Insecte") multiplicateur *= 2.0f;
                if (typeAttaque == "Eau" || typeAttaque == "Sol" || typeAttaque == "Roche") multiplicateur *= 0.5f;
            }
            else if (type == "Électrik") {
                if (typeAttaque == "Sol") multiplicateur *= 2.0f;
                if (typeAttaque == "Vol" || typeAttaque == "Acier" || typeAttaque == "Électrik") multiplicateur *= 0.5f;
            }
            else if (type == "Glace") {
                if (typeAttaque == "Feu" || typeAttaque == "Combat" || typeAttaque == "Roche" || 
                   typeAttaque == "Acier") multiplicateur *= 2.0f;
                if (typeAttaque == "Glace") multiplicateur *= 0.5f;
            }
            // Ajoutez les autres types ici...
        }
        
        return multiplicateur;
    }

    virtual int attaquer(Pokemon& cible) {
        std::string typeAttaque = getTypeAttaque();
        float multiplicateur = cible.getMultiplicateurEfficacite(typeAttaque);
        int degatsFinaux = static_cast<int>(degatsAttaque * multiplicateur);
        cible.setHp(cible.getHp() - degatsFinaux);

        std::cout << nom << " utilise " << nomAttaque << " sur " << cible.getNom() << "! ";
        
        if (multiplicateur > 1.0f) {
            std::cout << "C'est super efficace! ";
        } else if (multiplicateur < 1.0f) {
            std::cout << "Ce n'est pas très efficace... ";
        }
        
        std::cout << "Il inflige " << degatsFinaux << " dégâts." << std::endl;
        std::cout << cible.getNom() << " a maintenant " << cible.getHp() << "/" << cible.getHpMax() << " HP." << std::endl;
        
        return degatsFinaux;
    }

    std::string afficherInfos() const {
        std::stringstream ss;
        ss << "Nom: " << nom << " | Types: ";
        for (size_t i = 0; i < types.size(); ++i) {
            ss << types[i];
            if (i < types.size() - 1) ss << ", ";
        }
        ss << " | Attaque: " << nomAttaque << " (" << degatsAttaque << " dégâts)";
        return ss.str();
    }

    virtual std::string interagir() const {
        return "Le Pokémon " + nom + " semble heureux de vous voir!";
    }
};

// Classes dérivées pour chaque type de Pokémon
class PokemonFeu : public Pokemon {
public:
    PokemonFeu(const std::string& nom, const std::vector<std::string>& types, 
               int hp, const std::string& nomAttaque, int degatsAttaque, int numPokedex)
        : Pokemon(nom, types, hp, nomAttaque, degatsAttaque, numPokedex) {}

    virtual std::string interagir() const {
        return "Le Pokémon " + nom + " crache quelques flammes joyeusement!";
    }
};

class PokemonEau : public Pokemon {
public:
    PokemonEau(const std::string& nom, const std::vector<std::string>& types, 
               int hp, const std::string& nomAttaque, int degatsAttaque, int numPokedex)
        : Pokemon(nom, types, hp, nomAttaque, degatsAttaque, numPokedex) {}

    virtual std::string interagir() const {
        return "Le Pokémon " + nom + " fait des bulles et vous éclabousse!";
    }
};

class PokemonPlante : public Pokemon {
public:
    PokemonPlante(const std::string& nom, const std::vector<std::string>& types, 
                 int hp, const std::string& nomAttaque, int degatsAttaque, int numPokedex)
        : Pokemon(nom, types, hp, nomAttaque, degatsAttaque, numPokedex) {}

    virtual std::string interagir() const {
        return "Le Pokémon " + nom + " fait pousser une petite fleur près de vous!";
    }
};

void afficheHp(int hp, int hpMax)
{
    cout<<"   |";
    for(int i = 0 ; i < hpMax ; i++)
    {
        cout<<"-";
    }
    cout<<"|\nHP |";
    for(int i = 0 ; i < hpMax ; i++)
    {
        if(i<hp)
            cout<<"|";
        else 
            cout<<" ";
    }
    cout<<"|\n   |";
    for(int i = 0 ; i < hpMax ; i++)
    {
        cout<<"-";
    }
    cout<<"|"<<endl;

}

// Classe Entraîneur de base
class Entraineur : public Interagir {
protected:
    std::string nom;
    std::vector<std::shared_ptr<Pokemon> > equipe;
    bool vaincu;

public:
    Entraineur(const std::string& nom) : nom(nom), vaincu(false) {}
    virtual ~Entraineur() {}  // Virtual destructor with implementation

    const std::string& getNom() const { return nom; }
    const std::vector<std::shared_ptr<Pokemon> >& getEquipe() const { return equipe; }
    bool estVaincu() const { return vaincu; }
    void setVaincu(bool estVaincu) { vaincu = estVaincu; }

    void ajouterPokemon(std::shared_ptr<Pokemon> pokemon) {
        if (equipe.size() < 6) {
            equipe.push_back(pokemon);
        } else {
            std::cout << "L'équipe est déjà pleine (maximum 6 Pokémon)." << std::endl;
        }
    }

    void changerOrdrePokemon(int index1, int index2) {
        if (index1 >= 0 && index1 < static_cast<int>(equipe.size()) &&
            index2 >= 0 && index2 < static_cast<int>(equipe.size())) {
            std::swap(equipe[index1], equipe[index2]);
            std::cout << "L'ordre des Pokémon a été changé." << std::endl;
        } else {
            std::cout << "Indices invalides." << std::endl;
        }
    }

    bool tousLesPokemonKO() const {
        for (size_t i = 0; i < equipe.size(); ++i) {
            if (equipe[i]->estVivant()) {
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<Pokemon> getPremierPokemonVivant() const {
        for (size_t i = 0; i < equipe.size(); ++i) {
            if (equipe[i]->estVivant()) {
                return equipe[i];
            }
        }
        return std::shared_ptr<Pokemon>();  // Return null pointer
    }
    

    void afficherEquipe() const {
        std::cout << "Équipe de " << nom << ":" << std::endl;
        for (size_t i = 0; i < equipe.size(); ++i) {
            std::cout << i + 1 << ". " << equipe[i]->afficherInfos() << std::endl;
            afficheHp(equipe[i]->getHp(),equipe[i]->getHpMax());
            print_pokemon(equipe[i]->getNumPokedex());
        }

    }

    void recupererTousLesPokemon() {
        for (size_t i = 0; i < equipe.size(); ++i) {
            equipe[i]->recupererVie();
        }
        std::cout << "Tous les Pokémon de " << nom << " ont récupéré leurs points de vie!" << std::endl;
    }

    virtual std::string interagir() const {
        if (vaincu) {
            return nom + " dit: \"Tu m'as battu équitablement. Continue à t'entraîner pour devenir encore plus fort!\"";
        } else {
            return nom + " dit: \"Veux-tu me défier en combat?\"";
        }
    }
};

// Classe Joueur
class Joueur : public Entraineur {
private:
    int badges;
    int combatsGagnes;
    int combatsPerdus;

public:
    Joueur(const std::string& nom) 
        : Entraineur(nom), badges(0), combatsGagnes(0), combatsPerdus(0) {}

    int getBadges() const { return badges; }
    int getCombatsGagnes() const { return combatsGagnes; }
    int getCombatsPerdus() const { return combatsPerdus; }

    void ajouterBadge() { badges++; }
    void incrementerCombatsGagnes() { combatsGagnes++; }
    void incrementerCombatsPerdus() { combatsPerdus++; }

    void afficherStatistiques() const {
        std::cout << "Statistiques de " << nom << ":" << std::endl;
        std::cout << "Badges: " << badges << std::endl;
        std::cout << "Combats gagnés: " << combatsGagnes << std::endl;
        std::cout << "Combats perdus: " << combatsPerdus << std::endl;
    }

    virtual std::string interagir() const {
        return nom + " dit: \"Je m'entraîne dur pour devenir le meilleur dresseur!\"";
    }
};

// Classe Leader de Gymnase
class LeaderGym : public Entraineur {
private:
    std::string badge;
    std::string gymnase;

public:
    LeaderGym(const std::string& n, const std::string& g)
        : Entraineur(n), gymnase(g), badge("Badge de  " + g) {}


    const std::string& getBadge() const { return badge; }
    const std::string& getGymnase() const { return gymnase; }

    virtual std::string interagir() const {
        if (vaincu) {
            return nom + " dit: \"Tu as gagné le badge " + badge + "! Tu as beaucoup de talent.\"";
        } else {
            return nom + " dit: \"Je suis " + nom + ", le Champion d'Arène de " + gymnase + ". Défie-moi si tu l'oses!\"";
        }
    }
};

// Classe Maître Pokémon
class MaitrePokemon : public Entraineur {
public:
    MaitrePokemon(const std::string& nom) : Entraineur(nom) {}

    int attaquer(Pokemon& cible, Pokemon& attaquant) {
        float bonusMaitre = 1.25f; // Bonus de 25% pour les Maîtres Pokémon
        std::string typeAttaque = attaquant.getTypeAttaque();
        float multiplicateur = cible.getMultiplicateurEfficacite(typeAttaque);
        int degatsFinaux = static_cast<int>(attaquant.getDegatsAttaque() * multiplicateur * bonusMaitre);
        cible.setHp(cible.getHp() - degatsFinaux);

        std::cout << attaquant.getNom() << " du Maître " << nom << " utilise " << attaquant.getNomAttaque() 
                  << " avec un bonus de puissance! ";
        
        if (multiplicateur > 1.0f) {
            std::cout << "C'est super efficace! ";
        } else if (multiplicateur < 1.0f) {
            std::cout << "Ce n'est pas très efficace... ";
        }
        
        std::cout << "Il inflige " << degatsFinaux << " dégâts." << std::endl;
        std::cout << cible.getNom() << " a maintenant " << cible.getHp() << "/" << cible.getHpMax() << " HP." << std::endl;
        
        return degatsFinaux;
    }

    virtual std::string interagir() const {
        if (vaincu) {
            return nom + " dit: \"Impressionnant! Tu es vraiment un dresseur exceptionnel.\"";
        } else {
            return nom + " dit: \"Je suis " + nom + ", un Maître Pokémon. Montre-moi ta force!\"";
        }
    }
};

// Remplacer std::random_device et distributions par alternatives pre-C++11
// Cette fonction simple retourne un nombre aléatoire entre min et max
int getRandomNumber(int min, int max) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(NULL)));
        initialized = true;
    }
    return min + (rand() % (max - min + 1));
}

// Fonction pour charger les Pokémon à partir d'un fichier CSV
std::vector<std::shared_ptr<Pokemon> > chargerPokemonDepuisCSV(const std::string& filename) {
    std::vector<std::shared_ptr<Pokemon> > pokemons;
    std::ifstream file(filename.c_str());  // C-style string for older compilers
    
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier " << filename << std::endl;
        // Créer quelques Pokémon par défaut
        std::vector<std::string> typeFeu;
        typeFeu.push_back("Feu");
        pokemons.push_back(std::make_shared<PokemonFeu>("Salamèche", typeFeu, 39, "Flammèche", 70,2));
        
        std::vector<std::string> typeEau;
        typeEau.push_back("Eau");
        pokemons.push_back(std::make_shared<PokemonEau>("Carapuce", typeEau, 44, "Pistolet à O", 65,6));
        
        std::vector<std::string> typePlante;
        typePlante.push_back("Plante");
        typePlante.push_back("Poison");
        pokemons.push_back(std::make_shared<PokemonPlante>("Bulbizarre", typePlante, 45, "Fouet Lianes", 60,7));
        return pokemons;
    }
    
    std::string line;
    // Ignorer la première ligne (en-têtes)
    std::getline(file, line);

    int count = 1;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nom, typesStr, type2, hpStr, nomAttaque, degatsAttaqueStr, numPokedex;
        
        std::getline(ss, nom, ',');
        std::getline(ss, typesStr, ',');
        std::getline(ss, type2, ',');
        std::getline(ss, hpStr, ',');
        std::getline(ss, nomAttaque, ',');
        std::getline(ss, degatsAttaqueStr, ',');
        int numPokedexInt = count;
        
        // Convertir les types en vector
        std::vector<std::string> types;
        std::stringstream typesStream(typesStr);
        std::string type;
        while (std::getline(typesStream, type, '|')) {
            types.push_back(type);
        }
        
        int hp = std::atoi(hpStr.c_str());
        int degatsAttaque = std::atoi(degatsAttaqueStr.c_str());
        
        // Créer le Pokémon approprié basé sur son type principal
        if (!types.empty()) {
            if (types[0] == "Feu") {
                pokemons.push_back(std::make_shared<PokemonFeu>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            } else if (types[0] == "Eau") {
                pokemons.push_back(std::make_shared<PokemonEau>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            } else if (types[0] == "Plante") {
                pokemons.push_back(std::make_shared<PokemonPlante>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            } else {
                // Type générique
                pokemons.push_back(std::make_shared<Pokemon>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            }
            count+=1;
        }
    }
    
    file.close();
    return pokemons;
}
    /*
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier " << filename << std::endl;
        // Créer quelques leaders par défaut
        std::shared_ptr<LeaderGym> leader1(new LeaderGym("Pierre", "Roche", "Argenta"));
        std::vector<std::string> typeRoche;
        typeRoche.push_back("Roche");
        leader1->ajouterPokemon(std::make_shared<Pokemon>("Racaillou", typeRoche, 40, "Jet-Pierres", 60));
        
        std::shared_ptr<LeaderGym> leader2(new LeaderGym("Ondine", "Cascade", "Azuria"));
        std::vector<std::string> typeEauPsy;
        typeEauPsy.push_back("Eau");
        typeEauPsy.push_back("Psy");
        leader2->ajouterPokemon(std::make_shared<PokemonEau>("Staross", typeEauPsy, 45, "Hydrocanon", 75));
        
        std::shared_ptr<LeaderGym> leader3(new LeaderGym("Major Bob", "Tonnerre", "Carmin sur Mer"));
        std::vector<std::string> typeElectrik;
        typeElectrik.push_back("Électrik");
        leader3->ajouterPokemon(std::make_shared<Pokemon>("Pikachu", typeElectrik, 35, "Éclair", 65));
        
        std::shared_ptr<LeaderGym> leader4(new LeaderGym("Erika", "Arc-en-ciel", "Céladopole"));
        std::vector<std::string> typePlante;
        typePlante.push_back("Plante");
        typePlante.push_back("Poison");
        leader4->ajouterPokemon(std::make_shared<PokemonPlante>("Herbizarre", typePlante, 50, "Tranch'Herbe", 70));
        
        leaders.push_back(leader1);
        leaders.push_back(leader2);
        leaders.push_back(leader3);
        leaders.push_back(leader4);
        return leaders;
    }*/
// Fonction pour charger les leaders de gym à partir d'un fichier CSV
std::vector<std::shared_ptr<LeaderGym> > chargerLeadersDepuisCSV(const std::string& filename, const std::vector<std::shared_ptr<Pokemon> >& tousLesPokemon) {
    std::vector<std::shared_ptr<LeaderGym> > leaders;
    std::ifstream file(filename.c_str());
    
    std::string line;
    // Ignorer la première ligne (en-têtes)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nom, gymnase, pokemonNom;
        
        // Lire le nom et le gymnase
        std::getline(ss, nom, ',');
        std::getline(ss, gymnase, ',');
        //std::getline(ss, medaille, ',');  // Skip the medal field
        
        // Créer le leader
        std::shared_ptr<LeaderGym> leader(new LeaderGym(nom, gymnase));
        
        // Lire directement les noms des Pokémon (un par colonne)
        while (std::getline(ss, pokemonNom, ',')) {
            // Ignorer les entrées vides
            if (pokemonNom.empty()) {
                continue;
            }
            
            // Chercher le Pokémon correspondant dans la liste
            for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
                if (tousLesPokemon[i]->getNom() == pokemonNom) {
                    // Créer une copie du Pokémon et l'ajouter à l'équipe du leader
                    Pokemon* newPokemon = new Pokemon(*tousLesPokemon[i]);
                    leader->ajouterPokemon(std::shared_ptr<Pokemon>(newPokemon));
                    break;
                }
            }
        }
        
        leaders.push_back(leader);
    }
    
    file.close();
    return leaders;
}

// Fonction pour charger les maîtres Pokémon à partir d'un fichier CSV
std::vector<std::shared_ptr<MaitrePokemon> > chargerMaitresDepuisCSV(const std::string& filename, const std::vector<std::shared_ptr<Pokemon> >& tousLesPokemon) {
    std::vector<std::shared_ptr<MaitrePokemon> > maitres;
    std::ifstream file(filename.c_str());
    
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier " << filename << std::endl;
        // Créer quelques maîtres par défaut
        std::shared_ptr<MaitrePokemon> maitre1(new MaitrePokemon("Sacha"));
        std::vector<std::string> typeElectrik;
        typeElectrik.push_back("Électrik");
        maitre1->ajouterPokemon(std::make_shared<Pokemon>("Pikachu", typeElectrik, 60, "Fatal-Foudre", 90,25));
        
        std::vector<std::string> typeFeuVol;
        typeFeuVol.push_back("Feu");
        typeFeuVol.push_back("Vol");
        maitre1->ajouterPokemon(std::make_shared<PokemonFeu>("Dracaufeu", typeFeuVol, 78, "Lance-Flammes", 95,2));
        
        std::shared_ptr<MaitrePokemon> maitre2(new MaitrePokemon("Peter"));
        std::vector<std::string> typeDragonVol;
        typeDragonVol.push_back("Dragon");
        typeDragonVol.push_back("Vol");
        maitre2->ajouterPokemon(std::make_shared<Pokemon>("Dracolosse", typeDragonVol, 90, "Colère", 100,3));
        
        maitres.push_back(maitre1);
        maitres.push_back(maitre2);
        return maitres;
    }
    
    std::string line;
    // Ignorer la première ligne (en-têtes)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string nom, pokemonsStr;
        
        std::getline(ss, nom, ',');
        std::getline(ss, pokemonsStr, ',');
        
        std::shared_ptr<MaitrePokemon> maitre(new MaitrePokemon(nom));
        
        // Ajouter les Pokémon au maître
        std::stringstream pokemonsStream(pokemonsStr);
        std::string pokemonNom;
        while (std::getline(pokemonsStream, pokemonNom, '|')) {
            for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
                if (tousLesPokemon[i]->getNom() == pokemonNom) {
                    // Pre-C++11 doesn't have make_shared, use explicit new
                    Pokemon* newPokemon = new Pokemon(*tousLesPokemon[i]);
                    maitre->ajouterPokemon(std::shared_ptr<Pokemon>(newPokemon));
                    break;
                }
            }
        }
        
        maitres.push_back(maitre);
    }
    
    file.close();
    return maitres;
}

void debugAfficherEquipeHP(const std::shared_ptr<Entraineur>& entraineur) {
    std::cout << "\n----- DEBUG: ÉTAT DE L'ÉQUIPE DE " << entraineur->getNom() << " -----" << std::endl;
    const std::vector<std::shared_ptr<Pokemon> >& equipe = entraineur->getEquipe();
    
    // Afficher chaque Pokémon et son état
    for (size_t i = 0; i < equipe.size(); ++i) {
        std::cout << i + 1 << ". " << equipe[i]->getNom() 
                  << " (HP: " << equipe[i]->getHp() << "/" << equipe[i]->getHpMax() 
                  << ") - " << (equipe[i]->estVivant() ? "VIVANT" : "KO") << std::endl;
    }
    
    // Afficher le résultat de tousLesPokemonKO()
    std::cout << "tousLesPokemonKO() retourne: " 
              << (entraineur->tousLesPokemonKO() ? "true (tous KO)" : "false (certains vivants)") << std::endl;
    std::cout << "----------------------------------------" << std::endl;
}

void combat(std::shared_ptr<Joueur> joueur, std::shared_ptr<Entraineur> adversaire) 
{
    std::cout << "\n----- DÉBUT DU COMBAT -----" << std::endl;
    std::cout << joueur->getNom() << " VS " << adversaire->getNom() << std::endl;
    
    // Vérifier si les deux dresseurs ont des Pokémon
    if (joueur->getEquipe().empty() || adversaire->getEquipe().empty()) {
        std::cout << "Un des dresseurs n'a pas de Pokémon!" << std::endl;
        return;
    }
    //debugAfficherEquipeHP(adversaire);

    string inutile; //HERE

    // Déclarer pokemonJoueur et pokemonAdversaire
    std::shared_ptr<Pokemon> pokemonJoueur = joueur->getPremierPokemonVivant();
    std::shared_ptr<Pokemon> pokemonAdversaire = adversaire->getPremierPokemonVivant();
    
    // Combat au tour par tour
    while (!joueur->tousLesPokemonKO() && !adversaire->tousLesPokemonKO()) 
    {
        
        std::cout << "\n";
        std::cout << " #######                                                                                   \n";
        std::cout << "    #     ####  #    # #####     #####  ######     ####   ####  #    # #####    ##   ##### \n";
        std::cout << "    #    #    # #    # #    #    #    # #         #    # #    # ##  ## #    #  #  #    #   \n";
        std::cout << "    #    #    # #    # #    #    #    # #####     #      #    # # ## # #####  #    #   #   \n";
        std::cout << "    #    #    # #    # #####     #    # #         #      #    # #    # #    # ######   #   \n";
        std::cout << "    #    #    # #    # #   #     #    # #         #    # #    # #    # #    # #    #   #   \n";
        std::cout << "    #     ####   ####  #    #    #####  ######     ####   ####  #    # #####  #    #   #   \n\n";        
           
        
        std::cout << pokemonJoueur->getNom()<<endl;
        afficheHp(pokemonJoueur->getHp(),pokemonJoueur->getHpMax());
        print_pokemon(pokemonJoueur->getNumPokedex());


        std::cout << "\n\n #     #  #####  \n";
        std::cout << " #     # #     # \n";
        std::cout << " #     # #       \n";
        std::cout << " #     #  #####  \n";
        std::cout << "  #   #        # \n";
        std::cout << "   # #   #     # \n";
        std::cout << "    #     #####  \n";
        std::cout << "                 \n";
        std::cout << pokemonAdversaire->getNom()<<endl;
        afficheHp(pokemonAdversaire->getHp(),pokemonAdversaire->getHpMax());
        print_pokemon(pokemonAdversaire->getNumPokedex());


        // Le joueur attaque en premier
        pokemonJoueur->attaquer(*pokemonAdversaire);
        
        //debugAfficherEquipeHP(adversaire);
        // Vérifier si le Pokémon adversaire est KO
        if (!pokemonAdversaire->estVivant()) {
            std::cout << pokemonAdversaire->getNom() << " est KO!" << std::endl;
            
            // Vérifier si l'adversaire a encore des Pokémon vivants
            if (!adversaire->tousLesPokemonKO()) {
                // Obtenir le prochain Pokémon vivant
                pokemonAdversaire = adversaire->getPremierPokemonVivant();
                std::cout << adversaire->getNom() << " envoie " << pokemonAdversaire->getNom() << "!" << std::endl;
            } else {
                // Si tous les Pokémon de l'adversaire sont KO, terminer le tour
                break;
            }
        }
        std::cout << "\n PRESS N + ENTER TO CONTINUE \n";
        cin>>inutile; // Waits for Enter after a key press
        
        if (pokemonAdversaire->estVivant()) {
            MaitrePokemon* maitre = dynamic_cast<MaitrePokemon*>(adversaire.get());
            if (maitre) {
                maitre->attaquer(*pokemonJoueur, *pokemonAdversaire);
            } else {
                pokemonAdversaire->attaquer(*pokemonJoueur);
            }
            
            // Vérifier si le Pokémon du joueur est KO
            if (!pokemonJoueur->estVivant()) {
                std::cout << pokemonJoueur->getNom() << " est KO!" << std::endl;
                
                // Vérifier si le joueur a encore des Pokémon vivants
                if (!joueur->tousLesPokemonKO()) {
                    // Obtenir le prochain Pokémon vivant
                    pokemonJoueur = joueur->getPremierPokemonVivant();
                    std::cout << joueur->getNom() << " envoie " << pokemonJoueur->getNom() << "!" << std::endl;
                }
            }
        }
        std::cout << "\n PRESS N + ENTER TO CONTINUE \n";
        cin>>inutile; // Waits for Enter after a key press
    }
    
    // Déterminer le vainqueur
    bool joueurGagnant = !joueur->tousLesPokemonKO();
    
    std::cout << "\n----- FIN DU COMBAT -----" << std::endl;
    if (joueurGagnant) {
        std::cout << joueur->getNom() << " a gagné le combat!" << std::endl;
        joueur->incrementerCombatsGagnes();
        
        // Si l'adversaire est un Leader de Gym, le joueur gagne un badge
        LeaderGym* leader = dynamic_cast<LeaderGym*>(adversaire.get());
        if (leader) {
            std::cout << leader->getNom() << " dit: \"Félicitations! Tu as gagné le badge " << leader->getBadge() << "!\"" << std::endl;
            joueur->ajouterBadge();
            leader->setVaincu(true);
        }
        
        // Si l'adversaire est un Maître Pokémon, marquer comme vaincu
        MaitrePokemon* maitre = dynamic_cast<MaitrePokemon*>(adversaire.get());
        if (maitre) {
            maitre->setVaincu(true);
        }
    } else {
        std::cout << adversaire->getNom() << " a gagné le combat!" << std::endl;
        joueur->incrementerCombatsPerdus();
    }
    std::cout << "\n PRESS N + ENTER TO CONTINUE \n";
    cin>>inutile; // Waits for Enter after a key press

}
// Fonction qui gère le menu principal
void menuPrincipal(std::shared_ptr<Joueur> joueur, 
                  std::vector<std::shared_ptr<LeaderGym> > leaders,
                  std::vector<std::shared_ptr<MaitrePokemon> > maitres,
                  std::vector<std::shared_ptr<Entraineur> > entraineursVaincus) 
{
    
    bool quitter = false;
    
    while (!quitter) {
        system("clear");
        std::cout << "\n===== MENU PRINCIPAL =====" << std::endl;
        std::cout << "1. Afficher mes Pokémon" << std::endl;
        std::cout << "2. Récupérer les points de vie de mes Pokémon" << std::endl;
        std::cout << "3. Changer l'ordre de mes Pokémon" << std::endl;
        std::cout << "4. Afficher mes statistiques" << std::endl;
        std::cout << "5. Affronter un leader de gymnase" << std::endl;
        std::cout << "6. Affronter un Maître Pokémon" << std::endl;
        std::cout << "7. Interagir avec mes Pokémon" << std::endl;
        std::cout << "8. Interagir avec les entraîneurs vaincus" << std::endl;
        std::cout << "9. Quitter" << std::endl;
        std::cout << "Votre choix: ";
        
        int choix;
        std::cin >> choix;
        
        // Nettoyer le buffer d'entrée
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        //system("clear");
        switch (choix) {
            case 1: {
                // Afficher les Pokémon du joueur
                joueur->afficherEquipe();
                cout<<"Appuyer sur une Q puis ENTER pour continuer";
                string p;
                cin>>p;
                break;
            }
            case 2: {
                // Récupérer les points de vie des Pokémon
                joueur->recupererTousLesPokemon();
                cout<<"Point de vie recuperés"<<endl<<"Appuyer sur une Q puis ENTER pour continuer";
                string p;
                cin>>p;
                break;
            }
            case 3: {
                // Changer l'ordre des Pokémon
                joueur->afficherEquipe();
                std::cout << "Entrez l'indice du premier Pokémon à échanger (1-" << joueur->getEquipe().size() << "): ";
                int index1;
                std::cin >> index1;
                
                std::cout << "Entrez l'indice du second Pokémon à échanger (1-" << joueur->getEquipe().size() << "): ";
                int index2;
                std::cin >> index2;
                
                joueur->changerOrdrePokemon(index1 - 1, index2 - 1);
                break;
            }
            case 4: {
                // Afficher les statistiques du joueur
                joueur->afficherStatistiques();
                cout<<"Appuyer sur une Q puis ENTER pour continuer";
                string p;
                cin>>p;
                break;
            }
            case 5: {
                // Affronter un leader de gymnase
                std::cout << "\n=== LEADERS DE GYMNASE DISPONIBLES ===" << std::endl;
                for (size_t i = 0; i < leaders.size(); ++i) {
                    std::cout << i + 1 << ". " << leaders[i]->getNom() << " (" << leaders[i]->getGymnase() << ")";
                    if (leaders[i]->estVaincu()) {
                        std::cout << " [DÉJÀ VAINCU]";
                    }
                    std::cout << std::endl;
                }
                
                std::cout << "Choisissez un leader à affronter (0 pour revenir): ";
                int choixLeader = 1;
                std::cin >> choixLeader; 
                
                if (choixLeader > 0 && choixLeader <= static_cast<int>(leaders.size())) {
                    combat(joueur, leaders[choixLeader - 1]);
                    
                    // Ajouter le leader aux entraîneurs vaincus s'il a été battu
                    if (leaders[choixLeader - 1]->estVaincu()) {
                        bool dejaVaincu = false;
                        for (size_t i = 0; i < entraineursVaincus.size(); ++i) {
                            if (entraineursVaincus[i]->getNom() == leaders[choixLeader - 1]->getNom()) {
                                dejaVaincu = true;
                                break;
                            }
                        }
                        
                        if (!dejaVaincu) {
                            entraineursVaincus.push_back(leaders[choixLeader - 1]);
                        }
                    }
                }
                break;
            }
            case 6: {
                // Affronter un Maître Pokémon (seulement si tous les badges sont obtenus)
                if (joueur->getBadges() >= 4) {  // Minimum 4 badges requis
                    // Choisir un maître au hasard
                    if (!maitres.empty()) {
                        int indexMaitre = getRandomNumber(0, maitres.size() - 1);
                        std::cout << "Vous allez affronter le Maître Pokémon " << maitres[indexMaitre]->getNom() << "!" << std::endl;
                        combat(joueur, maitres[indexMaitre]);
                        
                        // Ajouter le maître aux entraîneurs vaincus s'il a été battu
                        if (maitres[indexMaitre]->estVaincu()) {
                            bool dejaVaincu = false;
                            for (size_t i = 0; i < entraineursVaincus.size(); ++i) {
                                if (entraineursVaincus[i]->getNom() == maitres[indexMaitre]->getNom()) {
                                    dejaVaincu = true;
                                    break;
                                }
                            }
                            
                            if (!dejaVaincu) {
                                entraineursVaincus.push_back(maitres[indexMaitre]);
                            }
                        }
                    } else {
                        std::cout << "Aucun Maître Pokémon disponible!" << std::endl;
                    }
                } else {
                    std::cout << "Vous devez obtenir au moins 4 badges avant de pouvoir affronter un Maître Pokémon!" << std::endl;
                }
                break;
            }
            case 7: {
                // Interagir avec les Pokémon du joueur
                joueur->afficherEquipe();
                std::cout << "Avec quel Pokémon voulez-vous interagir? (1-" << joueur->getEquipe().size() << ", 0 pour annuler): ";
                int choixPokemon;
                std::cin >> choixPokemon;
                
                if (choixPokemon > 0 && choixPokemon <= static_cast<int>(joueur->getEquipe().size())) {
                    //system("clear");
                    print_pokemon(joueur->getEquipe()[choixPokemon - 1]->getNumPokedex());
                    std::cout << joueur->getEquipe()[choixPokemon - 1]->interagir() << std::endl;
                }
                break;
            }
            case 8: {
                // Interagir avec les entraîneurs vaincus
                if (entraineursVaincus.empty()) {
                    std::cout << "Vous n'avez pas encore vaincu d'entraîneur!" << std::endl;
                } else {
                    std::cout << "\n=== ENTRAÎNEURS VAINCUS ===" << std::endl;
                    for (size_t i = 0; i < entraineursVaincus.size(); ++i) {
                        std::cout << i + 1 << ". " << entraineursVaincus[i]->getNom() << std::endl;
                    }
                    
                    std::cout << "Avec quel entraîneur voulez-vous interagir? (1-" << entraineursVaincus.size() << ", 0 pour annuler): ";
                    int choixEntraineur;
                    std::cin >> choixEntraineur;
                    
                    if (choixEntraineur > 0 && choixEntraineur <= static_cast<int>(entraineursVaincus.size())) {
                        std::cout << entraineursVaincus[choixEntraineur - 1]->interagir() << std::endl;
                    }
                }
                break;
            }
            case 9: {
                // Quitter le jeu
                std::cout << "Merci d'avoir joué! À bientôt!" << std::endl;
                quitter = true;
                break;
            }
            case 10:
                system("clear");
                break;
            default: {
                std::cout << "Choix invalide. Veuillez réessayer." << std::endl;
                break;
            }
        }
    }
}

// Fonction principale
int main() {
    system("clear");
    std::cout << "=== SIMULATEUR DE COMBAT POKÉMON ===" << std::endl;
    
    // Charger les données des fichiers CSV
    std::vector<std::shared_ptr<Pokemon> > tousLesPokemon = chargerPokemonDepuisCSV("pokemon.csv");
    std::vector<std::shared_ptr<LeaderGym> > leaders = chargerLeadersDepuisCSV("leaders.csv", tousLesPokemon);
    std::vector<std::shared_ptr<MaitrePokemon> > maitres = chargerMaitresDepuisCSV("maitres.csv", tousLesPokemon);
    
    // Créer un joueur
    std::string nomJoueur;
    std::cout << "Entrez votre nom: ";
    std::getline(std::cin, nomJoueur); 
    //nomJoueur = "Antoine Lesort";
    
    std::shared_ptr<Joueur> joueur(new Joueur(nomJoueur));
    
    std::cout << "\nChoisissez vos Pokémon de départ:" << std::endl;
    std::cout << "Pokémon disponibles:" << std::endl;
    
    for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
        std::cout << i + 1 << ". " << tousLesPokemon[i]->afficherInfos() << std::endl;
    }
    
    for (int i = 0; i < 3; ++i) {  // Sélection de 3 Pokémon initiaux
        std::cout << "\nChoisissez votre Pokémon #" << i + 1 << " (1-" << tousLesPokemon.size() << "): ";
        int choix;
        std::cin >> choix;
            if (choix > 0 && choix <= static_cast<int>(tousLesPokemon.size())) {
            // On crée une copie du Pokémon choisi pour éviter de partager la même instance
            Pokemon* newPokemon = nullptr;
            
            // Utiliser le bon type de Pokémon en fonction du type principal
            const std::vector<std::string>& types = tousLesPokemon[choix - 1]->getTypes();
            if (!types.empty()) {
                if (types[0] == "Feu") {
                    newPokemon = new PokemonFeu(
                        tousLesPokemon[choix - 1]->getNom(),
                        tousLesPokemon[choix - 1]->getTypes(),
                        tousLesPokemon[choix - 1]->getHpMax(),
                        tousLesPokemon[choix - 1]->getNomAttaque(),
                        tousLesPokemon[choix - 1]->getDegatsAttaque(),
                        tousLesPokemon[choix - 1]->getNumPokedex()

                    );
                } else if (types[0] == "Eau") {
                    newPokemon = new PokemonEau(
                        tousLesPokemon[choix - 1]->getNom(),
                        tousLesPokemon[choix - 1]->getTypes(),
                        tousLesPokemon[choix - 1]->getHpMax(),
                        tousLesPokemon[choix - 1]->getNomAttaque(),
                        tousLesPokemon[choix - 1]->getDegatsAttaque(),
                        tousLesPokemon[choix - 1]->getNumPokedex()
                    );
                } else if (types[0] == "Plante") {
                    newPokemon = new PokemonPlante(
                        tousLesPokemon[choix - 1]->getNom(),
                        tousLesPokemon[choix - 1]->getTypes(),
                        tousLesPokemon[choix - 1]->getHpMax(),
                        tousLesPokemon[choix - 1]->getNomAttaque(),
                        tousLesPokemon[choix - 1]->getDegatsAttaque(),
                        tousLesPokemon[choix - 1]->getNumPokedex()
                    );
                } else {
                    newPokemon = new Pokemon(
                        tousLesPokemon[choix - 1]->getNom(),
                        tousLesPokemon[choix - 1]->getTypes(),
                        tousLesPokemon[choix - 1]->getHpMax(),
                        tousLesPokemon[choix - 1]->getNomAttaque(),
                        tousLesPokemon[choix - 1]->getDegatsAttaque(),
                        tousLesPokemon[choix - 1]->getNumPokedex()
                    );
                }
                print_pokemon(choix);
            }
            
            if (newPokemon) {
                joueur->ajouterPokemon(std::shared_ptr<Pokemon>(newPokemon));
                std::cout << tousLesPokemon[choix - 1]->getNom() << " a été ajouté à votre équipe!" << std::endl;
            }
        } else {
            std::cout << "Choix invalide. Réessayez." << std::endl;
            i--; // Recommencer ce tour de boucle
        }
    }
    
    // Nettoyer le buffer d'entrée
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Liste des entraîneurs vaincus
    std::vector<std::shared_ptr<Entraineur> > entraineursVaincus;
    
    // Lancer le menu principal
    menuPrincipal(joueur, leaders, maitres, entraineursVaincus);
    
    return 0;
}
