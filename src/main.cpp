#include "../include/Employe.h"
#include <iostream>
#include <string>
#include <chrono>

int main(){
 
	int choix;
	std::string nom;
	std::string prenom;
	std::string email;
	std::string num;
	char reponse;

	std::cout <<"- Ajouter un nouvel employé tapez 1 "<< std::endl;
	std::cout <<"- Consulter la liste des employés tapez 2 " << std::endl;
	std::cout <<"- Modifier une info client tapez 3 " << std::endl;
	std::cout <<"- Supprimer client tapez 4 " << std::endl;
	std::cout << "Entrez votre choix: "; std::cin >> choix; 

	//Ajout d'un client 
	if (choix==1){
		std::cout << "Saisir nom : "; std::cin >> nom;
		std::cout << "Saisir prenom : "; std::cin >> prenom;
		std::cout << "Saisir email : "; std::cin >> email;
		std::cout << "Saisir numéro téléphone : "; std::cin >> num;

		Employe employe(nom,prenom,email,num);
		employe.ajouterEmploye();
		std::cout <<"L'employé a été ajouté avec succès.." << std::endl;
	}

	//Consultation de la liste des clients
	if (choix==2){
		Employe employe;
		employe.afficherListe();
		std::cout <<"\nLa liste a été ajoutée avec succès"<<std::endl;
	}

 
    if (choix==3){
    	Employe employe;
    	int option;
    	std::string newValue;

        std::cout << "Quel est votre numéro de téléphone ? : "; std::cin >> num;
    	std::cout << "Si vous voulez modifier le nom tapez 1: " << std::endl;
    	std::cout << "Si vous voulez modifier le prenom tapez 2: " << std::endl;
    	std::cout << "Si vous voulez modifier l'email tapez 3: " << std::endl;
    	std::cout << "Si vous voulez modifier le numéro de téléphone tapez 4: "; std::cin>>option;

    	std::cout << "Entrez la nouvelle valeur : "; std::cin >> newValue;
        
        auto start = std::chrono::high_resolution_clock::now();

    	employe.modifierEmploye(num, option, newValue);

    	auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(end - start);
   		std::cout << "Temps d'exécution : " << duration.count() << " secondes" << std::endl;
					
    }

    //Suppression d'un client 
	if (choix==4){
		Employe employe;
		std::string numero;
		bool stop = false;

      while(stop==false){
			std::cout <<"Veuillez saisir le numéro de téléphone : "; std::cin >> numero;

			if (numero.length()!=10){

				std::cout << "Veuillez entrer un numéro valide " <<std::endl;
			} else {
					if (employe.rechercheClient(numero)==true){
						 employe.supprimerEmploye(numero);

					} else { 
						std::cout << "Client non trouvé dans la base de données " << std::endl;
					}
			}

			std::cout << "Voulez-vous encore modifier ? tapez O ou N : "; std::cin >> reponse;

			if (reponse =='n'){stop=true;}
		}

	}

	 
	return 0;
}