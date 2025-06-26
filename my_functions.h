symbolData triple1;//Triple Order Book Ask Bid prices
symbolData triple2;
symbolData triple3;

void logData(string fileName, double balance){
    ofstream file(fileName,ios::app);

    file<<"Arbitrage succesfully. Your balance is: "<<balance<<"USDT"<<endl;
}

void disconnectWS(){//disconnect WS
    ws_ptr.reset();
    connected = false;
    cout<<"WebSocket connection is finished."<<endl;
}

json wsData() {
    lock_guard<mutex> lock(ws_mutex);
    if (!connected || !ws_ptr) {
        throw runtime_error("WebSocket connection is not opened. \"First use connect\" functions.");
    }

    beast::flat_buffer buffer;
    ws_ptr->read(buffer);

    string msg = beast::buffers_to_string(buffer.data());
    json MarketData = json::parse(msg);

    return MarketData;
}

void connectTripleOrderBookData(string symbol1, string symbol2, string symbol3) {
    lock_guard<mutex> lock(ws_mutex);

    if (connected) {
        cout << "Already connected!\n";
        return;
    }

    try {

        transform(symbol1.begin(), symbol1.end(), symbol1.begin(), ::tolower);
        transform(symbol2.begin(), symbol2.end(), symbol2.begin(), ::tolower);
        transform(symbol3.begin(), symbol3.end(), symbol3.begin(), ::tolower);

        string host = "stream.binance.com";
        string port = "9443";
        string target = "/stream?streams=" + 
                        symbol1 + "@bookTicker/" + 
                        symbol2 + "@bookTicker/" + 
                        symbol3 + "@bookTicker";

        ctx.set_verify_mode(ssl::verify_none);

        tcp::resolver resolver(ioc);
        auto results = resolver.resolve(host, port);

        beast::ssl_stream<beast::tcp_stream> ssl_stream(ioc, ctx);
        beast::get_lowest_layer(ssl_stream).connect(results);
        ssl_stream.handshake(ssl::stream_base::client);

        // WebSocket başlatma
        ws_ptr = make_unique<websocket::stream<beast::ssl_stream<beast::tcp_stream>>>(move(ssl_stream));
        ws_ptr->set_option(websocket::stream_base::decorator([](websocket::request_type& req) {
            req.set(http::field::user_agent, std::string("Boost.Beast WebSocket Client"));
        }));

        ws_ptr->handshake(host, target); 

        connected = true;
        cout << "WebSocket connection is done.\n";
    }
    catch (const exception& e) {
        cerr << "Connection error: " << e.what() << endl;
        connected = false;
    }
}

void getSetTripleOrderBookData(string symbol1, string symbol2, string symbol3) {//as askPrice, bidPrice
    json veri = wsData();

    if(veri["data"]["s"]==symbol1){
        triple1.askPrice=stod(veri["data"]["a"].get<string>());
        triple1.bidPrice=stod(veri["data"]["b"].get<string>());
    }
    else if(veri["data"]["s"]==symbol2){
        triple2.askPrice=stod(veri["data"]["a"].get<string>());
        triple2.bidPrice=stod(veri["data"]["b"].get<string>());
    }
    else if(veri["data"]["s"]==symbol3){
        triple3.askPrice=stod(veri["data"]["a"].get<string>());
        triple3.bidPrice=stod(veri["data"]["b"].get<string>());
    }
    
}


