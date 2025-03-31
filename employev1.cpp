#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>
#include <map>


using namespace std;


class Employe {

	public :
		string nom;
		string prenom;
		string email;
		string numero;


	public :
		Employe (string nom, string prenom, string email, string numero){
			this->nom = nom;
			this->prenom=prenom;
			this->email=email;
			this->numero=numero;
		}

	public :
		Employe()=default;


	void affichierInfo(){
		cout << "nom : " << nom << endl;
		cout << "Prenom: " << prenom << endl;
		cout << "Email: " << email << endl;
		cout << "Numero: " << numero << endl;
	}


	void ajouterEmploye(){
		ofstream fichier;
		string lien ="employe.txt";

		fichier.open(lien);

		if (!fichier){

			cout << "Le fichier n'existe pas "<< endl;
		}

		if (fichier.is_open()){

			fichier << nom << ";" << prenom <<";" << email <<";" << numero << endl;
		}


	}

	string trim(const string& str) {
		size_t first = str.find_first_not_of(" \t\n\r");
		if (first == string::npos) return "";
		size_t last = str.find_last_not_of(" \t\n\r");
		return str.substr(first, (last - first + 1));
	}


	void modifierEmploye(string num, int choix, string newValue){

		ifstream fichier_lecture;
		ofstream fichier_ecriture;
		string lien ="employe.txt";
		vector<Employe*> listeEmploye; 
		string ligne;

		fichier_lecture.open(lien);

		if(!fichier_lecture){
			cout << "Le fichier n'existe pas" << endl;
		}

		if (fichier_lecture.is_open()){
			while (getline (fichier_lecture, ligne)) {

				Employe* employe = new Employe();

				employe->recupEmploye(ligne);

				if (employe->numero == num){
					
					//je récupère la ligne je mets dans une liste indexé, puis je changeValue avec le choix (index) et le newValue
					//Pour le reste Je stocke la ligne avec getline(fichier, ligne) et push back  

					employe->changeValue(choix,newValue);
				}
				
				listeEmploye.push_back(employe);

			}

			fichier_lecture.close();

		}

		fichier_ecriture.open(lien);

		if (!fichier_ecriture){
			cout << "Le fichier n'existe pas" << endl;
		}

		if (fichier_ecriture.is_open()){

			for (Employe* employe : listeEmploye){
				fichier_ecriture << employe->nom <<';'<< employe->prenom << ";" << employe->email << ";" << employe->numero << endl;
				delete employe;
			}

			fichier_ecriture.close();
			
		}

		
	}

string trimString(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

	
void modifierEmploye2(string numero, int choix, string newValue){

		ifstream fichier_lecture;
		ofstream fichier_ecriture;
		string lien ="employe.txt";
		//vector<Employe*> listeEmploye; 
		vector<string> listeEmploye;
		map<string, string> employeModifie;
		string ligne;

		fichier_lecture.open(lien);

		if(!fichier_lecture){
			cout << "Le fichier n'existe pas" << endl;
		}

		if (fichier_lecture.is_open()){
			while (getline (fichier_lecture, ligne)) {

				//Employe* employe = new Employe();

				//employe->recupEmploye(ligne);

				if (ligne.find(numero) != string::npos){
					
					cout << "Client trouvé --> " << ligne << endl;
					
					//je récupère la ligne je mets dans une liste indexé, puis je changeValue avec le choix (index) et le newValue
					//Pour le reste Je stocke la ligne avec getline(fichier, ligne) et push back  

					string nom, prenom, email, num;
					stringstream fluxChaine(ligne);
					
					getline(fluxChaine, nom, ';');
					getline(fluxChaine, prenom, ';');
					getline(fluxChaine, email, ';');
					getline(fluxChaine, num, ';');
					
					// Trim les espaces (vous devriez implémenter une fonction trim)
					nom = trimString(nom);
					prenom = trimString(prenom);
					email = trimString(email);
					num = trimString(num);
					
					// Remplissage du dictionnaire
					
					// Remplissage du dictionnaire
					employeModifie["nom"] = nom;          // "Dupont"
					employeModifie["prenom"] = prenom;    // "Jean"
					employeModifie["email"] = email;      // "dupjean@gmail.com"
					employeModifie["numero"] = num;       // "056232622"
									
					if(choix==1){employeModifie["nom"] = newValue; }
					if(choix==2){employeModifie["prenom"] = newValue; }
					if(choix==3){employeModifie["email"] = newValue; }
					if(choix==4){employeModifie["numero"] = newValue; }
					
					string employeNouv = employeModifie["nom"] + ";" + employeModifie["prenom"] + ";" + employeModifie["email"] + ";" + employeModifie["numero"];
					cout << "Nouvel employé --> " << employeNouv << endl;
					
					listeEmploye.push_back(employeNouv);

				} else{listeEmploye.push_back(ligne);}

			}

			fichier_lecture.close();

		}

		fichier_ecriture.open(lien);

		if (!fichier_ecriture){
			cout << "Le fichier n'existe pas" << endl;
		}

		if (fichier_ecriture.is_open()){

			for ( const auto& element : listeEmploye){
				fichier_ecriture << element <<'\n';
				
			}

			fichier_ecriture.close();
			cout << "Modification réussie" << endl;
			
		}

		
	}

void changeValue(int choix, string newValue){
	switch(choix){
		case 1 :
			this->nom = newValue;
			break;
		case 2 :
			this->prenom = newValue;
			break;
		case 3:
			this->email = newValue;
			break;
		case 4 :
			this->numero = newValue;
			break;
		default :
			cout << "veuillez faire votre choix, aucune modification faite";
	}
}

void recupEmploye(string ligne){
	stringstream fluxChaine(ligne);
	getline(fluxChaine, this->nom, ';');
	this->nom = trim(this->nom);
	getline(fluxChaine, this->prenom, ';');
	this->prenom = trim(this->prenom);
	getline(fluxChaine, this->email, ';');
	this->email = trim(this->email);
	getline(fluxChaine, this->numero, ';');
	this->numero = trim(this->numero);
}


bool rechercheClient(string& numero){

	string lien ="employe.txt";
	ifstream fichier(lien);
	string ligne;

	fichier.open(lien);

	if (!fichier){

		cout << "Le fichier n'existe pas"<< endl;
		return false;
	}


	while(getline(fichier, ligne)){

		if (ligne.find(numero) != string::npos){
			fichier.close();
			return true;
		}

	}
	fichier.close();

	
	return false;
}



};


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
    	string newValue;

        cout << "Quel est votre numéro de téléphone ? : "; cin >> num;
    	cout << "Si vous voulez modifier le nom tapez 1: " << endl;
    	cout << "Si vous voulez modifier le prenom tapez 2: " << endl;
    	cout << "Si vous voulez modifier l'email tapez 3: " << endl;
    	cout << "Si vous voulez modifier le numéro de téléphone tapez 4: ";cin>>option;

    	cout<< "Entrez la nouvelle valeur : "; cin >> newValue;
        
        auto start = std::chrono::high_resolution_clock::now();

    	employe.modifierEmploye2(num, option, newValue);

    	auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration<double>(end - start);
   		std::cout << "Temps d'exécution : " << duration.count() << " secondes" << std::endl;
					
    }

	 
	return 0;
}