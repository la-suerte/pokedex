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
    virtual string interagir() const = 0;
    virtual ~Interagir() {}  // Virtual destructor with implementation
};

// Classe de base pour les Pokémon
class Pokemon : public Interagir {
protected:
    string nom;
    vector<string> types;
    int hp;
    int hpMax;
    string nomAttaque;
    int degatsAttaque;
    map<string, float> tableEfficacite;
    
    int numPokedex;

public:
    Pokemon(const string& nom, const vector<string>& types, 
            int hp, const string& nomAttaque, int degatsAttaque, int numPokedex)
        : nom(nom), types(types), hp(hp), hpMax(hp), nomAttaque(nomAttaque), degatsAttaque(degatsAttaque), numPokedex(numPokedex) {
        initTableEfficacite();
    }

    virtual ~Pokemon() {}  // Virtual destructor with implementation

    virtual void initTableEfficacite() {
        // Par défaut, tous les types ont une efficacité normale (x1)
        vector<string> allTypes;
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

    const string& getNom() const { return nom; }
    const vector<string>& getTypes() const { return types; }
    int getHp() const { return hp; }
    int getHpMax() const { return hpMax; }
    const string& getNomAttaque() const { return nomAttaque; }
    int getDegatsAttaque() const { return degatsAttaque; }
    int getNumPokedex() const { return numPokedex; }

    void setHp(int nouveauHp) { 
        hp = nouveauHp; 
        if (hp > hpMax) hp = hpMax;
        if (hp < 0) hp = 0;
    }

    void recupererVie() { hp = hpMax; }

    bool estVivant() const { return hp > 0; }

    virtual string getTypeAttaque() const {
        return !types.empty() ? types[0] : "Normal";
    }

    float getMultiplicateurEfficacite(const string& typeAttaque) const {
        float multiplicateur = 1.0f;
        
        for (size_t i = 0; i < types.size(); ++i) {
            const string& type = types[i];
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
        string typeAttaque = getTypeAttaque();
        float multiplicateur = cible.getMultiplicateurEfficacite(typeAttaque);
        int degatsFinaux = static_cast<int>(degatsAttaque * multiplicateur);
        cible.setHp(cible.getHp() - degatsFinaux);

        cout << nom << " utilise " << nomAttaque << " sur " << cible.getNom() << "! ";
        
        if (multiplicateur > 1.0f) {
            cout << "C'est super efficace! ";
        } else if (multiplicateur < 1.0f) {
            cout << "Ce n'est pas très efficace... ";
        }
        
        cout << "Il inflige " << degatsFinaux << " dégâts." << endl;
        cout << cible.getNom() << " a maintenant " << cible.getHp() << "/" << cible.getHpMax() << " HP." << endl;
        
        return degatsFinaux;
    }

    string afficherInfos() const {
        stringstream ss;
        ss << "Nom: " << nom << " | Types: ";
        for (size_t i = 0; i < types.size(); ++i) {
            ss << types[i];
            if (i < types.size() - 1) ss << ", ";
        }
        ss << " | Attaque: " << nomAttaque << " (" << degatsAttaque << " dégâts)";
        return ss.str();
    }

    virtual string interagir() const {
        return "Le Pokémon " + nom + " semble heureux de vous voir!";
    }
};

// Classes dérivées pour chaque type de Pokémon
class PokemonFeu : public Pokemon {
public:
    PokemonFeu(const string& nom, const vector<string>& types, 
               int hp, const string& nomAttaque, int degatsAttaque, int numPokedex)
        : Pokemon(nom, types, hp, nomAttaque, degatsAttaque, numPokedex) {}

    virtual string interagir() const {
        return "Le Pokémon " + nom + " crache quelques flammes joyeusement!";
    }
};

class PokemonEau : public Pokemon {
public:
    PokemonEau(const string& nom, const vector<string>& types, 
               int hp, const string& nomAttaque, int degatsAttaque, int numPokedex)
        : Pokemon(nom, types, hp, nomAttaque, degatsAttaque, numPokedex) {}

    virtual string interagir() const {
        return "Le Pokémon " + nom + " fait des bulles et vous éclabousse!";
    }
};

class PokemonPlante : public Pokemon {
public:
    PokemonPlante(const string& nom, const vector<string>& types, 
                 int hp, const string& nomAttaque, int degatsAttaque, int numPokedex)
        : Pokemon(nom, types, hp, nomAttaque, degatsAttaque, numPokedex) {}

    virtual string interagir() const {
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
    string nom;
    vector<shared_ptr<Pokemon> > equipe;
    bool vaincu;

public:
    Entraineur(const string& nom) : nom(nom), vaincu(false) {}
    virtual ~Entraineur() {}  // Virtual destructor with implementation

    const string& getNom() const { return nom; }
    const vector<shared_ptr<Pokemon> >& getEquipe() const { return equipe; }
    bool estVaincu() const { return vaincu; }
    void setVaincu(bool estVaincu) { vaincu = estVaincu; }

    void ajouterPokemon(shared_ptr<Pokemon> pokemon) {
        if (equipe.size() < 6) {
            equipe.push_back(pokemon);
        } else {
            cout << "L'équipe est déjà pleine (maximum 6 Pokémon)." << endl;
        }
    }

    void changerOrdrePokemon(int index1, int index2) {
        if (index1 >= 0 && index1 < static_cast<int>(equipe.size()) &&
            index2 >= 0 && index2 < static_cast<int>(equipe.size())) {
            swap(equipe[index1], equipe[index2]);
            cout << "L'ordre des Pokémon a été changé." << endl;
        } else {
            cout << "Indices invalides." << endl;
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

    shared_ptr<Pokemon> getPremierPokemonVivant() const {
        for (size_t i = 0; i < equipe.size(); ++i) {
            if (equipe[i]->estVivant()) {
                return equipe[i];
            }
        }
        return shared_ptr<Pokemon>();  // Return null pointer
    }
    

    void afficherEquipe() const {
        cout << "Équipe de " << nom << ":" << endl;
        for (size_t i = 0; i < equipe.size(); ++i) {
            cout << i + 1 << ". " << equipe[i]->afficherInfos() << endl;
            afficheHp(equipe[i]->getHp(),equipe[i]->getHpMax());
            print_pokemon(equipe[i]->getNumPokedex());
        }

    }

    void recupererTousLesPokemon() {
        for (size_t i = 0; i < equipe.size(); ++i) {
            equipe[i]->recupererVie();
        }
        cout << "Tous les Pokémon de " << nom << " ont récupéré leurs points de vie!" << endl;
    }

    virtual string interagir() const {
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
    Joueur(const string& nom) 
        : Entraineur(nom), badges(0), combatsGagnes(0), combatsPerdus(0) {}

    int getBadges() const { return badges; }
    int getCombatsGagnes() const { return combatsGagnes; }
    int getCombatsPerdus() const { return combatsPerdus; }

    void ajouterBadge() { badges++; }
    void incrementerCombatsGagnes() { combatsGagnes++; }
    void incrementerCombatsPerdus() { combatsPerdus++; }

    void afficherStatistiques() const {
        cout << "Statistiques de " << nom << ":" << endl;
        cout << "Badges: " << badges << endl;
        cout << "Combats gagnés: " << combatsGagnes << endl;
        cout << "Combats perdus: " << combatsPerdus << endl;
    }

    virtual string interagir() const {
        return nom + " dit: \"Je m'entraîne dur pour devenir le meilleur dresseur!\"";
    }
};

// Classe Leader de Gymnase
class LeaderGym : public Entraineur {
private:
    string badge;
    string gymnase;

public:
    LeaderGym(const string& n, const string& g)
        : Entraineur(n), gymnase(g), badge("Badge de  " + g) {}


    const string& getBadge() const { return badge; }
    const string& getGymnase() const { return gymnase; }

    virtual string interagir() const {
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
    MaitrePokemon(const string& nom) : Entraineur(nom) {}

    int attaquer(Pokemon& cible, Pokemon& attaquant) {
        float bonusMaitre = 1.25f; // Bonus de 25% pour les Maîtres Pokémon
        string typeAttaque = attaquant.getTypeAttaque();
        float multiplicateur = cible.getMultiplicateurEfficacite(typeAttaque);
        int degatsFinaux = static_cast<int>(attaquant.getDegatsAttaque() * multiplicateur * bonusMaitre);
        cible.setHp(cible.getHp() - degatsFinaux);

        cout << attaquant.getNom() << " du Maître " << nom << " utilise " << attaquant.getNomAttaque() 
                  << " avec un bonus de puissance! ";
        
        if (multiplicateur > 1.0f) {
            cout << "C'est super efficace! ";
        } else if (multiplicateur < 1.0f) {
            cout << "Ce n'est pas très efficace... ";
        }
        
        cout << "Il inflige " << degatsFinaux << " dégâts." << endl;
        cout << cible.getNom() << " a maintenant " << cible.getHp() << "/" << cible.getHpMax() << " HP." << endl;
        
        return degatsFinaux;
    }

    virtual string interagir() const {
        if (vaincu) {
            return nom + " dit: \"Impressionnant! Tu es vraiment un dresseur exceptionnel.\"";
        } else {
            return nom + " dit: \"Je suis " + nom + ", un Maître Pokémon. Montre-moi ta force!\"";
        }
    }
};

// Remplacer random_device et distributions par alternatives pre-C++11
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
vector<shared_ptr<Pokemon> > chargerPokemonDepuisCSV(const string& filename) {
    vector<shared_ptr<Pokemon> > pokemons;
    ifstream file(filename.c_str());  // C-style string for older compilers
    
    if (!file.is_open()) {
        cout << "Impossible d'ouvrir le fichier " << filename << endl;
        // Créer quelques Pokémon par défaut
        vector<string> typeFeu;
        typeFeu.push_back("Feu");
        pokemons.push_back(make_shared<PokemonFeu>("Salamèche", typeFeu, 39, "Flammèche", 70,2));
        
        vector<string> typeEau;
        typeEau.push_back("Eau");
        pokemons.push_back(make_shared<PokemonEau>("Carapuce", typeEau, 44, "Pistolet à O", 65,6));
        
        vector<string> typePlante;
        typePlante.push_back("Plante");
        typePlante.push_back("Poison");
        pokemons.push_back(make_shared<PokemonPlante>("Bulbizarre", typePlante, 45, "Fouet Lianes", 60,7));
        return pokemons;
    }
    
    string line;
    // Ignorer la première ligne (en-têtes)
    getline(file, line);

    int count = 1;

    while (getline(file, line)) {
        stringstream ss(line);
        string nom, typesStr, type2, hpStr, nomAttaque, degatsAttaqueStr, numPokedex;
        
        getline(ss, nom, ',');
        getline(ss, typesStr, ',');
        getline(ss, type2, ',');
        getline(ss, hpStr, ',');
        getline(ss, nomAttaque, ',');
        getline(ss, degatsAttaqueStr, ',');
        int numPokedexInt = count;
        
        // Convertir les types en vector
        vector<string> types;
        stringstream typesStream(typesStr);
        string type;
        while (getline(typesStream, type, '|')) {
            types.push_back(type);
        }
        
        int hp = atoi(hpStr.c_str());
        int degatsAttaque = atoi(degatsAttaqueStr.c_str());
        
        // Créer le Pokémon approprié basé sur son type principal
        if (!types.empty()) {
            if (types[0] == "Feu") {
                pokemons.push_back(make_shared<PokemonFeu>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            } else if (types[0] == "Eau") {
                pokemons.push_back(make_shared<PokemonEau>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            } else if (types[0] == "Plante") {
                pokemons.push_back(make_shared<PokemonPlante>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            } else {
                // Type générique
                pokemons.push_back(make_shared<Pokemon>(nom, types, hp, nomAttaque, degatsAttaque, numPokedexInt));
            }
            count+=1;
        }
    }
    
    file.close();
    return pokemons;
}
    /*
    if (!file.is_open()) {
        cout << "Impossible d'ouvrir le fichier " << filename << endl;
        // Créer quelques leaders par défaut
        shared_ptr<LeaderGym> leader1(new LeaderGym("Pierre", "Roche", "Argenta"));
        vector<string> typeRoche;
        typeRoche.push_back("Roche");
        leader1->ajouterPokemon(make_shared<Pokemon>("Racaillou", typeRoche, 40, "Jet-Pierres", 60));
        
        shared_ptr<LeaderGym> leader2(new LeaderGym("Ondine", "Cascade", "Azuria"));
        vector<string> typeEauPsy;
        typeEauPsy.push_back("Eau");
        typeEauPsy.push_back("Psy");
        leader2->ajouterPokemon(make_shared<PokemonEau>("Staross", typeEauPsy, 45, "Hydrocanon", 75));
        
        shared_ptr<LeaderGym> leader3(new LeaderGym("Major Bob", "Tonnerre", "Carmin sur Mer"));
        vector<string> typeElectrik;
        typeElectrik.push_back("Électrik");
        leader3->ajouterPokemon(make_shared<Pokemon>("Pikachu", typeElectrik, 35, "Éclair", 65));
        
        shared_ptr<LeaderGym> leader4(new LeaderGym("Erika", "Arc-en-ciel", "Céladopole"));
        vector<string> typePlante;
        typePlante.push_back("Plante");
        typePlante.push_back("Poison");
        leader4->ajouterPokemon(make_shared<PokemonPlante>("Herbizarre", typePlante, 50, "Tranch'Herbe", 70));
        
        leaders.push_back(leader1);
        leaders.push_back(leader2);
        leaders.push_back(leader3);
        leaders.push_back(leader4);
        return leaders;
    }*/


shared_ptr<Joueur>  chargerJoueurDepuisCSV(const string& filename, const vector<shared_ptr<Pokemon> >& tousLesPokemon) {
    shared_ptr<Joueur> joueur;
    ifstream file(filename.c_str());
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string nom, pokemonNom;
        
        getline(ss, nom, ',');
        
        joueur = shared_ptr<Joueur>(new Joueur(nom));
        
        while (getline(ss, pokemonNom, ',')) {
            if (pokemonNom.empty()) {
                continue;
            }
            
            for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
                if (tousLesPokemon[i]->getNom() == pokemonNom) {
                    // Créer une copie du Pokémon et l'ajouter à l'équipe du joueur
                    Pokemon* newPokemon = new Pokemon(*tousLesPokemon[i]);
                    joueur->ajouterPokemon(shared_ptr<Pokemon>(newPokemon));
                    break;
                }
            }
        }
        
    }
    
    file.close();
    return joueur;
}

vector<shared_ptr<LeaderGym> > chargerLeadersDepuisCSV(const string& filename, const vector<shared_ptr<Pokemon> >& tousLesPokemon) {
    vector<shared_ptr<LeaderGym> > leaders;
    ifstream file(filename.c_str());
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string nom, gymnase, pokemonNom;
        
        getline(ss, nom, ',');
        getline(ss, gymnase, ',');
        
        shared_ptr<LeaderGym> leader(new LeaderGym(nom, gymnase));
        
        while (getline(ss, pokemonNom, ',')) {
            if (pokemonNom.empty()) {
                continue;
            }
            
            for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
                if (tousLesPokemon[i]->getNom() == pokemonNom) {
                    // Créer une copie du Pokémon et l'ajouter à l'équipe du leader
                    Pokemon* newPokemon = new Pokemon(*tousLesPokemon[i]);
                    leader->ajouterPokemon(shared_ptr<Pokemon>(newPokemon));
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
vector<shared_ptr<MaitrePokemon> > chargerMaitresDepuisCSV(const string& filename, const vector<shared_ptr<Pokemon> >& tousLesPokemon) {
    vector<shared_ptr<MaitrePokemon> > maitres;
    ifstream file(filename.c_str());
    string line;
    // Ignorer la première ligne (en-têtes)
    getline(file, line);
    
    while (getline(file, line)) {
        stringstream ss(line);
        string nom, pokemonsStr;
        
        getline(ss, nom,',');
        getline(ss, pokemonsStr, ',');
        
        shared_ptr<MaitrePokemon> maitre(new MaitrePokemon(nom));
        
        // Ajouter les Pokémon au maître
        stringstream pokemonsStream(pokemonsStr);
        string pokemonNom;

        while (getline(ss, pokemonNom, ',')) {
            if (pokemonNom.empty()) {
                continue;
            }
            
            for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
                if (tousLesPokemon[i]->getNom() == pokemonNom) {
                    // Créer une copie du Pokémon et l'ajouter à l'équipe du leader
                    Pokemon* newPokemon = new Pokemon(*tousLesPokemon[i]);
                    maitre->ajouterPokemon(shared_ptr<Pokemon>(newPokemon));
                    break;
                }
            }
        }
        
        maitres.push_back(maitre);
    }
    
    file.close();
    return maitres;
}

void debugAfficherEquipeHP(const shared_ptr<Entraineur>& entraineur) {
    cout << "\n----- DEBUG: ÉTAT DE L'ÉQUIPE DE " << entraineur->getNom() << " -----" << endl;
    const vector<shared_ptr<Pokemon> >& equipe = entraineur->getEquipe();
    
    // Afficher chaque Pokémon et son état
    for (size_t i = 0; i < equipe.size(); ++i) {
        cout << i + 1 << ". " << equipe[i]->getNom() 
                  << " (HP: " << equipe[i]->getHp() << "/" << equipe[i]->getHpMax() 
                  << ") - " << (equipe[i]->estVivant() ? "VIVANT" : "KO") << endl;
    }
    
    // Afficher le résultat de tousLesPokemonKO()
    cout << "tousLesPokemonKO() retourne: " 
              << (entraineur->tousLesPokemonKO() ? "true (tous KO)" : "false (certains vivants)") << endl;
    cout << "----------------------------------------" << endl;
}

void combat(shared_ptr<Joueur> joueur, shared_ptr<Entraineur> adversaire) 
{
    cout << "\n----- DÉBUT DU COMBAT -----" << endl;
    cout << joueur->getNom() << " VS " << adversaire->getNom() << endl;
    
    // Vérifier si les deux dresseurs ont des Pokémon
    if (joueur->getEquipe().empty() || adversaire->getEquipe().empty()) {
        cout << "Un des dresseurs n'a pas de Pokémon!" << endl;
        return;
    }
    //debugAfficherEquipeHP(adversaire);

    string inutile; //HERE

    // Déclarer pokemonJoueur et pokemonAdversaire
    shared_ptr<Pokemon> pokemonJoueur = joueur->getPremierPokemonVivant();
    shared_ptr<Pokemon> pokemonAdversaire = adversaire->getPremierPokemonVivant();
    
    // Combat au tour par tour
    while (!joueur->tousLesPokemonKO() && !adversaire->tousLesPokemonKO()) 
    {
        
        cout << "\n";
        cout << " #######                                                                                   \n";
        cout << "    #     ####  #    # #####     #####  ######     ####   ####  #    # #####    ##   ##### \n";
        cout << "    #    #    # #    # #    #    #    # #         #    # #    # ##  ## #    #  #  #    #   \n";
        cout << "    #    #    # #    # #    #    #    # #####     #      #    # # ## # #####  #    #   #   \n";
        cout << "    #    #    # #    # #####     #    # #         #      #    # #    # #    # ######   #   \n";
        cout << "    #    #    # #    # #   #     #    # #         #    # #    # #    # #    # #    #   #   \n";
        cout << "    #     ####   ####  #    #    #####  ######     ####   ####  #    # #####  #    #   #   \n\n";        
           
        
        cout << pokemonJoueur->getNom()<<endl;
        afficheHp(pokemonJoueur->getHp(),pokemonJoueur->getHpMax());
        print_pokemon(pokemonJoueur->getNumPokedex());


        cout << "\n\n #     #  #####  \n";
        cout << " #     # #     # \n";
        cout << " #     # #       \n";
        cout << " #     #  #####  \n";
        cout << "  #   #        # \n";
        cout << "   # #   #     # \n";
        cout << "    #     #####  \n";
        cout << "                 \n";
        cout << pokemonAdversaire->getNom()<<endl;
        afficheHp(pokemonAdversaire->getHp(),pokemonAdversaire->getHpMax());
        print_pokemon(pokemonAdversaire->getNumPokedex());


        // Le joueur attaque en premier
        pokemonJoueur->attaquer(*pokemonAdversaire);
        
        //debugAfficherEquipeHP(adversaire);
        // Vérifier si le Pokémon adversaire est KO
        if (!pokemonAdversaire->estVivant()) {
            cout << pokemonAdversaire->getNom() << " est KO!" << endl;
            
            // Vérifier si l'adversaire a encore des Pokémon vivants
            if (!adversaire->tousLesPokemonKO()) {
                // Obtenir le prochain Pokémon vivant
                pokemonAdversaire = adversaire->getPremierPokemonVivant();
                cout << adversaire->getNom() << " envoie " << pokemonAdversaire->getNom() << "!" << endl;
            } else {
                // Si tous les Pokémon de l'adversaire sont KO, terminer le tour
                break;
            }
        }
        cout << "\n PRESS N + ENTER TO CONTINUE \n";
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
                cout << pokemonJoueur->getNom() << " est KO!" << endl;
                
                // Vérifier si le joueur a encore des Pokémon vivants
                if (!joueur->tousLesPokemonKO()) {
                    // Obtenir le prochain Pokémon vivant
                    pokemonJoueur = joueur->getPremierPokemonVivant();
                    cout << joueur->getNom() << " envoie " << pokemonJoueur->getNom() << "!" << endl;
                }
            }
        }
        cout << "\n PRESS N + ENTER TO CONTINUE \n";
        cin>>inutile; // Waits for Enter after a key press
    }
    
    // Déterminer le vainqueur
    bool joueurGagnant = !joueur->tousLesPokemonKO();
    
    cout << "\n----- FIN DU COMBAT -----" << endl;
    if (joueurGagnant) {
        cout << joueur->getNom() << " a gagné le combat!" << endl;
        joueur->incrementerCombatsGagnes();
        
        // Si l'adversaire est un Leader de Gym, le joueur gagne un badge
        LeaderGym* leader = dynamic_cast<LeaderGym*>(adversaire.get());
        if (leader) {
            cout << leader->getNom() << " dit: \"Félicitations! Tu as gagné le badge " << leader->getBadge() << "!\"" << endl;
            joueur->ajouterBadge();
            leader->setVaincu(true);
        }
        
        // Si l'adversaire est un Maître Pokémon, marquer comme vaincu
        MaitrePokemon* maitre = dynamic_cast<MaitrePokemon*>(adversaire.get());
        if (maitre) {
            maitre->setVaincu(true);
        }
    } else {
        cout << adversaire->getNom() << " a gagné le combat!" << endl;
        joueur->incrementerCombatsPerdus();
    }
    cout << "\n PRESS N + ENTER TO CONTINUE \n";
    cin>>inutile; // Waits for Enter after a key press

}
// Fonction qui gère le menu principal
void menuPrincipal(shared_ptr<Joueur> joueur, 
                  vector<shared_ptr<LeaderGym> > leaders,
                  vector<shared_ptr<MaitrePokemon> > maitres,
                  vector<shared_ptr<Entraineur> > entraineursVaincus) 
{
    
    bool quitter = false;
    
    while (!quitter) {
        system("clear");
        cout << "\n===== MENU PRINCIPAL =====" << endl;
        cout << "1. Afficher mes Pokémon" << endl;
        cout << "2. Récupérer les points de vie de mes Pokémon" << endl;
        cout << "3. Changer l'ordre de mes Pokémon" << endl;
        cout << "4. Afficher mes statistiques" << endl;
        cout << "5. Affronter un leader de gymnase" << endl;
        cout << "6. Affronter un Maître Pokémon" << endl;
        cout << "7. Interagir avec mes Pokémon" << endl;
        cout << "8. Interagir avec les entraîneurs vaincus" << endl;
        cout << "9. Quitter" << endl;
        cout << "Votre choix: ";
        
        int choix;
        cin >> choix;
        
        // Nettoyer le buffer d'entrée
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        //system("clear");
        switch (choix) {
            case 1: {
                // Afficher les Pokémon du joueur
                joueur->afficherEquipe();

                break;
            }
            case 2: {
                // Récupérer les points de vie des Pokémon
                joueur->recupererTousLesPokemon();
                cout<<"Point de vie recuperés"<<endl;
                break;
            }
            case 3: {
                // Changer l'ordre des Pokémon
                joueur->afficherEquipe();
                cout << "Entrez l'indice du premier Pokémon à échanger (1-" << joueur->getEquipe().size() << "): ";
                int index1;
                cin >> index1;
                
                cout << "Entrez l'indice du second Pokémon à échanger (1-" << joueur->getEquipe().size() << "): ";
                int index2;
                cin >> index2;
                joueur->changerOrdrePokemon(index1 - 1, index2 - 1);
                break;
            }
            case 4: {
                // Afficher les statistiques du joueur
                joueur->afficherStatistiques();
                break;
            }
            case 5: {
                // Affronter un leader de gymnase
                cout << "\n=== LEADERS DE GYMNASE DISPONIBLES ===" << endl;
                for (size_t i = 0; i < leaders.size(); ++i) {
                    cout << i + 1 << ". " << leaders[i]->getNom() << " (" << leaders[i]->getGymnase() << ")";
                    if (leaders[i]->estVaincu()) {
                        cout << " [DÉJÀ VAINCU]";
                    }
                    cout << endl;
                }
                
                cout << "Choisissez un leader à affronter (0 pour revenir): ";
                int choixLeader = 1;
                cin >> choixLeader; 
                
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
                //if (joueur->getBadges() >= 4) {  // Minimum 4 badges requis HERE
                {
                    // Choisir un maître au hasard
                    if (!maitres.empty()) {
                        int indexMaitre = getRandomNumber(0, maitres.size() - 1);
                        cout << "Vous allez affronter le Maître Pokémon " << maitres[indexMaitre]->getNom() << "!" << endl;
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
                        cout << "Aucun Maître Pokémon disponible!" << endl;
                    }
                //} else { HERE
                //    cout << "Vous devez obtenir au moins 4 badges avant de pouvoir affronter un Maître Pokémon!" << endl;
                }
                break;
            }

            case 7: {
                // Interagir avec les Pokémon du joueur
                joueur->afficherEquipe();
                cout << "Avec quel Pokémon voulez-vous interagir? (1-" << joueur->getEquipe().size() << ", 0 pour annuler): ";
                int choixPokemon;
                cin >> choixPokemon;
                
                if (choixPokemon > 0 && choixPokemon <= static_cast<int>(joueur->getEquipe().size())) {
                    //system("clear");
                    print_pokemon(joueur->getEquipe()[choixPokemon - 1]->getNumPokedex());
                    cout << joueur->getEquipe()[choixPokemon - 1]->interagir() << endl;
                }
                break;
            }
            case 8: {
                // Interagir avec les entraîneurs vaincus
                if (entraineursVaincus.empty()) {
                    cout << "Vous n'avez pas encore vaincu d'entraîneur!" << endl;
                } else {
                    cout << "\n=== ENTRAÎNEURS VAINCUS ===" << endl;
                    for (size_t i = 0; i < entraineursVaincus.size(); ++i) {
                        cout << i + 1 << ". " << entraineursVaincus[i]->getNom() << endl;
                    }
                    
                    cout << "Avec quel entraîneur voulez-vous interagir? (1-" << entraineursVaincus.size() << ", 0 pour annuler): ";
                    int choixEntraineur;
                    cin >> choixEntraineur;
                    
                    if (choixEntraineur > 0 && choixEntraineur <= static_cast<int>(entraineursVaincus.size())) {
                        cout << entraineursVaincus[choixEntraineur - 1]->interagir() << endl;
                    }

                }
                break;
            }
            case 9: {
                // Quitter le jeu
                cout << "Merci d'avoir joué! À bientôt!" << endl;
                quitter = true;
                break;
            }
            case 10:
                system("clear");
                break;
            default: {
                cout << "Choix invalide. Veuillez réessayer." << endl;
                break;
            }
        }
        cout<<"Appuyer sur une Q + ENTER pour continuer : ";
        string p;
        cin>>p;
    }
}

shared_ptr<Joueur> CreateJoueur(string filename, vector<shared_ptr<Pokemon> > tousLesPokemon)
{
    shared_ptr<Joueur> joueur;  // Déclaré ici pour être visible à la fin

    string rep;
    do {
        cout << "Voulez vous jouer vous meme (M) ou jouer avec un joueur pre-cree (P): ";
        cin >> rep;
    } while (rep != "M" && rep != "P");

    if (rep == "P") {
        joueur = chargerJoueurDepuisCSV(filename, tousLesPokemon);
    } else {
        string nomJoueur;
        cin.ignore(); // Important pour ignorer le '\n' après cin >> rep
        cout << "Entrez votre nom: ";
        getline(cin, nomJoueur);

        joueur = shared_ptr<Joueur>(new Joueur(nomJoueur));

        cout << "\nChoisissez vos Pokémon de départ:" << endl;
        for (size_t i = 0; i < tousLesPokemon.size(); ++i) {
            cout << i + 1 << ". " << tousLesPokemon[i]->afficherInfos() << endl;
        }

        for (int i = 0; i < 3; ++i) {
            cout << "\nChoisissez votre Pokémon #" << i + 1 << " (1-" << tousLesPokemon.size() << "): ";
            int choix;
            cin >> choix;

            if (choix > 0 && choix <= (int)tousLesPokemon.size()) {
                Pokemon* selected = tousLesPokemon[choix - 1].get();
                const vector<string>& types = selected->getTypes();
                Pokemon* newPokemon = NULL;

                if (!types.empty()) {
                    if (types[0] == "Feu") {
                        newPokemon = new PokemonFeu(
                            selected->getNom(),
                            selected->getTypes(),
                            selected->getHpMax(),
                            selected->getNomAttaque(),
                            selected->getDegatsAttaque(),
                            selected->getNumPokedex()
                        );
                    } else if (types[0] == "Eau") {
                        newPokemon = new PokemonEau(
                            selected->getNom(),
                            selected->getTypes(),
                            selected->getHpMax(),
                            selected->getNomAttaque(),
                            selected->getDegatsAttaque(),
                            selected->getNumPokedex()
                        );
                    } else if (types[0] == "Plante") {
                        newPokemon = new PokemonPlante(
                            selected->getNom(),
                            selected->getTypes(),
                            selected->getHpMax(),
                            selected->getNomAttaque(),
                            selected->getDegatsAttaque(),
                            selected->getNumPokedex()
                        );
                    } else {
                        newPokemon = new Pokemon(
                            selected->getNom(),
                            selected->getTypes(),
                            selected->getHpMax(),
                            selected->getNomAttaque(),
                            selected->getDegatsAttaque(),
                            selected->getNumPokedex()
                        );
                    }
                }

                if (newPokemon != NULL) {
                    joueur->ajouterPokemon(shared_ptr<Pokemon>(newPokemon));
                    cout << selected->getNom() << " a été ajouté à votre équipe!" << endl;
                }
            } else {
                cout << "Choix invalide. Réessayez." << endl;
                i--;
            }
        }
    }

    return joueur;
}

int main() {
    system("clear");
    cout << "=== SIMULATEUR DE COMBAT POKÉMON ===" << endl;
    
    vector<shared_ptr<Pokemon> > tousLesPokemon = chargerPokemonDepuisCSV("pokemon.csv");
    vector<shared_ptr<LeaderGym> > leaders = chargerLeadersDepuisCSV("leaders.csv", tousLesPokemon);
    vector<shared_ptr<MaitrePokemon> > maitres = chargerMaitresDepuisCSV("maitres.csv", tousLesPokemon);
    shared_ptr<Joueur> joueur = CreateJoueur("joueur.csv", tousLesPokemon);
    // Nettoyer le buffer d'entrée
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    // Liste des entraîneurs vaincus
    vector<shared_ptr<Entraineur> > entraineursVaincus;
    // Lancer le menu principal
    menuPrincipal(joueur, leaders, maitres, entraineursVaincus);
    
    return 0;
}
