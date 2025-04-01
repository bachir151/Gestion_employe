#include "../include/Employe.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

// Constructeur avec paramètres
Employe::Employe(const std::string& nom, const std::string& prenom, const std::string& email, const std::string& numero)
    : nom(nom), prenom(prenom), email(email), numero(numero), file_path("../base_donnees/employe.txt") {}

// Afficher les informations d'un employé
void Employe::afficherInfo() const {
    std::cout << "Nom : " << nom << "\nPrenom: " << prenom << "\nEmail: " << email << "\nNumero: " << numero << '\n';
}

// 1- Ajouter un employé
void Employe::ajouterEmploye() {
    std::ofstream fichier(file_path, std::ios::app); // Mode append
    if (!fichier) {
        std::cout << "Impossible d'ouvrir le fichier\n";
        return;
    }
    fichier << nom << ";" << prenom << ";" << email << ";" << numero << "\n";
    fichier.close();
    std::cout << "Employé ajouté avec succès.\n";
}

// 2- Afficher la liste des employés
void Employe::afficherListe() const {
    std::ifstream fichier(file_path);
    std::string ligne;

    if (!fichier) {
        std::cerr << "Impossible d'ouvrir le fichier\n";
        return;
    }
    while (std::getline(fichier, ligne)) {
        std::cout << ligne << "\n";
    }
    fichier.close();
}

// 3- Fonction utilitaire pour supprimer les espaces
std::string Employe::trimString(const std::string& str) const {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// 4- Fonction pour vérifier si une ligne contient le numéro d'un employé
bool Employe::trouveEmploye(const std::string& ligne, const std::string& numero) const {
    return ligne.find(numero) != std::string::npos;
}

// 5- Modifier les infos d'un employé
void Employe::modifierEmploye(const std::string& numeroRecherche, int choix, const std::string& newValue) {
    std::vector<std::string> listeEmploye;
    {
        std::ifstream fichier_lecture(file_path);
        if (!fichier_lecture) {
            std::cout << "Le fichier n'existe pas\n";
            return;
        }

        std::string ligne;
        while (std::getline(fichier_lecture, ligne)) {
            if (trouveEmploye(ligne, numeroRecherche)) {
                std::cout << "Employé trouvé --> " << ligne << "\n";

                std::string nom, prenom, email, num;
                std::stringstream flux(ligne);

                std::getline(flux, nom, ';');
                std::getline(flux, prenom, ';');
                std::getline(flux, email, ';');
                std::getline(flux, num, ';');

                // Trim les espaces
                nom = trimString(nom);
                prenom = trimString(prenom);
                email = trimString(email);
                num = trimString(num);

                // Choix du champ à modifier
                switch (choix) {
                    case 1: nom = newValue; break;
                    case 2: prenom = newValue; break;
                    case 3: email = newValue; break;
                    case 4: num = newValue; break;
                    default: std::cout << "Choix invalide, aucune modification\n";
                    continue;
                }

                ligne = nom + ";" + prenom + ";" + email + ";" + num;
                std::cout << "Nouvel employé --> " << ligne << "\n";
            }
            listeEmploye.push_back(ligne);
        }
        fichier_lecture.close();
    }

    std::ofstream fichier_ecriture(file_path);
    if (!fichier_ecriture) {
        std::cout << "Erreur : impossible d'ouvrir le fichier\n";
        return;
    }

    for (const auto& element : listeEmploye) {
        fichier_ecriture << element << '\n';
    }
    fichier_ecriture.close();
    std::cout << "Modification réussie\n";
}

// 6- Rechercher un employé
bool Employe::rechercheClient(const std::string& numero) const {
    std::ifstream fichier(file_path);
    if (!fichier) {
        std::cout << "Le fichier n'existe pas\n";
        return false;
    }
    std::string ligne;
    while (std::getline(fichier, ligne)) {
        if (trouveEmploye(ligne, numero)) {
            fichier.close();
            return true;
        }
    }
    fichier.close();
    return false;
}

// 7- Supprimer un employé
void Employe::supprimerEmploye(const std::string& numero) {
    std::vector<std::string> listeEmploye;
    {
        std::ifstream fichier_lecture(file_path);
        if (!fichier_lecture) {
            std::cerr << "Ce fichier n'existe pas\n";
            return;
        }
        std::string ligne;
        while (std::getline(fichier_lecture, ligne)) {
            if (!trouveEmploye(ligne, numero)) {
                listeEmploye.push_back(ligne);
            }
        }
        fichier_lecture.close();
    }

    std::ofstream fichier_ecriture(file_path);
    if (!fichier_ecriture) {
        std::cerr << "Ce fichier n'existe pas\n";
        return;
    }
    for (const auto& element : listeEmploye) {
        fichier_ecriture << element << '\n';
    }
    fichier_ecriture.close();
    std::cout << "Employé supprimé avec succès.\n";
}