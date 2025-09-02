#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <fstream>

// Fonction pour lire le PDF en binaire
std::vector<unsigned char> get_pdf_data(const std::string& filename)
{
    std::ifstream pdf_file(filename, std::ios::binary);
    if(!pdf_file)
    {
        std::cerr << "Impossible d'ouvrir le fichier " << filename << std::endl;
        exit(1);
    }
    std::vector<unsigned char> pdf_data((std::istreambuf_iterator<char>(pdf_file)),
                                        std::istreambuf_iterator<char>());
    return pdf_data;
}

int main(int argc, char *argv[]) {
    if(argc < 4)
    {
        std::cerr << "Usage : ./client my_id dest_id pdf_name" << std::endl;
        return 1;
    }

    std::string my_id = argv[1]; 
    std::string dest_id = argv[2];
    std::string pdf_name = argv[3];

    try {
        boost::asio::io_context io_context;

        // Résolution du serveur et port
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");

        // Création et connexion du socket
        boost::asio::ip::tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Connecté au serveur !" << std::endl;

        // --- Étape 1 : Envoi de my_id ---
        boost::asio::write(socket, boost::asio::buffer(my_id + "\n"));

        // Lecture de la réponse du serveur
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");
        std::istream is(&buffer);
        std::string response;
        std::getline(is, response);

        std::cout << "Réponse du serveur : " << response << std::endl;

        if(response == "OK")
        {
            // --- Étape 2 : Envoi du destinataire + PDF ---
            std::vector<unsigned char> pdf_data = get_pdf_data(pdf_name);
            std::string pdf_string(pdf_data.begin(), pdf_data.end());

            std::string send_msg = dest_id + " " + pdf_string;

            boost::asio::write(socket, boost::asio::buffer(send_msg));

            // Lecture de la réponse du serveur après l'envoi du PDF
            boost::asio::streambuf buffer2;
            boost::asio::read_until(socket, buffer2, "\n");
            std::istream is2(&buffer2);
            std::string reply;
            std::getline(is2, reply);

            std::cout << "Réponse reçue : " << reply << std::endl;
        }
        else
        {
            std::cerr << "Le serveur n'a pas accepté votre connexion" << std::endl;
            return 1;
        }

    } catch (std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}
