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

public :
	Employe()=default; 


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

	int choix;
	std::string nom;
	std::string prenom;
	std::string email;
	std::string num;

	//std::cout <<"Quest-ce que voulez faire ? " << std::endl;
	std::cout <<"- Ajouter un nouvel employé : tapez 1 "<< std::endl;
	std::cout <<"- Consulter la liste des employés : tapez 2 " << std::endl;
	std::cout << "Entrez votre choix:"; std::cin >> choix; 

	if (choix==1){
		std::cout << "Saisir nom : "; std::cin >> nom;
		std::cout << "Saisir prenom : "; std::cin >> prenom;
		std::cout << "Saisir email : "; std::cin >> email;
		std::cout << "Saisir numéro téléphone : "; std::cin >> num;

		Employe employe(nom,prenom,email,num);
		employe.ajouterEmploye();
		std::cout <<"L'employé a été ajouté avec succès.." << std::endl;
	}


	if (choix==2){
		Employe employe;
		employe.afficherListe();
		std::cout <<"\nLa liste a été ajoutée avec succès"<<std::endl;
	}

    

	return 0;
}