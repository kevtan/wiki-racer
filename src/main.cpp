#include <ostream>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"

using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::ostream;

// Function and operator prototypes
ostream& operator<<(ostream& output, const vector<string> vec);
int commonLinks(unordered_set<string>& link_set1, unordered_set<string>& link_set2);
vector<string> findWikiLadder(const string& start_page, const string& end_page);

int main () {

    auto ladder = findWikiLadder("Emu", "Stanford_University");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t";
        cout << ladder << endl;
    }

    return 0;
}


/* Finds the list of links to be followed form start page to end page.
 * @param const string& start_page - title of starting page
 * @param const string& end_page - title of target page
 * @return vector<string> - vector of intermediate page names
 */
vector<string> findWikiLadder(const string& start_page, const string& end_page) {

    // Create WikiScraper object
    WikiScraper scraper;

    // Create link set for target page and initial page
    auto target_set = scraper.getLinkSet(end_page);
    auto initial_set = scraper.getLinkSet(start_page);

    // Lambda comparator function for priority queue
    auto cmpFn = [&scraper, &target_set](vector<string> links1, vector<string> links2) {
        auto link_set1 = scraper.getLinkSet(links1.back());
        auto link_set2 = scraper.getLinkSet(links2.back());
        return commonLinks(link_set1, target_set) < commonLinks(link_set2, target_set);
    };

    // Create priority queue
    std::priority_queue<vector<string>, vector<vector<string>>, decltype(cmpFn)> ladderQueue(cmpFn);

    ladderQueue.push({start_page});

    while (!ladderQueue.empty()) {

        // dequeue ladder whose last page has most shared links with target page
        auto promising_ladder = ladderQueue.top();
        ladderQueue.pop();
        auto link_set = scraper.getLinkSet(promising_ladder.back());

        for (string link : link_set) {
            vector<string> ladder = promising_ladder;
            ladder.push_back(link);

            if (link == end_page) {return ladder;}

            ladderQueue.push(ladder);
        }
    }

    return {};

}

/* Finds the number of common links between two unordered sets.
 * @param unordered_set<string>& link_set1 - set of links on page 1
 * @param unordered_set<string>& link_set2 - set of links on page 2
 * @return int - number of common links
 */
int commonLinks(unordered_set<string>& link_set1, unordered_set<string>& link_set2) {
    int counter = 0;
    for (string link1 : link_set1) {
        if (link_set2.count(link1)) {
            counter++;
        }
    }
    return counter;
}

/* Defines the printing operation for a vector of string.
 * @param ostream& output - target stream for output
 * @param const vector<string> vec - vector to print
 * @return ostream& output - target stream, allows for nested printing
 */
ostream& operator<<(ostream& output, const vector<string> vec) {
    output << "{";
    for (string elem : vec) {
        output << elem << ", ";
    }
    output << "\b\b}" << std::flush;
    return output;
}




