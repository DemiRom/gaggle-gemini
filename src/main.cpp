#include <cstring>
#include <iostream>

#include "GeminiException.h"
#include "GeminiRequest.h"
#include "ResponseObject.h"
#include "ResponseParser.h"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "SQLiteCpp/Transaction.h"

#include <SQLiteCpp/SQLiteCpp.h>
#include <SQLiteCpp/VariadicBind.h>
#include <string>

#define HOST "geminiprotocol.net"
#define PORT 1965
#define MAX_CRAWL_COUNT 200

using namespace gg::Net;

size_t crawl_count = 0;
SQLite::Database db("gaggle-gemini.db3", SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE);

void crawl(Link_t &link) {
    crawl_count++;
    GeminiRequest *request = nullptr;
    ResponseParser *parser = nullptr;

    try {
    	request = new GeminiRequest(link.host, link.port);
	    std::string response = request->DoRequest(link.url);

	    parser = new ResponseParser(link.host, link.relative_url);
	    ResponseObject_t ro = parser->ParseResponse(response);


	    if (ro.header.status_code == StatusCode::SUCCESS && crawl_count < MAX_CRAWL_COUNT) {
		    std::cout << "BEGIN [" << crawl_count << "] -------------------------- [" << crawl_count << "] BEGIN" << std::endl
		    		  << "\tRequest: " << link.url << std::endl
		    		  << "\tResponse: " << response.substr(0, response.find("\n")) << std::endl
		    		  << "\tStatus: " << ro.header.status_code << std::endl
		    		  << "\tMimeType: " << ro.header.mime_type << std::endl
		    		  << "\tLang: " << ro.header.lang << std::endl
		              << "\tLinks Size: " << ro.content.links.size() << std::endl;
	        for (Link_t link : ro.content.links) {

				if(link.url.find("http://") != std::string::npos || link.url.find("https://") != std::string::npos)
	         		continue;

	        	SQLite::Statement query(db, "SELECT * FROM links WHERE url == ?");
	         	query.bind(1, link.url);
	        	if(query.executeStep()) { // If there are results returned that means we've seen the link before so we should continue
					std::cout << "Skipping: " << link.url << std::endl;
					continue;
				}

	        	std::cout << "\tLink: " << link.url << std::endl
						  << "\tHost: " << link.host << std::endl
						  << "\tText: " << link.text << std::endl
			              << std::endl;

				SQLite::Transaction transaction(db);
	        	SQLite::Statement insert(db, "INSERT INTO links VALUES (NULL, ?, ?)");
	         	insert.bind(1, link.url);
				insert.bind(2, link.text);
	        	insert.exec();
	        	transaction.commit();

	        	crawl(link);
	        }
		    std::cout << "END [" << crawl_count << "] -------------------------- [" << crawl_count << "] END" << std::endl << std::endl;
	    }

    } catch (Exceptions::SocketException& e) {
    	std::cerr << "Socket Exception: " << e.what() << " Host: " << link.host << " Port: " << link.port << std::endl;
    } catch (Exceptions::SSLException& e) {
    	std::cerr << "SSL Exception: " << e.what() << " Request: " << link.url << std::endl;
    }

    if(request != nullptr)
    	delete request;

    if(parser != nullptr)
    	delete parser;
}

int main() {
	std::cout << "SQLite3 Version " << SQLite::VERSION << SQLite::getLibVersion() << std::endl;

	db.exec("DROP TABLE IF EXISTS links");
    db.exec("CREATE TABLE links (id INTEGER PRIMARY KEY, url TEXT, text TEXT)");

    // // first row
    // int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");
    // std::cout << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb << std::endl;

    // // second row
    // nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");
    // std::cout << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb << std::endl;

    // // update the second row
    // nb = db.exec("UPDATE test SET value=\"second-updated\" WHERE id='2'");
    // std::cout << "UPDATE test SET value=\"second-updated\" WHERE id='2', returned " << nb << std::endl;


    GeminiRequest *request = new GeminiRequest(HOST, PORT);

    std::string response = request->DoRequest("gemini://geminiprotocol.net/");

    ResponseParser *parser = new ResponseParser("gemini://geminiprotocol.net/", "");
    ResponseObject_t ro = parser->ParseResponse(response);

    if (ro.header.status_code == StatusCode::SUCCESS) {
        for (Link_t link : ro.content.links) {

        	SQLite::Transaction transaction(db);
        	SQLite::Statement query(db, "INSERT INTO links VALUES (NULL, ?, ?)");
         	query.bind(1, link.url);
          	query.bind(2, link.text);
        	query.exec();
        	transaction.commit();
	      //   std::cout << "Link: " << link.url << std::endl
					  // << "Host: " << link.host << std::endl
					  // << "Text: " << link.text << std::endl
	      //             << std::endl;
	        crawl(link);
        }
        // for (Heading_t heading : ro.content.headings) {
        // 	std::cout << "Heading: " << heading.heading_text << std::endl;
        // }

    } else {
        std::cout << "Failed to request " << request->GetRequestString()
                << std::endl;
    }

    delete request;
    delete parser;

    return 0;
}
