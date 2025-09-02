#include "../include/server.h"

Server::Server() : _acceptor(_io,tcp::endpoint(tcp::v4(), 12345))
{
    //init de la liste des id des clients enregistrées par défaut
    _lists_of_client_id.push_back(125);
    _lists_of_client_id.push_back(203);
    _lists_of_client_id.push_back(943);
    _lists_of_client_id.push_back(342);
}

void Server::begin()
{
    try
    {
        _accept_connexion();//Démarre l'acceptation des clients
        _io.run();//démarre asio
    }
    catch(const std::exception &e)
    {
        std::cerr << "Erreur lors du démarrage du serveur : "<<e.what()<< std::endl;
    }
}

void Server::_accept_connexion()
{
    _acceptor.async_accept([&](const boost::system::error_code &error,tcp::socket socket)
        {
            std::cout << "Connexion accepté"<<std::endl;
            _work_session_ptr = std::make_unique<Session>(std::move(socket));//on crée une session de travail pour gérer ce client
            _work_session_ptr->read_available([&](const std::string &data){
                int id = converttoInt(data);
                if(_id_exits(id))
                {
                    std::cout << "Validé, j'obtiens ma propre session" << std::endl;
                    _work_session_ptr->send_data("OK\n");
                    _list_of_sessions.emplace(id,Session(std::move(*_work_session_ptr)));//on ajoute un nouvelle Session. on transfert la session de travail à cette session
                    auto it = _list_of_sessions.find(id);
                    if(it != _list_of_sessions.end())//on utilise un itérateur pour éviter d'utiliser le constructeur par défaut. Ne jamais utiliser les [] si on a pas de contru par def
                    {
                        it->second.read_available(
                            [this,id](const std::string data_receive){
                                _valide_client_receive_handler(data_receive,id);
                            }
                        );//on modifie la fonction a appelé si les données son reçus , vu qu'on sait que c'est un client valide
                    }
                }
                else
                {
                    std::cout << "Ce client n'est pas enregistré" <<std::endl;
                   _work_session_ptr->send_data("NO\n");
                }
            });//fonction qui est appelé si les données sont dispo dans le socket 
            _accept_connexion();//Accepter d'autres clients
        }
    ); //fonction qui s'execute quand un client est connecté
}

bool Server::_id_exits(int id){
    bool found = false;
    for(int id_in : _lists_of_client_id)
    {
        if(id == id_in)
        {
            found = true;
        }
    }
    return found;
}

//Ceci reçois ce qu'un client valide envoie
//Processus : 1-Reçois l'id du destinataire (celui qu'on veut envoyer le msg)
//2 - Reçois le message à envoyé et l'envoie
void Server::_valide_client_receive_handler(const std::string &data,int his_id)
{
    std::cout << data.size() << std::endl;
    auto it = _list_of_sessions.find(his_id);
    if(it != _list_of_sessions.end()) // on utilise les itérateurs pour se rassurer que la clé est valide , vu qu'on a pas de contructeur par défaut pour la classe
    {   
       //it->second.send_data(data);
       size_t pos = data.find(' ');
       std::string first_part = data.substr(0, pos);
       std::string second_part = data.substr(pos + 1);
       int dest_id = std::stod(first_part);
       auto dest_it = _list_of_sessions.find(dest_id);
       if(dest_it == _list_of_sessions.end())//si le destinataire n'est pas connecté on quitte
       {
            it->second.send_data("Ce client n'est pas connecté\n");
            return;
       }
        _dicussions_id[his_id] = dest_id;//on crée une discussion entre 2 client
        generate_file(second_part);
        dest_it->second.send_data(second_part);
    }
}

int Server::converttoInt(const std::string &data)
{
    int id = 0;
    try
    {
        id = static_cast<int>(std::stod(data));
    } catch(const std::out_of_range&)
    {
        std::cerr << "Erreur : nombre trop grand !" << std::endl;
    }
    catch (const std::invalid_argument&)
    {
        std::cerr << "Erreur : la chaîne n'est pas un nombre valide !" << std::endl;
    }
    return id;
}

void Server::generate_file(const std::string &data)
{
    std::ofstream file("result.pdf", std::ios::binary);
    if(!file)
    {
        std::cerr << "Erreur lors de l'ouverture du fichier result.pdf"<<std::endl;
        return;
    }
    file.write(data.data(), data.size()); // <<<<<<<< Correct pour le binaire
    file.close();
}