#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;


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

void modifier(std::string numero, std::string oldValue, std::string newValue){
	std::vector<std::string> listeEmploye;
	std::ifstream fichier_lecture;
	std::ofstream fichier_ecriture;
	std::string lien ="employe.txt";
	std::string ligne;
	bool trouve=false;
    
    //Lecture du fichier_lecture - on peut lire dans le fichier_lecture et écrire dedans
	fichier_lecture.open(lien);

	if(!fichier_lecture){
		std::cerr << "Le fichier n'existe pas"<< std::endl;
	}
    
	if (fichier_lecture.is_open()){
		while(getline(fichier_lecture,ligne)){
            
            //Recherche de l'employé dans la liste en cherchant son numéro de tel sur chaque ligne
			if (trouveEmploye(ligne,numero)==true){
				//std::cout << "Client trouvé ---> " << ligne << std::endl;
				
				 //Crée une référence ligne
				std::string& ligneRef = ligne;

				//modification de la ligne
				modifierEmploye(ligneRef,oldValue, newValue);

			   //Ajout de chaque ligne du client modifié dans le tableau
			   listeEmploye.push_back(ligneRef);
			   //
			   trouve = true;
				
			}
			else{
			   //Ajout des autres clients dans le fichier
			   listeEmploye.push_back(ligne);
			}
		}
		
     fichier_lecture.close();
	}


	if (trouve == false){
		 std::cout << "Client introuvable dans la base de données"<<std::endl;
	} else {
     
	    //Mise à jour de la base de données 
		fichier_ecriture.open(lien);

		if(!fichier_ecriture){
			std::cerr << "Le fichier n'existe pas"<< std::endl; 
		}

		if (!fichier_lecture.is_open()){
			for (const auto& element:listeEmploye){
			fichier_ecriture << element <<'\n'; 
			}
			fichier_ecriture.close();
		}

   }
	//listeEmploye.clear();
	//return listeEmploye;
}

//4- Suppression employé
void supprimerEmploye(std::string numero){
	std::ifstream fichier_lecture;
	std::ofstream fichier_ecriture;
	std::string lien ="employe.txt";
	std::vector<std::string> listeEmploye;
	std::string ligne;

	fichier_lecture.open(lien);

	if (!fichier_lecture){
		std::cerr << "Ce fichier n'existe pas" << std::endl;
	}


	if (fichier_lecture.is_open()){

		while (getline(fichier_lecture, ligne)){
			if (trouveEmploye(ligne,numero)==true){

			} else {
				listeEmploye.push_back(ligne);
			}
		}

		fichier_lecture.close();
	}
  
  fichier_ecriture.open(lien);

  if (!fichier_ecriture){
		std::cerr << "Ce fichier n'existe pas" << std::endl;
	}

	if (fichier_ecriture.is_open()){

		for (const auto& element : listeEmploye){
			fichier_ecriture << element <<'\n';
		}
		fichier_ecriture.close();
  }



}


bool trouveEmploye(std::string ligne, std::string numero){
	if (ligne.find(numero)!=std::string::npos){
		return true;
	}
	return false;
}

void modifierEmploye(std::string& ligne, std::string oldValue,std::string newValue){
	//trouve la position 
   size_t posOldValue=ligne.find(oldValue);
   //On remplace l'ancienne valeur par la nouvelle
   if (posOldValue != std::string::npos){
   	ligne.replace(posOldValue, oldValue.length(), newValue);
   }	

}


void remplirFichier(std::fstream& fichier, std::vector<std::string> newData){
	for (const auto& element:newData){
		fichier << element<<'\n'; 
	}
}

bool rechercherClient(std::string numero){
	std::ifstream fichier;
	std::string lien ="employe.txt";
	std::string ligne;

	fichier.open(lien);

	if(!fichier){
		std::cerr << "Le fichier n'existe pas" <<std::endl;
	}

	if (fichier.is_open()){

		while (getline(fichier, ligne)){

			if (trouveEmploye(ligne, numero)==true){
				std::cout << "Client trouvé --> " << ligne << std::endl;
				return true;
			}

		}

		fichier.close();
	}

	std::cout <<"Client introuvable dans la base de données"<< std::endl;

	return false;

}





//4-Supprimer un employé

//5-Rechercher un employé 


}; //fin classe





int main(){

	/*Employe employe;

	auto start = std::chrono::high_resolution_clock::now();

	employe.rechercherClient("0779733224");

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double>(end - start);

   std::cout << "Temps d'exécution : " << duration.count() << " secondes" << std::endl;
	*/


	int choix;
	std::string nom;
	std::string prenom;
	std::string email;
	std::string num;
	char reponse;

	std::cout <<"- Ajouter un nouvel employé : tapez 1 "<< std::endl;
	std::cout <<"- Consulter la liste des employés : tapez 2 " << std::endl;
	std::cout <<"- Modifier une info client : tapez 3 " << std::endl;
	std::cout <<"- Supprimer client : tapez 4 " << std::endl;
	std::cout << "Entrez votre choix:"; std::cin >> choix; 

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


	//Modification d'une info client
	if (choix==3){
      Employe employe;
		int choix;
		std::string numero;
		bool stop = false;
		std::string newValue;
		std::string oldValue;

		while (stop==false){
			std::cout << "Entrez votre numéro de téléphone: " ; std::cin >> numero;
			
			if (numero.length() !=10){
				std::cout << "Numéro incorrecte" << std::endl;
			}else{

         auto start = std::chrono::high_resolution_clock::now();

				if (employe.rechercherClient(numero)==true){
					//std::cout << "Saisissez l'ancienne valeur: "; std::cin >> oldValue; 
					//std::cout << "Saisissez la nouvelle valeur: "; std::cin >> newValue; 
					oldValue = "xxxx@xxx.com";
					newValue = "0000000@00000.com";
					employe.modifier(numero, oldValue,newValue);

			auto end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration<double>(end - start);
   		std::cout << "Temps d'exécution : " << duration.count() << " secondes" << std::endl;
					
				} else {std::cout <<"Client introuvable"<<std::endl;}				

			}

			std::cout << "Voulez-vous encore modifier ? tapez O ou N : "; std::cin >> reponse;

			if (reponse =='n'){stop=true;}
		}


	}


	//Suppression d'un client 
	if (choix==4){
		Employe employe;
		std::string numero;
		bool stop = false;

      while(stop==false){
			std::cout <<"Veuillez saisir le numéro de téléphone : "; cin >> numero;

			if (numero.length()!=10){

				std::cout << "Veuillez entrer un numéro valide " <<std::endl;
			} else {
					if (employe.rechercherClient(numero)==true){
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