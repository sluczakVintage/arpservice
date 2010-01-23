/**
* @author Cezary Zawadka 
* @date 2010.01.17
* @version 0.9
*
*	
*/

#ifndef	C_DATA_BASE_WRAPPER_H
#define C_DATA_BASE_WRAPPER_H
//#define HAVE_REMOTE 

//#include <pcap.h>

#include <string>
#include <map>
#include <boost/tuple/tuple.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <queue>
#include "../sqlite/sqlite3.h"

#include "utils.hpp" 
#include "CSingleton.hpp"
#include "ActiveHost.hpp"
#include "CGUI.hpp"

//#include "../sqlite/sqlite3ext.h"
//#include "sqlite3.h"

using namespace std;
using namespace utils;


///klasa potrzebna do porownywania adresow mac, aby mogly byc kluczem w std::map
struct lessMAC// : public binary_function<MacAdress, MacAdress, bool>
{	
	bool operator()(const MacAdress& m1, const MacAdress& m2) const;
};

///klasa przechowuje aktywne hosty i zapisuje do bazy danych.
class CDataBaseWrapper : public CSingleton<CDataBaseWrapper>
{
	friend class CSingleton<CDataBaseWrapper>;
	friend class CGUI;

public:

	void handleReceived();

	void enqueReceived(ActiveHost& host);

	void saveHostToDB(ActiveHost& host);

	std::vector<boost::tuple<IPAddress, MacAdress, int>> cguiQuery();

private:

	CDataBaseWrapper();
	
	~CDataBaseWrapper();

	//Laduje wszystkie histroryczne hosty, powinno byc odpalane na poczatku dzialania programu
	void loadAllHosts();

	//zapisuje wszystkie hosty i ustawia im ttl na -1, powinno byc odpalane prze zamykaniu programu
	void saveAllHosts();

	///kolejka FIFO hostow od ktorych zostaly odebrany ARP, ale jeszcze nie 
	queue <ActiveHost> received_;

	///kolejka aktywnych hostow
	std::map<utils::MacAdress,ActiveHost, lessMAC> activeHosts_;

	///do synchronizacji - zeby na raz kilku nie czytalo/zapisywali
	boost::mutex mutex_;

	///wska�nik na baze danych z ktora sie komunikujemy
	sqlite3 * database;
};



#endif