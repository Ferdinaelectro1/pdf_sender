#include "../include/session.h"

Session::Session(tcp::socket socket) : _socket(std::move(socket))
{
    _read_buffer = std::make_shared<std::array<char,1024>>();//init du buffer de reception
    _write_buffer = std::make_shared<std::string>("");//Init du buffer d'écriture
}

Session::Session(Session&& other) noexcept
    : _socket(std::move(other._socket)),
        _read_buffer(std::move(other._read_buffer)),
        _write_buffer(std::move(other._write_buffer))
{}

void Session::read_available(std::function<void(const std::string &)> func)
{
    
    _server_receive_callback = func; //cette fonction appelera la fonction du serveur en le passant les bons paramètres quand les données seront reçu
    _do_read();
}

void Session::_do_read()
{
    _socket.async_read_some(boost::asio::buffer(*_read_buffer),
    [&](boost::system::error_code ec, std::size_t length){
       if(!ec)
       {
            std::string msg(_read_buffer->data(),length);
            //std::cout << "Données reçu : "<<msg<< std::endl; 
            _server_receive_callback(msg);
            _do_read();
       }
    });//fonction qui serait appelé si des données sont reçu
}
void Session::send_data(const std::string &data)
{
    *_write_buffer = data;
    boost::asio::async_write(
        _socket,boost::asio::buffer(*_write_buffer),
        [&](const boost::system::error_code& ec,std::size_t) {
            if(!ec){
                std::cout << "Message envoyé !" << std::endl;
            }
            else
            {
                std::cerr << "Client déconnecté" << std::endl;
            }
        }
    );
}