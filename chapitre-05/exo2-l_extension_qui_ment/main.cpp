
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

int hexVal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return c - 'a' + 10;
}

vector<int> decode(const string& s) {
    vector<int> octets;

    if (s == "-") return octets;

    for (size_t i = 0; i + 1 < s.size(); i += 2) {
        octets.push_back(hexVal(s[i]) * 16 + hexVal(s[i + 1]));
    }

    return octets;
}

bool signature(const vector<int>& b, const vector<int>& sig) {
    if (b.size() < sig.size()) return false;

    return equal(sig.begin(), sig.end(), b.begin());
}

string reconnaitre(long long taille, const vector<int>& b) {
    if (taille < 4) return "REFUSE";

    if (taille >= 8 && signature(b, {0x89, 0x50, 0x4E, 0x47}))
        return "PNG";

    if (signature(b, {0xFF, 0xD8, 0xFF}))
        return "JPEG";

    if (signature(b, {0x42, 0x4D}))
        return "BMP";

    if (signature(b, {0x71, 0x6F, 0x69, 0x66}))
        return "QOI";

    if (signature(b, {0x47, 0x49, 0x46, 0x38}))
        return "GIF";

    if (b.size() >= 4 && b[0] == 0x00 && b[1] == 0x00 &&
        (b[2] == 0x01 || b[2] == 0x02) && b[3] == 0x00)
        return "ICO";

    if (taille >= 10 && signature(b, {0x23, 0x3F}))
        return "HDR";

    if (signature(b, {0x76, 0x2F, 0x31, 0x01}))
        return "EXR";

    if (b.size() >= 2 && b[0] == 0x50 &&
        b[1] >= 0x31 && b[1] <= 0x36) {
        if (b[1] == 0x31 || b[1] == 0x34) return "PBM";
        if (b[1] == 0x32 || b[1] == 0x35) return "PGM";
        return "PPM";
    }

    if (taille >= 18 && b.size() >= 3 &&
        (b[2] == 0x00 || b[2] == 0x01 || b[2] == 0x02 ||
         b[2] == 0x03 || b[2] == 0x09 || b[2] == 0x0A ||
         b[2] == 0x0B))
        return "TGA";

    size_t p = 0;

    if (b.size() >= 3 && b[0] == 0xEF &&
        b[1] == 0xBB && b[2] == 0xBF)
        p = 3;

    while (p < b.size() &&
           (b[p] == 0x20 || b[p] == 0x09 ||
            b[p] == 0x0A || b[p] == 0x0D)) {
        ++p;
    }

    if (p + 5 <= b.size() &&
        b[p] == 0x3C && b[p + 1] == 0x3F &&
        b[p + 2] == 0x78 && b[p + 3] == 0x6D &&
        b[p + 4] == 0x6C)
        return "SVG";

    if (p + 4 <= b.size() &&
        b[p] == 0x3C && b[p + 1] == 0x73 &&
        b[p + 2] == 0x76 && b[p + 3] == 0x67)
        return "SVG";

    return "REFUSE";
}

string extension(const string& nom) {
    size_t point = nom.find_last_of('.');

    if (point == string::npos) return "";

    string ext = nom.substr(point + 1);

    transform(ext.begin(), ext.end(), ext.begin(),
              [](unsigned char c) {
                  return static_cast<char>(tolower(c));
              });

    return ext;
}

bool extensionCorrecte(const string& format, const string& ext) {
    if (format == "PNG") return ext == "png";
    if (format == "JPEG") return ext == "jpg" || ext == "jpeg";
    if (format == "BMP") return ext == "bmp";
    if (format == "QOI") return ext == "qoi";
    if (format == "GIF") return ext == "gif";
    if (format == "ICO") return ext == "ico" || ext == "cur";
    if (format == "HDR") return ext == "hdr";
    if (format == "EXR") return ext == "exr";
    if (format == "PBM") return ext == "pbm";
    if (format == "PGM") return ext == "pgm";
    if (format == "PPM") return ext == "ppm";
    if (format == "TGA") return ext == "tga";
    if (format == "SVG") return ext == "svg";

    return false;
}

int main() {
    int n;
    cin >> n;

    int lus = 0;
    int mensonges = 0;
    int refuses = 0;

    for (int i = 0; i < n; ++i) {
        string nom, octetsHex;
        long long taille;
        cin >> nom >> taille >> octetsHex;

        vector<int> octets = decode(octetsHex);
        string format = reconnaitre(taille, octets);

        if (format == "REFUSE") {
            cout << nom << " REFUSE\n";
            ++refuses;
            continue;
        }

        string ext = extension(nom);
        bool ok = extensionCorrecte(format, ext);

        cout << nom << ' ' << format << ' '
             << (ok ? "OK" : "MENT") << '\n';

        ++lus;

        if (!ok) ++mensonges;
    }

    cout << "LUS " << lus << '\n';
    cout << "MENSONGES " << mensonges << '\n';
    cout << "REFUSES " << refuses << '\n';

    return 0;
}