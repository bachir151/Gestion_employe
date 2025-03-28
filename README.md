# Gestion des Employés en C++ avec Stockage dans un Fichier Texte

## Objectif
Développer une application en C++ permettant de gérer une liste d'employés. Les informations des employés seront stockées dans un fichier texte (`employes.txt`). L'application devra permettre d'effectuer des opérations CRUD (Créer, Lire, Mettre à jour, Supprimer) sur les employés.

## Fonctionnalités
### 1. Ajouter un employé
- Un formulaire permettant d'ajouter un nouvel employé avec les champs suivants :
  - Nom
  - Prénom
  - Email
  - Numéro de téléphone
- Lors de la soumission, les informations doivent être ajoutées à la fin du fichier texte.

### 2. Afficher la liste des employés
- Récupérer les informations depuis le fichier texte et les afficher sous la forme :
  - Nom
  - Prénom
  - Email
  - Numéro de téléphone

### 3. Modifier un employé
- Permettre la modification d'un employé en le sélectionnant dans la liste.
- Un formulaire pré-rempli avec les informations de l'employé doit s'afficher.
- Après modification, les informations doivent être mises à jour dans le fichier texte.

### 4. Supprimer un employé
- Ajouter une option permettant de supprimer un employé par son index.
- L'employé doit être retiré du fichier texte après confirmation.

### 5. Rechercher un employé
- Une fonctionnalité permettant de filtrer les employés par nom ou email.

## Exigences techniques
- Stockage des données dans un fichier texte (`employes.txt`).
- Chaque ligne représente un employé, avec les champs séparés par un caractère `;`.
- Utilisation de C++ natif sans bibliothèque externe.
- Mise à jour du fichier texte sans perte de données.

## Livrables
- Code source de l'application C++.
- Fichier texte (`employes.txt`) contenant les employés.

## Critères d'évaluation
- Fonctionnalité complète et correcte des opérations CRUD.
- Qualité du code (lisibilité, commentaires, bonnes pratiques en C++).
- Interface utilisateur simple et fonctionnelle.
- Gestion des erreurs et validation des données.

## Installation et exécution
1. Compiler le programme avec g++ :
   ```sh
   g++ -o gestion_employes main.cpp
   ```
2. Exécuter l'application :
   ```sh
   ./gestion_employes
   ```

## Auteur
[Votre Nom]

