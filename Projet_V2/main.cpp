#include <windows.h>
#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <string>
using namespace std;

// Gestion des retours avec diagnostic détaillé
bool checkRetour(SQLRETURN retour, string typeOperation, SQLHANDLE handle = NULL, SQLSMALLINT handleType = SQL_HANDLE_STMT) {
    if (retour != SQL_SUCCESS && retour != SQL_SUCCESS_WITH_INFO) {
        cout << "Operation " << typeOperation << " echoue" << endl;
        if (handle) {
            SQLCHAR sqlState[6];
            SQLINTEGER nativeError;
            SQLCHAR message[256];
            SQLSMALLINT msgLen;
            SQLGetDiagRec(handleType, handle, 1, sqlState, &nativeError, message, sizeof(message), &msgLen);
            cout << "Erreur : " << message << " (SQLSTATE: " << sqlState << ", Code: " << nativeError << ")" << endl;
        }
        return false;
    }
    cout << "Operation " << typeOperation << " reussie" << endl;
    return true;
}

// Allocation des handles
void allouerEnv(SQLHENV* envHandle) {
    SQLRETURN retour = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HENV, envHandle);
    checkRetour(retour, "Allocation Handle", *envHandle, SQL_HANDLE_ENV);
}

void defVersionOdbc(SQLHENV envHandle) {
    SQLRETURN retour = SQLSetEnvAttr(envHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    checkRetour(retour, "Definition version", envHandle, SQL_HANDLE_ENV);
}

void AlloueHandleConn(SQLHDBC* conHandle, SQLHENV envHandle) {
    SQLRETURN retour = SQLAllocHandle(SQL_HANDLE_DBC, envHandle, conHandle);
    checkRetour(retour, "handle connexion", *conHandle, SQL_HANDLE_DBC);
}

void etabliConnexion(SQLHDBC conHandle, string dsnName) {
    SQLRETURN retour = SQLConnect(conHandle, (SQLCHAR*)dsnName.c_str(), SQL_NTS, NULL, 0, NULL, 0);
    if (!checkRetour(retour, "creation connexion", conHandle, SQL_HANDLE_DBC)) return;
    retour = SQLSetConnectAttr(conHandle, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0);
    checkRetour(retour, "Desactivation auto-commit", conHandle, SQL_HANDLE_DBC);
}

void alloueHandleRq(SQLHDBC conHandle, SQLHSTMT* requeteHandle) {
    SQLRETURN retour = SQLAllocHandle(SQL_HANDLE_STMT, conHandle, requeteHandle);
    checkRetour(retour, "Allocation handle requete", *requeteHandle);
}

void executeRequete(SQLHSTMT requeteHandle, string requete) {
    SQLRETURN retour = SQLExecDirect(requeteHandle, (SQLCHAR*)requete.c_str(), SQL_NTS);
    checkRetour(retour, "Execution requete", requeteHandle);
}

void afficherResultats(SQLHSTMT requeteHandle) {
    SQLRETURN retour;
    char id[50], nom[50], prenom[50], email[100], numero[20];
    SQLLEN idLen, nomLen, prenomLen, emailLen, numeroLen;

    retour = SQLBindCol(requeteHandle, 1, SQL_C_CHAR, id, sizeof(id), &idLen);
    checkRetour(retour, "Liaison colonne id", requeteHandle);
    retour = SQLBindCol(requeteHandle, 2, SQL_C_CHAR, nom, sizeof(nom), &nomLen);
    checkRetour(retour, "Liaison colonne nom", requeteHandle);
    retour = SQLBindCol(requeteHandle, 3, SQL_C_CHAR, prenom, sizeof(prenom), &prenomLen);
    checkRetour(retour, "Liaison colonne prenom", requeteHandle);
    retour = SQLBindCol(requeteHandle, 4, SQL_C_CHAR, email, sizeof(email), &emailLen);
    checkRetour(retour, "Liaison colonne email", requeteHandle);
    retour = SQLBindCol(requeteHandle, 5, SQL_C_CHAR, numero, sizeof(numero), &numeroLen);
    checkRetour(retour, "Liaison colonne numero", requeteHandle);

    cout << "\nResultats de la requete :\n";
    cout << "ID\tNom\tPrenom\tEmail\tNumero\n";
    cout << "----------------------------------------\n";

    while ((retour = SQLFetch(requeteHandle)) == SQL_SUCCESS) {
        cout << id << "|" << nom << "|" << prenom << "|" << email << "|" << numero << "\n" << endl;
    }

    if (retour != SQL_NO_DATA) {
        checkRetour(retour, "Fetch des resultats", requeteHandle);
    } else {
        cout << "Fin des resultats\n";
    }

    SQLFreeStmt(requeteHandle, SQL_UNBIND);
    SQLFreeStmt(requeteHandle, SQL_CLOSE);
}

// Récupère le dernier ID comme chaîne et retourne une nouvelle valeur incrémentée
string dernierId(SQLHSTMT requeteHandle) {
    SQLFreeStmt(requeteHandle, SQL_CLOSE); // Réinitialiser avant utilisation
    SQLRETURN retour = SQLExecDirect(requeteHandle, (SQLCHAR*)"SELECT MAX(CAST(id AS INT)) FROM employe;", SQL_NTS);
    if (!checkRetour(retour, "Execution requete dernier ID", requeteHandle)) {
        return "1"; // Valeur par défaut si échec
    }

    char id[50];
    SQLLEN idLen;
    retour = SQLBindCol(requeteHandle, 1, SQL_C_CHAR, id, sizeof(id), &idLen);
    if (!checkRetour(retour, "Liaison colonne id", requeteHandle)) {
        return "1";
    }

    retour = SQLFetch(requeteHandle);
    if (retour == SQL_SUCCESS) {
        if (idLen == SQL_NULL_DATA) {
            cout << "Aucun ID trouve (table vide)\n";
            return "1"; // Table vide, commence à "1"
        }
        try {
            int idInt = stoi(id); // Conversion en int pour incrémentation
            cout << "Le dernier ID : " << id << endl;
            SQLFreeStmt(requeteHandle, SQL_CLOSE);
            return to_string(idInt + 1); // Retourne le nouvel ID comme chaîne
        } catch (const exception& e) {
            cout << "Erreur de conversion : " << e.what() << endl;
            return "1";
        }
    } else {
        checkRetour(retour, "Fetch dernier ID", requeteHandle);
        return "1";
    }
}

void insererEmploye(SQLHSTMT requeteHandle, string id, string nom, string prenom, string email, string numero) {
    SQLRETURN retour;
    string requete = "INSERT INTO employe (id, nom, prenom, email, numero) VALUES (?, ?, ?, ?, ?);";
    retour = SQLPrepare(requeteHandle, (SQLCHAR*)requete.c_str(), SQL_NTS);
    if (!checkRetour(retour, "Preparation insertion", requeteHandle)) return;

    SQLLEN len = SQL_NTS;
    retour = SQLBindParameter(requeteHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)id.c_str(), 0, &len);
    checkRetour(retour, "Bind id", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)nom.c_str(), 0, &len);
    checkRetour(retour, "Bind nom", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)prenom.c_str(), 0, &len);
    checkRetour(retour, "Bind prenom", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, (SQLCHAR*)email.c_str(), 0, &len);
    checkRetour(retour, "Bind email", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, (SQLCHAR*)numero.c_str(), 0, &len);
    checkRetour(retour, "Bind numero", requeteHandle);

    retour = SQLExecute(requeteHandle);
    checkRetour(retour, "Execution insertion", requeteHandle);
}

void mettreAJourEmploye(SQLHSTMT requeteHandle, string id, string nom, string prenom, string email, string numero) {
    SQLRETURN retour;
    string requete = "UPDATE employe SET nom = ?, prenom = ?, email = ?, numero = ? WHERE id = ?;";
    retour = SQLPrepare(requeteHandle, (SQLCHAR*)requete.c_str(), SQL_NTS);
    if (!checkRetour(retour, "Preparation mise a jour", requeteHandle)) return;

    SQLLEN len = SQL_NTS;
    retour = SQLBindParameter(requeteHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)nom.c_str(), 0, &len);
    checkRetour(retour, "Bind nom", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)prenom.c_str(), 0, &len);
    checkRetour(retour, "Bind prenom", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 100, 0, (SQLCHAR*)email.c_str(), 0, &len);
    checkRetour(retour, "Bind email", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 20, 0, (SQLCHAR*)numero.c_str(), 0, &len);
    checkRetour(retour, "Bind numero", requeteHandle);
    retour = SQLBindParameter(requeteHandle, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)id.c_str(), 0, &len);
    checkRetour(retour, "Bind id", requeteHandle);

    retour = SQLExecute(requeteHandle);
    checkRetour(retour, "Execution mise a jour", requeteHandle);
}

void supprimerEmploye(SQLHSTMT requeteHandle, string id) {
    SQLRETURN retour;
    string requete = "DELETE FROM employe WHERE id = ?;";
    retour = SQLPrepare(requeteHandle, (SQLCHAR*)requete.c_str(), SQL_NTS);
    if (!checkRetour(retour, "Preparation suppression", requeteHandle)) return;

    SQLLEN len = SQL_NTS;
    retour = SQLBindParameter(requeteHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)id.c_str(), 0, &len);
    checkRetour(retour, "Bind id", requeteHandle);

    retour = SQLExecute(requeteHandle);
    checkRetour(retour, "Execution suppression", requeteHandle);
}

void gererTransaction(SQLHDBC conHandle, bool commit) {
    SQLRETURN retour = SQLEndTran(SQL_HANDLE_DBC, conHandle, commit ? SQL_COMMIT : SQL_ROLLBACK);
    checkRetour(retour, commit ? "Commit transaction" : "Rollback transaction", conHandle, SQL_HANDLE_DBC);
}


// Recherche un employé par ID ou numéro de téléphone
bool rechercherEmploye(SQLHSTMT requeteHandle, string critere, bool parId, string& idTrouve, string& nom, string& prenom, string& email, string& numero) {
    SQLRETURN retour;
    string requete;
    if (parId) {
        requete = "SELECT id, nom, prenom, email, numero FROM employe WHERE id = ?;";
    } else {
        requete = "SELECT id, nom, prenom, email, numero FROM employe WHERE numero = ?;";
    }

    SQLFreeStmt(requeteHandle, SQL_CLOSE); // Réinitialiser le handle
    retour = SQLPrepare(requeteHandle, (SQLCHAR*)requete.c_str(), SQL_NTS);
    if (!checkRetour(retour, "Preparation recherche", requeteHandle)) return false;

    SQLLEN len = SQL_NTS;
    retour = SQLBindParameter(requeteHandle, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 50, 0, (SQLCHAR*)critere.c_str(), 0, &len);
    if (!checkRetour(retour, "Bind critere", requeteHandle)) return false;

    retour = SQLExecute(requeteHandle);
    if (!checkRetour(retour, "Execution recherche", requeteHandle)) return false;

    char idBuf[50], nomBuf[50], prenomBuf[50], emailBuf[100], numeroBuf[20];
    SQLLEN idLen, nomLen, prenomLen, emailLen, numeroLen;

    retour = SQLBindCol(requeteHandle, 1, SQL_C_CHAR, idBuf, sizeof(idBuf), &idLen);
    checkRetour(retour, "Liaison colonne id", requeteHandle);
    retour = SQLBindCol(requeteHandle, 2, SQL_C_CHAR, nomBuf, sizeof(nomBuf), &nomLen);
    checkRetour(retour, "Liaison colonne nom", requeteHandle);
    retour = SQLBindCol(requeteHandle, 3, SQL_C_CHAR, prenomBuf, sizeof(prenomBuf), &prenomLen);
    checkRetour(retour, "Liaison colonne prenom", requeteHandle);
    retour = SQLBindCol(requeteHandle, 4, SQL_C_CHAR, emailBuf, sizeof(emailBuf), &emailLen);
    checkRetour(retour, "Liaison colonne email", requeteHandle);
    retour = SQLBindCol(requeteHandle, 5, SQL_C_CHAR, numeroBuf, sizeof(numeroBuf), &numeroLen);
    checkRetour(retour, "Liaison colonne numero", requeteHandle);

    retour = SQLFetch(requeteHandle);
    if (retour == SQL_SUCCESS) {
        idTrouve = (idLen != SQL_NULL_DATA) ? idBuf : "";
        nom = (nomLen != SQL_NULL_DATA) ? nomBuf : "";
        prenom = (prenomLen != SQL_NULL_DATA) ? prenomBuf : "";
        email = (emailLen != SQL_NULL_DATA) ? emailBuf : "";
        numero = (numeroLen != SQL_NULL_DATA) ? numeroBuf : "";

        cout << "Employe trouve :\n";
        cout << "ID: " << idTrouve << " | Nom: " << nom << " | Prenom: " << prenom
             << " | Email: " << email << " | Numero: " << numero << endl;
        SQLFreeStmt(requeteHandle, SQL_UNBIND);
        SQLFreeStmt(requeteHandle, SQL_CLOSE);
        return true;
    } else {
        cout << "Aucun employe trouve avec ce " << (parId ? "ID" : "numero") << ".\n";
        SQLFreeStmt(requeteHandle, SQL_UNBIND);
        SQLFreeStmt(requeteHandle, SQL_CLOSE);
        return false;
    }
}

int main() {
    SQLHENV envHandle = NULL;
    SQLHDBC conHandle = NULL;
    SQLHSTMT requeteHandle = NULL;

    allouerEnv(&envHandle);
    if (!envHandle) return 1;
    defVersionOdbc(envHandle);
    AlloueHandleConn(&conHandle, envHandle);
    if (!conHandle) {
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
        return 1;
    }
    etabliConnexion(conHandle, "ODBCPP");
    if (!checkRetour(SQL_SUCCESS, "Verification connexion", conHandle, SQL_HANDLE_DBC)) {
        SQLFreeHandle(SQL_HANDLE_DBC, conHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
        return 1;
    }
    alloueHandleRq(conHandle, &requeteHandle);
    if (!requeteHandle) {
        SQLDisconnect(conHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, conHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, envHandle);
        return 1;
    }

    int choix;
    cout << "Tapez 1 pour afficher la liste des employes\n";
    cout << "Tapez 2 pour ajouter un employe\n";
    cout << "Tapez 3 pour mettre a jour un employe\n";
    cout << "Tapez 4 pour supprimer un employe\n";
    cout << ": "; cin >> choix;

    bool succes = true;
    switch (choix) {
    case 1:
        executeRequete(requeteHandle, "SELECT TOP 10 * FROM employe;");
        afficherResultats(requeteHandle);
        break;

    case 2: {
        string dernier = dernierId(requeteHandle);
        int dernierNum;
        try {
            dernierNum = stoi(dernier);
        } catch (...) {
            dernierNum = 0;
        }
        string nouvelId = to_string(dernierNum + 1);
        string nom, prenom, email, numero, confirmation = "O";

        while (confirmation == "o" || confirmation == "O") {
            cout << "Veuillez saisir le nom : "; cin >> nom;
            cout << "Veuillez saisir le prenom : "; cin >> prenom;
            cout << "Veuillez saisir l'email : "; cin >> email;
            cout << "Veuillez saisir le numero : "; cin >> numero;
            insererEmploye(requeteHandle, nouvelId, nom, prenom, email, numero);
            if (!checkRetour(SQL_SUCCESS, "Verification insertion", requeteHandle)) {
                succes = false;
                break;
            }
            nouvelId = to_string(++dernierNum);
            cout << "Voulez-vous inserer un nouvel employe ? (O/N) : "; cin >> confirmation;
            if (confirmation != "o" && confirmation != "O" && confirmation != "n" && confirmation != "N") {
                cout << "Choix invalide, fin de l'insertion.\n";
                break;
            }
        }
        break;
    }

    case 3: {
        string critere, idTrouve, nom, prenom, email, numero;
        int methode;
        cout << "Rechercher par : 1) ID, 2) Numero de telephone : "; cin >> methode;
        if (methode == 1) {
            cout << "Entrez l'ID de l'employe : "; cin >> critere;
            if (rechercherEmploye(requeteHandle, critere, true, idTrouve, nom, prenom, email, numero)) {
                cout << "Nouveau nom (actuel: " << nom << ") : "; cin >> nom;
                cout << "Nouveau prenom (actuel: " << prenom << ") : "; cin >> prenom;
                cout << "Nouvel email (actuel: " << email << ") : "; cin >> email;
                cout << "Nouveau numero (actuel: " << numero << ") : "; cin >> numero;
                mettreAJourEmploye(requeteHandle, idTrouve, nom, prenom, email, numero);
            } else {
                succes = false;
                cout << "Mise a jour annulee.\n";
            }
        } else if (methode == 2) {
            cout << "Entrez le numero de telephone de l'employe : "; cin >> critere;
            if (rechercherEmploye(requeteHandle, critere, false, idTrouve, nom, prenom, email, numero)) {
                cout << "Nouveau nom (actuel: " << nom << ") : "; cin >> nom;
                cout << "Nouveau prenom (actuel: " << prenom << ") : "; cin >> prenom;
                cout << "Nouvel email (actuel: " << email << ") : "; cin >> email;
                cout << "Nouveau numero (actuel: " << numero << ") : "; cin >> numero;
                mettreAJourEmploye(requeteHandle, idTrouve, nom, prenom, email, numero);
            } else {
                succes = false;
                cout << "Mise a jour annulee.\n";
            }
        } else {
            cout << "Methode de recherche invalide.\n";
            succes = false;
        }
        break;
    }

    case 4: {
        string critere, idTrouve, nom, prenom, email, numero;
        int methode;
        cout << "Rechercher par : 1) ID, 2) Numero de telephone : "; cin >> methode;
        if (methode == 1) {
            cout << "Entrez l'ID de l'employe : "; cin >> critere;
            if (rechercherEmploye(requeteHandle, critere, true, idTrouve, nom, prenom, email, numero)) {
                string confirmation;
                cout << "Confirmez la suppression (O/N) : "; cin >> confirmation;
                if (confirmation == "o" || confirmation == "O") {
                    supprimerEmploye(requeteHandle, idTrouve);
                } else {
                    cout << "Suppression annulee.\n";
                }
            } else {
                succes = false;
                cout << "Suppression annulee.\n";
            }
        } else if (methode == 2) {
            cout << "Entrez le numero de telephone de l'employe : "; cin >> critere;
            if (rechercherEmploye(requeteHandle, critere, false, idTrouve, nom, prenom, email, numero)) {
                string confirmation;
                cout << "Confirmez la suppression (O/N) : "; cin >> confirmation;
                if (confirmation == "o" || confirmation == "O") {
                    supprimerEmploye(requeteHandle, idTrouve);
                } else {
                    cout << "Suppression annulee.\n";
                }
            } else {
                succes = false;
                cout << "Suppression annulee.\n";
            }
        } else {
            cout << "Methode de recherche invalide.\n";
            succes = false;
        }
        break;
    }

    default:
        cout << "Choix invalide\n";
        succes = false;
    }

    gererTransaction(conHandle, succes);

    SQLFreeHandle(SQL_HANDLE_STMT, requeteHandle);
    SQLDisconnect(conHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, conHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, envHandle);

    return 0;
}
