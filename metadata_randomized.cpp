#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <exception>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;

string filetostring(const string);

bool copyFile(string SRC, string DEST);

int main (int argc, char* argv[]) {

    //populate a vector with numbers 1-10,000
    vector<int> randomNums;

    for(int i = 1; i <= 10000; i++) {
        randomNums.push_back(i);
    }


    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    //shuffle the numbers randomly
    shuffle( randomNums.begin(), randomNums.end(), std::default_random_engine(seed));

    string luminaries [100] = {"Albert Einstein", "Friedrich Nietzsche", "Julius Caesar","Joan of Arc","Aristotle","Alan Turing","Benito Mussolini","King Charlemagne","Napoleon Bonaparte","Soren Kierkegaard",
"Rumi","Dante Alighieri","Plato","WEB Du Bois","Henry David Thoreau","Thomas Hobbes","Averroes","Socrates","Moses","Leonardo da Vinci","Attila the Hun","Vladimir Lenin","Cicero",
"Hammurabi","Buddha","Theodore Roosevelt","Miyamoto Musashi","Marcus Aurelius","Augustus","Booker T. Washington","Marilyn Monroe","Thomas Jefferson","Graham Bell","Wolfgang Amadeus Mozart","Jean-Jacques Rousseau",
"Charles Darwin","Sir William Wallace","Abraham Lincoln","Otto von Bismarck","Andre Gide","Horace","Christopher Columbus","Ludwig van Beethoven","William Shakespeare","Benjamin Franklin","Sigmund Freud",
"Che Guevara","Marcus Garvey","Mahatma Gandhi","Carl Gustav Jung","Pythagoras","George Bernard Shaw","Shaka Zulu","Marie Curie","Marco Polo","Sir Francis Bacon","King Solomon","Elbert Hubbard",
"Saint Augustine","Anne Frank","John Locke","Frederick Douglass","Sir Francis Drake","Catherine The Great","Queen Elizabeth I","Nikola Tesla","Henry Ford","Jesus of Nazareth","Toyotomi Hideyoshi",
"Genghis Khan","John D. Rockefeller","Virgil","Vincent Van Gogh","Rene Descartes","Adam Smith","Alexander the Great","Simon Bolivar","Epicurus","Virginia Woolf","Lao Tzu","John F. Kennedy",
"Sun Tzu","Immanuel Kant","Scott Fitzgerald","Edgar Allan Poe","Karl Marx","Niccolo Machiavelli","Nicolaus Copernicus","Michelangelo","Oscar Wilde","Confucius","Marie Antoinette","Sir Isaac Newton",
"Voltaire","Ralph Waldo Emerson","George Washington","Thomas Edison","Seneca","Galileo Galilei","Pericles"};

    for (int k = 0; k<100; k++) {

        for (int j = 1; j<=10; j++) {

            string inputFolder = luminaries[k] + "/" + to_string(j);

            string icon = luminaries[k];

            string description = filetostring(inputFolder + "/Description.txt");

            /*
            //remove all double quotes so it doesn't fuck up the JSON
            size_t pos = description.find( "“" );
            while ( pos != string::npos) {
                description.erase(pos, 1);
                //description[pos] = '\\';
                //description[++pos] = '"';
                pos = description.find( "“" );
            } */

            //output for debugging
            cout << description << endl;

            string JSON;

            std::vector<string> files;

            //get all the files in the folder in a vector using std::filesystem (C++ 17+)
            for (const auto & entry : filesystem::directory_iterator(inputFolder)) {
                cout << entry.path() << endl;

                string fname = entry.path().u8string();

                //only add it if it's a png
                if ( fname.substr(fname.find_last_of(".") + 1) == "png" ) {
                    files.push_back(fname);
                }
            }

            for (int i = 0 ; i < 10; i++) {
                /*
                //copy the image from files<> to \images directory labelled as i.png
                filesystem::path sourceFile = files.back();
                filesystem::path targetParent = "images\\" + to_string(i) + ".png";
                auto target = targetParent;
                try
                {
                    filesystem::create_directories(targetParent); // Recursively create target directory if not existing.
                    filesystem::copy_file(sourceFile, targetParent, filesystem::copy_options::overwrite_existing);
                }
                catch (std::exception& e) 
                {
                    std::cout << e.what();
                } */

                int number = randomNums.back();

                string dest = "images/" + to_string(number) + ".png";
                string fname = files.back();

                bool worked = copyFile(fname, dest);

                //bool checkOnix = fname.substr(0, 4) == "Onix";
                //bool checkGold = fname.substr(0, 4) == "Gold";
                //bool checkSilver = fname.substr(0, 6) == "Silver";

                size_t checkOnix = fname.find("Onix");
                size_t checkGold = fname.find("Gold");
                size_t checkSilver = fname.find("Silver");

                //check files.back has "Gold" "Silver" or "Onix" in it
                //generate JSON metadata files as i.json and copy them to /metadata directory
                //onix
                if (checkOnix != std::string::npos){
                    JSON = "{ \n \"description\": \" " + description + " \", \n  \"image\": \"ipfs://someshithere/" + to_string(number) + ".png\", \n \"name\": \"Tempus #" + to_string(number) +  "\", \n \"edition\": " + to_string(number) + ", \n \"attributes\": [ \n { \n \"trait_type\": \"Luminary\", \n \"value\": \"" + icon + "\" \n }, \n { \n \"trait_type\": \"Legendary\", \n \"value\": \"Onix\" \n } \n ] \n } ";
                }
                //gold
                else if (checkGold != std::string::npos){
                    JSON = "{ \n \"description\": \" " + description + " \", \n  \"image\": \"ipfs://someshithere/" + to_string(number) + ".png\", \n \"name\": \"Tempus #" + to_string(number) +  "\", \n \"edition\": " + to_string(number) + ", \n \"attributes\": [ \n { \n \"trait_type\": \"Luminary\", \n \"value\": \"" + icon + "\" \n }, \n { \n \"trait_type\": \"Legendary\", \n \"value\": \"Gold\" \n } \n ] \n } ";
                }
                //silver
                else if (checkSilver != std::string::npos){
                    JSON = "{ \n \"description\": \" " + description + " \", \n  \"image\": \"ipfs://someshithere/" + to_string(number) + ".png\", \n \"name\": \"Tempus #" + to_string(number) +  "\", \n \"edition\": " + to_string(number) + ", \n \"attributes\": [ \n { \n \"trait_type\": \"Luminary\", \n \"value\": \"" + icon + "\" \n }, \n { \n \"trait_type\": \"Legendary\", \n \"value\": \"Silver\" \n } \n ] \n } ";
                }
                //no legendary
                else {
                    JSON = "{ \n \"description\": \" " + description + " \", \n  \"image\": \"ipfs://someshithere/" + to_string(number) + ".png\", \n \"name\": \"Tempus #" + to_string(number) +  "\", \n \"edition\": " + to_string(number) + ", \n \"attributes\": [ \n { \n \"trait_type\": \"Luminary\", \n \"value\": \"" + icon + "\" \n } \n ] \n } ";
                }

                //for debugging
                cout << JSON;

                std::ofstream outfile ("metadata/" + to_string(number) + ".json");

                outfile << JSON;

                outfile.close();
                files.pop_back();

                randomNums.pop_back();

            }

        }

    }

    return 0;
}

string filetostring(const string description){
	ifstream file(description, ios::binary);
    string fileStr;

    istreambuf_iterator<char> inputIt(file), emptyInputIt;
    back_insert_iterator<string> stringInsert(fileStr);

    copy(inputIt, emptyInputIt, stringInsert);

    return fileStr;
}

bool copyFile(string SRC, string DEST)
{
    std::ifstream src(SRC, std::ios::binary);
    std::ofstream dest(DEST, std::ios::binary);
    dest << src.rdbuf();
    return src && dest;
}