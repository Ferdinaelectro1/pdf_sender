#include <iostream>
#include <boost/asio.hpp>

int main() {
    try {
        boost::asio::io_context io_context;

        // Résolution du serveur et port
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "1234"); // IP et port du serveur

        // Création et connexion du socket
        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connecté au serveur !" << std::endl;

        // Envoi d'un message
        std::string id = "235";
        boost::asio::write(socket, boost::asio::buffer(id));

        // Lecture de la réponse du serveur
        char reply[1024];
        size_t reply_length = socket.read_some(boost::asio::buffer(reply, 1024));
        std::cout << "Réponse du serveur : ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;

        if(reply == "OK")
        {
        }
        else
        {
            std::cerr << "Le serveur n'a pas accepté votre connexion" << std::endl;
        }

    } catch (std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}
