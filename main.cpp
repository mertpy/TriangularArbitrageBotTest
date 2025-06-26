#include "header.h"
#include "structs.h"
#include "my_functions.h"

int main(){
    string
        symbol1="BNBUSDT",//ethusdt,solusdt
        symbol2="BNBETH",//ethbtc,solbtc
        symbol3="ETHUSDT";//btcusdt,btcusdt

    connectTripleOrderBookData(symbol1,symbol2,symbol3);//WS connection

    triple1.askPrice=0;
    triple1.bidPrice=0;
    triple2.askPrice=0;
    triple2.bidPrice=0;
    triple3.askPrice=0;
    triple3.bidPrice=0;

    bool ask, bid;

    double fee=0.001, factor=(1-fee)*(1-0.00075)*(1-fee);//BNB

    double mainUsdt=1000.0, lastUsdt=0;

    for(;;){
        getSetTripleOrderBookData(symbol1,symbol2,symbol3);
        ask=triple1.askPrice!=0 && triple2.askPrice!=0 && triple3.askPrice!=0;
        bid=triple1.bidPrice!=0 && triple2.bidPrice!=0 && triple3.bidPrice!=0;
        if(ask && bid){
            cout<<"Getting data. Searching for opportunities..."<<endl;
            cout<<"Your balance is "<<mainUsdt<<"USDT."<<endl;
            break;
        }
    }

    for(;;){
        
        getSetTripleOrderBookData(symbol1,symbol2,symbol3);

        lastUsdt=((mainUsdt/triple1.askPrice)*triple2.bidPrice)*triple3.bidPrice*factor;

        if(lastUsdt>mainUsdt){
            cout<<"Arbitrage succesfully, your balance is "<<lastUsdt<<"USDT"<<endl;
            mainUsdt=lastUsdt;
            lastUsdt=0;
            logData("log.txt",mainUsdt);
        }
    }
        

    disconnectWS();

    for(;;){}

    return 0;
}