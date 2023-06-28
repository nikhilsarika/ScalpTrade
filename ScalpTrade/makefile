all: ScalpTrade

ScalpTrade: ConcurrentQueue.o MessageHeader.o Order.o Quote.o Trade.o SocketClient.o SocketServer.o NewTrade.o NewQuote.o ScalpTrade.o 
	g++ -std=c++14 ConcurrentQueue.o MessageHeader.o Order.o Quote.o NewQuote.o Trade.o NewTrade.o SocketClient.o SocketServer.o ScalpTrade.o  -o ScalpTrade

ConcurrentQueue.o: ConcurrentQueue.cpp ConcurrentQueue.h Trade.h MessageHeader.h Order.h NewTrade.h
	g++ -std=c++14 -c ConcurrentQueue.cpp

MessageHeader.o: MessageHeader.cpp MessageHeader.h
	g++ -std=c++14 -c MessageHeader.cpp

Order.o: Order.cpp Order.h
	g++ -std=c++14 -c Order.cpp

Quote.o: Quote.cpp Quote.h
	g++ -std=c++14 -c Quote.cpp

NewQuote.o: NewQuote.cpp NewQuote.h
	g++ -std=c++14 -c NewQuote.cpp

Trade.o: Trade.cpp Trade.h
	g++ -std=c++14 -c Trade.cpp

NewTrade.o: NewTrade.cpp NewTrade.h
	g++ -std=c++14 -c NewTrade.cpp

SocketClient.o: SocketClient.cpp SocketClient.h global.h Trade.h MessageHeader.h
	g++ -std=c++14 -c SocketClient.cpp

SocketServer.o: SocketServer.cpp SocketServer.h global.h
	g++ -std=c++14 -c SocketServer.cpp

ScalpTrade.o: ScalpTrade.cpp ConcurrentQueue.h Order.h MessageHeader.h Quote.h Trade.h SocketClient.h SocketServer.h global.h
	g++ -std=c++14 -c ScalpTrade.cpp