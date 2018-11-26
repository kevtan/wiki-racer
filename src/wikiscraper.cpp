#include "wikiscraper.h"
#include "error.h"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <string>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;
using std::ifstream;        using std::cin;
using std::cerr;            using std::ostream;

unordered_set<string> findWikiLinks(const string& page_html);
ifstream getFile();
string fileToString(ifstream& fileName);

// operator overloading for unordered_set
ostream& operator <<(ostream& output, unordered_set<string> links) {
    for (string link : links) {
        output << link << endl;
    }
    return output;
}

bool validChar(char c) {
    return !(c == ':' || c == '#');
}

unordered_set<string> findWikiLinks(const string& page_html) {
    unordered_set<string> links;
    string frontTag = "<a href=\"/wiki/";
    string backTag = "\"";
    auto frontIter = page_html.begin();
    auto backIter = page_html.end();
    while (true) {
        auto front = std::search(frontIter, backIter, frontTag.begin(), frontTag.end());
        if (front == backIter) {
            break;
        }
        auto back = std::search(front + 15, backIter, backTag.begin(), backTag.end());

        string link = std::string(front + 15, back);

        if (std::all_of(link.begin(), link.end(), validChar)) {
            links.insert(link);
        }

        frontIter = back;
    }
    return links;
}

ifstream getFile() {
    ifstream file;
    while (true) {
        cout << "Please the file name of the Wikipedia page you would like to parse: ";
        string fileName;
        cin >> fileName;

        file.open(fileName);

        if (!file.fail()) {
            return file;
        }
        cerr << "\nThere was something wrong with your file name. Try again!" << endl;
    }
}

string fileToString(ifstream& fileName) {
    string container;
    string line;
    while (std::getline(fileName, line)) {
        container += line;
    }
    return container;
}

unordered_set<string> WikiScraper::getLinkSet(const string& page_name) {
    if(linkset_cache.find(page_name) == linkset_cache.end()) {
        auto links = findWikiLinks(getPageSource(page_name));
        linkset_cache[page_name] = links;
    }
    return linkset_cache[page_name];
}


WikiScraper::WikiScraper() {
    (void)getPageSource("Main_Page");
#ifdef _WIN32
    // define something for Windows
    system("cls");
#else
    // define it for a Unix machine
    system("clear");
#endif
}


string createPageUrl(const string& page_name) {
    return "https://en.wikipedia.org/wiki/" + page_name;
}

void notFoundError(const string& msg, const string& page_name, const string& url) {
    const string title = "    AN ERROR OCCURED DURING EXECUTION.    ";
    const string border(title.size() + 4, '*');
    cerr << endl;
    errorPrint(border);
    errorPrint("* " + title + " *");
    errorPrint(border);
    errorPrint();
    errorPrint("Reason: " + msg);
    errorPrint();
    errorPrint("Debug Information:");
    errorPrint();
    errorPrint("\t- Input parameter: " + page_name);
    errorPrint("\t- Attempted url: " + url);
    errorPrint();
}

string WikiScraper::getPageSource(const string &page_name) {
    const static string not_found = "Wikipedia does not have an article with this exact name.";
    if(page_cache.find(page_name) == page_cache.end()) {
        QUrl url(createPageUrl(page_name).c_str()); // need c string to convert to QString

        QNetworkRequest request(url);

        QNetworkReply *reply(manager.get(request));

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()) , &loop, SLOT(quit()));
        loop.exec();

        string ret = reply->readAll().toStdString();
        if(std::search(ret.begin(), ret.end(), not_found.begin(), not_found.end()) != ret.end()){
            notFoundError("Page does not exist!", page_name, url.toString().toStdString());
            return "";
        }
        size_t indx = ret.find("plainlinks hlist navbar mini");
        if(indx != string::npos) {
            return ret.substr(0, indx);
        }
        page_cache[page_name] = ret;
    }
    return page_cache[page_name];
}





