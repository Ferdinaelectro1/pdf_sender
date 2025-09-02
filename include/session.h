#ifndef __SESSION_H__
#define __SESSION_H__
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <array>

using boost::asio::ip::tcp;

class Session
{
    public:
      Session(tcp::socket socket);
      Session(Session&& other) noexcept;
      void read_available(std::function<void(const std::string &)> func);//fonction que le serveur peut implémenter quand les données seront reçu
      void send_data(const std::string &data);

    private:
      tcp::socket _socket;
      std::shared_ptr<std::array<char,102400>> _read_buffer;//buffer de lecture des données reçu
      std::function<void(const std::string &)> _server_receive_callback;//aller dans le cpp voir
      std::shared_ptr<std::string> _write_buffer;//buffer d'écriture des données
      void _do_read();
};

#endif