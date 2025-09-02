#ifndef __SERVER_H__
#define __SERVER_H__
#include <boost/asio.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include "session.h"

using boost::asio::ip::tcp;

class Server
{
    public:
        Server();
        void begin();//Démarre le serveur

    private:
        boost::asio::io_context _io; //Gère asio
        tcp::acceptor _acceptor; //accepte les connections
        void _accept_connexion(); //accpete une connexion entrante
        bool _id_exits(int id);
        std::unordered_map<int,Session> _list_of_sessions;//Liste des sessions actuels
        std::unique_ptr<Session> _work_session_ptr;//Session de travail utilisé pour acceuillir tout client
        std::vector<int> _lists_of_client_id;//Listes des ID des clients enregistrées
        void _valide_client_receive_handler(const std::string &data,int his_id);
        std::unordered_map<int,int> _dicussions_id;
        int converttoInt(const std::string &data);
        void generate_file(const std::string &data);
};


#endif