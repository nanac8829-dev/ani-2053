#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// Structure pour stocker une piste
struct Track {
    string type; // "vide" ou "soun"
    string code; // 4 caractères
};

// Structure pour stocker les informations d'un fichier
struct FileInfo {
    string name;
    string hexBytes;
    int numTracks;
    vector<Track> tracks;
    string container;
    bool isMp4 = false;
    bool isUnknown = false;
    string playerVerdict = "";
};

// Fonction pour convertir une chaîne hexadécimale en vecteur d'octets (valeurs entières)
vector<int> hexToBytes(const string& hexStr) {
    vector<int> bytes;
    if (hexStr == "-" || hexStr.empty()) return bytes;
    for (size_t i = 0; i + 1 < hexStr.length(); i += 2) {
        string byteString = hexStr.substr(i, 2);
        int byteValue = stoi(byteString, nullptr, 16);
        bytes.push_back(byteValue);
    }
    return bytes;
}

// Détermination du conteneur selon les octets
string detectContainer(const vector<int>& b) {
    int n = b.size();

    // 1. MP4 : n >= 12 et b[4..7] == 66 74 79 70 ("ftyp")
    if (n >= 12 && b[4] == 0x66 && b[5] == 0x74 && b[6] == 0x79 && b[7] == 0x70) {
        return "MP4";
    }
    // 2. WEBM : n >= 4 et b[0..3] == 1A 45 DF A3
    if (n >= 4 && b[0] == 0x1A && b[1] == 0x45 && b[2] == 0xDF && b[3] == 0xA3) {
        return "WEBM";
    }
    // 3. WAV : n >= 12, b[0..3] == "RIFF" (52 49 46 46) et b[8..11] == "WAVE" (57 41 56 45)
    if (n >= 12 && b[0] == 0x52 && b[1] == 0x49 && b[2] == 0x46 && b[3] == 0x46 &&
        b[8] == 0x57 && b[9] == 0x41 && b[10] == 0x56 && b[11] == 0x45) {
        return "WAV";
    }
    // 4. OGG : "OggS" (4F 67 67 53), n >= 4
    if (n >= 4 && b[0] == 0x4F && b[1] == 0x67 && b[2] == 0x67 && b[3] == 0x53) {
        return "OGG";
    }
    // 5. FLAC : "fLaC" (66 4C 61 43), n >= 4
    if (n >= 4 && b[0] == 0x66 && b[1] == 0x4C && b[2] == 0x61 && b[3] == 0x43) {
        return "FLAC";
    }
    // 6. MP3 : "ID3" (49 44 33), n >= 3
    if (n >= 3 && b[0] == 0x49 && b[1] == 0x44 && b[2] == 0x33) {
        return "MP3";
    }
    // 7. MP3 encore : n >= 2, b[0] == FF, b[1] >= E0
    if (n >= 2 && b[0] == 0xFF && b[1] >= 0xE0) {
        return "MP3";
    }

    // 8. Sinon : INCONNU
    return "INCONNU";
}

// Conversion du code de codec selon la table
string translateCodec(const string& code) {
    if (code == "mp4a") return "aac";
    if (code == "Opus" || code == "opus") return "opus";
    if (code == "avc1" || code == "avc3") return "h264";
    if (code == "hvc1" || code == "hev1") return "h265";
    if (code == "vp08") return "vp8";
    if (code == "vp09") return "vp9";
    if (code == "mp4v") return "mpeg4";
    if (code == ".mp3") return "mp3";
    if (code == "twos" || code == "sowt" || code == "lpcm") return "pcm";
    return code; // Par défaut, garde son nom tel quel
}

int main() {
    // Optimisation des flux d'E/S
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<FileInfo> files;
    int countMp4 = 0;
    int countLisibles = 0;
    int countInconnus = 0;

    for (int i = 0; i < N; ++i) {
        FileInfo file;
        cin >> file.name >> file.hexBytes >> file.numTracks;
        
        for (int t = 0; t < file.numTracks; ++t) {
            Track tr;
            cin >> tr.type >> tr.code;
            file.tracks.push_back(tr);
        }

        vector<int> bytes = hexToBytes(file.hexBytes);
        file.container = detectContainer(bytes);

        if (file.container == "MP4") {
            file.isMp4 = true;
            countMp4++;
            
            // Recherche de la première piste vide (type == "vide")
            string firstVideoCode = "";
            bool foundVideo = false;
            for (const auto& tr : file.tracks) {
                if (tr.type == "vide") {
                    firstVideoCode = tr.code;
                    foundVideo = true;
                    break;
                }
            }

            if (!foundVideo) {
                file.playerVerdict = "SANS_IMAGE";
            } else {
                // Le lecteur regarde si le code de la première piste vide vaut :
                // mjpa, jpeg, MJPG, avc1, avc3, hvc1, hev1 ou av01
                if (firstVideoCode == "mjpa" || firstVideoCode == "jpeg" || firstVideoCode == "MJPG" ||
                    firstVideoCode == "avc1" || firstVideoCode == "avc3" || firstVideoCode == "hvc1" ||
                    firstVideoCode == "hev1" || firstVideoCode == "av01") {
                    file.playerVerdict = "LISIBLE";
                    countLisibles++;
                } else {
                    file.playerVerdict = "ECHEC";
                }
            }
        } else if (file.container == "INCONNU") {
            file.isUnknown = true;
            countInconnus++;
        }

        files.push_back(file);
    }

    // Affichage des résultats selon le format strict
    for (const auto& file : files) {
        cout << file.name << " " << file.container << "\n";
        
        if (file.isMp4) {
            int k = 1;
            for (const auto& tr : file.tracks) {
                string trackTypeUpper = (tr.type == "vide") ? "VIDEO" : "AUDIO";
                string translated = translateCodec(tr.code);
                cout << file.name << " PISTE " << k << " " << trackTypeUpper << " " << translated << "\n";
                k++;
            }
            cout << file.name << " LECTEUR " << file.playerVerdict << "\n";
        }
    }

    cout << "MP4 " << countMp4 << "\n";
    cout << "LISIBLES " << countLisibles << "\n";
    cout << "INCONNUS " << countInconnus << "\n";

    return 0;
}