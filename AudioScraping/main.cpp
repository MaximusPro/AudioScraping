#include <iostream>
#include <string>
#include <curl/curl.h>
#include "Scraper.h"




int main(int argc, char* argv[]) {
    string client_id, link;
    if(!argc%3)
    {
        return -1;
    }
    vector<string> OldArgs;
    for (int i = 0; i < argc; ++i)
    {
        OldArgs.push_back(argv[i]);
    }
    for (int i = 1; i < OldArgs.size(); i += 2)
    {
        if (OldArgs[i] == "-client_id")
            client_id = OldArgs[i + 1];
        else if (OldArgs[i] == "-link")
            link = OldArgs[i + 1];
        else cerr << "Argument is not defined!" << endl;
    }

    std::string url = "https://api-v2.soundcloud.com/resolve?url=" + link + "&client_id=" + client_id;

    std::string html;
    auto err = DownloadHTML(url, &html);
    cout << "Lines on HTML: " << endl;
    cout << html << endl;
    ofstream file("Scraping.txt");
    if (file.is_open());
    {
        file << html;
        rapidjson::Document JSONDoc;
        JSONDoc.Parse(html.c_str());
    }
    file.close();
    ifstream ScrapingFile;
    ScrapingFile.open("Scraping.txt");
    string JSONTXT, line;
    string urnString, idString;
    if (ScrapingFile.is_open())
    {
        while (getline(ScrapingFile, line))
        {
            JSONTXT = JSONTXT + line + "\n";
        }
        size_t start = 0;
        if (JSONTXT.size() >= 3 &&
            static_cast<unsigned char>(JSONTXT[0]) == 0xEF &&
            static_cast<unsigned char>(JSONTXT[1]) == 0xBB &&
            static_cast<unsigned char>(JSONTXT[2]) == 0xBF)
        {
            start = 3;
            std::cout << "BOM found and deleted!\n";
        }
        if (JSONTXT.empty())
        {
            cerr << "ERROR: Data is empty!" << endl;
            return -1;
        }
        rapidjson::Document JSONDoc;
        JSONDoc.Parse<rapidjson::kParseStopWhenDoneFlag>(
            JSONTXT.data() + start,
            JSONTXT.size() - start
        );
        if (JSONDoc.HasParseError()) {
            std::cerr << "Error parsing on position: " << JSONDoc.GetErrorOffset() << "\n";
            //std::cerr << rapidjson::GetParseError_En(JSONDoc.GetParseError()) << "\n";

            // Показываем проблемный фрагмент
            size_t offset = JSONDoc.GetErrorOffset();
            size_t from = (offset > 20) ? offset - 20 : 0;
            std::string fragment = JSONTXT.substr(from, 40);
            std::cerr << "Fragment: ... " << fragment << " ...\n";
        }
        else {
            std::cout << "Scraping sucsessful!\n";
            std::cout << "track_count: " << JSONDoc["track_count"].GetInt() << "\n";
            std::cout << "username: " << JSONDoc["username"].GetString() << "\n";
        }
        assert(JSONDoc.IsObject());
        if (JSONDoc.HasMember("urn"))
        {
            if (JSONDoc["urn"].IsString())
                urnString.append(JSONDoc["urn"].GetString());
                cout << "urn = " << JSONDoc["urn"].GetString() << endl;
        }
        if (JSONDoc.HasMember("id"))
        {
            if (JSONDoc["id"].IsInt())
                idString.append(to_string(JSONDoc["id"].GetInt()));
            cout << "id = " << JSONDoc["id"].GetInt() << endl;
        }
    }
    if (!urnString.empty() || !idString.empty())
    {
        cout << "urn = " << urnString << endl;
        cout << "id = " << idString << endl;

    }
 
    auto TrackList = get_soundcloud_track_urls(idString, client_id, 80);
    if (TrackList.empty())
    {
        cerr << "Error: TrackList is empty!" << endl;
        return -3;
    }
    ofstream WriteFileLinks("soundcloud_links.txt");
    for (auto Track : TrackList)
    {
        Track += "\n";
        WriteFileLinks.write(Track.c_str(), Track.size());
        cout << Track << endl;
    }
    WriteFileLinks.close();
    std::ifstream ReadFileLinks("soundcloud_links.txt");
    std::string TrackLink;

    if (!ReadFileLinks.is_open()) {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return -1;
    }
    vector<pair<string, string>> TrackNameList;
    int i = 0;
    while (std::getline(ReadFileLinks, TrackLink) && i < 3) {
        /*std::string cmd =
            "cd /d \"C:\\Users\\Max\\AudioDownloader\" && "
            "\"C:\\Users\\Max\\AudioDownloader\\yt-dlp.exe\" --get-title \""
            + TrackLink + "\" 2>nul";*/
        std::string cmd = "yt-dlp --get-title " + TrackLink + " 2>nul";
        pair<string, string>PairStr;
        string StrCMD = exec(cmd.c_str());
        cout << StrCMD << endl;
        PairStr.first = StrCMD;
        PairStr.second = TrackLink;
        TrackNameList.push_back(PairStr);
        i++;
    }
    cout << "First 3 line from TrackList..." << endl;
    for (int i = 0; i < 3; i++)
    {
        pair<string, string> Pair = TrackNameList[i];
        cout << i << ". " << Pair.first << " " << Pair.second << endl;
    }
    cout << "Shaping CSV file..." << endl;
    WriteToCSV(string("TrackList.csv"), &TrackNameList);
    cout << "Done!";
    /*cout << "Download first 3 tracks..." << endl;
    string Command;
    for (int i = 0; i < 3; i++)
    {
        Command = "yt-dlp \"" + TrackList[i] + "\" -x --audio-format mp3 --ffmpeg-location";
        system(Command.c_str());
        Command.clear();
    }*/
    // std::string cmd = "yt-dlp \"" + url + "\" -x --audio-format mp3";
        // system(cmd.c_str());
   // string Command1 = "yt-dlp --flat-playlist --print url ""https://soundcloud.com/eatbrain/tracks"" > soundcloud_links.txt\n";
    //system(Command1.c_str());

    return 0;
}