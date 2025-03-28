#include <iostream>
#include <string>
#include <fstream>


class Employe {
public :
	std::string nom;
	std::string prenom;
	std::string email;
	std::string num;

public :
	Employe(std::string nom, std::string prenom, std::string email, std::string num){
		this->nom = nom;
		this->prenom = prenom;
		this->email = email;
		this->num= num;
		
	}


void afficherInfo(){
	std::cout << "Nom: " << nom << std::endl;
	std::cout << "prenom: " << prenom << std::endl;
	std::cout << "email: " << email << std::endl;
	std::cout << "numero de tel:  : " << num <<std::endl;
}

//1-Fonction qui permet d'ajouter un employé
void ajouterEmploye(){
    std::ofstream fichier;
    std::string lien ="employe.txt";
    
    //Ouverture du fichier 
    fichier.open(lien, std::ios::app);

    if (!fichier){
    	std::cerr << "Le fichier n'existe pas" << std::endl;
    } 

    	if(fichier.is_open()){
    	//écriture sur le fichier
    	fichier << nom << " ; " << prenom << " ; " << email << " ; " << num <<std::endl ;
    	fichier.close();
    	}       	
}

//2-Afficher la liste des employés
void afficherListe(){
	std::ifstream fichier;
	std::string lien ="employe.txt";
	std::string ligne;

	//ouverture du fichier
	fichier.open(lien);

	if(!fichier){
		std::cerr << "Le fichier n'existe pas" << std::endl; 
	}

	if(fichier.is_open()){
		while(getline(fichier,ligne)){
			//lecture ligne par ligne du fichier
			std::cout << ligne << std::endl;
		}
	}
	fichier.close();
}

//3-Modifier un employé
/*
   ◦ Permettre la modification d'un employé en le sélectionnant dans la liste.
   ◦ Un formulaire pré-rempli avec les informations de l’employé doit s'afficher.
   ◦ Après modification, les informations doivent être mises à jour dans le fichier texte.
*/

//4-Supprimer un employé

//5-Rechercher un employé 


}; //fin classe





int main(){
    //Instanciation
	Employe employe1("Diedhiou","Ahmed","albachirou@gmail.com", "0612219010");

	employe1.afficherInfo();
	//employe1.ajouterEmploye();

	std::cout << ".......Liste des client ........."<<std::endl;
	employe1.afficherListe();

	return 0;
}