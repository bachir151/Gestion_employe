#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <string>

class Employe {
private:
    std::string nom;
    std::string prenom;
    std::string email;
    std::string numero;
    std::string file_path="../base_donnees/employe.txt";  // Chemin du fichier pour stocker les données

public:
    // Constructeurs
    Employe() = default;
    Employe(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& numero);

    // Méthodes
    void afficherInfo() const;
    void ajouterEmploye();
    void afficherListe() const;
    void modifierEmploye(const std::string& numeroRecherche, int choix, const std::string& newValue);
    bool rechercheClient(const std::string& numero) const;
    void supprimerEmploye(const std::string& numero);

private:
    // Méthodes utilitaires
    std::string trimString(const std::string& str) const;
    bool trouveEmploye(const std::string& ligne, const std::string& numero) const;
};

#endif // EMPLOYE_H