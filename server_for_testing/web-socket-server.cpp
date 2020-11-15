/**
 * Diese Datei erzeugt einen Testserver, der Tastatureingaben sendet und
 * eingehende Nachrichten anzeigt.
 *
 * Leider scheint es keine überzeugende Möglichkeit zu geben, um reine
 * TCP/IP-Sockets mit browserseitigem JavaScript zu nutzen.
 * Ich habe mir ohne Erfolg angeschaut:
 *   - navigator.socket(s)
 *   - chrome.sockets.tcp
 *   - chrome.experimental.socket
 *   - net.Socket()
 *   - TCPSocket()
 *
 */

#include "Simple-WebSocket-Server/client_ws.hpp"
#include "Simple-WebSocket-Server/server_ws.hpp"
#include <future>

using namespace std;
using namespace boost::asio;

using WsServer = SimpleWeb::SocketServer<SimpleWeb::WS>;

WsServer server;

/**
 * Wird aufgerufen, wenn auf der Tastatur eine Zahl gedrückt wird.
 */
void on_number_pressed(char number, map<char, int> &input_states)
{
    int new_value = 0;
    auto old_value = input_states.find(number);
    if (old_value == input_states.end())
    {
        input_states.insert(std::pair<char, int>(number, new_value));
    }
    else
    {
        new_value = 1 - old_value->second;
        old_value->second = new_value;
    }

    stringstream out_message;
    out_message << "{\"event\": \"update-input\", "
                   "\"payload\": {"
                << "\"name\": \"LS-"
                << number << "\", "
                << "\"value\": "
                << new_value << "}}";

    std::cout << "[raus] " << out_message.str() << endl;
    for (auto &a_connection : server.get_connections())
        a_connection->send(out_message.str());
}

void register_keyboard()
{
    io_service ioservice;
    posix::stream_descriptor stream(ioservice, STDIN_FILENO);
    map<char, int> input_states;

    // Genau so initialisieren wie unten bei "get-inputs"
    input_states.insert(std::pair<char, int>('1', 0));
    input_states.insert(std::pair<char, int>('2', 1));
    input_states.insert(std::pair<char, int>('3', 1));

    char buf[1] = {};

    std::function<void(boost::system::error_code, size_t)> read_handler;

    read_handler = [&](boost::system::error_code ec, size_t len) {
        if (!ec && len == 1 && buf[0] >= '0' && buf[0] <= '9')
        {
            on_number_pressed(buf[0], input_states);
        }

        async_read(stream, buffer(buf), read_handler);
    };

    async_read(stream, buffer(buf), read_handler);

    std::cout << "[hülf] Tippe auf der Tastatur die Nummer der Lichtschranke + Enter, um sie umzuschalten." << endl;
    ioservice.run();
}

void register_websocket_thread()
{
    server.config.port = 5000;

    // Test with the following JavaScript:
    //   var ws=new WebSocket("ws://localhost:5000/");
    //   ws.onmessage=function(evt){console.log(evt.data);};
    //   ws.send("test");
    auto &api_endpoint = server.endpoint["^/?$"];

    api_endpoint.on_message = [](shared_ptr<WsServer::Connection> connection, shared_ptr<WsServer::InMessage> incoming_wrapped_message) {
        string incoming_message = incoming_wrapped_message->string();
        string outgoing_message;
        cout << "[rein] " << incoming_message << endl;
        if (incoming_message.find("get-inputs") == 0)
        {
            outgoing_message = "{"
                               "\"event\": \"set-inputs\","
                               "\"payload\": ["
                               "{\"name\": \"LS-1\", \"value\": 0}, "
                               "{\"name\": \"LS-2\", \"value\": 1}, "
                               "{\"name\": \"LS-3\", \"value\": 1}"
                               "]"
                               "}";
            cout << "[raus] " << outgoing_message << endl;
            connection->send(outgoing_message);
            return;
        }

        cout << "[hülf] Der Client hat eine unbekannte Art von Anfrage geschickt." << endl;
    };

    api_endpoint.on_open = [](shared_ptr<WsServer::Connection> connection) {
        cout << "[info] ein neuer Client ist nun verbunden." << endl;
    };

    api_endpoint.on_close = [](shared_ptr<WsServer::Connection> connection, int status_code, const string & /*reason*/) {
        // See RFC 6455 7.4.1. for status codes
        cout << "[info] ein Client ist gegangen." << endl;
    };

    // Start server and receive assigned port when server is listening for requests
    promise<unsigned short> server_port;
    thread server_thread([&server_port]() {
        // Start server
        server.start([&server_port](unsigned short port) {
            server_port.set_value(port);
        });
    });
    cout << "[info] Der Testserver läuft nun auf Port " << server_port.get_future().get() << "." << endl;

    // Server im Hintergrund laufen lassen, damit wir im Vordergrund auf die Tastatur warten können.
    server_thread.detach();
}

int main()
{
    register_websocket_thread();
    register_keyboard();
    return 0;
}
